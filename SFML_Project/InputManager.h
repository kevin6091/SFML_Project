#pragma once
#include "include.h"

enum class EKeyState
{
    Down,  
    Press, 
    Up,
    None
};

class InputManager
{
private:
    array<EKeyState, 256> keyStates;

    array<EKeyState, 8> mouseStates;

    Vector2i mouseScreenPos; // 윈도우 기준 픽셀 좌표
    Vector2f mouseWorldPos;  // 인게임 카메라(View) 기준 월드 좌표

public:
    InputManager();
    ~InputManager() = default;

    void Initialize();
    void Update();
    void Release();

    EKeyState GetKeyState(Keyboard::Key key) const;

    bool GetKeyNone(Keyboard::Key key) const;
    bool GetKeyDown(Keyboard::Key key) const;
    bool GetKeyPress(Keyboard::Key key) const;
    bool GetKeyUp(Keyboard::Key key) const;

    bool GetMouseDown(Mouse::Button button) const;
    bool GetMousePress(Mouse::Button button) const;
    bool GetMouseUp(Mouse::Button button) const;

    Vector2i GetMouseScreenPos() const { return mouseScreenPos; }
    Vector2f GetMouseWorldPos() const { return mouseWorldPos; }
};