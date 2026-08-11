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

static sf::FloatRect CreateRectFromCenter(const sf::Vector2f& center, const sf::Vector2f& size)
{
    float left = center.x - (size.x * 0.5f);
    float top = center.y - (size.y * 0.5f);

    return sf::FloatRect({ left, top }, size);

}