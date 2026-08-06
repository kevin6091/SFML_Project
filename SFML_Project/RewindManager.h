#pragma once
#include "include.h"

class RewindManager
{
public:
    static RewindManager& GetInstance()
    {
        static RewindManager instance;
        return instance;
    }

    bool IsRewinding() const { return bIsRewinding; }
    int GetRewindSpeed() const { return rewindSpeed; }

    void SetRewinding(bool boolean);

private:
    RewindManager() = default;

    bool bIsRewinding = false;
    int rewindSpeed = 3; // 기본 3배속
};