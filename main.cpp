#include <vector>
#include "raylib.h"
#include "raymath.h"

const int ScreenWidth = 1280;
const int ScreenHeight = 800;
float GRAVITY_CONSTANT = 10.0f;
float soft = 5.0f; // to prevent Infinity Gravity when particles are too close


struct Particle {
    Vector2 pos, vel, acc;
    float radius, mass;
    Color c;
    bool is_active;

    Particle(Vector2 p, Vector2 v, Vector2 a, float r, float m, Color col) 
        : pos(p), vel(v), acc(a), radius(r), mass(m), c(col), is_active(true) {}

    void Draw(){
        if (is_active)
        DrawCircleV(pos, radius, c);
    }

    void Update() {
        if (is_active) pos = Vector2Add(pos, vel);
    }

};
    // F = G (m1 * m2) / r^2
    // G = 6.674e-11 m^3 kg^-1 s^-2
void apply_gravity(Particle& a, Particle& b) {
    // Calculate direction
    Vector2 direc = Vector2Subtract(b.pos, a.pos);

    //  prevent division by near-zero
    float distSq = Vector2LengthSqr(direc) + soft; 

    // Calculate force magnitude
    float magnitude = (GRAVITY_CONSTANT * a.mass * b.mass) / distSq;
    Vector2 force = Vector2Scale(Vector2Normalize(direc), magnitude);
    
    // a's acceleration = F / m_a
    a.vel = Vector2Add(a.vel, Vector2Scale(force, 1.0f / a.mass));
    // b's acceleration = F / m_b but in opposite direction
    b.vel = Vector2Subtract(b.vel, Vector2Scale(force, 1.0f / b.mass));
}

bool is_colliding(Particle& a, Particle& b) {
    return CheckCollisionCircles(a.pos, a.radius, b.pos, b.radius);
};

void combine(Particle& a, Particle& b) {
    // Determine which particle is larger
    Particle* winner = (a.mass >= b.mass) ? &a : &b;
    Particle* loser = (a.mass >= b.mass) ? &b : &a;

    // Add masses
    winner->mass += loser->mass;

    // Calculate new radius based on (A = pi * r^2)
    winner->radius = sqrtf((winner->radius * winner->radius) + (loser->radius * loser->radius));

    // Conserve Momentum: (m1*v1 + m2*v2) / (m1 + m2)
    Vector2 momentumA = Vector2Scale(a.vel, a.mass);
    Vector2 momentumB = Vector2Scale(b.vel, b.mass);
    winner->vel = Vector2Scale(Vector2Add(momentumA, momentumB), 1.0f / winner->mass);

    // Deactivate the loser
    loser->is_active = false;
}


int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Gravity Simulation");
    SetTargetFPS(120);

    std::vector<Particle> particles;
    for (int i{0}; i < 10; i++) {
        particles.push_back(Particle{
    {(float)GetRandomValue(0, ScreenWidth), (float)GetRandomValue(0, ScreenHeight)}, // pos
    {(float)GetRandomValue(-1, 1), (float)GetRandomValue(-1, 1)},                   // vel
    {(float)GetRandomValue(-2, 2), (float)GetRandomValue(-2, 2)},                   // acc
    5.0f,                                                                           // radius
    10.0f,                                                                          // mass
    BLUE                                                                            // Color
    });
    }

    while (!WindowShouldClose()) {
        // Apply gravity to both particles Newton's 3rd law F = -F
        for (size_t i = 0; i < particles.size(); i++) {
            if (!particles[i].is_active) continue;

            for (size_t j = i + 1; j < particles.size(); j++) {
                if (!particles[j].is_active) continue; // Skip inactive particles

                if (is_colliding(particles[i], particles[j])) {
                    combine(particles[i], particles[j]);
                } else {
                    apply_gravity(particles[i], particles[j]);
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (auto& p : particles) {
            p.Update();
            p.Draw();
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

