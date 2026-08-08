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

	animator.AddClip(SLASH_IMPACT1, 0.03f, false);
	animator.AddClip(SLASH_IMPACT2, 0.03f, false);
	sprite.emplace(*resource.GetTexture("default"));

	if (impactNum == 1)
		animator.Play(SLASH_IMPACT1, true);
	else if (impactNum == 2)
		animator.Play(SLASH_IMPACT2, true);

	Vector2f dir = -hitDir;
	float angleRadian = atan2(dir.y, dir.x);
	sprite->setRotation(radians(angleRadian));
	
	if (dir.x >= 0.f)
		sprite->setScale({ 1.3f, 1.3f });
	else
		sprite->setScale({ 1.3f, -1.3f });

	if(impactNum == 1)
		sprite->setPosition(position);
	else
		sprite->setPosition(position + Vector2f(0.f, 20.f));

}

void SlashImpact::Update(float deltaTime)
{
	if(impactNum == 1)
		animator.Update(deltaTime, *sprite, false);
	else
		animator.Update(deltaTime, *sprite);


	if (animator.IsFinished())
		Destroy();
}

void SlashImpact::LateUpdate(float deltaTime)
{
}

void SlashImpact::Render()
{
	RenderStates states(&GameInstance::GetInstance().GetSlashImpactShader());
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*sprite, states);
}

void SlashImpact::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}

void SlashImpact::Release()
{
	animator.Release();
}
