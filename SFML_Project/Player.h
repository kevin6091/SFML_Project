#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"
#include "PlayerFSM.h"

enum class EPlayerState
{
	Idle,
	Run,
	Idle_To_Run,
	Run_To_Idle,
	Crouch,
	Roll,
	Jump,
	Attack,
	End
};

class Player : public GameObject
{
public:
	Player();
	virtual ~Player() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

public:
	__forceinline Animator& GetAnimator()	{ return animator; }
	__forceinline EPlayerState GetState()	{ return  curState; }
	__forceinline void SetState(EPlayerState eState) { curState = eState; }

private:
	uptr<PlayerFSM> curFSM;
	EPlayerState curState = EPlayerState::End;


	Animator animator;
};



