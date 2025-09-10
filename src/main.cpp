#include "raylib.h"
#include "../include/core/custom_camera.h"
#include <vector>

// g++ src/main.cpp -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

struct Projectile {
	bool obj_should_move;
	double obj_move;
	Vector3 forward;			//aiming at ... // Vector3Normalize(Vector3Subtract(camera.target, camera.position))
	Vector3 muzzlePosition;		// position of muzzle
};

double obj_move_base = 0.5f;
bool should_fire_new_projectile = false;

void FireNewProjectile(bool& should_fire_new_projectile, std::vector<Projectile>& projectiles);
void LoopProjectiles(std::vector<Projectile>& projectiles);

Camera camera { 0 };

int main(void)
{
	std::vector<Projectile> projectiles;
    const int screenWidth = 4400;
    const int screenHeight = 2800;

    InitWindow(screenWidth, screenHeight, "RAYLIB PROJECTILES");

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
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE)) {
            should_fire_new_projectile = true;
        }

        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                if (should_fire_new_projectile) 
			        FireNewProjectile(should_fire_new_projectile, projectiles);

                LoopProjectiles(projectiles);

                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY);
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, GOLD);
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, GOLD); 
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD); 

            EndMode3D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void FireNewProjectile(bool& should_fire_new_projectile, std::vector<Projectile>& projectiles) {
	Vector3 right = GetCameraRight(&camera); // Returns the camera's right vector (normalized)
	Vector3 up    = GetCameraUp(&camera);
	// Vector3Normalize returns a new vector pointing in the same direction as the input but with length 1
	Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));

	// offset for muzzle position
	float rightOffset = 0.3f;
	float downOffset  = -0.2f;

	// Vector3Scale multiply vector by scalar
	Vector3 muzzlePosition = Vector3Add(camera.position, Vector3Scale(right, rightOffset));
	muzzlePosition = Vector3Add(muzzlePosition, Vector3Scale(up, downOffset));

	Projectile newProjectile = { 
		true,                // bool obj_should_move;
		obj_move_base,        // double obj_move;
		forward,
		muzzlePosition
	};

	projectiles.push_back(newProjectile);

	should_fire_new_projectile = false;
}

void LoopProjectiles(std::vector<Projectile>& projectiles) {
	size_t iter {0};
	for (auto& p : projectiles) {
		if (p.obj_should_move) {
			p.obj_move += obj_move_base;

			if (p.obj_move >= 200.0f) {
				p.obj_should_move = false; // Refactor: A
				// projectilesToRemove.push_back(iter);
			}

			Vector3 projectilePosition = Vector3Add(p.muzzlePosition, Vector3Scale(p.forward, p.obj_move));

			// (projectile arch) apply a downward trajectory to the object's position by subtracting a value proportional to the square of obj_move from its y-coordinate
			projectilePosition.y -= (p.obj_move * p.obj_move) / 200.0f;
			float sphereRadius = 0.1f;
			DrawSphere(projectilePosition, sphereRadius, BLUE);
		}
		++iter;
	}
}