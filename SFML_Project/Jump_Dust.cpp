#include "Jump_Dust.h"
#include "GameInstance.h"
#include "ResourceManager.h"

Jump_Dust::Jump_Dust()
{
	eObjectTag = EObjectTag::Default;
	eRenderLayer = ERenderLayer::Effect;
}

Jump_Dust::~Jump_Dust()
{
}

void Jump_Dust::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();

	animator.AddClip(JUMP_DUST, 0.08f, false);
	sprite.emplace(*resource.GetTexture("default"));

	animator.UpdateSpriteTexture(*sprite, true);
	SetPosition(descStatus.vSpawnPoint);
	sprite->setScale({ 1.3f,1.3f });
	SetFace(descStatus.bFace);

	animator.Play(JUMP_DUST, true);
}

void Jump_Dust::Update(float deltaTime)
{
	animator.Update(deltaTime, *sprite, true);
	if (animator.IsFinished())
		Destroy();
}

void Jump_Dust::LateUpdate(float deltaTime)
{
}

void Jump_Dust::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*this, BlendAlpha);
}

void Jump_Dust::Release()
{
}

void Jump_Dust::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}
