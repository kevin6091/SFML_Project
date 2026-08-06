#include "SceneManager.h"
#include "FadeManager.h"
#include "GameInstance.h"
#include "SceneTitle.h"
#include "SceneStage1.h"


SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
    uFadeManager = make_unique<FadeManager>();
}

void SceneManager::ChangeScene(ESceneType _eSceneType, float fadeDuration)
{
    eSceneType = _eSceneType;

    switch (eSceneType)
    {
    case ESceneType::Title:    
        uNextScene = move(make_unique<SceneTitle>());
        isChangingScene = true;
        break;
    case ESceneType::Stage1:
        FadeOutScene(1.0f);
        uNextScene = move(make_unique<SceneStage1>());
        isChangingScene = true;
        break;
    case ESceneType::End:
        break;
    default:
        break;
    }
    // FadeManager를 넘겨받아 페이드 효과 트리거
    // (직접 소유하지 않고 참조만 활용)
}

void SceneManager::FadeInScene(float fadeDuration)
{
    uFadeManager->StartFadeIn(fadeDuration);
}

void SceneManager::FadeOutScene(float fadeDuration)
{
    uFadeManager->StartFadeOut(fadeDuration);
}

void SceneManager::Update(float deltaTime)
{
    // 페이드가 끝나는 지점에 교체
    if (isChangingScene && !uFadeManager->IsFading()) 
    { 
        if (uCurrentScene) uCurrentScene->Release();

        uCurrentScene = move(uNextScene);

        FadeInScene(2.0f);

        if (uCurrentScene) uCurrentScene->Initialize();

        isChangingScene = false;
    }

    uFadeManager->Update(deltaTime);

    if (uCurrentScene)
    {
        uCurrentScene->Update(deltaTime);
    }
}

void SceneManager::LateUpdate(float deltaTime)
{
    if (uCurrentScene)
    {
        uCurrentScene->LateUpdate(deltaTime);
    }
}

void SceneManager::Render()
{
    if (uCurrentScene) 
    {
        uCurrentScene->Render();
    }

    uFadeManager->Render();
}

void SceneManager::RenderUI()
{
    if (uCurrentScene)
    {
        uCurrentScene->RenderUI();
    }
}

void SceneManager::Release()
{
    if (uCurrentScene)
        uCurrentScene->Release();
}
