#include "helpers.h"

// reprojection
#define W 1600
#define H 1200
#define DT 1e-3
#define BB_X 20.f
#define BB_Y (((float)H/W)*BB_X)

// data types
#define X 0
#define Y 1
#define DIM 2
typedef int tPointi[DIM];

/** BEGIN: Visualization and logging helpers **/
void print_tPointi(tPointi pt) {
    printf("{ .x = %d, .y = %d }\n", pt[X], pt[Y]);
}

Vector2 to_screen(tPointi pt) {
    assert(-BB_X <= pt[X] && pt[X] <= BB_X);
    assert(-BB_Y <= pt[Y] && pt[Y] <= BB_Y);
    Vector2 spt = {0};
    // [-BB_X,BB_X] -> [0,W]
    spt.x = ((float)pt[X] + BB_X) / (2.f * BB_X) * W;
    // [-BB_Y,BB_Y] -> [0,H]
    spt.y = ((float)pt[Y] + BB_Y) / (2.f * BB_Y) * H;
    return spt;
}

void from_screen(Vector2 pt, tPointi spt) {
    assert(0 <= pt.x && pt.x <= W);
    assert(0 <= pt.y && pt.y <= H);
    spt[X] = pt.x / W * 2.f * BB_X - BB_X;
    spt[Y] = pt.y / H * 2.f * BB_Y - BB_Y;
}
/** END: Visualization and logging helpers **/

// points must be counter clockwise
double polygon_area(Vector2 *points, size_t n) {
    double A = 0.f;
    for (size_t i = 0; i + 1 < n; ++i) {
        A += (points[i].x - points[i+1].x) * (points[i+1].y - points[i].y);
    }
    return A * 0.5f;
}

int main(void)
{
    srand(time(NULL));
    InitWindow(W, H, "Raylib Template");
    SetTargetFPS(60);

    tPointi pt = { 0, 0 };
    tPointi pt2;
    print_Vector2(to_screen(pt));
    from_screen(to_screen(pt), pt2);
    print_tPointi(pt2);

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
