#pragma once
#include "include.h"

class ResourceManager;
class ObjectManager;
class SceneManager;
class InputManager;
class CollisionManager;
class GameObject;
class Camera;

class GameInstance 
{
    friend struct default_delete<GameInstance>;
private:
    GameInstance();
    ~GameInstance();

    GameInstance(const GameInstance&) = delete;
    GameInstance& operator=(const GameInstance&) = delete;
    GameInstance(GameInstance&&) = delete;
    GameInstance& operator=(GameInstance&&) = delete;

    RenderWindow window;
    float timeScale = 1.0f;

    uptr<ResourceManager>   uResourceManager;
    uptr<ObjectManager>     uObjectManager;
    uptr<SceneManager>      uSceneManager;
    uptr<CollisionManager>  uCollisionManager;
    uptr<InputManager>      uInputManager;
    uptr<Camera>            uCamera;

public:
    __forceinline static GameInstance& GetInstance() 
    {
        static uptr<GameInstance> uInstance(new GameInstance());
        
        return *uInstance;
    }

    __forceinline ResourceManager& GetResourceManager()             { return *uResourceManager; }
    __forceinline ObjectManager& GetObjectManager()                 { return *uObjectManager; }
    __forceinline SceneManager& GetSceneManager()                   { return *uSceneManager; }
    __forceinline InputManager& GetInputManager()                   { return *uInputManager; }
    __forceinline CollisionManager& GetCollisionManager()           { return *uCollisionManager; }
    __forceinline Camera& GetCamera()                               { return *uCamera; }
    __forceinline RenderWindow& GetWindow()                         { return window; }

    void Initialize(uint width, uint height, const string& title);
    void Run();
    void Release();

    __forceinline float GetTimeScale() const            { return timeScale; }
    __forceinline void  SetTimeScale(float scale)       { timeScale = scale; }

    void Draw(const GameObject& gameObject, RenderStates states = RenderStates::Default);
    // CreateObject() -> ResourceManager, Ç®¸µ
    // AddObject() -> ObjectManager
};