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

private:
    uptr<Scene> uCurrentScene;
    uptr<Scene> uNextScene;
    uptr<FadeManager> uFadeManager;
    bool isChangingScene = false;
};

