#include "Player.h"
#include "GameInstance.h"
#include "Animator.h"
#include "ResourceManager.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();
	
	animator.AddClip(PLAYER_IDLE, 0.08f, true);
	animator.AddClip(PLAYER_RUN, 0.05f, true);
	animator.AddClip(PLAYER_IDLE_TO_RUN, 0.05f, false);
	animator.AddClip(PLAYER_RUN_TO_IDLE, 0.1f, false);
	
	animator.AddClip(PLAYER_CROUCH, 0.05f, false);
	animator.AddClip(PLAYER_CROUCH_TO_IDLE, 0.05f, false);

	animator.AddClip(PLAYER_ROLL, 0.05f, false);
	
	animator.AddClip(PLAYER_ATTACK, 0.05f, false);


	sprite.emplace(*resource.GetTexture("default"));
	(*sprite).setScale(Vector2f(1.5f, 1.5f));

	curState = EPlayerState::Idle;
	curFSM = make_unique<Player_Idle>(*this);
	curFSM->Enter();

	SetPosition(Vector2f(0.0f, 0.0f));
}

void Player::Update(float deltaTime)
{
	if (curFSM)
	{
		if (uptr<PlayerFSM> nextFSM = curFSM.get()->Update(deltaTime))
		{
			curFSM->Exit();
			curFSM = move(nextFSM);
			curFSM->Enter();
		}
	}

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
}

void Player::Release()
{
	animator.Release();
}
