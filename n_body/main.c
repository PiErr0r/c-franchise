#include <math.h>
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UNUSED(x) (void)(x)

#define MIN_DISTANCE 50
#define MIN_R 5
#define MAX_R 20
#define G 500

typedef struct {
    Vector2 a;
    Vector2 v;
    Vector2 s;
    int r;
} Point;

Color COLORS[15] = {
    WHITE,
    GRAY,
    BLUE,
    GREEN,
    YELLOW,
    ORANGE,
    RED,
    PURPLE,
    PINK,
    VIOLET,
    MAROON,
    LIME,
    BEIGE,
    BROWN,
    SKYBLUE
};

void check_bounds(Point *point) {
    if (point->s.x < 0.f) {
        point->v.x *= -.5f;
        point->s.x = 0.f;
    }
    if (point->s.x > 800.f) {
        point->v.x *= -.5f;
        point->s.x = 800.f;
    }
    if (point->s.y < 0.f) {
        point->v.y *= -.5f;
        point->s.y = 0.f;
    }
    if (point->s.y > 600.f) {
        point->v.y *= -.5f;
        point->s.y = 600.f;
    }
}
Vector2 distance(Vector2 a, Vector2 b) {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return (Vector2){ .x = dx, .y = dy };
}

void update_points(Point *points, const size_t N, const double dt) {
    for (size_t i = 0; i < N; ++i) {
        double ax = 0;
        double ay = 0;
        for (size_t j = 0; j < N; ++j) {
            if (j == i) continue;
            double m = (double)points[j].r;
            Vector2 d = distance(points[i].s, points[j].s);
            double r2 = d.x * d.x + d.y * d.y;
            int sign = sqrt(r2) > MIN_DISTANCE ? 1 : -7;
            r2 = fmaxf(r2, 0.1);
            ax += sign * m * G * d.x / r2;
            ay += sign * m * G * d.y / r2;
        }
        points[i].a.x = ax;
        points[i].a.y = ay;
        points[i].v.x += ax * dt;
        points[i].v.y += ay * dt;
        points[i].s.x += ax * dt * dt / 2.f + points[i].v.x * dt;
        points[i].s.y += ay * dt * dt / 2.f + points[i].v.y * dt;
        check_bounds(&points[i]);
    }
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    if (argc != 2) {
        printf("Expected one integer argument but got %d\n", argc);
        return 1;
    }
    const size_t N = atoi(argv[1]);
    const size_t W = 800;
    const size_t H = 600;
    InitWindow(W, H, "Raylib Template");
    SetTargetFPS(60);

    Point* points = malloc(sizeof(Point) * N);

    for (size_t i = 0; i < N; ++i) {
        const int x = rand() % W;
        const int y = rand() % H;
        points[i] = (Point){
            .a = {0},
            .v = {0},
            .s = { .x = x, .y = y },
            .r = MIN_R + (rand() % (MAX_R - MIN_R))
        };
    }

    const double dt = 16.66e-3;
    while (!WindowShouldClose()) {
        update_points(points, N, dt);
        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        for (size_t pti = 0; pti < N; pti += 1) {
            DrawCircle(points[pti].s.x, points[pti].s.y, points[pti].r, COLORS[points[pti].r - MIN_R]);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
