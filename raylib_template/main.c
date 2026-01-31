#include <math.h>
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

#define LEN 266
#define F 1000.0

int main(void)
{
    const int W = 800;
    const int H = 600;
    InitWindow(W, H, "Raylib Template");
    SetTargetFPS(60);

    Vector2 points[LEN];
    const double step = 40.0;
    for (size_t i = 0; i < (size_t)(W / step - 1); ++i) {
        for (size_t j = 0; j < (size_t)(H / step - 1); ++j) {
            Vector2 v;
            v.x = (i + 1) * step;
            v.y = (j + 1) * step;
            size_t idx = i * (H / step - 1) + j;
            points[idx] = v;
        }
    }

    int is_pressed = 0;
    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            is_pressed = -1;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            is_pressed = 1;
        }
        if (IsKeyPressed(KEY_SPACE)) {
            is_pressed = 0;
        }
        if (is_pressed) {
            Vector2 pos = GetMousePosition();
            for (size_t i = 0; i < LEN; ++i) {
                double dx = pos.x - points[i].x;
                double sx = dx > 0 ? 1.0 : -1.0;
                double dy = pos.y - points[i].y;
                double sy = dy > 0 ? 1.0 : -1.0;
                double dd = dx * dx + dy * dy;
                if (fabs(dd) > 200) {
                    points[i].x += is_pressed * sx * F / dd;
                    points[i].y += is_pressed * sy * F / dd;
                }
            }
        }
        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        for (size_t pti = 0; pti < LEN; ++pti) {
            DrawCircleV(points[pti], 5, RED);
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
