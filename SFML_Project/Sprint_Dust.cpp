#include "Sprint_Dust.h"
#include "GameInstance.h"
#include "ResourceManager.h"

Sprint_Dust::Sprint_Dust()
{
	eObjectTag = EObjectTag::Default;
	eRenderLayer = ERenderLayer::Effect;
}

Sprint_Dust::~Sprint_Dust()
{
}

void Sprint_Dust::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();

	animator.AddClip(SPRINT_DUST, 0.05f, false);
	sprite.emplace(*resource.GetTexture("default"));

	animator.UpdateSpriteTexture(*sprite, true);
	SetPosition(descStatus.vSpawnPoint);
	sprite->setScale({ 1.f,1.f });
	SetFace(descStatus.bFace);

	animator.Play(SPRINT_DUST, true);
}

void Sprint_Dust::Update(float deltaTime)
{
	Vector2f pos = GetPosition();
	if (sprite->getScale().x > 0)
	{
		Vector2f vel = Vector2f(1.f, -RandomFloat(0.2f, 0.7f)).normalized();
		pos += vel * deltaTime * 3.f;
	}
	else
	{
		Vector2f vel = Vector2f(-1.f, -RandomFloat(0.2f, 0.7f)).normalized();
		pos += vel * deltaTime * 3.f;
	}

	SetPosition(pos);

	animator.Update(deltaTime, *sprite, true);
	if (animator.IsFinished())
		Destroy();
}

void Sprint_Dust::LateUpdate(float deltaTime)
{
}

void Sprint_Dust::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*this, BlendAlpha);
}

void Sprint_Dust::Release()
{
}

void Sprint_Dust::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}
