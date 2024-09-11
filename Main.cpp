#include <raylib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

/*

g++ Main.cpp -o Main -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm
./Main.exe

*/

struct Particle
{
    bool isActive = false;
    Vector2 position;
    Vector2 direction;
    float speed;
    float lifeTime;
    float totalTime; // same value as lifetime but doesn't get subtracted while updating
    Color color;
};

// Source: https://www.youtube.com/watch?v=vGlvTWUctTQ
// Tried doing a timer but failed, not sure how to limit the spacebar holding input
struct Timer
{
    float lifeTime; // Lifetime (seconds)
};

void StartTimer(Timer *timer, float lifetime)
{
    if (timer != NULL)
    {
        timer->lifeTime = lifetime;
    }
}

void UpdateTimer(Timer *timer)
{
    if (timer != NULL && timer->lifeTime > 0) // make sure timer has not expired
    {
        timer->lifeTime -= GetFrameTime();
    }
}

bool TimerDone(Timer *timer)
{
    if (timer != NULL)
    {
        return timer->lifeTime <= 0;
    }
}

float RandomDirection()
{
    float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    // Make it [-1, 1]
    return x * 2.0f - 1.0f;
}

void InitializeKeyParticle(Particle *array, int arraySize)
{
    for (size_t i = 0; i < arraySize; i++)
    {
        if (array[i].isActive == false)
        {
            array[i].isActive = true;
            array[i].position.x = 400; // changed so that all are spawned in the middle
            array[i].position.y = 600;
            array[i].direction.x = RandomDirection();
            array[i].direction.y = -1;
            array[i].speed = GetRandomValue(50, 100);
            array[i].lifeTime = GetRandomValue(2.0, 5.0);
            array[i].totalTime = array[i].lifeTime;
            Color randomColor = {
                GetRandomValue(0, 255),
                GetRandomValue(0, 255),
                GetRandomValue(0, 255),
                255};
            array[i].color = randomColor;
            // std::cout << "Initialized Particle: " << i << std::endl;
            break;
        }
    }
}

void InitializeMouseParticle(Particle *array, int arraySize)
{
    for (size_t i = 0; i < arraySize; i++)
    {
        if (array[i].isActive == false)
        {
            array[i].isActive = true;
            array[i].position.x = GetMousePosition().x;
            array[i].position.y = GetMousePosition().y;
            array[i].direction.x = RandomDirection();
            array[i].direction.y = RandomDirection();
            array[i].speed = GetRandomValue(50, 100);
            array[i].lifeTime = GetRandomValue(0.5, 2.0);
            Color randomColor = {
                GetRandomValue(0, 255),
                GetRandomValue(0, 255),
                GetRandomValue(0, 255),
                255};
            array[i].color = randomColor;
            break;
        }
    }
}

void UpdateParticle(Particle *particle, float deltaTime)
{
    if (particle->lifeTime <= 0)
    {
        particle->isActive = false;
    }
    else
    {
        particle->position.x += (particle->speed * particle->direction.x) * deltaTime;
        particle->position.y += (particle->speed * particle->direction.y) * deltaTime;
        particle->lifeTime -= deltaTime;
        particle->color.a = 255 * (particle->lifeTime / particle->totalTime);
    }
}

// void initializeParticleArray(Particle *array, int arraySize)
// {
//     for (size_t i = 0; i < arraySize; i++)
//     {
//         InitializeKeyParticle(array[i]); // passing pointers wrong
//     }
// }

void EmitParticles(Particle *array, int arraySize, float deltaTime)
{
    for (size_t i = 0; i < arraySize; i++)
    {
        UpdateParticle(&array[i], deltaTime);
        if (array[i].lifeTime != 0)
        {
            // std::cout<< array[i].position.x << " " << array[i].position.y << std::endl;
            DrawCircle(array[i].position.x, array[i].position.y, 5, array[i].color);
        }
    }
}

