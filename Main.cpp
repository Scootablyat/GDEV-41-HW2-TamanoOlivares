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

int GetKey(std::string key)
{
    if (key == "KEY_RIGHT")
    {
        return KEY_RIGHT;
    }
    if (key == "KEY_LEFT")
    {
        return KEY_LEFT;
    }
    if (key == "KEY_UP")
    {
        return KEY_UP;
    }
    if (key == "KEY_DOWN")
    {
        return KEY_DOWN;
    }
    if (key == "MOUSE_LEFT")
    {
        return MOUSE_BUTTON_LEFT;
    }
    if (key == "MOUSE_RIGHT")
    {
        return MOUSE_BUTTON_RIGHT;
    }
    if (key == "MOUSE_MIDDLE")
    {
        return MOUSE_BUTTON_MIDDLE;
    }
    if (key == "KEY_SPACE")
    {
        return KEY_SPACE;
    }
}

int GetKeyCode(std::string x)
{
    if (x.length() == 1)
    {
        char y = x[0];
        return y - '0' + 48;
    }
    else
    {
        return GetKey(x);
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
                return GetKeyCode(line.erase(0, line.find(" = ") + 3));
            }
            if (line.find("reduceKeyEmissionRate") == 0 && setting == "reduceKeyEmissionRate")
            {
                return GetKeyCode(line.erase(0, line.find(" = ") + 3));
            }
            if (line.find("increaseMouseEmissionRate") == 0 && setting == "increaseMouseEmissionRate")
            {
                return GetKeyCode(line.erase(0, line.find(" = ") + 3));
            }
            if (line.find("reduceMouseEmissionRate") == 0 && setting == "reduceMouseEmissionRate")
            {
                return GetKeyCode(line.erase(0, line.find(" = ") + 3));
            }
            if (line.find("activateKeyParticles") == 0 && setting == "activateKeyParticles")
            {
                return GetKeyCode(line.erase(0, line.find(" = ") + 3));
            }
            if (line.find("activateMouseParticles") == 0 && setting == "activateMouseParticles")
            {
                return GetKeyCode(line.erase(0, line.find(" = ") + 3));
            }
        }
    }
    return 0;
}

float GetRandomDirection()
{
    float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    // Make it [-1, 1]
    return x * 2.0f - 1.0f;
}

void InitializeParticle(Particle *array, int arraySize, bool usingMouse)
{
    for (size_t i = 0; i < arraySize; i++)
    {
        if (array[i].isActive == false)
        {
            if (usingMouse)
            {
                array[i].isActive = true;
                array[i].position.x = GetMousePosition().x;
                array[i].position.y = GetMousePosition().y;
                array[i].direction.x = GetRandomDirection();
                array[i].direction.y = GetRandomDirection();
                array[i].speed = GetRandomValue(50, 100);
                array[i].lifeTime = GetRandomValue(0.5, 2.0);
                array[i].totalTime = array[i].lifeTime;
                Color randomColor = {
                    GetRandomValue(0, 255),
                    GetRandomValue(0, 255),
                    GetRandomValue(0, 255),
                    255};
                array[i].color = randomColor;
                std::cout << "Initialized Particle: " << i << std::endl;
                break;
            }
            else if (!usingMouse)
            {
                array[i].isActive = true;
                array[i].position.x = 400.0f; // changed so that all are spawned in the middle
                array[i].position.y = 600.0f;
                array[i].direction.x = GetRandomDirection();
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
                std::cout << "Initialized Particle: " << i << std::endl;
                break;
            }
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

void EmitParticles(Particle *array, int arraySize, float deltaTime)
{
    for (size_t i = 0; i < arraySize; i++)
    {
        UpdateParticle(&array[i], deltaTime);
        if (array[i].lifeTime != 0 && array[i].isActive == true)
        {
            // std::cout << array[i].position.x << " " << array[i].position.y << std::endl;
            DrawCircle(array[i].position.x, array[i].position.y, 5, array[i].color);
        }
    }
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
    int KeyEmissionRate = 30;
    int mouseEmissionRate = 30; // particles per second
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

    while (!WindowShouldClose())
    {
        // calculations
        float deltaTime = GetFrameTime();
        timeElapsed += deltaTime;

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
            while (timeElapsed >= secondsPerMouseEmission)
            {
                InitializeParticle(particleArray, particleArraySize, true);
                timeElapsed -= secondsPerMouseEmission;
            }
        }

        if (IsKeyDown(activateKeyParticles))
        {
            while (timeElapsed >= secondsPerKeyEmission)
            {
                InitializeParticle(particleArray, particleArraySize, false);
            }
        }

        // drawstep
        BeginDrawing();

        ClearBackground(BLACK); // black so it's easier to see the particles
        // EmitParticles(particleArray, particleArraySize, deltaTime);

        EndDrawing();
    }
    delete particleArray;
    CloseWindow();
    return 0;
}