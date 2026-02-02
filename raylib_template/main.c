#include "helpers.h"

#define W 1600
#define H 1200

int main(void)
{
    InitWindow(W, H, "Raylib Template");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        }
        if (IsKeyPressed(KEY_SPACE)) {
        }
        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
