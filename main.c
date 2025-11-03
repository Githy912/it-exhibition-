#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <math.h>

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Sunnyside-Drifter 1.0");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 6.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 carPos = { 0.0f, 0.5f, 0.0f };
    float carRot = 0.0f;
    float carSpeed = 0.0f;
    float time = 0.0f;

    while (!WindowShouldClose())
    {
        // === TIME + SKY ===
        time += GetFrameTime() * 0.25f;
        if (time > 2 * PI) time = 0.0f;

        float skyMix = (sinf(time) + 1.0f) / 2.0f;
        Color skyColor = {
            (unsigned char)(10 + 60 * skyMix),
            (unsigned char)(20 + 90 * skyMix),
            (unsigned char)(60 + 180 * skyMix),
            255
        };

        // === INPUT ===
        float moveSpeed = 0.1f;
        float turnSpeed = 90.0f; // degrees per second

        if (IsKeyDown(KEY_W)) carSpeed += 0.005f;
        if (IsKeyDown(KEY_S)) carSpeed -= 0.005f;
        carSpeed *= 0.98f;

        if (IsKeyDown(KEY_A)) carRot += turnSpeed * GetFrameTime() * (carSpeed != 0 ? (carSpeed > 0 ? 1 : -1) : 0);
        if (IsKeyDown(KEY_D)) carRot -= turnSpeed * GetFrameTime() * (carSpeed != 0 ? (carSpeed > 0 ? 1 : -1) : 0);

        carPos.x += sinf(DEG2RAD * carRot) * carSpeed * 10;
        carPos.z += cosf(DEG2RAD * carRot) * carSpeed * 10;

        // Camera follows car
        Vector3 camOffset = { sinf(DEG2RAD * carRot) * -6.0f, 3.0f, cosf(DEG2RAD * carRot) * -6.0f };
        camera.position = Vector3Add(carPos, camOffset);
        camera.target = Vector3Add(carPos, (Vector3) { 0.0f, 0.8f, 0.0f });

        BeginDrawing();
        ClearBackground(skyColor);
        BeginMode3D(camera);

        // === ENVIRONMENT ===
        DrawPlane((Vector3) { 0, 0, 0 }, (Vector2) { 100, 100 }, (Color) { 40, 180, 40, 255 });
        DrawGrid(50, 1.0f);

        // === PARKING AREA ===
        DrawCube((Vector3) { 5, 0.01f, 5 }, 4, 0.05f, 2, (Color) { 230, 230, 230, 255 });
        DrawCube((Vector3) { 5, 0.02f, 5 }, 3.8f, 0.02f, 1.8f, (Color) { 180, 180, 180, 255 });

        // === CAR BODY ===
        Color bodyColor = (Color){ 220, 40, 40, 255 };
        Color windowColor = (Color){ 50, 80, 130, 200 };
        Color tireColor = (Color){ 20, 20, 20, 255 };
        Color rimColor = (Color){ 180, 180, 180, 255 };

        rlPushMatrix();
        rlTranslatef(carPos.x, carPos.y, carPos.z);
        rlRotatef(carRot, 0, 1, 0);

        // Main car body
        DrawSphereEx((Vector3) { 0, 0.5f, 0 }, 1.0f, 20, 20, bodyColor);
        DrawCube((Vector3) { 0, 0.5f, 0 }, 2.2f, 0.6f, 1.2f, bodyColor);
        DrawSphereEx((Vector3) { 0, 1.0f, 0 }, 0.8f, 20, 20, bodyColor);
        DrawSphereEx((Vector3) { 1.0f, 0.4f, 0 }, 0.7f, 20, 20, bodyColor);
        DrawSphereEx((Vector3) { -1.0f, 0.4f, 0 }, 0.7f, 20, 20, bodyColor);

        // Windows
        DrawCube((Vector3) { 0, 1.0f, 0 }, 1.3f, 0.5f, 0.9f, windowColor);

        // === WHEELS ===
        float wx = 1.0f, wz = 0.7f;
        // Front Right
        DrawCylinderEx((Vector3) { wx, -0.2f, -wz }, (Vector3) { wx, -0.2f, -wz - 0.2f }, 0.3f, 0.3f, 20, tireColor);
        DrawCylinderEx((Vector3) { wx, -0.2f, -wz }, (Vector3) { wx, -0.2f, -wz - 0.15f }, 0.15f, 0.15f, 20, rimColor);
        // Front Left
        DrawCylinderEx((Vector3) { wx, -0.2f, wz }, (Vector3) { wx, -0.2f, wz + 0.2f }, 0.3f, 0.3f, 20, tireColor);
        DrawCylinderEx((Vector3) { wx, -0.2f, wz }, (Vector3) { wx, -0.2f, wz + 0.15f }, 0.15f, 0.15f, 20, rimColor);
        // Rear Right
        DrawCylinderEx((Vector3) { -wx, -0.2f, -wz }, (Vector3) { -wx, -0.2f, -wz - 0.2f }, 0.3f, 0.3f, 20, tireColor);
        DrawCylinderEx((Vector3) { -wx, -0.2f, -wz }, (Vector3) { -wx, -0.2f, -wz - 0.15f }, 0.15f, 0.15f, 20, rimColor);
        // Rear Left
        DrawCylinderEx((Vector3) { -wx, -0.2f, wz }, (Vector3) { -wx, -0.2f, wz + 0.2f }, 0.3f, 0.3f, 20, tireColor);
        DrawCylinderEx((Vector3) { -wx, -0.2f, wz }, (Vector3) { -wx, -0.2f, wz + 0.15f }, 0.15f, 0.15f, 20, rimColor);

        rlPopMatrix();

        EndMode3D();

        DrawText("Use W/S to move, A/D to turn", 20, 20, 20, RAYWHITE);
        DrawText("Dynamic Day/Night Cycle Active 🌅🌙", 20, 50, 20, RAYWHITE);
        DrawFPS(screenWidth - 100, 20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
