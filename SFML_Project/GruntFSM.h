#pragma once
#include "FSM.h"

class Grunt;


class GruntFSM : public FSM
{
public:
	GruntFSM() = delete;
	GruntFSM(Grunt& context);
	virtual ~GruntFSM();

	virtual void Enter() override;
	virtual uptr<GruntFSM> Update(float deltaTime);
	virtual void Exit() override;

protected:
	virtual void Play(const string& name, bool forceReset);
	virtual void MoveLeft();
	virtual void MoveRight();
	virtual void BreakX(float deltaTime);

protected:
	Grunt& context;
};

class Grunt_Idle : public GruntFSM
{
public:
	Grunt_Idle() = delete;
	Grunt_Idle(Grunt& context);
	virtual ~Grunt_Idle();

	virtual void Enter() override;
	virtual uptr<GruntFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Grunt_Run : public GruntFSM
{
public:
	Grunt_Run() = delete;
	Grunt_Run(Grunt& context);
	virtual ~Grunt_Run();

	virtual void Enter() override;
	virtual uptr<GruntFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Grunt_Hit : public GruntFSM
{
public:
	Grunt_Hit() = delete;
	Grunt_Hit(Grunt& context);
	virtual ~Grunt_Hit();

	virtual void Enter() override;
	virtual uptr<GruntFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Grunt_Hit_Roll : public GruntFSM
{
public:
	Grunt_Hit_Roll() = delete;
	Grunt_Hit_Roll(Grunt& context);
	virtual ~Grunt_Hit_Roll();

	virtual void Enter() override;
	virtual uptr<GruntFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Grunt_Hit_Ground : public GruntFSM
{
public:
	Grunt_Hit_Ground() = delete;
	Grunt_Hit_Ground(Grunt& context);
	virtual ~Grunt_Hit_Ground();

	virtual void Enter() override;
	virtual uptr<GruntFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};