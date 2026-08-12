#include "GangsterFSM.h"
#include "Gangster.h"
#include "GameInstance.h"
#include "InputManager.h"
#include "ObjectManager.h"	
#include "BloodDecal.h"
#include "Blood.h"
#include "Camera.h"
#include "Player.h"
#include "Collider.h"
#include "HitLine.h"
#include "Bullet.h"
#include "SingleEffect.h"

#pragma region Macro

#define GAME		GameInstance::GetInstance()
#define INPUT		GAME.GetInputManager()
#define KEY			Keyboard::Key
#define MOUSE		Mouse::Button
#define ANIM		context.GetAnimator()

#pragma endregion

#pragma region GangsterFSM

GangsterFSM::GangsterFSM(Gangster& context)
	: context(context)
{
}

GangsterFSM::~GangsterFSM()
{
}

void GangsterFSM::Enter()
{
}

uptr<GangsterFSM> GangsterFSM::Update(float deltaTime)
{
	return nullptr;
}

void GangsterFSM::Exit()
{
}

void GangsterFSM::Play(const string& name, bool forceReset)
{
	context.GetAnimator().Play(name, forceReset);
}

void GangsterFSM::MoveLeft()
{
	Vector2f newVelocity = context.GetVelocity();
	if (newVelocity.x > -context.GetSpeed())
		newVelocity.x = -context.GetSpeed();
	context.SetVelocity(newVelocity);
	context.SetFace(false);
}

void GangsterFSM::MoveRight()
{
	context.SetFace(true);
	Vector2f newVelocity = context.GetVelocity();
	if (newVelocity.x <= context.GetSpeed())
		newVelocity.x = context.GetSpeed();
	context.SetVelocity(newVelocity);
}

void GangsterFSM::BreakX(float deltaTime)
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

#pragma region Gangster_Idle

Gangster_Idle::Gangster_Idle(Gangster& context)
	:GangsterFSM(context)
{
}

Gangster_Idle::~Gangster_Idle()
{
}

void Gangster_Idle::Enter()
{
	context.SetState(EGangsterState::Idle);
	Play(GANGSTER_IDLE, true);
}

uptr<GangsterFSM> Gangster_Idle::Update(float deltaTime)
{
	float disY = abs(context.GetDirToPlayer().y);
	float disX = abs(context.GetDirToPlayer().x);

	if ((context.GetIsFindPlayer() || (disX <= 300 && disY <= 50)) &&
		context.GetAttackCool() >= 2.f)
	{
		context.SetIsFindPlayer(true);
		return make_unique<Gangster_Run>(context);
	}

	return nullptr;
}

void Gangster_Idle::Exit()
{
}

#pragma endregion

#pragma region Gangster_Run

Gangster_Run::Gangster_Run(Gangster& context)
	:GangsterFSM(context)
{
}

Gangster_Run::~Gangster_Run()
{
}

void Gangster_Run::Enter()
{
	context.SetState(EGangsterState::Run);
	Play(GANGSTER_RUN, true);
}

uptr<GangsterFSM> Gangster_Run::Update(float deltaTime)
{
	float disY = abs(context.GetDirToPlayer().y);
	float disX = abs(context.GetDirToPlayer().x);
	if (disY >= 200)
	{
		context.SetIsFindPlayer(false);
		return make_unique<Gangster_Idle>(context);
	}

	context.GetDirToPlayer().x >= 0 ? MoveRight() : MoveLeft();

	if (disY <= 60.f && disX <= 250.f)
	{
		context.SetIsFindPlayer(true);
		return make_unique<Gangster_Attack>(context);
	}

	return nullptr;
}

void Gangster_Run::Exit()
{
}

#pragma endregion

#pragma region Gangster_Attack

Gangster_Attack::Gangster_Attack(Gangster& context)
	:GangsterFSM(context)
{
}

Gangster_Attack::~Gangster_Attack()
{
}

void Gangster_Attack::Enter()
{
	context.SetState(EGangsterState::Attack);
	Play(GANGSTER_ATTACK, true);

	context.SetVelocity({ 0.f,0.f });
}

