#include <vector>
#include "raylib.h"
#include "raymath.h"

const int ScreenWidth = 1280;
const int ScreenHeight = 800;

int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Gravity Simulation");
    SetTargetFPS(120);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(0, 0);
        EndDrawing();
}
    CloseWindow();
    return 0;

}

