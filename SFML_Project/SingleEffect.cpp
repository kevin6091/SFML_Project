#include "SingleEffect.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "Collider.h"

SingleEffect::SingleEffect()
{
	eObjectTag = EObjectTag::EnemyAttack;
	eRenderLayer = ERenderLayer::Effect;
}

SingleEffect::~SingleEffect()
{
}

void SingleEffect::Initialize()
{
#pragma region Resource

	auto& resource = GameInstance::GetInstance().GetResourceManager();

	sprite.emplace(*resource.GetTexture("default"));

	switch (eEffectType)
	{
	case Bullet_Smoke:
		animator.AddClip(BULLET_SMOKE, 0.08f, false);
		animator.Play(BULLET_SMOKE, true);
		sprite->setScale({ 1.3f,1.3f });
		break;

	case Bullet_Parry:
		animator.AddClip(BULLET_PARRY, 0.08f, false);
		animator.Play(BULLET_PARRY, true);
		sprite->setScale({ 1.3f,1.3f });
		break;

	case Bullet_Spark:
		animator.AddClip(BULLET_SPARK, 0.08f, false);
		animator.Play(BULLET_SPARK, true);
		sprite->setScale({ 1.3f,1.3f });
		break;

	default:
		break;
	}

	auto size = sprite->getTexture().getSize();
	sprite->setOrigin({ (float)size.x * 0.5f, (float)size.y * 0.5f });

#pragma endregion

	SetFace(descStatus.bFace);
	SetPosition(descStatus.vSpawnPoint);
}

void SingleEffect::Update(float deltaTime)
{
	animator.Update(deltaTime, *sprite, true);
	if (animator.IsFinished())
		Destroy();
}

void SingleEffect::LateUpdate(float deltaTime)
{
}

void SingleEffect::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*this, BlendAlpha);
}

void SingleEffect::Release()
{
	animator.Release();
}

void SingleEffect::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}
