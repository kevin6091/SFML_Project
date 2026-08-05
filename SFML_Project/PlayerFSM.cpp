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

void PlayerFSM::MoveLeft()
{
	Vector2f newVelocity = context.GetVelocity();
	if (newVelocity.x > -context.GetSpeed())
		newVelocity.x = -context.GetSpeed();
	context.SetVelocity(newVelocity);
	context.SetFace(false);
}

void PlayerFSM::MoveRight()
{
	context.SetFace(true);
	Vector2f newVelocity = context.GetVelocity();
	if (newVelocity.x <= context.GetSpeed())
		newVelocity.x = context.GetSpeed();
	context.SetVelocity(newVelocity);
}

void PlayerFSM::BreakX(float deltaTime)
{
	Vector2f newVelocity = context.GetVelocity();
	if (newVelocity.x > 0)
		newVelocity.x -= context.GetSpeed() * 2 * deltaTime;
	else
		newVelocity.x += context.GetSpeed() * 2 * deltaTime;
	if (fabsf(newVelocity.x) <= 0.01f)
		newVelocity.x = 0.0f;
	context.SetVelocity(newVelocity);
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
	else if (INPUT.GetKeyDown(KEY::W))
	{
		return make_unique<Player_Jump>(context);
	}

	if (INPUT.GetMouseDown(MOUSE::Left) && context.GetAttackCool() >= 0.3f)
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
		MoveLeft();
	else if (INPUT.GetKeyPress(KEY::D))
		MoveRight();
	
	if (INPUT.GetKeyPress(KEY::W))
		return make_unique<Player_Jump>(context);

	if (INPUT.GetKeyPress(KEY::S))
		return make_unique<Player_Roll>(context);

	if (INPUT.GetKeyNone(KEY::A) && INPUT.GetKeyNone(KEY::D) && INPUT.GetKeyNone(KEY::S))
		return make_unique<Player_Run_To_Idle>(context);
		
	if (INPUT.GetMouseDown(MOUSE::Left) && context.GetAttackCool() >= 0.3f)
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
		MoveLeft();
	else if (INPUT.GetKeyPress(KEY::D))
		MoveRight();
	else if (INPUT.GetKeyPress(KEY::S))
		return make_unique<Player_Roll>(context);
	else if (INPUT.GetKeyPress(KEY::W))
		return make_unique<Player_Jump>(context);

	if (INPUT.GetKeyNone(KEY::A) && INPUT.GetKeyNone(KEY::D) && INPUT.GetKeyNone(KEY::S))
		return make_unique<Player_Run_To_Idle>(context);

	if(context.GetAnimator().IsFinished())
		return make_unique<Player_Run>(context);

	if (INPUT.GetMouseDown(MOUSE::Left) && context.GetAttackCool() >= 0.3f)
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
	// 브레이크, 바로 멈추진 않음
	BreakX(deltaTime);

	if (INPUT.GetKeyPress(KEY::A) || INPUT.GetKeyPress(KEY::D))
		return make_unique<Player_Idle_To_Run>(context);
	else if (INPUT.GetKeyPress(KEY::W))
		return make_unique<Player_Jump>(context);
	else if (INPUT.GetKeyPress(KEY::S))
		return make_unique<Player_Crouch>(context);

	if (context.GetAnimator().IsFinished())
		return make_unique<Player_Idle>(context);

	if (INPUT.GetMouseDown(MOUSE::Left) && context.GetAttackCool() >= 0.3f)
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
		MoveLeft();
		return make_unique<Player_Roll>(context);
	}
	else if (INPUT.GetKeyDown(KEY::D))
	{
		MoveRight();
		return make_unique<Player_Roll>(context);
	}
	else if (INPUT.GetKeyPress(KEY::W))
	{
		return make_unique<Player_Jump>(context);
	}

	if(!INPUT.GetKeyPress(KEY::S))
		return make_unique<Player_Crouch_To_Idle>(context);

	if (INPUT.GetMouseDown(MOUSE::Left) && context.GetAttackCool() >= 0.3f)
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

	if (INPUT.GetMouseDown(MOUSE::Left) && context.GetAttackCool() >= 0.3f)
		return make_unique<Player_Attack>(context);

	if (INPUT.GetKeyPress(KEY::W))
		return make_unique<Player_Jump>(context);

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

	Vector2f vel = context.GetVelocity();
	if (context.GetDesc().bFace == false)
	{
		vel.x = -ATTACK_FORCE;
		vel.y += ATTACK_FORCE;
	}
	else
	{
		vel.x = +ATTACK_FORCE;
		vel.y += ATTACK_FORCE;
	}

	context.SetVelocity(vel);
}

