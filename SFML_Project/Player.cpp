#include "Player.h"
#include "GameInstance.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "PlayerFSM.h"

Player::Player()
{
	eObjectTag = EObjectTag::Player;
	eRenderLayer = ERenderLayer::Player;
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
	animator.AddClip(PLAYER_FLIP, 0.05f, false);
	
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
}

void Player::Update(float deltaTime)
{
#pragma region Velocity

	velocity.y += GRAVITY * deltaTime * gravityFactor;
	if (velocity.y > MAX_FALL)
		velocity.y = MAX_FALL;

	Vector2f newVelocity = velocity;
	if (newVelocity.x > 0)
		newVelocity.x -= GRAVITY * deltaTime * 0.5f;
	else
		newVelocity.x += GRAVITY * deltaTime * 0.5f;
	if (fabsf(newVelocity.x) <= 0.0001f)
		newVelocity.x = 0.0f;
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

	if (grippableEnd <= position.y)
		isGrippable = false;

	animator.Update(deltaTime, *sprite);
}

void Player::LateUpdate(float deltaTime)
{
}

void Player::Render()
{
	RenderStates states = BlendAlpha;

	GameInstance::GetInstance().Draw(*this, states);
}

void Player::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);

	RectangleShape debugBox(uCollider->GetBounds().size);
	debugBox.setPosition(uCollider->GetBounds().position);
	debugBox.setFillColor(Color(0, 0, 255, 10));

	debugBox.setOutlineThickness(1.0f);

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

void Player::ForceChangeFSM(uptr<PlayerFSM> fsm)
{
	curFSM->Exit();
	curFSM = move(fsm);
	curFSM->Enter();
}

void Player::Release()
{
	animator.Release();
}
