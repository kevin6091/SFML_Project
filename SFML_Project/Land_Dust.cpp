#include "Land_Dust.h"
#include "GameInstance.h"
#include "ResourceManager.h"

Land_Dust::Land_Dust()
{
	eObjectTag = EObjectTag::Default;
	eRenderLayer = ERenderLayer::Effect;
}

Land_Dust::~Land_Dust()
{
}

void Land_Dust::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();

	animator.AddClip(LAND_DUST, 0.05f, false);
	sprite.emplace(*resource.GetTexture("default"));

	animator.UpdateSpriteTexture(*sprite, true);
	SetPosition(descStatus.vSpawnPoint);
	sprite->setScale({ 1.3f, 1.3f });
	SetFace(descStatus.bFace);

	animator.Play(LAND_DUST, true);
}

void Land_Dust::Update(float deltaTime)
{
	animator.Update(deltaTime, *sprite, true);
	if (animator.IsFinished())
		Destroy();
}

void Land_Dust::LateUpdate(float deltaTime)
{
}

void Land_Dust::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*this, BlendAlpha);
}

void Land_Dust::Release()
{
}

void Land_Dust::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}
