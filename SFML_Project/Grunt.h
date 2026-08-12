#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"
#include "GruntFSM.h"
#include "RewindTracker.h"

enum class EGruntState
{
	Idle,
	Run,
	Attack,
	Hit,
	Hit_Roll,
	Hit_Ground,
	End,
};

struct GruntSnapshot
{
	Vector2f position;
	Vector2f velocity;
	Vector2f scale;
	EGruntState fsmState;
	IntRect textureRect;
	const Texture* texture;
};

class Grunt : public GameObject
{
public:
	Grunt();
	virtual ~Grunt() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;
	virtual void RestartObject() override;

	virtual void CollisionEvent(GameObject& other) override;
	virtual void CollisionBounce() override;
	virtual void CollisionBounceEnd() override;

	virtual void FanHit() override;
public:
	__forceinline Animator& GetAnimator()			{ return animator; }
	__forceinline EGruntState GetState()			{ return  curState; }
	__forceinline void SetState(EGruntState eState) { curState = eState; }

	__forceinline Vector2f GetDirToPlayer()			{ return dirToPlayer; }
	__forceinline Vector2f GetDirHit()				{ return dirHit; }

	__forceinline void SetAttackCool(float f)		{ accAttackCool = f; }
	__forceinline float GetAttackCool()				{ return accAttackCool; }
	__forceinline void SetIsFindPlayer(bool b)		{ isFindPlayer = b;}
	__forceinline bool GetIsFindPlayer()			{ return isFindPlayer;}
	__forceinline void SetIsGrunt(bool b)			{ isGrunt = b; }
	__forceinline bool GetIsGrunt()					{ return isGrunt; }

	void ForceChangeFSM(uptr<GruntFSM> fsm);

private:
	uptr<GruntFSM> curFSM;
	EGruntState curState = EGruntState::End;
	Animator animator;

	bool isGrunt = true;

#pragma region Rewind

	RewindTracker<GruntSnapshot> rewinder;
	float rewindSpeed = 1.f;
	float	rewinderTime = 0.f;

#pragma endregion

};