uptr<PlayerFSM> Player_Roll::Update(float deltaTime)
{
	if (INPUT.GetKeyDown(KEY::W))
		return make_unique<Player_Jump>(context);
	else if (INPUT.GetKeyPress(KEY::W))
		return make_unique<Player_Jump>(context);

	if (ANIM.IsFinished())
	{
		if (INPUT.GetKeyPress(KEY::A) || INPUT.GetKeyPress(KEY::D))
			return make_unique<Player_Idle_To_Run>(context);
		else
			return make_unique<Player_Run_To_Idle>(context);
	}

	if (INPUT.GetMouseDown(MOUSE::Left) && context.GetAttackCool() >= 0.3f)
		return make_unique<Player_Attack>(context);

	return nullptr;
}

void Player_Roll::Exit()
{
}

#pragma endregion

#pragma region Player_Jump

Player_Jump::Player_Jump(Player& context)
	:PlayerFSM(context)
{
}

Player_Jump::~Player_Jump()
{
}

void Player_Jump::Enter()
{
	context.SetState(EPlayerState::Jump);
	Play(PLAYER_JUMP, true);
	context.SetIsGrounded(false);
	context.SetVelocity({ context.GetVelocity().x, context.GetVelocity().y - JUMP_FORCE });
}

uptr<PlayerFSM> Player_Jump::Update(float deltaTime)
{
	if (context.GetIsGrippable())
		return make_unique<Player_WallSlide>(context);
	

	if (INPUT.GetKeyPress(KEY::A))
		MoveLeft();
	else if (INPUT.GetKeyPress(KEY::D))
		MoveRight();
	else if (INPUT.GetKeyPress(KEY::S))
		context.SetGravityFactor(2.5f);

	if (INPUT.GetMouseDown(MOUSE::Left) && context.GetAttackCool() >= 0.3f)
		return make_unique<Player_Attack>(context);

	if (context.GetVelocity().y >= 0.f)
		return make_unique<Player_Fall>(context);

	return nullptr;
}

void Player_Jump::Exit()
{
}

#pragma endregion

#pragma region Player_Fall

Player_Fall::Player_Fall(Player& context)
	:PlayerFSM(context)
{
}

Player_Fall::~Player_Fall()
{
}

void Player_Fall::Enter()
{
	context.SetState(EPlayerState::Fall);
	Play(PLAYER_FALL, true);
}

uptr<PlayerFSM> Player_Fall::Update(float deltaTime)
{
	if (INPUT.GetKeyPress(KEY::A))
	{
		if (context.GetIsGrippable())
			return make_unique<Player_WallSlide>(context);

		if(context.GetVelocity().x <= 0.05f)
			MoveLeft();
	}
	else if (INPUT.GetKeyPress(KEY::D))
	{
		if (context.GetIsGrippable())
			return make_unique<Player_WallSlide>(context);

		if (context.GetVelocity().x >= -0.051f)
			MoveRight();
	}
	
	if (INPUT.GetKeyPress(KEY::S))
	{
		context.SetGravityFactor(2.5f);
	}

	if (INPUT.GetMouseDown(MOUSE::Left) && context.GetAttackCool() >= 0.3f)
		return make_unique<Player_Attack>(context);

	if (context.GetIsGrounded())
	{
		if (INPUT.GetKeyPress(KEY::S) && INPUT.GetKeyPress(KEY::A))
		{
			MoveLeft();
			return make_unique<Player_Roll>(context);
		}
		else if (INPUT.GetKeyPress(KEY::S) && INPUT.GetKeyPress(KEY::D))
		{
			MoveRight();
			return make_unique<Player_Roll>(context);
		}
		context.ResetAttackCount();
		return make_unique<Player_Run_To_Idle>(context);
	}

	return nullptr;
}

