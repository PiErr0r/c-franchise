#include <assert.h>
#include <time.h>

#include "helpers.h"

#define WIDTH 1600
#define WIDTH_PRINCIPAL 1.f
#define HEIGHT 1200
#define HEIGHT_PRINCIPAL 0.75f
#define FOCAL_LENGTH 1.f

#define DP 0.05         // unitary position
#define DTHETA PI/200   // unitary rotation delta
#define DT 1e-3         // delta time
#define G 9.81f

#define BALLS_NUM 10
#define BALL_INIT_V_SCALE 40.f
#define BALL_A_SCALE 100.f
#define BALL_R 0.07f

#define BB_X_MAX  1.f
#define BB_X_MIN -1.f
#define BB_Y_MAX  1.f
#define BB_Y_MIN -1.f
#define BB_Z_MAX  2.f
#define BB_Z_MIN  0.f

#define EGO_INIT_X 4.5f
#define EGO_INIT_Y -3.f
#define EGO_INIT_Z 4.f
#define EGO_INIT_ROLL (PI * 0.6666f)
#define EGO_INIT_PITCH 0.f
#define EGO_INIT_YAW (PI * -0.6666f)

typedef struct {
    Vector3* items;
    size_t count;
    size_t capacity;
} Points;

typedef struct {
    double r;
    bool alive;
    Vector3 p;
    Vector3 v;
    Vector3 a;
    Color c;
} Ball;

typedef struct {
    Ball* items;
    size_t count;
    size_t capacity;
} Balls;

void print_Vector2(Vector2 v) {
    printf("{x = %f, y = %f\n", v.x, v.y);
}

void print_Vector3(Vector3 v) {
    printf("{x = %f, y = %f, z = %f}\n", v.x, v.y, v.z);
}

void print_Quaternion(Quaternion q) {
    printf("{w = %f, x = %f, y = %f, z = %f}\n", q.w, q.x, q.y, q.z);
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
}

Vector2 global_to_screen(Vector3 v, Vector3 ego_p, Quaternion ego_r) {
    Vector3 local = global_to_local(v, ego_p, ego_r);
    Vector2 image = local_to_image(local);
    Vector2 screen = image_to_screen(image);
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
    if (IsKeyDown(KEY_Z)) {
        pt.y -= DP;
    }
    if (IsKeyDown(KEY_X)) {
        pt.y += DP;
    }

    Vector3 move = local_vector_to_global(pt, *ego_r);
    *ego_p = Vector3Add(*ego_p, move);
}

Vector3 get_random_position() {
    return (Vector3){
        // [0,1] => [-1,1]
        .x = drand48() * 2.f - 1.f,
        .y = drand48() * 2.f - 1.f,
        // [0,1] => [0, 2]
        .z = drand48() * 2.f
    };
}

Vector3 get_random_velocity() {
    return (Vector3){
        // [0,1] => [-1,1]
        .x = drand48() * 2.f - 1.f,
        .y = drand48() * 2.f - 1.f,
        // [0,1] => [-2, 0]
        .z = drand48() * 2.f * -1
    };
}

Color get_random_color() {
    size_t idx = rand() % C_N;
    return COLORS[idx];
}

Ball create_random_ball(void) {
    Ball b = {
        .alive = true,
        .p = get_random_position(),
        .v = Vector3Scale(get_random_velocity(), BALL_INIT_V_SCALE),
        .a = {.z = -G},
        .r = BALL_R,
        .c = get_random_color()
    };
    return b;
}

double clamp(double value, double resolution) {
    assert(resolution > 0.f);
    long m_value = value / resolution;
    if (m_value < 0) m_value--;
    return (double)m_value * resolution;
}

double clamp_to_one(double value) {
    return value < 0 ? -1.f : 1.f;
}

void update_ball(Ball *b, Balls *balls) {
    if (b->p.z <= BB_Z_MAX && fabs(b->p.x) < BB_X_MAX && fabs(b->p.y) < BB_Y_MAX) {
        if (b->p.x - BALL_R <= BB_X_MIN || b->p.x + BALL_R >= BB_X_MAX) {
            b->v.x *= -1.f;
        }
        if (b->p.y - BALL_R <= BB_Y_MIN || b->p.y + BALL_R >= BB_Y_MAX) {
            b->v.y *= -1.f;
        }
        if (b->p.z - BALL_R <= BB_Z_MIN) {
            b->v.z *= -1.f;
        }
    } else {
        if (b->p.z - BALL_R <= BB_Z_MIN) {
            b->alive = false;
            return;
        }
    }
    da_foreach(balls, Ball, ball) {
        if (ball == b) continue;
        double d = Vector3Distance(ball->p, b->p);
        double min_d = ball->r + b->r;
        if (d <= min_d) {
            Vector3 dp = Vector3Subtract(ball->p, b->p);
            dp = Vector3Normalize(dp);
            double v = Vector3Length(b->v) + Vector3Length(ball->v);
            b->v = Vector3Scale(dp, -v * 0.5f);
            ball->v = Vector3Scale(dp, v * 0.5f);
        }
    }

    b->p = Vector3Add(b->p, Vector3Scale(b->v, DT));
    b->v = Vector3Add(b->v, Vector3Scale(b->a, BALL_A_SCALE * DT));
}

