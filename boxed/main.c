#include "helpers.h"

#define W 1600
#define H 1200

#define R 40
#define DP 0.05 // unitary position
#define DTHETA PI/200 // unitary rotation delta

typedef struct {
    Vector3* items;
    size_t count;
    size_t capacity;
} Points;

typedef struct {
    Vector2 point;
    double s;
} ScreenPoint;


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

Vector2 project(Vector3 v) {
    return (Vector2){ .x = v.x / v.z, .y = v.y / v.z};
}

Vector2 screen(Vector2 v) {
    return (Vector2){
        .x = (v.x + 1) * W / 2,
        .y = (v.y - 0.75) * (-H) / 1.5
    };
}

double scale(double r, Vector3 pt) {
    return r / pt.z;
}

Vector3 local_vector_to_global(Vector3 pt, Quaternion ego_r) {
    Quaternion p = { .w = 0, .x = pt.x, .y = pt.y, .z = pt.z };
    Quaternion qi = QuaternionInvert(ego_r);
    Quaternion res = QuaternionMultiply3(qi, p, ego_r);
    return (Vector3){ .x = res.x, .y = res.y, .z = res.z };
}


Vector3 global_to_local(Vector3 *pt, Vector3 ego_p, Quaternion ego_r) {

    Vector3 pt_R = Vector3RotateByQuaternion(*pt, ego_r);
    Vector3 pt_T = Vector3Subtract(pt_R, Vector3RotateByQuaternion(ego_p, ego_r));
    return pt_T;
}

void test() {
    print_Vector3(QuaternionToEuler(QuaternionFromEuler(PI/2, 0, 0)));
    print_Vector3(QuaternionToEuler(QuaternionFromEuler(0, PI/2, 0)));
    print_Vector3(QuaternionToEuler(QuaternionFromEuler(PI/2, 0, PI/2)));
}

void rotate_ego(Quaternion *ego_r, double dz, double dy, double dx) {
    Quaternion q = *ego_r;
    Quaternion qi = QuaternionInvert(q);
    Quaternion p = QuaternionFromEuler(dy, dz, dx);
    Quaternion qp = QuaternionMultiply(q, p);
    Quaternion qpqi = QuaternionMultiply(qp, qi);
    print_Vector3(QuaternionToEuler(qpqi));
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

ScreenPoint global_to_screen(Vector3 *v, Vector3 ego_p, Quaternion ego_r) {
    Vector3 pt_l = global_to_local(v, ego_p, ego_r);
    Vector2 local = project(pt_l);
    return (ScreenPoint){
        .point = screen(local),
        .s = scale(R, pt_l)
    };
}

void register_movement(Vector3 *ego_p, Quaternion *ego_r) {
    double yaw = 0;
    double pitch = 0;
    double roll = 0;
    if (IsKeyDown(KEY_LEFT)) {
        pitch += DTHETA;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        pitch -= DTHETA;
    }
    if (IsKeyDown(KEY_UP)) {
        roll += DTHETA;
    }
    if (IsKeyDown(KEY_DOWN)) {
        roll -= DTHETA;
    }
    if (IsKeyDown(KEY_Q)) {
        yaw += DTHETA;
    }
    if (IsKeyDown(KEY_E)) {
        yaw -= DTHETA;
    }

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
    Quaternion rot = construct_quaternion_from_angles(yaw, pitch, roll);
    Vector3 move = local_vector_to_global(pt, *ego_r);
    *ego_r = QuaternionMultiply(rot, *ego_r);
    *ego_p = Vector3Add(*ego_p, move);
}

int main(void)
{
    InitWindow(W, H, "Raylib Template");
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

    Vector3 ego_p = { .x = -4.f, .y = 0.f, .z = 0.f };
    Quaternion ego_r = construct_quaternion_from_angles(PI/2, 0, PI/2);

    double t = 0.f;
    double dt = 1e-3;
    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        }
        register_movement(&ego_p, &ego_r);

        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        for (size_t i = 0; i < 6; ++i) {
            size_t sz = i < 2 ? 4 : 2;
            for (size_t j = 0; j < sz; ++j) {
                size_t idx1 = lines[i][j];
                size_t idx2 = lines[i][(j + 1) % sz];
                ScreenPoint p1 = global_to_screen(&points.items[idx1], ego_p, ego_r);
                ScreenPoint p2 = global_to_screen(&points.items[idx2], ego_p, ego_r);
                DrawLineV(p1.point, p2.point, BLUE);
            }
        }
        EndDrawing();
        t += dt;
    }
    CloseWindow();

    return 0;
}
