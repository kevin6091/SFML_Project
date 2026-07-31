#pragma once
#include "include.h"

class ResourceManager;
class SceneManager;

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

    uptr<ResourceManager> uResourceManager;
    uptr<SceneManager>    uSceneManager;

public:
    __forceinline static GameInstance* GetInstance() 
    {
        static uptr<GameInstance> uInstance(new GameInstance());
        return uInstance.get();
    }

    void Initialize(uint width, uint height, const string& title);
    void Run();

    RenderWindow& GetWindow() { return window; }
    ResourceManager& GetResource() { return *uResourceManager; }
    SceneManager& GetScene() { return *uSceneManager; }

    float GetTimeScale() const { return timeScale; }
    void  SetTimeScale(float scale) { timeScale = scale; }

    void Draw();
};