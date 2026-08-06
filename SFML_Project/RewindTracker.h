#pragma once
#include "include.h"

template <typename T>
class RewindTracker
{
private:
    deque<T> history;
    const size_t MAX_HISTORY = 2000;

public:
    // 과거 기록 추가
    void Record(const T& snapshot)
    {
        history.push_back(snapshot);
        if (history.size() > MAX_HISTORY)
        {
            history.pop_front();
        }
    }

    bool Rewind(int speed, T& outSnapshot)
    {
        if (history.empty())
            return false;

        for (int i = 0; i < speed; ++i)
        {
            if (history.empty())
                return false;

            outSnapshot = history.back();
            history.pop_back();
        }

        return true;
    }

    bool IsEmpty() const { return history.empty(); }
    void Clear() { history.clear(); }
};