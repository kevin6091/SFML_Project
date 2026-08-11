#include "GruntFSM.h"
#include "Grunt.h"
#include "GameInstance.h"
#include "InputManager.h"
#include "ObjectManager.h"	
#include "BloodDecal.h"
#include "Blood.h"
#include "Camera.h"
#include "Player.h"
#include "Collider.h"
#include "HitLine.h"
#include "GruntSlash.h"

#pragma region Macro

#define GAME		GameInstance::GetInstance()
#define INPUT		GAME.GetInputManager()
#define KEY			Keyboard::Key
#define MOUSE		Mouse::Button
#define ANIM		context.GetAnimator()

#pragma endregion

GruntFSM::GruntFSM(Grunt& _context)
	:context(_context)
{
}

GruntFSM::~GruntFSM()
{
}

void GruntFSM::Enter()
{
}

uptr<GruntFSM> GruntFSM::Update(float deltaTime)
{
	return nullptr;
}

void GruntFSM::Exit()
{
}

void GruntFSM::Play(const string& name, bool forceReset)
{
	context.GetAnimator().Play(name, forceReset);
}

void GruntFSM::MoveLeft()
{
	Vector2f newVelocity = context.GetVelocity();
	if (newVelocity.x > -context.GetSpeed())
		newVelocity.x = -context.GetSpeed();
	context.SetVelocity(newVelocity);
	context.SetFace(false);
}

void GruntFSM::MoveRight()
{
	context.SetFace(true);
	Vector2f newVelocity = context.GetVelocity();
	if (newVelocity.x <= context.GetSpeed())
		newVelocity.x = context.GetSpeed();
	context.SetVelocity(newVelocity);
}

void GruntFSM::BreakX(float deltaTime)
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

#pragma region Grunt_Idle

Grunt_Idle::Grunt_Idle(Grunt& context)
	:GruntFSM(context)
{
}

Grunt_Idle::~Grunt_Idle()
{
}

void Grunt_Idle::Enter()
{
	context.SetState(EGruntState::Idle);
	Play(GRUNT_IDLE, true);
}

uptr<GruntFSM> Grunt_Idle::Update(float deltaTime)
{
	if ((context.GetIsFindPlayer() || context.GetDirToPlayer().length() <= 200) &&
		context.GetAttackCool() >= 2.f)
	{
		return make_unique<Grunt_Run>(context);
	}

	return nullptr;
}

void Grunt_Idle::Exit()
{
}

#pragma endregion

#pragma region Grunt_Run

Grunt_Run::Grunt_Run(Grunt& context)
	:GruntFSM(context)
{
}

Grunt_Run::~Grunt_Run()
{
}

void Grunt_Run::Enter()
{
	context.SetState(EGruntState::Run);
	Play(GRUNT_RUN, true);
}

uptr<GruntFSM> Grunt_Run::Update(float deltaTime)
{
	context.GetDirToPlayer().x >= 0 ? MoveRight() : MoveLeft();

	if (context.GetDirToPlayer().length() <= 70.f)
	{
		context.SetIsFindPlayer(true);
		return make_unique<Grunt_Attack>(context);
	}

	return nullptr;
}

void Grunt_Run::Exit()
{
}

#pragma endregion

#pragma region Grunt_Hit

Grunt_Hit::Grunt_Hit(Grunt& context)
	:GruntFSM(context)
{
}

Grunt_Hit::~Grunt_Hit()
{
}

