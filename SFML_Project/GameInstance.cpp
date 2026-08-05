#include "GameInstance.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Camera.h"

GameInstance::GameInstance() = default;
GameInstance::~GameInstance() = default;

void GameInstance::Initialize(uint width, uint height, const string& title)
{
    window.create(VideoMode({ width, height }), title);
    
    uCamera = uptr<Camera>(new Camera());
    uResourceManager = uptr<ResourceManager>(new ResourceManager());
    uObjectManager = uptr<ObjectManager>(new ObjectManager());
    uSceneManager  = uptr<SceneManager>(new SceneManager());
    uCollisionManager = uptr<CollisionManager>(new CollisionManager());
    uInputManager = uptr<InputManager>(new InputManager());

    uCamera->Initialize(Vector2f((float)width, (float)height));
    uResourceManager->Initialzie();
    uObjectManager->Initialize();
    uSceneManager->Initialize();
    uInputManager->Initialize();

    uSceneManager->ChangeScene(ESceneType::Title, 0.0f);
}

void GameInstance::Run()
{
    Clock clock;

    while (window.isOpen()) 
    {
        while (const optional event = window.pollEvent()) 
        {
            if (event->is<Event::Closed>()) 
            {
                window.close();
            }
        }

        float rawDeltaTime = clock.restart().asSeconds();
        float gameDeltaTime = rawDeltaTime * timeScale;
        
        uInputManager->Update();
        uSceneManager->Update(gameDeltaTime);
        uSceneManager->LateUpdate(gameDeltaTime);
        uCamera->Update(rawDeltaTime);

        window.clear(Color(0, 0, 0));

        window.setView(uCamera->GetView());
        uSceneManager->Render();

        window.setView(window.getDefaultView());
        uSceneManager->RenderUI();

        window.display();
    }
}

void GameInstance::Draw(const GameObject& gameObject, RenderStates states)
{
    window.draw(gameObject, states);
}

void GameInstance::Release()
{
    uSceneManager->Release();
    uObjectManager->Release();
    uResourceManager->Release();
    uInputManager->Release();
}