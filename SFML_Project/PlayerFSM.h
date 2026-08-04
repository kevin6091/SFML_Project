#pragma once
#include "FSM.h"

class Player;

class PlayerFSM : public FSM
{
public:
	PlayerFSM() = delete;
	PlayerFSM(Player& context);
	virtual ~PlayerFSM();

	virtual void Enter() override;
	virtual uptr<PlayerFSM> Update(float deltaTime);
	virtual void Exit() override;

protected:
	virtual void Play(const string& name, bool forceReset);

protected:
	Player& context;
};

class Player_Idle : public PlayerFSM
{
public:
	Player_Idle(Player& context);
	virtual ~Player_Idle();

	virtual void Enter() override;
	virtual uptr<PlayerFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Player_Run : public PlayerFSM
{
public:
	Player_Run(Player& context);
	virtual ~Player_Run();

	virtual void Enter() override;
	virtual uptr<PlayerFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Player_Idle_To_Run : public PlayerFSM
{
public:
	Player_Idle_To_Run(Player& context);
	virtual ~Player_Idle_To_Run();

	virtual void Enter() override;
	virtual uptr<PlayerFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Player_Run_To_Idle : public PlayerFSM
{
public:
	Player_Run_To_Idle(Player& context);
	virtual ~Player_Run_To_Idle();

	virtual void Enter() override;
	virtual uptr<PlayerFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Player_Crouch : public PlayerFSM
{
public:
	Player_Crouch(Player& context);
	virtual ~Player_Crouch();

	virtual void Enter() override;
	virtual uptr<PlayerFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Player_Crouch_To_Idle : public PlayerFSM
{
public:
	Player_Crouch_To_Idle(Player& context);
	virtual ~Player_Crouch_To_Idle();

	virtual void Enter() override;
	virtual uptr<PlayerFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Player_Roll : public PlayerFSM
{
public:
	Player_Roll(Player& context);
	virtual ~Player_Roll();

	virtual void Enter() override;
	virtual uptr<PlayerFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};

class Player_Attack : public PlayerFSM
{
public:
	Player_Attack(Player& context);
	virtual ~Player_Attack();

	virtual void Enter() override;
	virtual uptr<PlayerFSM> Update(float deltaTime) override;
	virtual void Exit() override;
};