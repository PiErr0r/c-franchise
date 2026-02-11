#include <assert.h>
#include <time.h>

#include "helpers.h"

#define WIDTH 1600
#define WIDTH_PRINCIPAL 1.f
#define HEIGHT 1200
#define HEIGHT_PRINCIPAL 0.75f
#define FOCAL_LENGTH 1.f

#define DP 0.05         // unitary position
#define DTHETA PI/200   // unitary rotation delta
#define DT 1e-3         // delta time

#define BALL_INIT_V_SCALE 30.f
#define BALL_R 0.05f

#define BOX_BOUND 1.f

#define EGO_INIT_X -5.f
#define EGO_INIT_Y 0.f
#define EGO_INIT_Z 0.f
#define EGO_INIT_ROLL PI*0.5f
#define EGO_INIT_PITCH 0.f
#define EGO_INIT_YAW PI*0.5f

#define TILE_FADE_TIME 0.09f
#define TILE_CLAMP_RESOLUTION 0.1f

typedef struct {
    Vector3* items;
    size_t count;
    size_t capacity;
} Points;

typedef struct {
    Vector3 p;
    Vector3 v;
    Vector3 a;
    int r;
} Ball;

typedef struct {
    Color color;
    Vector3 hit_point;
    double hit_time;
} Tile;

typedef struct {
    Tile* items;
    size_t count;
    size_t capacity;
} Tiles;

Vector2 TILE_D[4] = {
    // dx,dy
    {-TILE_CLAMP_RESOLUTION, TILE_CLAMP_RESOLUTION},
    {TILE_CLAMP_RESOLUTION, TILE_CLAMP_RESOLUTION},
    {TILE_CLAMP_RESOLUTION, -TILE_CLAMP_RESOLUTION},
    {-TILE_CLAMP_RESOLUTION, -TILE_CLAMP_RESOLUTION}
};

void print_Vector2(Vector2 v) {
    printf("{x = %f, y = %f\n", v.x, v.y);
}

void print_Vector3(Vector3 v) {
    printf("{x = %f, y = %f, z = %f}\n", v.x, v.y, v.z);
}

void print_Quaternion(Quaternion q) {
    printf("{w = %f, x = %f, y = %f, z = %f}\n", q.w, q.x, q.y, q.z);
}

Vector2 local_to_image(Vector3 v) {
    return (Vector2){
        .x = FOCAL_LENGTH * v.x / v.z,
        .y = FOCAL_LENGTH * v.y / v.z
    };
}

Vector2 image_to_screen(Vector2 v) {
    return (Vector2){
        .x = (v.x + WIDTH_PRINCIPAL) * WIDTH / (WIDTH_PRINCIPAL * 2.f),
        .y = (v.y - HEIGHT_PRINCIPAL) * (-HEIGHT) / (HEIGHT_PRINCIPAL * 2.f)
    };
}

Vector3 local_vector_to_global(Vector3 pt, Quaternion ego_r) {
    return Vector3RotateByQuaternion(pt, ego_r);
}

Vector3 global_to_local(Vector3 pt, Vector3 ego_p, Quaternion ego_r) {
    Quaternion qi = QuaternionInvert(ego_r);
    Vector3 pt_R = Vector3RotateByQuaternion(pt, qi);
    Vector3 pt_T = Vector3Subtract(pt_R, Vector3RotateByQuaternion(ego_p, qi));
    return pt_T;
}

double scale_local_to_screen(double r, Vector3 pt_global, Vector3 ego_p, Quaternion ego_r) {
    Vector3 pt = global_to_local(pt_global, ego_p, ego_r);
    double r_local = r / pt.z;
    double r_screen = r_local * WIDTH;
    return r_screen;
}

Vector2 global_to_screen(Vector3 v, Vector3 ego_p, Quaternion ego_r) {
    Vector3 local = global_to_local(v, ego_p, ego_r);
    Vector2 image = local_to_image(local);
    Vector2 screen = image_to_screen(image);
    return screen;
}

