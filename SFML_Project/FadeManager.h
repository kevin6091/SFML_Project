#pragma once
#include "include.h"

class FadeManager
{
public:
    enum class FadeState { None, FadeIn, FadeOut };

public:
    FadeManager() = default;
    ~FadeManager() = default;

public:
    void StartFadeIn(float duration, Color color = Color::Black);

    void StartFadeOut(float duration, Color color = Color::Black);

    void Update(float deltaTime);
    void Render();

    __forceinline bool IsFading() const { return m_fadeState != FadeState::None; }

private:
    FadeState m_fadeState = FadeState::None;
    float m_fadeAlpha = 0.0f;
    float m_fadeSpeed = 0.0f;
    RectangleShape m_fadeOverlay;
    Color m_fadeColor = Color::Black;

};