#pragma once

#include "include.h"

class FadeManager;
class Scene;

class SceneManager
{

public:
    SceneManager();
    ~SceneManager();

public:
    void Initialize();

    void ChangeScene(ESceneType eSceneType, float fadeDuration = 0.3f);

    void FadeInScene(float fadeDuration = 0.3f);
    void FadeOutScene(float fadeDuration = 0.3f);

    void Update(float deltaTime);
    void LateUpdate(float deltaTime);

    void Render();
    void RenderUI();

    void Release();

    ESceneType GetSceneType() { return eSceneType; }
    bool GetIsChangingScene() { return isChangingScene; }
    FadeManager* GetFadeManager() { return uFadeManager.get(); }

private:
    uptr<Scene> uCurrentScene;
    uptr<Scene> uNextScene;
    uptr<FadeManager> uFadeManager;
    
    ESceneType eSceneType = ESceneType::End;
    bool isChangingScene = false;
};

