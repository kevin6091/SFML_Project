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
	Fall,
	Attack,
	WallSlide,
	Flip,

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

	virtual void CollisionEvent(GameObject& other);

public:
	__forceinline Animator& GetAnimator()				{ return animator; }
	__forceinline EPlayerState GetState()				{ return  curState; }
	__forceinline void SetState(EPlayerState eState)	{ curState = eState; }

	__forceinline void AddAttackCount()			{ attackCount++; }
	__forceinline void ResetAttackCount()		{ attackCount = 0; }
	__forceinline int GetAttackCount()			{ return attackCount; }
	__forceinline float GetAttackCool()			{ return accAttackCool; }
	__forceinline void ResetAttackCool()		{ accAttackCool = 0.0f; }

	__forceinline void SetGravityFactor(float _factor)	{ gravityFactor = _factor; } // 중력 조절

	__forceinline void SetIsGrippable(bool boolean)		{ isGrippable = boolean; }	
	__forceinline bool GetIsGrippable()					{ return isGrippable; }	// 벽타기 벽에 출동중인지

private:
	void ForceChangeFSM(uptr<PlayerFSM> fsm);

private:
	uptr<PlayerFSM> curFSM;
	EPlayerState curState = EPlayerState::End;

	Animator animator;

	int		attackCount = 0;
	float	accAttackCool = 0.0f;
	float	gravityFactor = 1.0f;

	float	grippableEnd = 0.f;
	bool	isGrippable = false;
};



