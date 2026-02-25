#include "helpers.h"

#define W 1600
#define H 1200
#define DT 1e-3
#define BB_X 2.f
#define BB_Y (((float)H/W)*BB_X)

Vector2 to_screen(Vector2 pt) {
    assert(-BB_X <= pt.x && pt.x <= BB_X);
    assert(-BB_Y <= pt.y && pt.y <= BB_Y);
    Vector2 spt = {0};
    // [-BB_X,BB_X] -> [0,W]
    spt.x = (pt.x + BB_X) / (2.f * BB_X) * W;
    // [-BB_Y,BB_Y] -> [0,H]
    spt.y = (pt.y + BB_Y) / (2.f * BB_Y) * H;
    return spt;
}

Vector2 from_screen(Vector2 pt) {
    assert(0 <= pt.x && pt.x <= W);
    assert(0 <= pt.y && pt.y <= H);
    Vector2 spt = {0};
    spt.x = pt.x / W * 2.f * BB_X - BB_X;
    spt.y = pt.y / H * 2.f * BB_Y - BB_Y;
    return spt;
}

int main(void)
{
    srand(time(NULL));
    InitWindow(W, H, "Raylib Template");
    SetTargetFPS(60);

    float t = 0.f;
    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        }
        if (IsKeyPressed(KEY_SPACE)) {
        }
        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        EndDrawing();
        t += DT;
    }
    CloseWindow();

    return 0;
}
