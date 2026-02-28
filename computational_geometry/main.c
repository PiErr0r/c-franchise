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

typedef struct tVertexStructure tsVertex;
typedef tsVertex* tVertex;
struct tVertexStructure {
    int vnum;
    tPointi v;
    bool ear;
    tVertex next, prev;
};
tVertex vertices = NULL;

#define EXIT_FAILURE 1

#define NEW(p, type)\
    if ((p = (type *)malloc(sizeof(type))) == NULL) {\
        printf("NEW: Out of memory");\
        exit(EXIT_FAILURE);\
    }

#define ADD(head, p)\
    if (head) {\
        p->next = head;\
        p->prev = head->prev;\
        head->prev->next = p;\
        head->prev = p;\
    } else {\
        head = p;\
        head->next = head->prev = p;\
    }

#define FREE(p) if (p) { free((char*)p); p = NULL; }

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

int Area2(tPointi a, tPointi b, tPointi c) {
    return (b[X]-a[X])*(c[Y]-a[Y]) -
           (b[Y]-a[Y])*(c[X]-a[X]);
}

int AreaPoly2(void) {
    int sum = 0;
    tVertex p, a;
    p = vertices;
    a = p->next;
    do {
        sum += Area2(p->v, a->v, a->next->v);
        a = a->next;
    } while (a->next != vertices);
    return sum;
}

bool Left(tPointi a, tPointi b, tPointi c) {
    return Area2(a, b, c) > 0;
}

bool LeftOn(tPointi a, tPointi b, tPointi c) {
    return Area2(a, b, c) >= 0;
}

bool Collinear(tPointi a, tPointi b, tPointi c) {
    return Area2(a, b, c) == 0;
}

bool Xor(bool a, bool b) {
    // negate values to ensure 0/1
    return !a ^ !b;
}

bool Between(tPointi a, tPointi b, tPointi c) {
    if (!Collinear(a, b, c)) {
        return false;
    }
    if (a[X] != b[X])
        return a[X] <= c[X] && c[X] <= b[X] ||
               a[X] >= c[X] && c[X] >= b[X];
    else
        return a[Y] <= c[Y] && c[Y] <= b[Y] ||
               a[Y] >= c[Y] && c[Y] >= b[Y];
}

bool IntersectProp(tPointi a, tPointi b, tPointi c, tPointi d) {
    if (Collinear(a, b, c) ||
        Collinear(a, b, d) ||
        Collinear(c, d, a) ||
        Collinear(c, d, b)) {
        return false;
    }
    return Xor(Left(a, b, c), Left(a, b, d)) &&
           Xor(Left(c, d, a), Left(c, d, b));
}

bool Intersect(tPointi a, tPointi b, tPointi c, tPointi d) {
    if        (   IntersectProp(a, b, c, d)) {
        return true;
    } else if (   Between(a, b, c)
               || Between(a, b, d)
               || Between(c, d, a)
               || Between(c, d, b)
              ) {
        return true;
    }
    return false;
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