uptr<GangsterFSM> Gangster_Attack::Update(float deltaTime)
{
	if (ANIM.IsFinished())
		return make_unique<Gangster_Idle>(context);

	int frame = ANIM.GetCurrentFrameIndex();

	if (frame >= 3 && !isAttack)
	{
		isAttack = true;
		auto bullet = make_unique<Bullet>();
		Vector2f offset = Vector2f(0.f, -22.f);
		context.GetDesc().bFace ? offset.x = 25.f : offset.x = -25.f;
		bullet->GetDesc().vSpawnPoint = context.GetPosition() + offset;
		context.GetDesc().bFace ? bullet->SetVelocity({ 10.f,0.f }) : bullet->SetVelocity({ -10.f, 0.f });
		GAME.GetObjectManager().AddObject(EObjectTag::EnemyAttack, ERenderLayer::Effect, move(bullet));

		auto effect = make_unique<SingleEffect>();
		offset = Vector2f(0.f, -22.f);
		context.GetDesc().bFace ? offset.x = 55.f : offset.x = -55.f;
		effect->GetDesc().vSpawnPoint = context.GetPosition() + offset;
		effect->GetDesc().bFace = context.GetDesc().bFace;
		effect->SetEffectType(ESingleEffectType::Bullet_Smoke);
		GAME.GetObjectManager().AddObject(EObjectTag::Default, ERenderLayer::Effect, move(effect));

		effect = make_unique<SingleEffect>();
		offset = Vector2f(0.f, -22.f);
		context.GetDesc().bFace ? offset.x = 55.f : offset.x = -55.f;
		effect->GetDesc().vSpawnPoint = context.GetPosition() + offset;
		effect->GetDesc().bFace = context.GetDesc().bFace;
		effect->SetEffectType(ESingleEffectType::Bullet_Spark);
		GAME.GetObjectManager().AddObject(EObjectTag::Default, ERenderLayer::Effect, move(effect));
	}

	return nullptr;
}

void Gangster_Attack::Exit()
{
	context.SetAttackCool(0.f);
}

#pragma endregion


#pragma region Gangster_Hit

Gangster_Hit::Gangster_Hit(Gangster& context)
	:GangsterFSM(context)
{
}

Gangster_Hit::~Gangster_Hit()
{
}

void Gangster_Hit::Enter()
{
	context.SetState(EGangsterState::Hit);
	Play(GANGSTER_HIT, true);
	context.SetActive(false);

	Vector2f dirHit = context.GetDirHit();

	if (dirHit.y >= 0)
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

uptr<GangsterFSM> Gangster_Hit::Update(float deltaTime)
{
	return nullptr;
}

void Gangster_Hit::Exit()
{
}

#pragma endregion

#pragma region Gangster_Hit_Roll

Gangster_Hit_Roll::Gangster_Hit_Roll(Gangster& context)
	:GangsterFSM(context)
{
}

Gangster_Hit_Roll::~Gangster_Hit_Roll()
{
}

void Gangster_Hit_Roll::Enter()
{
	context.SetState(EGangsterState::Hit_Roll);
	Play(GANGSTER_HIT_ROLL, true);
}

uptr<GangsterFSM> Gangster_Hit_Roll::Update(float deltaTime)
{
	return nullptr;
}

void Gangster_Hit_Roll::Exit()
{
}

#pragma endregion

#pragma region Gangster_Hit_Ground

Gangster_Hit_Ground::Gangster_Hit_Ground(Gangster& context)
	:GangsterFSM(context)
{
}

Gangster_Hit_Ground::~Gangster_Hit_Ground()
{
}

void Gangster_Hit_Ground::Enter()
{
	context.SetState(EGangsterState::Hit_Ground);
	Play(GANGSTER_HIT_GROUND, true);
}

uptr<GangsterFSM> Gangster_Hit_Ground::Update(float deltaTime)
{
	return nullptr;
}

void Gangster_Hit_Ground::Exit()
{
}

#pragma endregion
