#include "Door.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "SingleEffect.h"
#include "Block.h"
#include "Player.h"
#include "SoundManager.h"

Door::Door()
{
	eObjectTag = EObjectTag::Door;
	eRenderLayer = ERenderLayer::Background;
}

Door::~Door()
{
}

void Door::Initialize()
{
#pragma region Resource

	auto& resource = GameInstance::GetInstance().GetResourceManager();

	sprite.emplace(*resource.GetTexture("default"));
	animator.AddClip(DOOR_OPEN, 0.08f, false);
	
#pragma endregion

	animator.Play(DOOR_OPEN, true);
	animator.UpdateSpriteTexture(*sprite, true);

	auto size = sprite->getTexture().getSize();
	sprite->setOrigin({ size.x * 0.5f, 0.f});


	descStatus.bFace ? sprite->setScale({ 2.f, 2.f }) : sprite->setScale({ -2.f, 2.f });
	bFirstFace = descStatus.bFace;

	Vector2f offset;
	descStatus.bFace ? offset = Vector2f{ -32.f, 0.f } : offset = Vector2f{ 0.f, 0.f };
	position = descStatus.vSpawnPoint;
	sprite->setPosition(descStatus.vSpawnPoint + offset);
}

void Door::Update(float deltaTime)
{
	if(isDoorOpen)
		animator.Update(deltaTime, *sprite);
	sprite->setOrigin({ 0.f, 0.f });

	float disX = GameInstance::GetInstance().GetPlayer()->GetPosition().x - GetPosition().x;
	float disY = GameInstance::GetInstance().GetPlayer()->GetPosition().y - (GetPosition().y + 64.f);
	if (abs(disX) <= 80.f && GameInstance::GetInstance().GetPlayer()->GetState() == EPlayerState::Attack)
	{
		if (abs(disY) <= 100.f)
		{
			if (!isDoorOpen)
			{
				isDoorOpen = true;
				SoundManager::GetInstance().PlaySFXWithReverb(S_DOOR_OPEN, 50.f);
			}
		}
	}
}

void Door::LateUpdate(float deltaTime)
{
}

void Door::Render()
{
	GameInstance::GetInstance().GetRenderTarget_BG().draw(*this, BlendAlpha);
}

void Door::Release()
{
	animator.Release();
}

void Door::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}

void Door::CollisionEvent(GameObject& other)
{

}

void Door::RestartObject()
{
	Vector2f offset;
	descStatus.bFace ? offset = Vector2f{ 0.f, 0.f } : offset = Vector2f{ -32.f, 0.f };

	if (isDoorOpen)
	{
		ColliderDesc col{ "", EColliderType::Block ,FloatRect(descStatus.vSpawnPoint + offset, Vector2f{32.f, 128.f}) };
		auto block = make_unique<Block>(col);
		block->SetIsDoor(true);
		GameInstance::GetInstance().GetObjectManager().AddObject(EObjectTag::Wall, ERenderLayer::Background, move(block));
	}
	
	isDoorOpen = false;
	animator.Play(DOOR_OPEN, true);
	animator.UpdateSpriteTexture(*sprite, true);
	auto size = sprite->getTexture().getSize();
	sprite->setOrigin({ size.x * 0.5f, 0.f });
	sprite->setOrigin({ 0.f, 0.f });

	descStatus.bFace ? offset = Vector2f{ -32.f, 0.f } : offset = Vector2f{ 0.f, 0.f };

	position = descStatus.vSpawnPoint;
	sprite->setPosition(descStatus.vSpawnPoint + offset);
}