void register_movement(Vector3 *ego_p, Quaternion *ego_r) {
    double yaw = 0;
    double pitch = 0;
    double roll = 0;
    if (IsKeyDown(KEY_LEFT)) {
        pitch -= DTHETA * 0.5f;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        pitch += DTHETA * 0.5f;
    }
    if (IsKeyDown(KEY_UP)) {
        roll -= DTHETA * 0.5f;
    }
    if (IsKeyDown(KEY_DOWN)) {
        roll += DTHETA * 0.5f;
    }
    if (IsKeyDown(KEY_Q)) {
        yaw += DTHETA * 0.5f;
    }
    if (IsKeyDown(KEY_E)) {
        yaw -= DTHETA * 0.5f;
    }
    Quaternion rot_pitch = { .w = cosf(pitch), .x = 0.f, .y = sinf(pitch), .z = 0.f };
    Quaternion rot_roll = { .w = cosf(roll), .x = sinf(roll), .y = 0.f, .z = 0.f };
    Quaternion rot_yaw = { .w = cosf(yaw), .x = 0.f, .y = 0.f, .z = sinf(yaw) };
    *ego_r = QuaternionMultiply(*ego_r, rot_pitch);
    *ego_r = QuaternionMultiply(*ego_r, rot_roll);
    *ego_r = QuaternionMultiply(*ego_r, rot_yaw);

    Vector3 pt = {0};
    if (IsKeyDown(KEY_W)) {
        pt.z += DP;
    }
    if (IsKeyDown(KEY_S)) {
        pt.z -= DP;
    }
    if (IsKeyDown(KEY_A)) {
        pt.x -= DP;
    }
    if (IsKeyDown(KEY_D)) {
        pt.x += DP;
    }
    if (IsKeyDown(KEY_Z)) {
        pt.y -= DP;
    }
    if (IsKeyDown(KEY_X)) {
        pt.y += DP;
    }

    Vector3 move = local_vector_to_global(pt, *ego_r);
    *ego_p = Vector3Add(*ego_p, move);
}

Vector3 get_random_vector() {
    return (Vector3){
        // [0,1] => [-1,1]
        .x = drand48() * 2.f - 1.f,
        .y = drand48() * 2.f - 1.f,
        .z = drand48() * 2.f - 1.f
    };
}

Ball create_random_ball(void) {
    Ball b = {
        .p = get_random_vector(),
        .v = Vector3Scale(get_random_vector(), BALL_INIT_V_SCALE),
        .a = {0}
    };
    return b;
}

double clamp(double value, double resolution) {
    assert(resolution > 0.f);
    long m_value = value / resolution;
    if (m_value < 0) m_value--;
    return (double)m_value * resolution;
}

double clamp_to_one(double value) {
    return value < 0 ? -1.f : 1.f;
}

bool update_ball(Ball *b, Tile* tile) {
    bool did_bounce = false;
    b->p = Vector3Add(b->p, Vector3Scale(b->v, DT));
    b->v = Vector3Add(b->v, Vector3Scale(b->a, DT));
    if (fabsf(b->p.x) + BALL_R >= BOX_BOUND) {
        b->v.x *= -1.f;
        tile->hit_point = (Vector3){
            .x = clamp_to_one(b->p.x),
            .y = clamp(b->p.y, TILE_CLAMP_RESOLUTION),
            .z = clamp(b->p.z, TILE_CLAMP_RESOLUTION)
        };
        did_bounce = true;
    }
    if (fabsf(b->p.y) + BALL_R >= BOX_BOUND) {
        b->v.y *= -1.f;
        tile->hit_point = (Vector3){
            .x = clamp(b->p.x, TILE_CLAMP_RESOLUTION),
            .y = clamp_to_one(b->p.y),
            .z = clamp(b->p.z, TILE_CLAMP_RESOLUTION)
        };
        did_bounce = true;
    }
    if (fabsf(b->p.z) + BALL_R >= BOX_BOUND) {
        b->v.z *= -1.f;
        tile->hit_point = (Vector3){
            .x = clamp(b->p.x, TILE_CLAMP_RESOLUTION),
            .y = clamp(b->p.y, TILE_CLAMP_RESOLUTION),
            .z = clamp_to_one(b->p.z)
        };
        did_bounce = true;
    }
    return did_bounce;
}

Vector3 add_to_tile(Vector3 v, size_t idx) {
    if (fabs(fabs(v.x) - BOX_BOUND) < EPSILON) {
        return (Vector3){ .x = v.x, .y = v.y + TILE_D[idx].x, .z = v.z + TILE_D[idx].y };
    } else if (fabs(fabs(v.y) - BOX_BOUND) < EPSILON) {
        return (Vector3){ .x = v.x + TILE_D[idx].x, .y = v.y, .z = v.z + TILE_D[idx].y};
    } else if (fabs(fabs(v.z) - BOX_BOUND) < EPSILON) {
        return (Vector3){ .x = v.x + TILE_D[idx].x, .y = v.y + TILE_D[idx].y, .z = v.z };
    } else {
        UNREACHABLE("add_to_tile");
        return (Vector3){0};
    }
}