int getSettings(std::string setting)
{
    std::fstream settingsFile;
    settingsFile.open("config.ini", std::ios::in);
    std::string line;
    if (settingsFile.is_open())
    {
        while (std::getline(settingsFile, line))
        {
            if (line.find("increaseKeyEmissionRate") == 0 && setting == "increaseKeyEmissionRate")
            {
                // std::cout << "KeyCode: " << line.erase(0, line.find(" ") + 1) << std::endl;
                return stoi(line.erase(0, line.find(" ") + 1));
            }
            if (line.find("reduceKeyEmissionRate") == 0 && setting == "reduceKeyEmissionRate")
            {
                return stoi(line.erase(0, line.find(" ") + 1));
            }
            if (line.find("increaseMouseEmissionRate") == 0 && setting == "increaseMouseEmissionRate")
            {
                return stoi(line.erase(0, line.find(" ") + 1));
            }
            if (line.find("reduceMouseEmissionRate") == 0 && setting == "reduceMouseEmissionRate")
            {
                return stoi(line.erase(0, line.find(" ") + 1));
            }
            if (line.find("activateKeyParticles") == 0 && setting == "activateKeyParticles")
            {
                return stoi(line.erase(0, line.find(" ") + 1));
            }
            if (line.find("activateMouseParticles") == 0 && setting == "activateMouseParticles")
            {
                return stoi(line.erase(0, line.find(" ") + 1));
            }
        }
    }
    return 0;
}

int main()
{
    int increaseKeyEmissionRate = getSettings("increaseKeyEmissionRate");
    int reduceKeyEmissionRate = getSettings("reduceKeyEmissionRate");
    int increaseMouseEmissionRate = getSettings("increaseMouseEmissionRate");
    int reduceMouseEmissionRate = getSettings("reduceMouseEmissionRate");
    int activateKeyParticles = getSettings("activateKeyParticles");
    int activateMouseParticles = getSettings("activateMouseParticles");

    int particleArraySize = 1000;
    int KeyEmissionRate = 30, mouseEmissionRate = 30; // particles per second
    float secondsPerKeyEmission = 1.0f / KeyEmissionRate; 
    float secondsPerMouseEmission = 1.0f / mouseEmissionRate;
    Particle *particleArray = new Particle[particleArraySize];
    int width = 800;
    int height = 600;
    InitWindow(width, height, "Homework 2 - Tamano/Olivares - Particle System");

    std::srand(GetFrameTime());
    Timer particleTimer = {1};
    int particlesInitialized = 0;
    float timeElapsed;
    // initializeParticleArray(particleArray, particleArraySize);

    while (!WindowShouldClose())
    {
        // calculations
        float deltaTime = GetFrameTime();
        timeElapsed += deltaTime;
        //float emissionsPerSecond = KeyEmissionRate / 1000.0f;
        // std::cout << emissionsPerSecond << std::endl;

        // controls
        if (IsKeyPressed(increaseKeyEmissionRate))
        {
            KeyEmissionRate = (KeyEmissionRate >= 50) ? 50 : KeyEmissionRate + 1;
        }
        if (IsKeyPressed(reduceKeyEmissionRate))
        {
            KeyEmissionRate = (KeyEmissionRate <= 1) ? 1 : KeyEmissionRate - 1;
        }
        if (IsKeyPressed(increaseMouseEmissionRate))
        {
            mouseEmissionRate = (mouseEmissionRate >= 50) ? 50 : mouseEmissionRate + 1;
        }
        if (IsKeyPressed(reduceMouseEmissionRate))
        {
            mouseEmissionRate = (mouseEmissionRate <= 1) ? 1 : mouseEmissionRate - 1;
        }

        if (IsMouseButtonDown(activateMouseParticles))
        {
            while (timeElapsed >= secondsPerMouseEmission){
                InitializeMouseParticle(particleArray, particleArraySize);
                timeElapsed -= secondsPerMouseEmission;
            }
            

        }

        if (IsKeyDown(activateKeyParticles))
        {
            // one at a time
            // emissions per second, so wait __ milliseconds

            // StartTimer(&particleTimer, emissionsPerSecond);

            // UpdateTimer(&particleTimer);
            // wait for __ miliseconds

            // if (TimerDone(&particleTimer))
            // {
            while (timeElapsed >= secondsPerKeyEmission){
                InitializeKeyParticle(particleArray, particleArraySize);
                timeElapsed -= secondsPerKeyEmission;
            }
            // std::cout << "timer done" << std::endl;
            // }
        }

        // drawstep
        BeginDrawing();

        ClearBackground(BLACK); // black so it's easier to see the particles
        EmitParticles(particleArray, KeyEmissionRate, deltaTime);

        EndDrawing();
    }
    delete particleArray;
    CloseWindow();
    return 0;
}