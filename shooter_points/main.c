#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED(x) (void)(x)
#define da_realloc(da)\
            (da)->items = \
                realloc((da)->items, (da)->capacity * sizeof(*(da)->items))

#define DA_INIT_CAPACITY 256
#define da_append(da, item)\
    do {\
        if ((da)->capacity == 0) {\
            (da)->capacity = DA_INIT_CAPACITY;\
        } else if ((da)->count + 1 > (da)->capacity) {\
            (da)->capacity *= 2;\
        }\
        da_realloc(da);\
        (da)->items[(da)->count++] = (item);\
    } while (0)

#define da_last(da) (da)->items[(da)->count-1]
#define da_pop(da)\
    do {\
        (da)->count--;\
        if ((da)->count * 2 <= (da)->capacity) {\
            (da)->capacity /= 2;\
            da_realloc(da);\
        }\
    } while (0)

#define da_remove(da, idx)\
    do {\
        for (size_t i = idx; i < (da)->count; i++) {\
            (da)->items[(idx)] = (da)->items[(idx) + 1];\
        }\
        (da)->count--;\
    } while (0)

#define da_foreach(da, Type, it)\
    for (Type *it = (da)->items; it < (da)->items + (da)->count; ++it)


#define W 1600
#define H 1200
#define C_N 20

#define BG_N 30
#define BG_VX 0.75f
#define BG_VY 0.66f
#define BG_MINR 2
#define BG_MAXR 7

#define P_N 16
#define P_DYING_TIME 50.f
#define P_INIT_R 15
#define P_KILL_R 10
#define P_RELOAD_TIME 300.f

#define B_INIT 30

typedef struct {
    Vector2 p;
    Vector2 v;
    Vector2 a;
    int r;
} Point;

typedef enum {
    DEAD,
    DYING,
    ALIVE
} State;

typedef struct {
    Point pt;
    State state;
    double time_of_death; // time of death
    Color color;
    size_t bullets;
    double last_shot_time;
    double reload_time;
    size_t kill_count;
} Player;

typedef struct {
    size_t owner;
    Vector2 curr;
    Vector2 next;
} Bullet;

typedef struct {
    Bullet *items;
    size_t count;
    size_t capacity;
} Bullets;

typedef struct {
    size_t *items;
    size_t count;
    size_t capacity;
} Nums;

Color COLORS[C_N] = {
//	LIGHTGRAY, used for DYING
	GRAY,
	DARKGRAY,
//	YELLOW, used for DYING
	GOLD,
	ORANGE,
	PINK,
	RED,
	MAROON,
	GREEN,
	LIME,
	DARKGREEN,
	SKYBLUE,
	BLUE,
	DARKBLUE,
	PURPLE,
	VIOLET,
	DARKPURPLE,
	BEIGE,
	BROWN,
	DARKBROWN,
	MAGENTA
};

Vector2 get_random_vector() {
    return (Vector2){ .x = rand() % W, .y = rand() % H };
}

Color get_random_color() {
    size_t idx = rand() % C_N;
    return COLORS[idx];
}

void check_bounds(Point *point) {
    if (point->p.x <= 0.0 || point->p.x > W) {
        point->v.x *= -1;
        point->p.x = fminf(fmaxf(0.0, point->p.x), W);
    }
    if (point->p.y <= 0.0 || point->p.y > H) {
        point->v.y *= -1;
        point->p.y = fminf(fmaxf(0.0, point->p.y), H);
    }
}

void update_point(Point *point, double dt, bool wrap, bool bounce) {
       point->v.x += point->a.x * dt;
       point->v.y += point->a.y * dt;
       if (wrap) {
           point->p.x = fmodf(point->p.x + point->v.x * dt, W);
           point->p.y = fmodf(point->p.y + point->v.y * dt, H);
       } else {
           point->p.x += point->v.x * dt;
           point->p.y += point->v.y * dt;
           if (bounce) {
               check_bounds(point);
           }
       }
}

void update_points(Point *points, size_t N, double dt, bool wrap, bool bounce) {
   for (size_t i = 0; i < N; ++i) {
       update_point(&points[i], dt, wrap, bounce);
   }
}

void change_direction(Player *player, Vector2 bullet_c, Vector2 player_c) {
    Vector2 pcd = Vector2Normalize(Vector2Subtract(player_c, player->pt.p));
    Vector2 bcd = Vector2Normalize(Vector2Subtract(bullet_c, player->pt.p));
    int D = (rand() % 8) * 2 * PI / 8;
    player->pt.v.x = sin(D) + pcd.x - bcd.x;
    player->pt.v.y = cos(D) + pcd.y - bcd.y;
}

void spawn_new_player(Player *players, size_t i) {
    Player player = {
        .pt = { 
            .p = get_random_vector(),
            .v = Vector2Zero(),
            .a = Vector2Zero(),
            .r = P_INIT_R
        },
        .state = ALIVE,
        .time_of_death = 0.f,
        .color = get_random_color(),
        .bullets = B_INIT,
        .kill_count = 0,
        .last_shot_time = 0.f,
        .reload_time = P_RELOAD_TIME
    };
    change_direction(&player, Vector2Zero(), Vector2Zero());
    players[i] = player;
}

Player* find_closest_player(Player *players, size_t N, size_t idx) {
    double min_d = W * H;
    size_t min_idx;
    bool found = false;
    for (size_t i = 0; i < N; ++i) {
        if (i == idx) continue;
        if (players[i].state != ALIVE) continue;
        double d = Vector2Distance(players[idx].pt.p, players[i].pt.p);
        if (d < min_d) {
            min_d = d;
            min_idx = i;
            found = true;
        }
    }
    return !found ? NULL : &players[min_idx];
}

