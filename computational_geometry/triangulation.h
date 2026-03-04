#include "helpers.h"

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
    bool active;
    tVertex next, prev;
};
tVertex vertices = NULL;

typedef void (*fPrintDiagonal)(tVertex v1, tVertex v2);

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
        return (a[X] <= c[X] && c[X] <= b[X]) ||
               (a[X] >= c[X] && c[X] >= b[X]);
    else
        return (a[Y] <= c[Y] && c[Y] <= b[Y]) ||
               (a[Y] >= c[Y] && c[Y] >= b[Y]);
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

bool Diagonalie(tVertex a, tVertex b) {
    tVertex c, c1;
    c = vertices;
    do {
        c1 = c->next;
        if (a != c && a != c1 && b != c && b != c1 &&
            Intersect(a->v, b->v, c->v, c1->v)) {
            return false;
        }
        c = c->next;
    } while (c != vertices);
    return true;
}

bool InCone(tVertex a, tVertex b) {
    tVertex a0, a1;
    a0 = a->prev;
    a1 = a->next;

    /*
    // a is convex vertex
    if (LeftOn(a->v, a1->v, a0->v)) {
        return Left(a->v, b->v, a0->v)
            && Left(b->v, a->v, a1->v);
    }

    // a is reflex
    return !(  LeftOn(a->v, b->v, a1->v)
            && LeftOn(b->v, a->v, a0->v));
    */
    // Improved version
    // at most one of the left calls is false
    int result = 0;
    result += (int)Left(a->v, a1->v, b->v);
    result += (int)Left(a->v, b->v, a0->v);
    result += (int)Left(a->v, a0->v, a1->v);
    return result >= 2;
}

bool Diagonal(tVertex a, tVertex b) {
    // InCone has constant time complexity while Diagonal has n, so to short circuit unnecessary calls
    // they are called first
    // Improvement: one InCone is unnecessary
    return /*InCone(a, b) &&*/ InCone(b, a) && Diagonalie(a, b);
}

void EarInit(void) {
    tVertex v0, v1, v2;
    v1 = vertices;
    do {
        v1 = v1->next;
        v0 = v1->prev;
        v2 = v1->next;
        v1->ear = Diagonal(v0, v2);
        v1->active = true;
    } while (v1 != vertices);
}

tVertex find_next(tVertex v) {
    tVertex p = v;
    if (p->active) p = p->next;
    while (!p->active)
        p = p->next;
    return p;
}

tVertex find_prev(tVertex v) {
    tVertex p = v;
    if (p->active) p = p->prev;
    while (!p->active)
        p = p->prev;
    return p;
}

void Triangulate(fPrintDiagonal print_diagonal_ptr) {
    tVertex v0, v1, v2, v3, v4;
    int n = 0;
    v2 = vertices;
    do {
        ++n;
        v2 = v2->next;
    } while (v2 != vertices);
    while (n > 3) {
        v2 = vertices;
        do {
            assert(v2->active);
            if (v2->ear) {
                v3 = find_next(v2); v4 = find_next(v3);
                v1 = find_prev(v2); v0 = find_prev(v1);
                assert(v0->active);
                assert(v1->active);
                assert(v3->active);
                assert(v4->active);

                (*print_diagonal_ptr)(v1, v3);

                v1->ear = Diagonal(v0, v3);
                v3->ear = Diagonal(v1, v4);

                v2->active = false;
                vertices = v1;;

                --n;
                break;
            }
            v2 = find_next(v2);
        } while (v2 != vertices);
    }
    printf("DONE\n");
}
