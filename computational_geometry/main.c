#include "triangulation.h"

// reprojection
#define W 1600
#define H 1200
#define DT 1e-3
#define BB_X 20.f
#define BB_Y (((float)H/W)*BB_X)

typedef struct {
    tVertex a;
    tVertex b;
} PDiag;

typedef struct {
    PDiag* items;
    size_t count;
    size_t capacity;
} PDiags;

PDiags pdiags = {0};

/** BEGIN: Visualization and logging helpers **/
void print_tPointi(tPointi pt) {
    printf("{ .x = %d, .y = %d }\n", pt[X], pt[Y]);
}

void print_diagonal(tVertex v1, tVertex v2) {
    printf("### Diagonal ###\n");
    PDiag pdiag = { .a = v1, .b = v2};
    da_append(&pdiags, pdiag);
    print_tPointi(v1->v);
    print_tPointi(v2->v);
}

Vector2 to_screen(tPointi pt) {
    assert(-BB_X <= pt[X] && pt[X] <= BB_X);
    assert(-BB_Y <= pt[Y] && pt[Y] <= BB_Y);
    Vector2 spt = {0};
    // [-BB_X,BB_X] -> [0,W]
    spt.x = ((float)pt[X] + BB_X) * W / (2.f * BB_X);
    // [-BB_Y,BB_Y] -> [H, 0]
    spt.y = (BB_Y - (float)pt[Y]) * H / (2.f * BB_Y);
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


void make_polygon() {
    tVertex pt;
    NEW(pt, tsVertex); pt->v[X] =  -1; pt->v[Y] = -8;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] =   8; pt->v[Y] = -2;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] =  12; pt->v[Y] = -5;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] =  15; pt->v[Y] =  0;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] =   8; pt->v[Y] = 10;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] =   2; pt->v[Y] =  0;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] =  -1; pt->v[Y] =  4;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] =   6; pt->v[Y] =  8;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] =  -9; pt->v[Y] = 11;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] = -13; pt->v[Y] =  9;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] = -11; pt->v[Y] =  2;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] =  -3; pt->v[Y] =  0;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] = -14; pt->v[Y] = -1;
    ADD(vertices, pt);
    NEW(pt, tsVertex); pt->v[X] =  -1; pt->v[Y] = -3;
    ADD(vertices, pt);
}

int main(void)
{
    srand(time(NULL));
    InitWindow(W, H, "Raylib Template");
    SetTargetFPS(60);

    make_polygon();
    vertices = vertices->next->next->next->next->next->next->next;
    EarInit();
    Triangulate(print_diagonal);

    float t = 0.f;
    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        }
        if (IsKeyPressed(KEY_SPACE)) {
        }
        BeginDrawing();
        tVertex v = vertices;
        int i = 0;
        do {
            ++i;
            Vector2 p = to_screen(v->v);
            // DrawCircleV(p, 15, RED);
            Vector2 prev = to_screen(v->prev->v);
            DrawLineV(prev, p, GREEN);
            v = v->next;
        } while (v != vertices);
        da_foreach(&pdiags, PDiag, pd) {
            Vector2 p1 = to_screen(pd->a->v);
            Vector2 p2 = to_screen(pd->b->v);
            DrawLineV(p1, p2, BLUE);
        }
        ClearBackground(GetColor(0x181818FF));
        EndDrawing();
        t += DT;
    }
    CloseWindow();

    return 0;
}
