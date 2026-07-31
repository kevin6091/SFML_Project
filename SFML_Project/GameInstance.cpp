#include "GameInstance.h"
#include "ResourceManager.h"
#include "SceneManager.h"

GameInstance::GameInstance() = default;
GameInstance::~GameInstance() = default;

void GameInstance::Initialize(uint width, uint height, const string& title)
{
    window.create(sf::VideoMode({ width, height }), title);

    uResourceManager = uptr<ResourceManager>(new ResourceManager());
    uSceneManager  = uptr<SceneManager>(new SceneManager());
}

void GameInstance::Run()
{
    sf::Clock clock;

    while (window.isOpen()) 
    {
        while (const optional event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
            }
        }

        float rawDeltaTime = clock.restart().asSeconds();
        float gameDeltaTime = rawDeltaTime * timeScale; // 카타나 제로 시간 감속 적용

        // Update & Render
        //sceneManager->Update(gameDt);

        window.clear(sf::Color(15, 15, 20));
        //sceneManager->Render(window);
        window.display();
    }
}