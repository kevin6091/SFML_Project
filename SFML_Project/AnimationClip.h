#pragma once
#include "include.h"    

class AnimationClip 
{
public:
    AnimationClip(const vector<sptr<Texture>>& frames, float frameDuration, bool isLoop)
        : m_frames(frames), m_frameDuration(frameDuration), m_isLoop(isLoop) 
    {}
    
    __forceinline sptr<Texture> GetFrame(size_t index) const
    {
        if (index < m_frames.size()) return m_frames[index];
        return nullptr;
    }

    __forceinline size_t GetFrameCount() const { return m_frames.size(); }
    __forceinline float GetFrameDuration() const { return m_frameDuration; }
    __forceinline bool IsLoop() const { return m_isLoop; }

public:
    void Initialize();
    void Release();

private:
    vector<sptr<Texture>> m_frames;
    float m_frameDuration = 0.2f;
    bool m_isLoop = false;
};