void shoot_closest(Player *players, size_t N, size_t idx, Bullets *bullets) {
    Player *closest = find_closest_player(players, N, idx);
    if (closest == NULL) return;
    Player ego = players[idx];
    Vector2 d = {
        .x = closest->pt.p.x - ego.pt.p.x,
        .y = closest->pt.p.y - ego.pt.p.y
    };
    Vector2 nd = Vector2Scale(Vector2Normalize(d), 8);
    Bullet b = {
        .owner = idx,
        .curr = Vector2Add(ego.pt.p, nd),
        .next = nd
    };
    da_append(bullets, b);
}

Vector2 calculate_bullet_center(Bullets *bullets) {
    Vector2 bc = Vector2Zero();
    da_foreach(bullets, Bullet, b) {
        bc = Vector2Add(bc, b->curr);
    }
    return (Vector2){ .x = bc.x / bullets->count, .y = bc.y / bullets->count };
}

Vector2 calculate_player_center(Player *players, size_t N) {
    Vector2 pc = Vector2Zero();
    for (size_t i = 0; i < N; ++i) {
        if (players[i].state != ALIVE) continue;
        pc = Vector2Add(pc, players[i].pt.p);
    }
    return (Vector2){ .x = pc.x / N, .y = pc.y / N };
}

void update_players(Player *players, Bullets *bullets, size_t N, double dt, double t, bool wrap, bool bounce) {
    for (size_t i = 0; i < N; ++i) {
        switch (players[i].state) {
            case ALIVE: 
            {
                if (players[i].bullets) {
                    if (players[i].bullets % 10 == 0) {
                        shoot_closest(players, N, i, bullets);
                        players[i].last_shot_time = t;
                    }
                    players[i].bullets--;
                } else if (t - players[i].last_shot_time >= players[i].reload_time) {
                    players[i].bullets = B_INIT + players[i].kill_count;
                }
                Vector2 bc = calculate_bullet_center(bullets);
                Vector2 pc = calculate_player_center(players, N);
                change_direction(&players[i], bc, pc);
                update_point(&players[i].pt, dt, wrap, bounce);
                break;
            }
            case DYING: 
            {
                if (t - players[i].time_of_death > P_DYING_TIME) {
                    players[i].state = DEAD;
                }
                break;
            }
            case DEAD: 
            {
                spawn_new_player(players, i);
                break;
            }
            default:
            {
            }    
        }
    }
}

void draw_player(Player *player, double t) {
    size_t R = player->pt.r + player->kill_count;
    switch (player->state) {
        case ALIVE: 
            DrawCircleV(player->pt.p, R, player->color);
            break;
        case DYING: {
            Color color = (t - player->time_of_death < P_DYING_TIME / 2.f) ? YELLOW : WHITE;
            DrawCircleV(player->pt.p, R * 2.f, color);
            break;
        }
        default:
            // pass
            break;
    }
}

Player* is_hit(Bullet *bullet, Player *players, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        if (players[i].state != ALIVE) continue;
        if (Vector2Distance(bullet->curr, players[i].pt.p) < P_KILL_R) {
            return &players[i];
        }
    }
    return NULL;
}

void update_bullets(Bullets *bullets, Player *players, size_t N, double t) {
    size_t i = 0;
    Nums to_remove = {0};
    da_foreach(bullets, Bullet, b) {
        b->curr = Vector2Add(b->curr, b->next);
        Player *p;
        if ((p = is_hit(b, players, N)) != NULL) {
            p->state = DYING;
            p->time_of_death = t;
            players[b->owner].kill_count++;
            da_append(&to_remove, i);
        } else {
            if (players[b->owner].state != ALIVE) {
                da_append(&to_remove, i);
            }
        }
        ++i;
    }
    while (to_remove.count) {
        da_remove(bullets, da_last(&to_remove));
        da_pop(&to_remove);
    }
}

int main(void) {
    Point bg[BG_N] = {0};
    for (size_t i = 0; i < BG_N; ++i) {
        Point np = {
            .p = get_random_vector(),
            .v = { .x = BG_VX, .y = BG_VY },
            .a = Vector2Zero(),
            .r = BG_MINR + (rand() % (BG_MAXR - BG_MINR))
        };
        bg[i] = np;
    }

    Player players[P_N] = {0};
    for (size_t i = 0; i < P_N; ++i) {
        spawn_new_player(players, i);
    }

    Bullets bullets = {0};

    InitWindow(W, H, "Raylib Template");
    SetTargetFPS(60);

    double dt = 3.f;
    double t = 0.f;
    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        }
        update_points(bg, BG_N, dt, true, false);
        update_players(players, &bullets, P_N, dt, t, false, true);
        update_bullets(&bullets, players, P_N, t);
        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        for (size_t i = 0; i < BG_N; ++i) {
            DrawCircleV(bg[i].p, bg[i].r, WHITE);
        }
        for (size_t i = 0; i < P_N; ++i) {
            draw_player(&players[i], t);
        }
        da_foreach(&bullets, Bullet, b) {
            DrawLineV(b->curr, Vector2Add(b->curr, b->next), players[b->owner].color);
        }
        EndDrawing();
        t += dt;
    }
    CloseWindow();

    return 0;
}
