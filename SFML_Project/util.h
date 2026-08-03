#pragma once
#include <random>

static int RandomInt(int min, int max)
{
    thread_local static std::random_device rd;
    thread_local static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

static float RandomFloat(float min, float max)
{
    thread_local static std::random_device rd;
    thread_local static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}