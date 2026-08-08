#include "Player.h"
#include "GameInstance.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "PlayerFSM.h"
#include "Slash.h"
#include "ObjectManager.h"
#include "RewindManager.h"
#include "RewindTracker.h"

Player::Player()
{
	eObjectTag = EObjectTag::Player;
	eRenderLayer = ERenderLayer::Actor;
	speed = 250.f;
}

Player::~Player()
{
}

void Player::Initialize()
{
#pragma region Resource

	auto& resource = GameInstance::GetInstance().GetResourceManager();
	
	animator.AddClip(PLAYER_IDLE, 0.08f, true);
	animator.AddClip(PLAYER_RUN, 0.05f, true);
	animator.AddClip(PLAYER_IDLE_TO_RUN, 0.05f, false);
	animator.AddClip(PLAYER_RUN_TO_IDLE, 0.1f, false);
	
	animator.AddClip(PLAYER_CROUCH, 0.05f, false);
	animator.AddClip(PLAYER_CROUCH_TO_IDLE, 0.05f, false);

	animator.AddClip(PLAYER_ROLL, 0.05f, false);
	animator.AddClip(PLAYER_JUMP, 0.1f, true);
	animator.AddClip(PLAYER_FALL, 0.1f, true);
	animator.AddClip(PLAYER_WALLSLIDE, 0.1f, true);
	animator.AddClip(PLAYER_FLIP, 0.03f, false);
	
	animator.AddClip(PLAYER_ATTACK, 0.02f, false);

	sprite.emplace(*resource.GetTexture("default"));

#pragma endregion

#pragma region FSM

	curState = EPlayerState::Idle;
	curFSM = make_unique<Player_Idle>(*this);
	curFSM->Enter();

#pragma endregion

	uCollider = make_unique<Collider>(this);
	Vector2f topLeftOffset = { -9.f, -44.f };
	ColliderDesc desc = { "", EColliderType::DynamicObject, FloatRect(topLeftOffset, {18.f, 44.f}) };	
	uCollider->Initialize(desc);

	SetPosition(descStatus.vSpawnPoint);
	sprite->setScale({ 1.5f,1.5f });

	uptr<Slash> slash = make_unique<Slash>(this);
	pSlash = slash.get();
	auto& objectManager = GameInstance::GetInstance().GetObjectManager();
	objectManager.AddObject(EObjectTag::PlayerAttack, ERenderLayer::Effect, move(slash));
}

void Player::Update(float deltaTime)
{
#pragma region Rewind

	if (RewindManager::GetInstance().IsRewinding())
	{
		PlayerSnapshot snap;
		rewindSpeed += deltaTime * 0.5f;

		if (rewinder.Rewind((int)rewindSpeed, snap))

		{
			SetPosition(snap.position);
			velocity = snap.velocity;
			sprite->setScale(snap.scale);

			if (snap.texture != nullptr)
				sprite->setTexture(*snap.texture);

			sprite->setTextureRect(snap.textureRect);
			curState = snap.fsmState;
		}
		else
		{
			rewindSpeed = 1.0f;
			RewindManager::GetInstance().SetRewinding(false); // 전체동기화필요.
		}

		return;
	}

#pragma endregion

#pragma region Velocity

	velocity.y += GRAVITY * deltaTime * gravityFactor;
	if (velocity.y > MAX_FALL)
		velocity.y = MAX_FALL;

	Vector2f newVelocity = velocity;
	float damping = GRAVITY * deltaTime * 0.5f;
	if (newVelocity.x > 0.0f)
	{
		newVelocity.x -= damping;
		if (newVelocity.x < 0.0f) newVelocity.x = 0.0f;
	}
	else if (newVelocity.x < 0.0f)
	{
		newVelocity.x += damping;
		if (newVelocity.x > 0.0f) newVelocity.x = 0.0f;
	}
	velocity = newVelocity;

#pragma endregion

	if ((accAttackCool += deltaTime) >= 1.0f)
		attackCount = 0;

	if (curFSM)
	{
		if (uptr<PlayerFSM> nextFSM = curFSM.get()->Update(deltaTime))
		{
			curFSM->Exit();
			curFSM = move(nextFSM);
			curFSM->Enter();
		}
	}

	if (grippableEnd <= position.y || isGrounded)
		isGrippable = false;

	animator.Update(deltaTime, *sprite);

	TrailUpdate(deltaTime);

#pragma region Rewind

	PlayerSnapshot currentSnap
	{
		GetPosition(),
		velocity,
		sprite->getScale(),
		curState,
		sprite->getTextureRect(),
		& sprite->getTexture()
	};

	if ((rewinderTime += deltaTime) >= deltaTime * 3.f)
	{
		rewinderTime = 0.f;
		rewinder.Record(currentSnap);
	}

#pragma endregion
}

