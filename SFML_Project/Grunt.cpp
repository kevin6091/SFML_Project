#include "Grunt.h"
#include "GameInstance.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "GruntFSM.h"
#include "ObjectManager.h"
#include "RewindManager.h"
#include "RewindTracker.h"
#include "Player.h"

Grunt::Grunt()
{
	eObjectTag = EObjectTag::Enemy;
	eRenderLayer = ERenderLayer::Actor;
	speed = 200.f;
}

Grunt::~Grunt()
{
}

void Grunt::Initialize()
{
#pragma region Resource

	auto& resource = GameInstance::GetInstance().GetResourceManager();

	animator.AddClip(GRUNT_IDLE, 0.08f, true);
	animator.AddClip(GRUNT_RUN, 0.05f, true);
	animator.AddClip(GRUNT_ATTACK, 0.05f, false);
	animator.AddClip(GRUNT_HIT, 0.05f, false);
	animator.AddClip(GRUNT_HIT_ROLL, 0.05f, true);
	animator.AddClip(GRUNT_HIT_GROUND, 0.05f, false);

	sprite.emplace(*resource.GetTexture("default"));

#pragma endregion

#pragma region FSM

	curState = EGruntState::Idle;
	curFSM = make_unique<Grunt_Idle>(*this);
	curFSM->Enter();

#pragma endregion

	uCollider = make_unique<Collider>(this);
	Vector2f topLeftOffset = { -9.f, -44.f };
	ColliderDesc desc = { "", EColliderType::DynamicObject, FloatRect(topLeftOffset, {18.f, 44.f}) };
	uCollider->Initialize(desc);

	SetPosition(descStatus.vSpawnPoint);
	sprite->setScale({ 1.5f,1.5f });
}

void Grunt::Update(float deltaTime)
{
#pragma region Velocity

	velocity.y += GRAVITY * deltaTime;
	if (velocity.y > MAX_FALL)
		velocity.y = MAX_FALL;

#pragma endregion

	dirToPlayer = GameInstance::GetInstance().GetPlayer()->GetPosition() - position;

	if (curFSM)
	{
		if (uptr<GruntFSM> nextFSM = curFSM.get()->Update(deltaTime))
		{
			curFSM->Exit();
			curFSM = move(nextFSM);
			curFSM->Enter();
		}
	}

	if (velocity.x > 0)
		SetFace(true);
	else
		SetFace(false);

	animator.Update(deltaTime, *sprite);
}

void Grunt::LateUpdate(float deltaTime)
{
}

void Grunt::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Actor().draw(*this, BlendAlpha);
}

void Grunt::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);

	RectangleShape debugBox(uCollider->GetBounds().size);
	debugBox.setPosition(uCollider->GetBounds().position);
	debugBox.setFillColor(Color::Transparent);
	if(curState == EGruntState::Hit)
		debugBox.setOutlineColor(Color::Red);
	else
		debugBox.setOutlineColor(Color::White);


	debugBox.setOutlineThickness(1.0f);

	if (GameInstance::GetInstance().GetIsRenderDebug())
		target.draw(debugBox);
}

void Grunt::CollisionEvent(GameObject& other)
{
	if (other.GetTag() == EObjectTag::PlayerAttack && other.GetCollider().GetColliderType() == EColliderType::LineAttack)
	{
		dirHit = (other.GetCollider().GetBounds().size - other.GetCollider().GetBounds().position).normalized();
		ForceChangeFSM(make_unique<Grunt_Hit>(*this));
	}
}

void Grunt::CollisionBounce()
{
	if(curState != EGruntState::Hit_Ground) bounceCount++;
	if (bounceCount == 1 && isGrounded == true)	return;

	if (curState == EGruntState::Hit) ForceChangeFSM(make_unique<Grunt_Hit_Roll>(*this));
}

void Grunt::CollisionBounceEnd()
{
	if (curState == EGruntState::Hit_Roll)
	{
		ForceChangeFSM(make_unique<Grunt_Hit_Ground>(*this));
		bounceCount = 0;
	}
}

void Grunt::RestartObject()
{
	SetPosition(descStatus.vSpawnPoint);
	velocity = { 0.0f,0.0f };
	descStatus.bFace = true;
	if (sprite->getScale().x <= 0.f)
		sprite->setScale({ -1.f,1.f });

	ForceChangeFSM(make_unique<Grunt_Idle>(*this));

	rewindSpeed = 1.0f;
	rewinder.Clear();
	isGrounded = true;
}

void Grunt::ForceChangeFSM(uptr<GruntFSM> fsm)
{
	curFSM->Exit();
	curFSM = move(fsm);
	curFSM->Enter();
}

void Grunt::Release()
{
	animator.Release();
}
