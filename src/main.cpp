#include "raylib.h"
#include "../include/core/custom_camera.h"

// g++ src/main.cpp -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

int main(void)
{
    const int screenWidth = 4400;
    const int screenHeight = 2800;

    InitWindow(screenWidth, screenHeight, "RAYLIB PROJECTILES");

    Camera camera { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };   
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };    
    camera.fovy = 60.0f;                          
    camera.projection = CAMERA_PERSPECTIVE;       

    int cameraMode = CAMERA_FIRST_PERSON;

    DisableCursor();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode3D(camera);

                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY);
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME); 
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD); 

            EndMode3D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
