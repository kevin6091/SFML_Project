#include "PlayerFSM.h"
#include "Player.h"
#include "GameInstance.h"
#include "InputManager.h"

#pragma region Macro

#define GAME		GameInstance::GetInstance()
#define INPUT		GAME.GetInputManager()
#define KEY			Keyboard::Key
#define MOUSE		Mouse::Button
#define ANIM		context.GetAnimator()

#pragma endregion


#pragma region PlayerFSM

PlayerFSM::PlayerFSM(Player& _context)
	: context(_context)
{
}

PlayerFSM::~PlayerFSM()
{
}

void PlayerFSM::Enter()
{

}

uptr<PlayerFSM> PlayerFSM::Update(float deltaTime)
{
	return nullptr;
}

void PlayerFSM::Exit()
{

}

void PlayerFSM::Play(const string& name, bool forceReset)
{
	context.GetAnimator().Play(name, forceReset);
}

#pragma endregion

#pragma region Player_Idle

Player_Idle::Player_Idle(Player& context)
	: PlayerFSM(context)
{
}

Player_Idle::~Player_Idle()
{
}

void Player_Idle::Enter()
{
	context.SetState(EPlayerState::Idle);
	context.GetAnimator().Play(PLAYER_IDLE, true);
}

uptr<PlayerFSM> Player_Idle::Update(float deltaTime)
{
	if(INPUT.GetKeyPress(KEY::A) || INPUT.GetKeyPress(KEY::D))
	{
		return make_unique<Player_Idle_To_Run>(context);
	}
	else if (INPUT.GetKeyPress(KEY::S))
	{
		return make_unique<Player_Crouch>(context);
	}

	if (INPUT.GetMouseDown(MOUSE::Left))
	{
		return make_unique<Player_Attack>(context);
	}

	return nullptr;
}

void Player_Idle::Exit()
{
}

#pragma endregion

#pragma region Player_Run

Player_Run::Player_Run(Player& context)
	: PlayerFSM(context)
{
}

Player_Run::~Player_Run()
{
}

void Player_Run::Enter()
{
	context.SetState(EPlayerState::Run);
	context.GetAnimator().Play(PLAYER_RUN, true);
}

uptr<PlayerFSM> Player_Run::Update(float deltaTime)
{
	if (INPUT.GetKeyPress(KEY::A))
		context.SetFace(false);
	else if (INPUT.GetKeyPress(KEY::D))
		context.SetFace(true);

	if (INPUT.GetKeyPress(KEY::S))
		return make_unique<Player_Roll>(context);

	if (INPUT.GetKeyNone(KEY::A) && INPUT.GetKeyNone(KEY::D) && INPUT.GetKeyNone(KEY::S))
		return make_unique<Player_Run_To_Idle>(context);
		
	if (INPUT.GetMouseDown(MOUSE::Left))
		return make_unique<Player_Attack>(context);

	return nullptr;
}

void Player_Run::Exit()
{
}

#pragma endregion

#pragma region Player_Idle_To_Run

Player_Idle_To_Run::Player_Idle_To_Run(Player& context)
	: PlayerFSM(context)
{
}

Player_Idle_To_Run::~Player_Idle_To_Run()
{
}

void Player_Idle_To_Run::Enter()
{
	context.SetState(EPlayerState::Idle_To_Run);
	context.GetAnimator().Play(PLAYER_IDLE_TO_RUN, true);
}

uptr<PlayerFSM> Player_Idle_To_Run::Update(float deltaTime)
{
	if (INPUT.GetKeyPress(KEY::A))
		context.SetFace(false);
	else if (INPUT.GetKeyPress(KEY::D))
		context.SetFace(true);
	else if (INPUT.GetKeyPress(KEY::S))
		return make_unique<Player_Roll>(context);

	if (INPUT.GetKeyNone(KEY::A) && INPUT.GetKeyNone(KEY::D) && INPUT.GetKeyNone(KEY::S))
		return make_unique<Player_Run_To_Idle>(context);

	if(context.GetAnimator().IsFinished())
		return make_unique<Player_Run>(context);

	if (INPUT.GetMouseDown(MOUSE::Left))
		return make_unique<Player_Attack>(context);

	return nullptr;
}

void Player_Idle_To_Run::Exit()
{
}

#pragma endregion

#pragma region Player_Run_To_Idle

Player_Run_To_Idle::Player_Run_To_Idle(Player& context)
	:PlayerFSM(context)
{
}

Player_Run_To_Idle::~Player_Run_To_Idle()
{
}

