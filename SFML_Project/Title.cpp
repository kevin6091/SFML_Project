#include "Title.h"
#include "GameInstance.h"
#include "ResourceManager.h"

Title::Title()
{
    eObjectTag = EObjectTag::Default;
    eRenderLayer = ERenderLayer::Background;
}

Title::~Title() {}

void Title::Initialize()
{
    auto& resource = GameInstance::GetInstance().GetResourceManager();
    
#pragma region ZERO
    
    sprite_ZERO = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_zero")));
    sprite_ZER = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_zer")));
    sprite_O = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_o")));

    (*sprite_ZERO).setPosition(Vector2f(WIDTH / 2, (HEIGHT / 2) + 60));
    (*sprite_ZER).setPosition(Vector2f(WIDTH / 2, (HEIGHT / 2) + 60));
    (*sprite_O).setPosition(Vector2f(WIDTH / 2, (HEIGHT / 2) + 60));

    (*sprite_ZERO).setScale(Vector2f(1.2f, 1.2f));
    (*sprite_ZER).setScale(Vector2f(1.2f, 1.2f));
    (*sprite_O).setScale(Vector2f(1.2f, 1.2f));

#pragma endregion

#pragma region Katana
    
    sprite_Katana = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_katana")));

    (*sprite_Katana).setPosition(Vector2f(WIDTH / 2, (HEIGHT / 2) - 40));
    (*sprite_Katana).setScale(Vector2f(1.2f, 1.2f));

#pragma endregion

#pragma region BackGround
    
    sprite_BG = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_bg")));
    sprite_Fence = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_fence")));

    (*sprite_BG).setPosition(Vector2f(WIDTH / 2, HEIGHT / 2 - 20));
    (*sprite_Fence).setPosition(Vector2f(WIDTH / 2, HEIGHT / 2 - 20));

    FloatRect bounds = (*sprite_BG).getLocalBounds();
    float scaleX = WIDTH / bounds.size.x;
    (*sprite_BG).setScale({ scaleX, 1.0f });
    (*sprite_Fence).setScale({ scaleX, 1.0f });

#pragma endregion

#pragma region Plant

    sprite_grass = CenterAlign_Sprite(Sprite(*resource.GetTexture("title_grass")));
    animator.AddClip("anim_title_plant", 0.1f, true);
    animator.Play("anim_title_plant");
    seq_sprite_plant.emplace(*resource.GetTexture("title_zero"));

    (*seq_sprite_plant).setPosition(Vector2f(WIDTH / 2, HEIGHT + 80));
    (*seq_sprite_plant).setScale({ scaleX, 1.5f });
    (*sprite_grass).setPosition(Vector2f(WIDTH / 2, HEIGHT - 20));
    (*sprite_grass).setScale({ scaleX, 1.5f });

#pragma endregion

#pragma region SetPosition
    position = Vector2f(WIDTH / 2, -2 * HEIGHT);
#pragma endregion

}

void Title::Update(float deltaTime)
{
    animator.Update(deltaTime, *seq_sprite_plant);

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
            accO = (float)RandomInt(1, 10) / 10.0f;
            isO = true;
        }
    }
    else
    {
        accO -= deltaTime;
        if (accO <= 0.0f)
        {
            accO = (float)RandomInt(1, 2) / 10.0f;
            isO = false;
        }
    }
}

void Title::LateUpdate(float deltaTime)
{
}

void Title::Render()
{
    GameInstance::GetInstance().GetRenderTarget_BG().draw(*this, BlendAlpha);
}

void Title::Release()
{
    animator.Release();
}

void Title::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(*sprite_BG, states);
    target.draw(*sprite_Fence, states);

    target.draw(*sprite_ZERO, states);

    if (isZER)
        target.draw(*sprite_ZER, states);
    if (isO)
        target.draw(*sprite_O, states);

    target.draw(*sprite_Katana, states);

    target.draw(*seq_sprite_plant, states);
    target.draw(*sprite_grass, states);
}