void Player::LateUpdate(float deltaTime)
{
}

void Player::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Player().draw(*this, BlendAlpha);
}

void Player::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
	TrailRender(target);

	RectangleShape debugBox(uCollider->GetBounds().size);
	debugBox.setPosition(uCollider->GetBounds().position);
	debugBox.setFillColor(Color(0, 0, 255, 10));

	debugBox.setOutlineThickness(1.0f);

	if(GameInstance::GetInstance().GetIsRenderDebug())
		target.draw(debugBox);
}

void Player::CollisionEvent(GameObject& other)
{
	if (other.GetCollider().GetColliderType() == EColliderType::Grippable)
	{
		if ((grippableEnd = other.GetCollider().GetBounds().getCenter().y + (other.GetCollider().GetBounds().size.y / 2.f)) >= position.y)
			isGrippable = true;
	}
}

void Player::RestartObject()
{
	SetPosition(descStatus.vSpawnPoint);
	velocity = { 0.0f,0.0f };
	descStatus.bFace = true;
	if (sprite->getScale().x <= 0.f)
		sprite->setScale({ -1.f,1.f });

	ForceChangeFSM(make_unique<Player_Idle>(*this));

	rewindSpeed = 1.0f;
	rewinder.Clear();
	isGrounded = true;
}

void Player::ForceChangeFSM(uptr<PlayerFSM> fsm)
{
	curFSM->Exit();
	curFSM = move(fsm);
	curFSM->Enter();
}

void Player::TrailUpdate(float deltaTime)
{
	if (curState == EPlayerState::Flip ||
		GameInstance::GetInstance().GetIsSlow())
	{
		trail_SpawnInteval = 0.005f;
		trail_LifeTime = 0.4f;
	}
	else if (accAttackCool <= 1.0f)
	{
		trail_SpawnInteval = 0.005f;
		trail_LifeTime = 0.15f;
	}
	else
	{
		trail_SpawnInteval = 0.01f;
		trail_LifeTime = 0.2f;
	}

	if (curState == EPlayerState::Roll ||
		curState == EPlayerState::Attack || 
		curState == EPlayerState::Jump ||
		curState == EPlayerState::Fall || 
		curState == EPlayerState::Flip || 
		GameInstance::GetInstance().GetIsSlow())
	{
		trailSpawnTimer += deltaTime;
		// 1프레임 동안 여러 번 생성 주기가 지났다면 그만큼 잔상을 여러 개 생성
		while (trailSpawnTimer >= trail_SpawnInteval)
		{
			trailSpawnTimer -= trail_SpawnInteval; //시간을 이월

			TrailInfo t{ *sprite, trail_LifeTime, trail_LifeTime };
			t.sprite.setColor(Color(0, 255, 255, 50));
			trails.push_back(t);
		}
	}
	else
	{
		trailSpawnTimer = 0.f;
	}

	// Trail 업데이트 (투명해지다가 수명이 다하면 삭제)
	for (auto it = trails.begin(); it != trails.end(); )
	{
		it->lifeTime -= deltaTime;

		if (it->lifeTime <= 0.f)
		{
			it = trails.erase(it);
		}
		else
		{
			float ratio = it->lifeTime / it->maxLifeTime;

			Color c = it->sprite.getColor();
			c.a = static_cast<uint8_t>(50 * ratio);
			it->sprite.setColor(c);

			++it;
		}
	}
}

void Player::TrailRender(RenderTarget& target) const
{
	for (const auto& t : trails)
	{
		target.draw(t.sprite, BlendAdd);
	}
}

void Player::Release()
{
	animator.Release();
}
