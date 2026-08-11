#include "InputManager.h"
#include "GameInstance.h"
#include "Camera.h"
#include "SceneManager.h"
#include "FadeManager.h"

InputManager::InputManager()
{
    keyStates.fill(EKeyState::None);
    mouseStates.fill(EKeyState::None);
}

void InputManager::Initialize()
{
}

void InputManager::Update()
{
    if (GameInstance::GetInstance().GetSceneManager().GetFadeManager()->IsFading())
    {
        keyStates.fill(EKeyState::None);
        mouseStates.fill(EKeyState::None);
        return;
    }

    RenderWindow& window = GameInstance::GetInstance().GetWindow();

    for (int i = 0; i < 256; ++i)
    {
        bool isPressed = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(i));
        EKeyState& state = keyStates[i];

        if (isPressed)
        {
            if (state == EKeyState::None || state == EKeyState::Up)
                state = EKeyState::Down;     
            else if (state == EKeyState::Down)
                state = EKeyState::Press;    
        }
        else
        {
            if (state == EKeyState::Down || state == EKeyState::Press)
                state = EKeyState::Up;       
            else if (state == EKeyState::Up)
                state = EKeyState::None;
        }
    }

    for (int i = 0; i < Mouse::ButtonCount; ++i)
    {
        bool isPressed = Mouse::isButtonPressed(static_cast<Mouse::Button>(i));
        EKeyState& state = mouseStates[i];

        if (isPressed)
        {
            if (state == EKeyState::None || state == EKeyState::Up) state = EKeyState::Down;
            else if (state == EKeyState::Down) state = EKeyState::Press;
        }
        else
        {
            if (state == EKeyState::Down || state == EKeyState::Press) state = EKeyState::Up;
            else if (state == EKeyState::Up) state = EKeyState::None;
        }
    }

    mouseScreenPos = Mouse::getPosition(window);

    mouseWorldPos = window.mapPixelToCoords(mouseScreenPos, GameInstance::GetInstance().GetCamera().GetView());
}

EKeyState InputManager::GetKeyState(Keyboard::Key key) const
{
    return keyStates[(int)key];
}

bool InputManager::GetKeyNone(Keyboard::Key key) const
{
    return keyStates[(int)key] == EKeyState::None;
}

bool InputManager::GetKeyDown(Keyboard::Key key) const
{
    return keyStates[(int)key] == EKeyState::Down;
}

bool InputManager::GetKeyPress(Keyboard::Key key) const 
{
    return keyStates[(int)key] == EKeyState::Press ||
        keyStates[(int)key] == EKeyState::Down;
}

bool InputManager::GetKeyUp(Keyboard::Key key) const 
{
    return keyStates[(int)key] == EKeyState::Up;
}

bool InputManager::GetMouseDown(Mouse::Button button) const 
{
    return mouseStates[(int)button] == EKeyState::Down;
}

bool InputManager::GetMousePress(Mouse::Button button) const
{
    return mouseStates[(int)button] == EKeyState::Press ||
        mouseStates[(int)button] == EKeyState::Down;
}

bool InputManager::GetMouseUp(Mouse::Button button) const 
{
    return mouseStates[(int)button] == EKeyState::Up;
}

void InputManager::Release()
{
}
