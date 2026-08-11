#include "GameInstance.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "RewindManager.h"
#include "Player.h" 

GameInstance::GameInstance() = default;
GameInstance::~GameInstance() = default;

void GameInstance::Initialize(uint width, uint height, const string& title)
{
    window.create(VideoMode({ width, height }), title);
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(240);

    if (!renderTarget_BG.resize(Vector2u(width, height)))
        cout << "렌더타겟 생성실패!!!" << endl;

    if (!renderTarget_Actor.resize(Vector2u(width, height)))
        cout << "렌더타겟 생성실패!!!" << endl;

    if (!renderTarget_Player.resize(Vector2u(width, height)))
        cout << "렌더타겟 생성실패!!!" << endl;

    if (!renderTarget_Effect.resize(Vector2u(width, height)))
        cout << "렌더타겟 생성실패!!!" << endl;

    if (!renderTarget_Effect_Glow.resize(Vector2u(width, height)))
        cout << "렌더타겟 생성실패!!!" << endl;

    if (!renderTarget_Composite.resize(Vector2u(width, height)))
        cout << "렌더타겟 생성실패!!!" << endl;

    if (!renderTarget_Final.resize(Vector2u(width, height)))
        cout << "렌더타겟 생성실패!!!" << endl;

    if (!Shader::isAvailable())
        cout << "셰이더 로드 실패!!!" << endl;
   
    if (!postShader.loadFromFile("shader.frag", Shader::Type::Fragment))
        cout << "셰이더 로드 실패!!!" << endl;
   
    if (!compositeShader.loadFromFile("composite_shader.frag", Shader::Type::Fragment))
        cout << "셰이더 로드 실패!!!" << endl;
   
    if (!glowShader.loadFromFile("glow_shader.frag", Shader::Type::Fragment))
        cout << "셰이더 로드 실패!!!" << endl;
   
    if (!bloodShader.loadFromFile("blood_shader.frag", Shader::Type::Fragment))
        cout << "셰이더 로드 실패!!!" << endl;
   
    if (!fanShader.loadFromFile("fan_Shader.frag", Shader::Type::Fragment))
        cout << "셰이더 로드 실패!!!" << endl;
   
    if (!yShader.loadFromFile("y_shader.frag", Shader::Type::Fragment))
        cout << "셰이더 로드 실패!!!" << endl;

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

#pragma region Update

        if (uInputManager->GetKeyPress(Keyboard::Key::LShift))
        {
            isSlow = true;
            timeScale = 0.3f;
        }
        else
        {
            isSlow = false; 
            timeScale = 1.f;
            accSlow = 0.0f;
        }

        float rawDeltaTime = clock.restart().asSeconds();
        float gameDeltaTime = rawDeltaTime * timeScale;
        if ((accSlow += gameDeltaTime) >= 0.5f)
            accSlow = 0.5f;

        uInputManager->Update();
        uSceneManager->Update(gameDeltaTime);
        uSceneManager->LateUpdate(gameDeltaTime);
        uCamera->Update(rawDeltaTime);
        
        if (uInputManager->GetKeyDown(Keyboard::Key::Tab))
            isRenderDebug = !isRenderDebug;
        if (uInputManager->GetKeyDown(Keyboard::Key::R) && !RewindManager::GetInstance().IsRewinding())
            RewindManager::GetInstance().SetRewinding(true); 

#pragma endregion

#pragma region Render

        // RenderTarget 초기화
        renderTarget_BG.clear(Color(0, 0, 0));
        renderTarget_Actor.clear(Color::Transparent);
        renderTarget_Player.clear(Color::Transparent);
        renderTarget_Effect_Glow.clear(Color::Transparent);
        renderTarget_Effect.clear(Color::Transparent);

        renderTarget_BG.setView(uCamera->GetView());
        renderTarget_Actor.setView(uCamera->GetView());
        renderTarget_Player.setView(uCamera->GetView());
        renderTarget_Effect.setView(uCamera->GetView());
        renderTarget_Effect_Glow.setView(uCamera->GetView());

        // Target에 Render
        uSceneManager->Render();

        //renderTarget_BG.setView(renderTarget_BG.getDefaultView());
        uSceneManager->RenderUI();

        // Target 완성
        renderTarget_BG.display();
        renderTarget_Actor.display();
        renderTarget_Player.display();
        renderTarget_Effect_Glow.display();
        renderTarget_Effect.display();

        renderTarget_Composite.clear(Color(0, 0, 0));
        renderTarget_Final.clear(Color(0, 0, 0));

#pragma region Composite

        // 전처리 Target들 합치기 -> Final
        compositeShader.setUniform("textureBG", renderTarget_BG.getTexture());
        compositeShader.setUniform("textureActor", renderTarget_Actor.getTexture());
        compositeShader.setUniform("texturePlayer", renderTarget_Player.getTexture());
        compositeShader.setUniform("textureEffect", renderTarget_Effect.getTexture());
        compositeShader.setUniform("isSlow", isSlow);
        compositeShader.setUniform("accSlow", accSlow);

        Sprite spriteTemp(renderTarget_BG.getTexture());
        RenderStates compositeStates(&compositeShader);
        renderTarget_Composite.draw(spriteTemp, compositeStates);

#pragma endregion

#pragma region Glow

        renderTarget_Final.setView(renderTarget_Final.getDefaultView());
        glowShader.setUniform("isReverse", isReverse);
        glowShader.setUniform("resolution", Vector2f{ (float)WIDTH, (float)HEIGHT });
        glowShader.setUniform("radius", 4.0f);
        glowShader.setUniform("textureGlow", renderTarget_Effect_Glow.getTexture());
        glowShader.setUniform("textureComposite", renderTarget_Composite.getTexture());

        RenderStates glowStates(&glowShader);
        renderTarget_Final.draw(spriteTemp, glowStates);

#pragma endregion

        Sprite spriteFinal(renderTarget_Final.getTexture());

        window.clear();

        if (RewindManager::GetInstance().IsRewinding())
        {
            if ((accTime1 += gameDeltaTime * 0.5f * 2.f) >= 20.0f)
                accTime1 = 20.0f;
            if ((accTime2 += gameDeltaTime * 1.5f) >= 10.0f)
                accTime2 = 10.0f;

            postShader.setUniform("time", gameDeltaTime);
            postShader.setUniform("accTime1", accTime1);
            postShader.setUniform("accTime2", accTime2);

            postShader.setUniform("textureFinal", renderTarget_Composite.getTexture());

            RenderStates states(&postShader);
            window.draw(spriteFinal, states);
            
        }
        else
        {
            // 평상시
            accTime1 = 0.0f;
            accTime2 = 0.0f;

            window.draw(spriteFinal);
        }

        window.display();

#pragma endregion

    }
}

Player* GameInstance::GetPlayer()
{
    return uObjectManager->GetPlayer();
}

void GameInstance::Release()
{
    uSceneManager->Release();
    uObjectManager->Release();
    uResourceManager->Release();
    uInputManager->Release();
}