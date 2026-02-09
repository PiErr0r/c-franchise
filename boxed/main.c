#include <time.h>

#include "helpers.h"

#define WIDTH 1600
#define WIDTH_PRINCIPAL 1.f
#define HEIGHT 1200
#define HEIGHT_PRINCIPAL 0.75f
#define FOCAL_LENGTH 1.f

#define DP 0.05 // unitary position
#define DTHETA PI/200 // unitary rotation delta

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

void print_Vector2(Vector2 v) {
    printf("{x = %f, y = %f\n", v.x, v.y);
}

void print_Vector3(Vector3 v) {
    printf("{x = %f, y = %f, z = %f}\n", v.x, v.y, v.z);
}

void print_Quaternion(Quaternion q) {
    printf("{w = %f, x = %f, y = %f, z = %f}\n", q.w, q.x, q.y, q.z);
}

Quaternion QuaternionMultiply3(Quaternion q1, Quaternion q2, Quaternion q3) {
    return QuaternionMultiply(QuaternionMultiply(q1, q2), q3);
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
    return r / pt.z;
}

void rotate_ego(Quaternion *ego_r, double dz, double dy, double dx) {
    Quaternion q = *ego_r;
    Quaternion qi = QuaternionInvert(q);
    Quaternion p = QuaternionFromEuler(dy, dz, dx);
    Quaternion qp = QuaternionMultiply(q, p);
    Quaternion qpqi = QuaternionMultiply(qp, qi);
    *ego_r = QuaternionAdd(*ego_r, qpqi);
}

Quaternion transform_by_quaternions(Quaternion p, Quaternion q1, Quaternion q2, Quaternion q3) {
    Quaternion q1i = QuaternionInvert(q1);
    Quaternion q2i = QuaternionInvert(q2);
    Quaternion q3i = QuaternionInvert(q3);
    Quaternion Qp = QuaternionMultiply(QuaternionMultiply(q3, q2), q1);
    Quaternion Qpi = QuaternionMultiply(QuaternionMultiply(q1i, q2i), q3i);
    return QuaternionMultiply(QuaternionMultiply(Qp, p), Qpi);
}

Quaternion construct_quaternion_from_angles(double yaw, double pitch, double roll) {
    double cz = cosf(yaw/2.f);
    double sz = sinf(yaw/2.f);
    double cy = cosf(pitch/2.f);
    double sy = sinf(pitch/2.f);
    double cx = cosf(roll/2.f);
    double sx = sinf(roll/2.f);
    Quaternion qz = { .w = cz, .x = 0, .y = 0, .z = sz };
    Quaternion qy = { .w = cy, .x = 0, .y = sy, .z = 0 };
    Quaternion qx = { .w = cx, .x = sx, .y = 0, .z = 0 };
    return QuaternionMultiply3(qx, qy, qz);
}

Vector2 global_to_screen(Vector3 v, Vector3 ego_p, Quaternion ego_r) {
    Vector3 local = global_to_local(v, ego_p, ego_r);
    Vector2 image = local_to_image(local);
    Vector2 screen = image_to_screen(image);
    printf("!!\n");
    print_Vector3(v);
    print_Vector3(local);
    print_Vector2(image);
    print_Vector2(screen);
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

    Vector3 move = local_vector_to_global(pt, *ego_r);
    *ego_p = Vector3Add(*ego_p, move);
}

Vector3 get_random_vector() {
    return (Vector3){
        .x = drand48() * 2.f - 1.f,
        .y = drand48() * 2.f - 1.f,
        .z = drand48() * 2.f - 1.f
    };
}

Ball create_random_ball(void) {
    Ball b = {
        .p = get_random_vector(),
        .v = Vector3Scale(get_random_vector(), 10.f),
        .a = {0}
    };
    return b;
}

void update_ball(Ball *b, double dt) {
    b->p = Vector3Add(b->p, Vector3Scale(b->v, dt));
    b->v = Vector3Add(b->v, Vector3Scale(b->a, dt));
    if (b->p.x <= -1 || b->p.x >= 1) {
        b->v.x *= -1.f;
    }
    if (b->p.y <= -1 || b->p.y >= 1) {
        b->v.y *= -1.f;
    }
    if (b->p.z <= -1 || b->p.z >= 1) {
        b->v.z *= -1.f;
    }
}

int main(void)
{
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

    Vector3 ego_p = { .x = -5.f, .y = 0.f, .z = 0.f };
    Quaternion a = { .w = cosf(PI*0.25f), .x = sinf(PI*0.25f), .y = 0.f, .z = 0.f };
    Quaternion b = { .w = cosf(PI*0.25f), .x = 0.f, .y = 0.f, .z = sinf(PI*0.25f) };
    Quaternion ego_r = QuaternionMultiply(b, a);

    double t = 0.f;
    double dt = 1e-3;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        register_movement(&ego_p, &ego_r);

        update_ball(&ball, dt);
        Vector2 ball_screen = global_to_screen(ball.p, ego_p, ego_r);
        double ball_r = scale_local_to_screen(0.05f, ball.p, ego_p, ego_r);
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
        Vector3 V = {0,0,1};
        Vector2 pt = global_to_screen(V, ego_p, ego_r);
        DrawCircleV(pt, scale_local_to_screen(0.03f, V, ego_p, ego_r), RED);
        EndDrawing();
        t += dt;
    }
    CloseWindow();

    return 0;
}