void Player_Run_To_Idle::Enter()
{
	context.SetState(EPlayerState::Run_To_Idle);
	context.GetAnimator().Play(PLAYER_RUN_TO_IDLE, true);
}

uptr<PlayerFSM> Player_Run_To_Idle::Update(float deltaTime)
{
	if (INPUT.GetKeyPress(KEY::A) || INPUT.GetKeyPress(KEY::D))
	{
		return make_unique<Player_Idle_To_Run>(context);
	}

	if (context.GetAnimator().IsFinished())
		return make_unique<Player_Idle>(context);

	if (INPUT.GetMouseDown(MOUSE::Left))
		return make_unique<Player_Attack>(context);

	return nullptr;
}

void Player_Run_To_Idle::Exit()
{
}

#pragma endregion

#pragma region Player_Crouch

Player_Crouch::Player_Crouch(Player& context)
	: PlayerFSM(context)
{
}

Player_Crouch::~Player_Crouch()
{
}

void Player_Crouch::Enter()
{
	context.SetState(EPlayerState::Crouch);
	context.GetAnimator().Play(PLAYER_CROUCH, true);
}

uptr<PlayerFSM> Player_Crouch::Update(float deltaTime)
{
	if (INPUT.GetKeyDown(KEY::A))
	{
		context.SetFace(false);
		return make_unique<Player_Roll>(context);
	}
	else if (INPUT.GetKeyDown(KEY::D))
	{
		context.SetFace(true);
		return make_unique<Player_Roll>(context);
	}

	if(!INPUT.GetKeyPress(KEY::S))
		return make_unique<Player_Crouch_To_Idle>(context);

	if (INPUT.GetMouseDown(MOUSE::Left))
		return make_unique<Player_Attack>(context);

	return nullptr;
}

void Player_Crouch::Exit()
{
}

#pragma endregion

#pragma region Player_Crouch_To_Idle

Player_Crouch_To_Idle::Player_Crouch_To_Idle(Player& context)
	:PlayerFSM(context)
{
}

Player_Crouch_To_Idle::~Player_Crouch_To_Idle()
{
}

void Player_Crouch_To_Idle::Enter()
{
	context.SetState(EPlayerState::Crouch);
	Play(PLAYER_CROUCH_TO_IDLE, true);
}

uptr<PlayerFSM> Player_Crouch_To_Idle::Update(float deltaTime)
{
	if (context.GetAnimator().IsFinished())
		return make_unique<Player_Idle>(context);

	if (INPUT.GetMouseDown(MOUSE::Left))
		return make_unique<Player_Attack>(context);

	return nullptr;
}

void Player_Crouch_To_Idle::Exit()
{
}

#pragma endregion

#pragma region Player_Roll

Player_Roll::Player_Roll(Player& context)
	: PlayerFSM(context)
{
}

Player_Roll::~Player_Roll()
{
}

void Player_Roll::Enter()
{
 	context.SetState(EPlayerState::Roll);
	Play(PLAYER_ROLL, true);
}

uptr<PlayerFSM> Player_Roll::Update(float deltaTime)
{
	if (INPUT.GetKeyDown(KEY::W))
	{
		// มกวม
	}

	if (ANIM.IsFinished())
	{
		if (INPUT.GetKeyPress(KEY::A) || INPUT.GetKeyPress(KEY::D))
			return make_unique<Player_Idle_To_Run>(context);
		else
			return make_unique<Player_Run_To_Idle>(context);
	}

	if (INPUT.GetMouseDown(MOUSE::Left))
		return make_unique<Player_Attack>(context);

	return nullptr;
}

void Player_Roll::Exit()
{
}

#pragma endregion

#pragma region Player_Jump

#pragma endregion

#pragma region Player_Fall

#pragma endregion

#pragma region Player_Landing

#pragma endregion

#pragma region Player_Attack

Player_Attack::Player_Attack(Player& context)
	:PlayerFSM(context)
{
}

Player_Attack::~Player_Attack()
{
}

void Player_Attack::Enter()
{
	context.SetState(EPlayerState::Attack);
	Play(PLAYER_ATTACK, true);

	Vector2f mousePos = INPUT.GetMouseWorldPos();
	Vector2f playerPos = context.GetPosition();

	if (mousePos.x >= playerPos.x)
		context.SetFace(true);
	else
		context.SetFace(false);
}

uptr<PlayerFSM> Player_Attack::Update(float deltaTime)
{
	if (ANIM.IsFinished())
	{
		return make_unique<Player_Run_To_Idle>(context);
	}

	return nullptr;
}

void Player_Attack::Exit()
{
}

#pragma endregion
