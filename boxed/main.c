#include "helpers.h"

#define W 1600
#define H 1200

typedef struct {
    Vector3* items;
    size_t count;
    size_t capacity;
} Points;

Vector2 project(Vector3 *v) {
    return (Vector2){ .x = v->x / v->z, .y = v->y / v->z};
}

Vector2 screen(Vector2 v) {
    return (Vector2){
        .x = (v.x + 1) * W / 2,
        .y = (v.y - 0.75) * (-H) / 1.5
    };
}

double scale(double r, Vector3 *pt) {
    return r / pt->z;
}

int main(void)
{
    InitWindow(W, H, "Raylib Template");
    SetTargetFPS(60);
    Points points = {0};
    Vector3 v = { .x = .5, .y = .5, .z = 0.25 };
    double dz = 0.25;
    for (size_t i = 0; i < 8; ++i) {
        v.z += dz;
        if (i % 2 == 0)
            v.x *= -1;
        else
            v.y *= -1;
        da_append(&points, v);
    }
                
    double ddz = 0.025;
    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        }
        if (IsKeyDown(KEY_SPACE)) {
        }
        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        da_foreach(&points, Vector3, pt) {
            Vector2 local = project(pt);
            Vector2 s = screen(local);
            const double R = 15;
            DrawCircleV(s, scale(R, pt), RED);
            pt->z += ddz;
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