void Player_Fall::Exit()
{
	context.SetGravityFactor(1.0f);
}

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

	context.ResetAttackCool();
	context.AddAttackCount();

	mousePos = INPUT.GetMouseWorldPos();
	playerPos = context.GetPosition();
	attackDir = (mousePos - playerPos).normalized();
	Vector2f newVelocity = context.GetVelocity();
	float decrease = (float)context.GetAttackCount();
	if (decrease > 1.f)
		decrease *= 0.8f;

	newVelocity.x = (attackDir * ATTACK_FORCE).x;
	newVelocity.y = (attackDir * (ATTACK_FORCE / decrease)).y;
	context.SetVelocity(newVelocity);
	
	if (mousePos.x >= playerPos.x)
		context.SetFace(true);
	else
		context.SetFace(false);
}

uptr<PlayerFSM> Player_Attack::Update(float deltaTime)
{
	if (ANIM.IsFinished())
	{
		if (context.GetIsGrounded())
			return make_unique<Player_Run_To_Idle>(context);
		else
			return make_unique<Player_Fall>(context);
	}

	return nullptr;
}

void Player_Attack::Exit()
{
}

#pragma endregion

#pragma region Player_WallSlide

Player_WallSlide::Player_WallSlide(Player& context)
	:PlayerFSM(context)
{
}

Player_WallSlide::~Player_WallSlide()
{
}

void Player_WallSlide::Enter()
{
	context.SetState(EPlayerState::WallSlide);
	Play(PLAYER_WALLSLIDE, true);
}

uptr<PlayerFSM> Player_WallSlide::Update(float deltaTime)
{
	if (context.GetIsGrounded())
		return make_unique<Player_Run_To_Idle>(context);
	
	if (!context.GetIsGrippable())
		return make_unique<Player_Fall>(context);

	if (INPUT.GetKeyDown(KEY::W))
		return make_unique<Player_Flip>(context);

	context.SetGravityFactor(0.4f);

	return nullptr;
}

void Player_WallSlide::Exit()
{
	context.SetGravityFactor(1.0f);
	
	if(context.GetDesc().bFace)
		context.SetFace(false);
	else
		context.SetFace(true);
}

#pragma endregion

#pragma region Player_Flip

Player_Flip::Player_Flip(Player& context)
	:PlayerFSM(context)
{
}

Player_Flip::~Player_Flip()
{
}

void Player_Flip::Enter()
{
	context.SetState(EPlayerState::Flip);
	Play(PLAYER_FLIP, true);

	if (context.GetDesc().bFace)
		context.SetVelocity({ ATTACK_FORCE, -JUMP_FORCE});
	else
		context.SetVelocity({ -ATTACK_FORCE, -JUMP_FORCE});

	context.SetIsGrippable(false);
}

uptr<PlayerFSM> Player_Flip::Update(float deltaTime)
{
	if (ANIM.IsFinished())
		return make_unique<Player_Fall>(context);

	if (context.GetIsGrippable() && ANIM.GetCurrentFrameIndex() >= 4)
		return make_unique<Player_WallSlide>(context);

	if (context.GetIsGrounded())
		return make_unique<Player_Run_To_Idle>(context);

	return nullptr;
}

void Player_Flip::Exit()
{
}

#pragma endregion
