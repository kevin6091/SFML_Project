#pragma once
#include "include.h"

class FSM
{
public:
    virtual ~FSM() = default;

    virtual void Enter() {}
    //Update는 알아서,.
    virtual void Exit() {}
};

