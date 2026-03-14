#include <vector>
#include "raylib.h"
#include "raymath.h"

const int ScreenWidth = 1280;
const int ScreenHeight = 800;
float GRAVITY_CONSTANT = 5;

struct Particle {
    Vector2 pos, vel, acc;
    float radius, mass;
    Color c;

    void Draw(){
        DrawCircleV(pos, radius, c);
    }

    void change_pos() {
        pos = Vector2Add(pos, vel);
    }

};

    // F = G (m1 * m2) / r^2
    // G = 6.674e-11 m^3 kg^-1 s^-2
    void gravity(Particle& a, Particle& b) {
        // Calculate direction
        Vector2 direc = Vector2Subtract(b.pos, a.pos);

        // r^2
        float r = Vector2LengthSqr(direc);
        if (r == 0) return; // Avoid division by zero

        // Calculate Magnitude (F)
        float magnitude = GRAVITY_CONSTANT * (a.mass * b.mass) / r;

        Vector2 force = Vector2Scale(Vector2Normalize(direc), magnitude);
        Vector2 accel = Vector2Scale(force, 1.0f/ a.mass);

        a.vel = Vector2Add(a.vel, accel);

    }

int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Gravity Simulation");
    SetTargetFPS(120);

    std::vector<Particle> particles;
    for (int i{0}; i < 10; i++) {
        particles.push_back(Particle{
    {(float)GetRandomValue(0, ScreenWidth), (float)GetRandomValue(0, ScreenHeight)}, // pos
    {(float)GetRandomValue(-1, 1), (float)GetRandomValue(-1, 1)},                   // vel
    {0, 0},                                                                         // acc
    5.0f,                                                                           // radius
    10.0f,                                                                          // mass
    BLUE                                                                            // Color
    });
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(0, 0);

        for (size_t i{0}; i < particles.size(); i++) {
            for (size_t j{0}; j < particles.size(); j++) {
                if (i == j) {
                    continue;
                }
            gravity(particles[i], particles[j]);
        }

        }

        for (auto& particle : particles) {
            particle.Draw();
            particle.change_pos();
        }

        EndDrawing();
}
    CloseWindow();
    return 0;

}