double cross_direction(Vector2 vw0, Vector2 v1, Vector2 w1) {
    Vector2 a = Vector2Subtract(v1, vw0);
    Vector2 b = Vector2Subtract(w1, vw0);
    return a.x * b.y - a.y * b.x;
}

double lerp(double i_min, double i_max, double o_min, double o_max, double t) {
    double p = (t - i_min) / (i_max - i_min);
    return p * (o_max - o_min) + o_min;
}

void clean_tiles(Tiles* tiles, double t) {
    size_t i = tiles->count;
    if (i == 0) return;
    do {
        --i;
        if (t - tiles->items[i].hit_time > TILE_FADE_TIME + 1.f) {
            da_remove(tiles, i);
        }
    } while ( i != 0);
}

int main(void)
{
    srand(time(NULL));
    srand48(time(NULL));
    InitWindow(WIDTH, HEIGHT, "Raylib Template");
    SetTargetFPS(60);
    Points points = {0};
    Vector3 v = { .x = 1.f, .y = 1.f, .z = 1.f };
    for (size_t i = 0; i < 8; ++i) {
        if (i % 4 == 0)
            v.z *= -1;
        if (i % 2 == 0)
            v.y *= -1;
        else
            v.x *= -1;
        da_append(&points, v);
    }
    size_t lines[6][4] = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}
    };
    Ball ball = create_random_ball();

    Vector3 ego_p = { .x = EGO_INIT_X, .y = EGO_INIT_Y, .z = EGO_INIT_Z };
    Quaternion rot_x = {
        .w = cosf(EGO_INIT_ROLL*0.5f),
        .x = sinf(EGO_INIT_ROLL*0.5f),
        .y = 0.f,
        .z = 0.f
    };
    Quaternion rot_z = {
        .w = cosf(EGO_INIT_YAW*0.5f),
        .x = 0.f,
        .y = 0.f,
        .z = sinf(EGO_INIT_YAW*0.5f)
    };
    // right multiply since in local crs
    Quaternion ego_r = QuaternionMultiply(rot_z, rot_x);

    Tiles tiles = {0};
    double t = 0.f;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        register_movement(&ego_p, &ego_r);

        Tile tile = { .hit_time = t, .color = COLORS[(size_t)rand() % C_N] };
        bool did_bounce = update_ball(&ball, &tile);
        if (did_bounce) {
            da_append(&tiles, tile);
        }
        clean_tiles(&tiles, t);

        da_foreach(&tiles, Tile, tt) {
            if (t - tt->hit_time > TILE_FADE_TIME) continue;
            Vector2 pts[4] = {0};
            for (size_t i = 0; i < 4; ++i) {
                Vector3 p = add_to_tile(tt->hit_point, i);
                pts[i] = global_to_screen(p, ego_p, ego_r);
            }
            tt->color.a = lerp(0.f, TILE_FADE_TIME, 255, 0, t - tt->hit_time);
            // direction is reversed since y is 0 on the top and increases down
            if (cross_direction(pts[0], pts[1], pts[3]) < 0.f) {
                DrawTriangle(pts[0], pts[1], pts[3], tt->color);
            } else {
                DrawTriangle(pts[0], pts[3], pts[1], tt->color);
            }
            if (cross_direction(pts[2], pts[1], pts[3]) < 0.f) {
                DrawTriangle(pts[2], pts[1], pts[3], tt->color);
            } else {
                DrawTriangle(pts[2], pts[3], pts[1], tt->color);
            }
        }

        Vector2 ball_screen = global_to_screen(ball.p, ego_p, ego_r);
        double ball_r = scale_local_to_screen(BALL_R, ball.p, ego_p, ego_r);
        DrawCircleV(ball_screen, ball_r, WHITE);
        for (size_t i = 0; i < 6; ++i) {
            size_t sz = i < 2 ? 4 : 2;
            for (size_t j = 0; j < sz; ++j) {
                size_t idx1 = lines[i][j];
                size_t idx2 = lines[i][(j + 1) % sz];
                Vector2 p1 = global_to_screen(points.items[idx1], ego_p, ego_r);
                Vector2 p2 = global_to_screen(points.items[idx2], ego_p, ego_r);
                DrawLineV(p1, p2, BLUE);
            }
        }
        EndDrawing();
        t += DT;
    }
    CloseWindow();

    return 0;
}
