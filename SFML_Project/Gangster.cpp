#include "Gangster.h"
#include "GameInstance.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "GangsterFSM.h"
#include "ObjectManager.h"
#include "RewindManager.h"
#include "RewindTracker.h"
#include "Player.h"
#include "Blood.h"
#include "BloodDecal.h"

Gangster::Gangster()
{
	eObjectTag = EObjectTag::Enemy;
	eRenderLayer = ERenderLayer::Actor;
	speed = 150.f;
}

Gangster::~Gangster()
{
}

void Gangster::Initialize()
{
#pragma region Resource

	auto& resource = GameInstance::GetInstance().GetResourceManager();

	animator.AddClip(GANGSTER_IDLE, 0.08f, true);
	animator.AddClip(GANGSTER_RUN, 0.05f, true);
	animator.AddClip(GANGSTER_ATTACK, 0.05f, false);
	animator.AddClip(GANGSTER_HIT, 0.05f, false);
	animator.AddClip(GANGSTER_HIT_ROLL, 0.05f, true);
	animator.AddClip(GANGSTER_HIT_GROUND, 0.05f, false);

	sprite.emplace(*resource.GetTexture("default"));

#pragma endregion

#pragma region FSM

	curState = EGangsterState::Idle;
	curFSM = make_unique<Gangster_Idle>(*this);
	curFSM->Enter();

#pragma endregion

	uCollider = make_unique<Collider>(this);
	Vector2f topLeftOffset = { -9.f, -44.f };
	ColliderDesc desc = { "", EColliderType::DynamicObject, FloatRect(topLeftOffset, {18.f, 44.f}) };
	uCollider->Initialize(desc);

	SetPosition(descStatus.vSpawnPoint);
	descStatus.bFace ? sprite->setScale({ 1.5f,1.5f }) : sprite->setScale({ -1.5f,1.5f });
	bFirstFace = descStatus.bFace;
}

void Gangster::Update(float deltaTime)
{
#pragma region Rewind

	if (RewindManager::GetInstance().IsRewinding())
	{
		GangsterSnapshot snap;
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

	velocity.y += GRAVITY * deltaTime;
	if (velocity.y > MAX_FALL)
		velocity.y = MAX_FALL;

#pragma endregion

	accAttackCool += deltaTime;

	dirToPlayer = GameInstance::GetInstance().GetPlayer()->GetPosition() - position;

	if (curFSM)
	{
		if (uptr<GangsterFSM> nextFSM = curFSM.get()->Update(deltaTime))
		{
			curFSM->Exit();
			curFSM = move(nextFSM);
			curFSM->Enter();
		}
	}

	if (!(abs(velocity.x - 0.0f) <= FLT_EPSILON))
	{
		if (velocity.x > 0)
			SetFace(true);
		else
			SetFace(false);
	}

	animator.Update(deltaTime, *sprite);

#pragma region Rewind

	GangsterSnapshot currentSnap
	{
		GetPosition(),
		velocity,
		sprite->getScale(),
		curState,
		sprite->getTextureRect(),
		&sprite->getTexture()
	};

	if ((rewinderTime += deltaTime) >= deltaTime * 3.f)
	{
		rewinderTime = 0.f;
		rewinder.Record(currentSnap);
	}

#pragma endregion

}

void Gangster::LateUpdate(float deltaTime)
{
}

void Gangster::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Actor().draw(*this, BlendAlpha);
}

void Gangster::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);

	RectangleShape debugBox(uCollider->GetBounds().size);
	debugBox.setPosition(uCollider->GetBounds().position);
	debugBox.setFillColor(Color::Transparent);
	if (curState == EGangsterState::Hit)
		debugBox.setOutlineColor(Color::Red);
	else
		debugBox.setOutlineColor(Color::White);


	debugBox.setOutlineThickness(1.0f);

	if (GameInstance::GetInstance().GetIsRenderDebug())
		target.draw(debugBox);
}

void Gangster::RestartObject()
{
	SetPosition(descStatus.vSpawnPoint);
	velocity = { 0.0f,0.0f };
	descStatus.bFace = bFirstFace;

	SetFace(bFirstFace);

	ForceChangeFSM(make_unique<Gangster_Idle>(*this));

	rewindSpeed = 1.0f;
	rewinder.Clear();
	isGrounded = true;
	isActive = true;
	isFindPlayer = false;
	accAttackCool = 0.0f;
	accBloodTime = 1.f;
}

void Gangster::ForceChangeFSM(uptr<GangsterFSM> fsm)
{
	curFSM->Exit();
	curFSM = move(fsm);
	curFSM->Enter();
}

void Gangster::CollisionEvent(GameObject& other)
{
	if (other.GetTag() == EObjectTag::PlayerAttack && other.GetCollider().GetColliderType() == EColliderType::LineAttack)
	{
		dirHit = (other.GetCollider().GetBounds().size - other.GetCollider().GetBounds().position).normalized();
		ForceChangeFSM(make_unique<Gangster_Hit>(*this));
	}
	else if (other.GetTag() == EObjectTag::PlayerAttack && other.GetCollider().GetColliderType() == EColliderType::RectAttack)
	{
		-dirToPlayer.x >= 0 ? dirHit = Vector2f(1.f, 0.0f) : dirHit = Vector2f(-1.f, 0.0f);
		ForceChangeFSM(make_unique<Gangster_Hit>(*this));
	}
}

void Gangster::CollisionBounce()
{
	if (curState != EGangsterState::Hit_Ground)
	{
		if (bounceCount <= 5)
		{
			uptr<Blood> blood = make_unique<Blood>();
			blood->SetDir(velocity.normalized());
			blood->GetDesc().vSpawnPoint = position;
			GameInstance::GetInstance().GetObjectManager().AddObject(EObjectTag::Default, ERenderLayer::Effect, move(blood));

			uptr<BloodDecal> bloodDecal = make_unique<BloodDecal>();
			bloodDecal->GetDesc().vSpawnPoint = position + Vector2f(0.f, -22.f);
			bloodDecal->SetAttackDir(velocity.normalized());
			bloodDecal->SetIsSingle(true);
			GameInstance::GetInstance().GetObjectManager().AddObject(EObjectTag::Default, ERenderLayer::Decal, move(bloodDecal));
		}
		bounceCount++;
	}
	if (bounceCount == 1 && isGrounded == true)	return;

	if (curState == EGangsterState::Hit) ForceChangeFSM(make_unique<Gangster_Hit_Roll>(*this));
}

void Gangster::CollisionBounceEnd()
{
	if (curState == EGangsterState::Hit_Roll)
	{
		ForceChangeFSM(make_unique<Gangster_Hit_Ground>(*this));
		bounceCount = 0;
	}
}

void Gangster::Release()
{
	animator.Release();
}