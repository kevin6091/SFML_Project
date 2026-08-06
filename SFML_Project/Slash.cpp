#include "Slash.h"
#include "GameInstance.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "Player.h"
#include "RewindManager.h"

Slash::Slash(Player* _player)
{
	player = _player;
	eObjectTag = EObjectTag::Slash;
	eRenderLayer = ERenderLayer::Effect;
}

Slash::~Slash()
{
}

void Slash::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();
	
	animator_main.AddClip(SLASH_MAIN, 0.05f, false);
	animator_sub.AddClip(SLASH_SUB, 0.05f, false);
	

	sprite.emplace(*resource.GetTexture("default"));
	sprite_sub.emplace(*resource.GetTexture("default"));

	animator_main.UpdateSpriteTexture(*sprite);
	animator_sub.UpdateSpriteTexture(*sprite_sub);

	isPlayerAttack = false;
}

void Slash::Update(float deltaTime)
{
#pragma region Rewind

	if (RewindManager::GetInstance().IsRewinding())
	{
		SlashSnapshot snap;
		rewindSpeed += deltaTime * 0.5f;

		if (rewinder.Rewind((int)rewindSpeed, snap))
		{
			sprite->setPosition(snap.position1);
			sprite->setScale(snap.scale1);
			sprite_sub->setPosition(snap.position2);
			sprite_sub->setScale(snap.scale2);

			if (snap.texture1 != nullptr)
				sprite->setTexture(*snap.texture1);
			if(snap.texture2 != nullptr)
				sprite_sub->setTexture(*snap.texture2);

			sprite->setTextureRect(snap.textureRect1);
			sprite_sub->setTextureRect(snap.textureRect2);

			isPlayerAttack = snap.isRender;
		}
		else
		{
			rewindSpeed = 1.0f;
			RewindManager::GetInstance().SetRewinding(false); // 전체동기화필요.
		}

		return;
	}

#pragma endregion

	Vector2f offset{ 0.f, -40.f };
	Vector2f offset_sub{ 0.f, -20.f };
	offset_sub += player->GetAttackDir() * 20.f;
	
	sprite->setPosition(player->GetPosition() + offset);
	sprite_sub->setPosition(player->GetPosition() + offset_sub);

	animator_main.Update(deltaTime, *sprite, false);
	animator_sub.Update(deltaTime, *sprite_sub, true);
	
	if(animator_main.IsFinished() && animator_sub.IsFinished())
		isPlayerAttack = false;

#pragma region Rewind

	SlashSnapshot currentSnap
	{
		sprite->getPosition(),
		sprite_sub->getPosition(),
		sprite->getScale(),
		sprite_sub->getScale(),
		sprite->getTextureRect(),
		sprite_sub->getTextureRect(),
		&sprite->getTexture(),
		&sprite_sub->getTexture(),
		isPlayerAttack
	};

	if ((rewinderTime += deltaTime) >= deltaTime * 3.f)
	{
		rewinderTime = 0.f;
		rewinder.Record(currentSnap);
	}

#pragma endregion

}

void Slash::LateUpdate(float deltaTime)
{
}

void Slash::Render()
{
	if(isPlayerAttack)
	{
		GameInstance::GetInstance().Draw(*this, BlendAlpha);
	}
}

void Slash::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, BlendAlpha);
	target.draw(*sprite_sub, BlendAlpha);
}

void Slash::RestartObject()
{
	isPlayerAttack = false;
	rewinder.Clear();
	rewindSpeed = 1.0f;
}

void Slash::PlayerAttack()
{
	isPlayerAttack = true;
	animator_main.Play(SLASH_MAIN, true);
	animator_sub.Play(SLASH_SUB, true);
	
	Vector2f dir = player->GetAttackDir();
	float angleRadian = atan2(dir.y, dir.x);
	sprite->setRotation(radians(angleRadian));
	sprite_sub->setRotation(radians(angleRadian));

	if (player->GetDesc().bFace)
	{
		sprite->setScale({ 1.3f, 1.3f });
		sprite_sub->setScale({ 1.3f, 1.3f });
	}
	else
	{
		sprite->setScale({ 1.3f, -1.3f });
		sprite_sub->setScale({ 1.3f, -1.3f });
	}
}

void Slash::Release()
{
	animator_main.Release();
	animator_sub.Release();
}
