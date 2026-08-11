#include "WallSlide_Dust.h"
#include "GameInstance.h"
#include "ResourceManager.h"

WallSlide_Dust::WallSlide_Dust()
{
	eObjectTag = EObjectTag::Default;
	eRenderLayer = ERenderLayer::Effect;
}

WallSlide_Dust::~WallSlide_Dust()
{
}

void WallSlide_Dust::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();

	animator.AddClip(WALLSLIDE_DUST, RandomFloat(0.03f, 0.1f), false);
	sprite.emplace(*resource.GetTexture("default"));

	animator.UpdateSpriteTexture(*sprite, true);
	SetPosition(descStatus.vSpawnPoint);
	float ranFloat = RandomFloat(1.0f, 1.4f);
	sprite->setScale({ ranFloat,ranFloat });
	SetFace(descStatus.bFace);

	animator.Play(WALLSLIDE_DUST, true);
}

void WallSlide_Dust::Update(float deltaTime)
{
	Vector2f pos = GetPosition();
	if (sprite->getScale().x > 0)
	{
		Vector2f vel = Vector2f(1.f, -RandomFloat(0.2f, 1.0f)).normalized();
		pos += vel * deltaTime * RandomFloat(50.f, 100.f);
	}
	else
	{
		Vector2f vel = Vector2f(-1.f, -RandomFloat(0.2f, 1.0f)).normalized();
		pos += vel * deltaTime * RandomFloat(50.f, 100.f);
	}

	SetPosition(pos);

	animator.Update(deltaTime, *sprite, true);
	if (animator.IsFinished())
		Destroy();
}

void WallSlide_Dust::LateUpdate(float deltaTime)
{
}

void WallSlide_Dust::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*this, BlendAlpha);
}

void WallSlide_Dust::Release()
{
}

void WallSlide_Dust::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}
