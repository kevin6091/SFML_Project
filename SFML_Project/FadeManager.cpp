#include "FadeManager.h"
#include "GameInstance.h"
#include "Camera.h"

void FadeManager::StartFadeIn(float duration, Color color)
{
    m_fadeColor = color;
    m_fadeAlpha = 255.0f;
    m_fadeSpeed = 255.0f / duration;
    m_fadeState = FadeState::FadeIn;
}

void FadeManager::StartFadeOut(float duration, Color color)
{
    m_fadeColor = color;
    m_fadeAlpha = 0.0f;
    m_fadeSpeed = 255.0f / duration;
    m_fadeState = FadeState::FadeOut;
}

void FadeManager::Update(float deltaTime)
{
    if (m_fadeState == FadeState::None) return;

    m_fadeOverlay.setSize({ WIDTH * 10.f, HEIGHT * 10.f });
    m_fadeOverlay.setPosition(GameInstance::GetInstance().GetCamera().GetPosition());
    m_fadeOverlay.setOrigin({ WIDTH * 5.f, HEIGHT * 5.f });

    if (m_fadeState == FadeState::FadeIn)
    {
        m_fadeAlpha -= m_fadeSpeed * deltaTime;
        if (m_fadeAlpha <= 0.0f) 
        {
            m_fadeAlpha = 0.0f;
            m_fadeState = FadeState::None;
        }

        //나중에 왼쪽으로 밀면서 알파적용
    }
    else if (m_fadeState == FadeState::FadeOut)
    {
        m_fadeAlpha += m_fadeSpeed * deltaTime;
        if (m_fadeAlpha >= 255.0f) 
        { 
            m_fadeAlpha = 255.0f;
            m_fadeState = FadeState::None;
        }
    }

    m_fadeColor.a = static_cast<std::uint8_t>(m_fadeAlpha);
    m_fadeOverlay.setFillColor(m_fadeColor);
}

void FadeManager::Render()
{
    GameInstance::GetInstance().GetRenderTarget_Effect().draw(m_fadeOverlay, BlendAlpha);
}