void Grunt_Hit::Enter()
{
	context.SetState(EGruntState::Hit);
	Play(GRUNT_HIT, true);
	context.SetActive(false);
	
	Vector2f dirHit = context.GetDirHit();

	if(dirHit.y >= 0)
	{
		dirHit.y *= 2.f;
		dirHit.y = clamp(dirHit.y, 0.9f, 1.7f);
	}
	else
		dirHit.y = clamp(dirHit.y, -1.f, -0.5f);

	context.SetVelocity(dirHit * 700.f);

	uptr<BloodDecal> bloodDecal = make_unique<BloodDecal>();
	bloodDecal->GetDesc().vSpawnPoint = context.GetPosition() + Vector2f(0.f, -22.f);
	bloodDecal->SetAttackDir(context.GetDirHit().normalized());
	GAME.GetObjectManager().AddObject(EObjectTag::Default, ERenderLayer::Decal, move(bloodDecal));

	uptr<Blood> blood = make_unique<Blood>();
	blood->SetDir(context.GetVelocity().normalized());
	blood->GetDesc().vSpawnPoint = context.GetPosition();
	GameInstance::GetInstance().GetObjectManager().AddObject(EObjectTag::Default, ERenderLayer::Effect, move(blood));

	blood = make_unique<Blood>();
	blood->SetDir(context.GetVelocity().normalized());
	blood->GetDesc().vSpawnPoint = context.GetPosition() + context.GetDirToPlayer().normalized() * 1.5f;
	GameInstance::GetInstance().GetObjectManager().AddObject(EObjectTag::Default, ERenderLayer::Effect, move(blood));

	if (!GAME.GetIsHitLine())
	{
		uptr<HitLine> line = make_unique<HitLine>();
		line->SetDir(context.GetDirHit().normalized());
		line->GetDesc().vSpawnPoint = context.GetPosition();
		GameInstance::GetInstance().GetObjectManager().AddObject(EObjectTag::Default, ERenderLayer::Effect, move(line));
		GAME.SetIsHitLine(true);
	}
}

uptr<GruntFSM> Grunt_Hit::Update(float deltaTime)
{
	return nullptr;
}

void Grunt_Hit::Exit()
{
}

#pragma endregion

#pragma region Grunt_Hit_Roll

Grunt_Hit_Roll::Grunt_Hit_Roll(Grunt& context)
	:GruntFSM(context)
{
}

Grunt_Hit_Roll::~Grunt_Hit_Roll()
{
}

void Grunt_Hit_Roll::Enter()
{
	context.SetState(EGruntState::Hit_Roll);
	Play(GRUNT_HIT_ROLL, true);
}

uptr<GruntFSM> Grunt_Hit_Roll::Update(float deltaTime)
{
	return nullptr;
}

void Grunt_Hit_Roll::Exit()
{
}

#pragma endregion

#pragma region Grunt_Hit_Ground

Grunt_Hit_Ground::Grunt_Hit_Ground(Grunt& context)
	:GruntFSM(context)
{
}

Grunt_Hit_Ground::~Grunt_Hit_Ground()
{
}

void Grunt_Hit_Ground::Enter()
{
	context.SetState(EGruntState::Hit_Ground);
	Play(GRUNT_HIT_GROUND, true);
}

uptr<GruntFSM> Grunt_Hit_Ground::Update(float deltaTime)
{
	return nullptr;
}

void Grunt_Hit_Ground::Exit()
{
}

#pragma endregion

#pragma region Grunt_Attack

Grunt_Attack::Grunt_Attack(Grunt& context)
	:GruntFSM(context)
{
}

Grunt_Attack::~Grunt_Attack()
{
}

void Grunt_Attack::Enter()
{
	context.SetState(EGruntState::Attack);
	Play(GRUNT_ATTACK, true); 

	context.SetVelocity({0.f,0.f});

	auto slash = make_unique<GruntSlash>();
	slash->GetDesc().bFace = context.GetDesc().bFace;
	slash->GetDesc().vSpawnPoint = context.GetPosition();
	GameInstance::GetInstance().GetObjectManager().AddObject(EObjectTag::EnemyAttack, ERenderLayer::Effect, move(slash));
}

uptr<GruntFSM> Grunt_Attack::Update(float deltaTime)
{
	if (ANIM.IsFinished())
		return make_unique<Grunt_Idle>(context);

	return nullptr;
}

void Grunt_Attack::Exit()
{
	context.SetAttackCool(0.f);
}

#pragma endregion