double cross_direction(Vector2 vw0, Vector2 v1, Vector2 w1) {
    Vector2 a = Vector2Subtract(v1, vw0);
    Vector2 b = Vector2Subtract(w1, vw0);
    return a.x * b.y - a.y * b.x;
}

double lerp(double i_min, double i_max, double o_min, double o_max, double t) {
    double p = (t - i_min) / (i_max - i_min);
    return p * (o_max - o_min) + o_min;
}

void create_cube(Points *points) {
    Vector3 v;
    v = (Vector3){ .x = BB_X_MAX, .y = BB_Y_MAX, .z = BB_Z_MIN };
    da_append(points, v);
    v = (Vector3){ .x = BB_X_MIN, .y = BB_Y_MAX, .z = BB_Z_MIN };
    da_append(points, v);
    v = (Vector3){ .x = BB_X_MIN, .y = BB_Y_MIN, .z = BB_Z_MIN };
    da_append(points, v);
    v = (Vector3){ .x = BB_X_MAX, .y = BB_Y_MIN, .z = BB_Z_MIN };
    da_append(points, v);
    v = (Vector3){ .x = BB_X_MAX, .y = BB_Y_MAX, .z = BB_Z_MAX };
    da_append(points, v);
    v = (Vector3){ .x = BB_X_MIN, .y = BB_Y_MAX, .z = BB_Z_MAX };
    da_append(points, v);
    v = (Vector3){ .x = BB_X_MIN, .y = BB_Y_MIN, .z = BB_Z_MAX };
    da_append(points, v);
    v = (Vector3){ .x = BB_X_MAX, .y = BB_Y_MIN, .z = BB_Z_MAX };
    da_append(points, v);
}

void spawn_new_balls(Balls *balls) {
    for (size_t i = balls->count; i < BALLS_NUM; ++i) {
        Ball b = create_random_ball();
        da_append(balls, b);
    }
}

void remove_dead_balls(Balls *balls) {
    size_t i = balls->count - 1;
    do {
        if (!balls->items[i].alive) {
            // for (size_t j = i; j < balls->count; ++j) {
            //     balls->items[j] = balls->items[j+1];
            // }
            // --balls->count;

            da_remove(balls, i);
        }
        --i;
    } while (i != 0);
}

int main(void)
{
    srand(time(NULL));
    srand48(time(NULL));
    InitWindow(WIDTH, HEIGHT, "Raylib Template");
    SetTargetFPS(60);
    Points points = {0};
    create_cube(&points);
    size_t lines[6][4] = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}
    };
    Balls balls = {0};
    for (size_t i = 0; i < BALLS_NUM; ++i) {
        Ball ball = create_random_ball();
        da_append(&balls, ball);
    }

    Vector3 ego_p = { .x = EGO_INIT_X, .y = EGO_INIT_Y, .z = EGO_INIT_Z };
    Quaternion rot_x = {
        .w = cosf(EGO_INIT_ROLL*0.5f),
        .x = sinf(EGO_INIT_ROLL*0.5f),
        .y = 0.f,
        .z = 0.f
    };
    Quaternion rot_z = {
        .w = cosf(EGO_INIT_YAW*0.5f),
        .x = 0.f,
        .y = 0.f,
        .z = sinf(EGO_INIT_YAW*0.5f)
    };
    // right multiply since in local crs
    Quaternion ego_r = QuaternionMultiply(rot_z, rot_x);

    double t = 0.f;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        register_movement(&ego_p, &ego_r);

        spawn_new_balls(&balls);
        da_foreach(&balls, Ball, ball) {
            if (!ball->alive) continue;
            update_ball(ball, &balls);
            Vector2 ball_screen = global_to_screen(ball->p, ego_p, ego_r);
            double ball_r = scale_local_to_screen(ball->r, ball->p, ego_p, ego_r);
            DrawCircleV(ball_screen, ball_r, ball->c);
        }
        remove_dead_balls(&balls);

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
        EndDrawing();
        t += DT;
    }
    CloseWindow();

    return 0;
}
