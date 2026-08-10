#include "Blood.h"
#include "GameInstance.h"
#include "ResourceManager.h"

Blood::Blood()
{
	eObjectTag = EObjectTag::Default;
	eRenderLayer = ERenderLayer::Effect;
}

Blood::~Blood()
{
}

void Blood::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();
	
	for (int i = 0; i < 10; i++)
	{
        int randomInt = RandomInt(0, 2);
        float randomFloat = RandomFloat(0.02f, 0.08f);
		vecSprites.push_back(Sprite(*resource.GetTexture("default")));
        vecAnimator.push_back(make_unique<Animator>());
        vecAnimator[i]->AddClip(("anim_blood_" + to_string(randomInt)), randomFloat, false);
        vecAnimator[i]->Play("anim_blood_" + to_string(randomInt), true);
        vecAnimator[i]->UpdateSpriteTexture(*vecSprites[i], false);

        vecSprites[i]->setScale({ RandomFloat(1.0,2.0), RandomFloat(1.5,2.5) });
	}

    Angle baseAngle = radians(atan2(0.f, 0.f));

    for (int i = 0; i < 10; i++)
    {
        FloatRect bounds = vecSprites[i]->getLocalBounds();
        vecSprites[i]->setOrigin({ bounds.position.x + bounds.size.x * 0.5f,
                                  bounds.position.y + bounds.size.y * 0.5f });

        Angle offsetAngle;
        if (RandomInt(0, 1))
            offsetAngle = degrees((float)RandomInt(-70, 70));
        else
            offsetAngle = degrees((float)RandomInt(110, 250));

        vecSprites[i]->setRotation(baseAngle + offsetAngle);
        Angle finalAngle = baseAngle + offsetAngle;
        vecSprites[i]->setRotation(finalAngle);

        float offsetDistance = (float)RandomInt(-1, 1);

        float rad = finalAngle.asRadians();
        Vector2f dirOffset(cos(rad), sin(rad));

        Vector2f finalPosition = descStatus.vSpawnPoint + (dirOffset * offsetDistance);

        vecSprites[i]->setPosition(finalPosition);
    }
}

void Blood::Update(float deltaTime)
{
    for (int i = 0; i < 8; i++)
    {
        vecAnimator[i]->Update(deltaTime, *vecSprites[i], false);
    }
}

void Blood::LateUpdate(float deltaTime)
{
    bool isFinished = false;
    for (auto& animator : vecAnimator)
    {
        if (animator->IsFinished())
            isFinished = true;
    }

    if (isFinished)
        Destroy();
}

void Blood::Render()
{
    GameInstance::GetInstance().GetBloodShader().setUniform("currentTexture", Shader::CurrentTexture);
    RenderStates states(&GameInstance::GetInstance().GetBloodShader());
    GameInstance::GetInstance().GetRenderTarget_Effect().draw(*this, states);
}

void Blood::draw(RenderTarget& target, RenderStates states) const
{
    for (auto& spr : vecSprites)
    {
        target.draw(*spr, states);
    }
}

void Blood::Release()
{
    for (auto& animator : vecAnimator)
        animator->Release();
    vecAnimator.clear();
}