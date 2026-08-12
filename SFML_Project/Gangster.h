#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"
#include "GangsterFSM.h"
#include "RewindTracker.h"

enum class EGangsterState
{
	Idle,
	Run,
	Attack,
	Hit,
	Hit_Roll,
	Hit_Ground,
	End,
};

struct GangsterSnapshot
{
	Vector2f position;
	Vector2f velocity;
	Vector2f scale;
	EGangsterState fsmState;
	IntRect textureRect;
	const Texture* texture;
};


class Gangster : public GameObject
{
public:
	Gangster();
	virtual ~Gangster() override;

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

public:
	__forceinline Animator& GetAnimator() { return animator; }
	__forceinline EGangsterState GetState() { return  curState; }
	__forceinline void SetState(EGangsterState eState) { curState = eState; }

	__forceinline Vector2f GetDirToPlayer() { return dirToPlayer; }
	__forceinline Vector2f GetDirHit() { return dirHit; }

	__forceinline void SetAttackCool(float f) { accAttackCool = f; }
	__forceinline float GetAttackCool() { return accAttackCool; }
	__forceinline void SetIsFindPlayer(bool b) { isFindPlayer = b; }
	__forceinline bool GetIsFindPlayer() { return isFindPlayer; }

	void ForceChangeFSM(uptr<GangsterFSM> fsm);

private:
	uptr<GangsterFSM> curFSM;
	EGangsterState curState = EGangsterState::End;
	Animator animator;

#pragma region Rewind

	RewindTracker<GangsterSnapshot> rewinder;
	float rewindSpeed = 1.f;
	float	rewinderTime = 0.f;

#pragma endregion

};

