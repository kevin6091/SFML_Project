#include "Title.h"
#include "GameInstance.h"
#include "ResourceManager.h"

Title::Title()
{
}

Title::~Title() {}

void Title::Initialize()
{
    auto& resource = GameInstance::GetInstance().GetResourceManager();
    
#pragma region ZERO
    
    sprite_ZERO = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_zero")));
    sprite_ZER = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_zer")));
    sprite_O = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_o")));

    (*sprite_ZERO).setPosition(Vector2f(WIDTH / 2, (HEIGHT / 2) + 100));
    (*sprite_ZER).setPosition(Vector2f(WIDTH / 2, (HEIGHT / 2) + 100));
    (*sprite_O).setPosition(Vector2f(WIDTH / 2, (HEIGHT / 2) + 100));

#pragma endregion

#pragma region Katana
    
    sprite_Katana = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_katana")));
    sprite_Katana2 = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_katana2")));

    (*sprite_Katana).setPosition(Vector2f(WIDTH / 2, (HEIGHT / 2)));

#pragma endregion

#pragma region BackGround
    
    sprite_BG = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_bg")));
    sprite_Fence = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_fence")));

    (*sprite_BG).setPosition(Vector2f(WIDTH / 2, HEIGHT / 2));
    (*sprite_Fence).setPosition(Vector2f(WIDTH / 2, HEIGHT / 2));

    FloatRect bounds = (*sprite_BG).getLocalBounds();
    float scaleX = WIDTH / bounds.size.x;
    (*sprite_BG).setScale({ scaleX, 1.0f });
    (*sprite_Fence).setScale({ scaleX, 1.0f });

#pragma endregion

    
    // 1. ResourceManager에서 텍스처 시퀀스 가져와 애니메이션 클립 생성
    Init_Sprites(animator_run, sprite_run, "zero_run", 0.1f, true);

    // 2. 기본 애니메이션 시작
    animator_run.Play("zero_run");
    animator_run.UpdateSpriteTexture(*sprite_run); // 첫 프레임 초기화
    (*sprite_run).setPosition(Vector2f(WIDTH / 2, HEIGHT / 2));
    (*sprite_run).setScale(Vector2f(3.0f, 3.0f));



#pragma region SetPosition
    position = Vector2f(WIDTH / 2, -2 * HEIGHT);
#pragma endregion

}

void Title::Update(float deltaTime)
{
    //animator_run.Update(deltaTime, *sprite_run);
    
    if (position.y >= 360)
        position.y = 360;
    else
    {
        accTargetMove += deltaTime * deltaTime;
        position.y += HEIGHT * accTargetMove;
    }

    if (isZER == false)
    {
        accZER += deltaTime;
        if (accZER >= 0.2f)
        {
            accZER = 0.0f;
            isZER = true;
        }
    }
    else
    {
        accZER += deltaTime;
        if (accZER >= 3.0f)
        {
            accZER = 0.0f;
            isZER = false;
        }
    }

    if (isO == false)
    {
        accO -= deltaTime;
        if (accO <= 0.0f)
        {
            accO = (float)RandomInt(1, 20) / 10.0f;
            isO = true;
        }
    }
    else
    {
        accO -= deltaTime;
        if (accO <= 0.0f)
        {
            accO = (float)RandomInt(1, 3) / 10.0f;
            isO = false;
        }
    }
}

void Title::LateUpdate(float deltaTime)
{
}

void Title::Render()
{
    GameInstance::GetInstance().Draw(*this, BlendAlpha);
}

void Title::Release()
{
    animator_run.Release();
}

void Title::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(*sprite_BG, states);
    target.draw(*sprite_Fence, states);
    
    target.draw(*sprite_ZERO, states);
    
    if(isZER)
        target.draw(*sprite_ZER, states);
    if(isO)
        target.draw(*sprite_O, states);
    
    target.draw(*sprite_Katana, states);

    //target.draw(*sprite_run, states);
}
