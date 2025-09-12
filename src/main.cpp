#include "raylib.h"
#include <iostream>
#include "../include/core/custom_camera.h"
#include "../include/core/launcher.h"

// g++ -std=c++23 src/main.cpp src/core/gear_config.cpp src/core/launcher.cpp src/core/base_projectile.cpp src/core/dodge_ball.cpp src/core/projectile_two.cpp -o main -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

// release build targeting C++23 with warnings and optimizations
// g++ -std=c++23 -O2 -march=native -flto -Wall -Wextra -Wpedantic src/main.cpp src/core/gear_config.cpp src/core/launcher.cpp src/core/base_projectile.cpp src/core/dodge_ball.cpp src/core/projectile_two.cpp -o main -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

int main(void) {
    const int screenWidth = 4400;
    const int screenHeight = 2800;
    const std::string configFile = "assets/configs/gear.json";

    InitWindow(screenWidth, screenHeight, "Raylib Projectiles with JSON Hot Reload");

    Camera camera { 0 };
    camera.position = { 0.0f, 2.0f, 4.0f };
    camera.target   = { 0.0f, 2.0f, 0.0f };
    camera.up       = { 0.0f, 1.0f, 0.0f };
    camera.fovy     = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    int cameraMode = CAMERA_FIRST_PERSON;
    DisableCursor();
    SetTargetFPS(60);

    // load configs and track modification time
    std::map<std::string, GearConfig> configs = LoadGearConfigs(configFile);
    std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(configFile);

    Launcher projectileOne(configs["projectileOne"]);
    Launcher projectileTwo(configs["projectileTwo"]);
    Launcher dodgeBall(configs["dodgeBall"]);

    std::vector<std::unique_ptr<BaseProjectile>> projectiles;
    int projectileType = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // hot reload check
        std::filesystem::file_time_type currentWriteTime = std::filesystem::last_write_time(configFile);
        if (currentWriteTime != lastWriteTime) {
            try {
                configs = LoadGearConfigs(configFile);
                projectileOne.setConfig(configs["projectileOne"]);
                projectileTwo.setConfig(configs["projectileTwo"]);
                dodgeBall.setConfig(configs["dodgeBall"]);
                lastWriteTime = currentWriteTime;
                std::cout << "Reloaded gear configs!\n";
            } catch (const std::exception& e) {
                std::cerr << "Failed to reload configs: " << e.what() << "\n";
            }
        }

        // TODO: change to enum
        if (IsKeyPressed(KEY_ONE)) projectileType = 0;
        if (IsKeyPressed(KEY_TWO)) projectileType = 1;
        if (IsKeyPressed(KEY_THREE)) projectileType = 2;

        projectileOne.update(dt);
        projectileTwo.update(dt);
        dodgeBall.update(dt);

        if (projectileType == 0) projectileOne.tryFire(camera, projectiles, 0);
        else if (projectileType == 1) projectileTwo.tryFire(camera, projectiles, 1);
        else if (projectileType == 2) dodgeBall.tryFire(camera, projectiles, 2);

        for (std::unique_ptr<BaseProjectile>& p : projectiles) p->update(dt);
        std::erase_if(projectiles, [](const std::unique_ptr<BaseProjectile>& p) { return !p->isAlive(); });

        UpdateCamera(&camera, cameraMode);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);

                for (std::unique_ptr<BaseProjectile>& p : projectiles) p->draw();

                DrawPlane({ 0.0f, 0.0f, 0.0f }, { 32.0f, 32.0f }, LIGHTGRAY);
                DrawCube({ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, GOLD);
                DrawCube({ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, GOLD);
                DrawCube({ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);

            EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
