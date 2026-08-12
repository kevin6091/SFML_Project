#pragma once
#include "include.h"
#include "FSM.h"

class Gangster;

class GangsterFSM : FSM
{
public:
	GangsterFSM() = delete;
	GangsterFSM(Gangster& context);
	virtual ~GangsterFSM() override;

	virtual void Enter() override;
	virtual uptr<GangsterFSM> Update(float deltaTime);
	virtual void Exit() override;

protected:
	virtual void Play(const string& name, bool forceReset);
	virtual void MoveLeft();
	virtual void MoveRight();
	virtual void BreakX(float deltaTime);

protected:
	Gangster& context;
};

class Gangster_Idle : public GangsterFSM
{
public:
	Gangster_Idle() = delete;
	Gangster_Idle(Gangster& context);
	virtual ~Gangster_Idle();

	virtual void Enter() override;
	virtual uptr<GangsterFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Gangster_Run : public GangsterFSM
{
public:
	Gangster_Run() = delete;
	Gangster_Run(Gangster& context);
	virtual ~Gangster_Run();

	virtual void Enter() override;
	virtual uptr<GangsterFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Gangster_Attack : public GangsterFSM
{
public:
	Gangster_Attack() = delete;
	Gangster_Attack(Gangster& context);
	virtual ~Gangster_Attack();

	virtual void Enter() override;
	virtual uptr<GangsterFSM> Update(float deltaTime) override;
	virtual void Exit() override;

private:
	bool isAttack = false;
};

class Gangster_Hit : public GangsterFSM
{
public:
	Gangster_Hit() = delete;
	Gangster_Hit(Gangster& context);
	virtual ~Gangster_Hit();

	virtual void Enter() override;
	virtual uptr<GangsterFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Gangster_Hit_Roll : public GangsterFSM
{
public:
	Gangster_Hit_Roll() = delete;
	Gangster_Hit_Roll(Gangster& context);
	virtual ~Gangster_Hit_Roll();

	virtual void Enter() override;
	virtual uptr<GangsterFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Gangster_Hit_Ground : public GangsterFSM
{
public:
	Gangster_Hit_Ground() = delete;
	Gangster_Hit_Ground(Gangster& context);
	virtual ~Gangster_Hit_Ground();

	virtual void Enter() override;
	virtual uptr<GangsterFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};