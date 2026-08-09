#include "SlashImpact.h"
#include "GameInstance.h"
#include "ResourceManager.h"

SlashImpact::SlashImpact()
{
	eObjectTag = EObjectTag::Default;
	eRenderLayer = ERenderLayer::Effect;
}

SlashImpact::~SlashImpact()
{
}

void SlashImpact::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();

	animator.AddClip(SLASH_IMPACT1, 0.05f, false);
	animator.AddClip(SLASH_IMPACT2, 0.03f, false);
	sprite.emplace(*resource.GetTexture("default"));

	if (impactNum == 1)
		animator.Play(SLASH_IMPACT1, true);
	else if (impactNum == 2)
		animator.Play(SLASH_IMPACT2, true);

	SetPosition(descStatus.vSpawnPoint + Vector2f(0.f, -50.f));

	animator.UpdateSpriteTexture(*sprite, true);
	sprite->setScale({ 1.5f, 1.5f });
	Vector2f dir = hitDir;
	float angleRadian = atan2(dir.y, dir.x);
	sprite->setRotation(radians(angleRadian));
}

void SlashImpact::Update(float deltaTime)
{
	animator.Update(deltaTime, *sprite);

	if (animator.IsFinished())
		Destroy();
}

void SlashImpact::LateUpdate(float deltaTime)
{
}

void SlashImpact::Render()
{
}

void SlashImpact::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}

void SlashImpact::Release()
{
	animator.Release();
}
