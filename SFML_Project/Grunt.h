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
	Hit_Fly,
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

	virtual void CollisionEvent(GameObject& other) override;
	virtual void RestartObject() override;

public:
	__forceinline Animator& GetAnimator() { return animator; }
	__forceinline EGruntState GetState() { return  curState; }
	__forceinline void SetState(EGruntState eState) { curState = eState; }

private:
	void ForceChangeFSM(uptr<GruntFSM> fsm);

private:
	uptr<GruntFSM> curFSM;
	EGruntState curState = EGruntState::End;
	Animator animator;

#pragma region Rewind

	RewindTracker<GruntSnapshot> rewinder;
	float rewindSpeed = 1.f;
	float	rewinderTime = 0.f;

#pragma endregion

};

