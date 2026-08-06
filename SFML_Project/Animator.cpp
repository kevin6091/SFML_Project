#include "Animator.h"
#include "GameInstance.h"
#include "ResourceManager.h"

Animator::Animator()
{
}

Animator::~Animator()
{
}

void Animator::Initialize()
{

}

void Animator::Play(const string& name, bool forceReset)
{
    if (currentClipName == name && !forceReset && m_isPlaying) return;

    auto it = clips.find(name);
    if (it != clips.end()) {
        currentClip = &(it->second);
        currentClipName = name;
        currentFrameIndex = 0;
        m_elapsedTime = 0.0f;
        m_isPlaying = true;
        m_isFinished = false;
    }
}

void Animator::Update(float deltaTime, Sprite& targetSprite, bool alignCenter)
{
    if (!m_isPlaying || !currentClip) return;

    m_elapsedTime += deltaTime;

    // 프레임 교체 타임 체크
    if (m_elapsedTime >= currentClip->GetFrameDuration()) 
    {
        m_elapsedTime -= currentClip->GetFrameDuration();
        currentFrameIndex++;

        // 애니메이션의 끝에 도달했을 때
        if (currentFrameIndex >= currentClip->GetFrameCount()) 
        {
            if (currentClip->IsLoop()) 
            {
                currentFrameIndex = 0;
            }
            else
            {
                currentFrameIndex = currentClip->GetFrameCount() - 1; // 마지막 프레임 고정
                m_isPlaying = false;
                m_isFinished = true;
            }
        }

        // 스프라이트에 새로운 프레임 텍스처 적용
        UpdateSpriteTexture(targetSprite, alignCenter);
    }
}

void Animator::UpdateSpriteTexture(Sprite& targetSprite, bool alignCenter) const
{
    if (currentClip) 
    {
        auto tex = currentClip->GetFrame(currentFrameIndex);
        if (tex)
        {
            targetSprite.setTexture(*tex, true); // true: 텍스처 크기에 맞게 스프라이트 Rect 자동 맞춤

            if (alignCenter)
            {
                // 중심점(Origin)을 프레임 중앙으로 자동 정렬
                Vector2u size = tex->getSize();
                targetSprite.setOrigin({ static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) });
            }
            else
            {
                Vector2u size = tex->getSize();
                targetSprite.setOrigin({ 0.f, static_cast<float>(size.y) * 0.5f });
            }
        }
    }
}

void Animator::AddClip(const string& name, float duration, bool loop)
{
    auto& resourceManager = GameInstance::GetInstance().GetResourceManager();

    if (auto runFrames = resourceManager.GetTextureSequence(name))
    {
        clips.emplace(name, AnimationClip(*runFrames, duration, loop));
    }
}

void Animator::Release()
{
    clips.clear();
}