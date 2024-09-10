#include <raylib.h>
#include <iostream>
#include <random>
#include <bits/stdc++.h>

/*

g++ Main.cpp -o Main -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm
./Main.exe

*/

struct Particle
{
    bool isActive;
    Vector2 position;
    Vector2 direction;
    float speed;
    float lifeTime;
    Color color;
};

float RandomDirectionX()
{
    float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    // Make it [-1, 1]
    return x * 2.0f - 1.0f;
}

void initializeParticle(Particle &particle)
{
    particle.isActive = true;
    Color randomColor = {
        GetRandomValue(0, 255),
        GetRandomValue(0, 255),
        GetRandomValue(0, 255),
        255};
    // particle.position.x = GetRandomValue(1, 800);
    particle.position.x = 400; // changed so that all are spawned in the middle
    particle.position.y = 600;
    // particle.direction.x = GetRandomValue(-1, 1);
    particle.direction.x = RandomDirectionX();
    particle.direction.y = -1;
    particle.speed = GetRandomValue(50, 100);
    particle.lifeTime = GetRandomValue(2.0, 5.0);
    particle.color = randomColor;
    // std::cout<< "Initialized Particle: " << particle.isActive << std::endl;
}

void UpdateParticle(Particle &particle, float deltaTime)
{
    if (particle.lifeTime <= 0)
    {
        particle.isActive = false;
    }
    particle.position.x += (particle.speed * particle.direction.x) * deltaTime;
    particle.position.y += (particle.speed * particle.direction.y) * deltaTime;
    particle.lifeTime -= deltaTime;
}

void initializeParticleArray(Particle *array, int arraySize)
{
    for (size_t i = 0; i < arraySize; i++)
    {
        if (!array[i].isActive)
        {
            initializeParticle(array[i]); // passing pointers wrong
        }
    }
}

void EmitParticles(Particle *array, int arraySize, float deltaTime)
{
    for (size_t i = 0; i < arraySize; i++)
    {
        UpdateParticle(array[i], deltaTime);
        if (array[i].lifeTime != 0)
        {
            // std::cout<< array[i].position.x << " " << array[i].position.y << std::endl;
            DrawCircle(array[i].position.x, array[i].position.y, 15, array[i].color);
        }
    }
}

void changeEmissionRate(int &emissionRate)
{
    if (emissionRate <= 0)
    {
        emissionRate = 1;
    }
    if (IsKeyPressed(KEY_RIGHT))
    {
        emissionRate += 1;
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        emissionRate -= 1;
    }
}

int main()
{
    int activateParticles = KEY_SPACE;
    int particleArraySize = 1000;
    int emissionRate = 30;
    Particle *particleArray = new Particle[particleArraySize];
    int width = 800;
    int height = 600;
    InitWindow(width, height, "Homework 2 - Tamano/Olivares - Particle System");
    std::srand(GetFrameTime());

    while (!WindowShouldClose())
    {
        // calculations
        float deltaTime = GetFrameTime();
        changeEmissionRate(emissionRate);
        if (IsKeyDown(activateParticles))
        {
            // std::cout<< "spacebar is being pressed." << std::endl;
            initializeParticleArray(particleArray, particleArraySize);
        }

        // drawstep
        BeginDrawing();

        ClearBackground(BLACK); // black so it's easier to see the particles

        EmitParticles(particleArray, emissionRate, deltaTime);

        EndDrawing();
    }
    delete particleArray;
    CloseWindow();
    return 0;
}