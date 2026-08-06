#pragma once
#include "include.h"
#include "AnimationClip.h"

class Animator 
{
public:
    Animator();
    ~Animator();

public:
    void Initialize();
    void Release();

    // 애니메이션 재생
    void Play(const string& name, bool forceReset = false);
    // 매 프레임 업데이트
    void Update(float deltaTime, Sprite& targetSprite, bool alignCenter = true);
    void UpdateSpriteTexture(Sprite& targetSprite, bool alignCenter = true) const;

    __forceinline bool IsFinished() const { return m_isFinished; }
    __forceinline string GetCurrentClipName() const { return currentClipName; }
    __forceinline size_t GetCurrentFrameIndex() const { return currentFrameIndex; }

    void AddClip(const string& name, float duration, bool loop);

private:
    unordered_map<string, AnimationClip> clips;
    const AnimationClip* currentClip = nullptr;
    string currentClipName = "";

    size_t currentFrameIndex = 0;
    float m_elapsedTime = 0.0f;
    bool m_isPlaying = false;
    bool m_isFinished = false;
};