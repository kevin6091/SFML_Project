#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"
#include "PlayerFSM.h"
#include "RewindTracker.h"

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
	Hit_Begin,
	Hit_Loop,
	Hit_Ground,
	Hit_Recover,
	End
};

class Slash;

struct TrailInfo
{
	Sprite sprite;  
	float lifeTime;     
	float maxLifeTime;  
};

struct PlayerSnapshot 
{
	Vector2f position;
	Vector2f velocity;
	Vector2f scale;
	EPlayerState fsmState;
	IntRect textureRect;
	const Texture* texture;
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
	virtual void RestartObject() override;

public:
	__forceinline Animator& GetAnimator()				{ return animator; }
	__forceinline EPlayerState GetState()				{ return  curState; }
	__forceinline void SetState(EPlayerState eState)	{ curState = eState; }

	__forceinline void AddAttackCount()					{ attackCount++; }
	__forceinline void ResetAttackCount()				{ attackCount = 0; }
	__forceinline int GetAttackCount()					{ return attackCount; }
	__forceinline float GetAttackCool()					{ return accAttackCool; }
	__forceinline void ResetAttackCool()				{ accAttackCool = 0.0f; }
	__forceinline Vector2f GetAttackDir()				{ return attackDir; }
	__forceinline void SetAttackDir(Vector2f dir)		{ attackDir = dir; }

	__forceinline void SetGravityFactor(float _factor)	{ gravityFactor = _factor; } // 중력 조절

	__forceinline void SetIsGrippable(bool boolean)		{ isGrippable = boolean; }	
	__forceinline bool GetIsGrippable()					{ return isGrippable; }	// 벽타기 벽에 출동중인지

	__forceinline Slash& GetSlash()						{ return *pSlash; }
	__forceinline void SetIsDead(bool b)				{ isDead = b; }
	__forceinline bool GetIsDead()						{ return isDead; }

	__forceinline void SetHitDir(Vector2f dir)			{ hitDir = dir; }
	__forceinline Vector2f GetHitDir()					{ return hitDir; }

	void ForceChangeFSM(uptr<PlayerFSM> fsm);
private:
	void TrailUpdate(float deltaTime);
	void TrailRender(RenderTarget& target) const;

private:
	uptr<PlayerFSM> curFSM;
	EPlayerState curState = EPlayerState::End;
	Animator animator;
	
	Slash* pSlash = nullptr;

	Vector2f hitDir;

#pragma region Trail
	
	list<TrailInfo> trails;
	float trailSpawnTimer = 0.f;
	float trail_SpawnInteval = 0.01f; // 잔상이 생성되는 간격 (초)
	float trail_LifeTime = 0.2f;       // 잔상이 지속되는 시간 (초)

#pragma endregion

#pragma region Rewind

	RewindTracker<PlayerSnapshot> rewinder;
	float rewindSpeed = 1.f;
	float	rewinderTime = 0.f;

#pragma endregion

#pragma region Attack

	int			attackCount = 0;
	float		accAttackCool = 0.0f;
	Vector2f	attackDir;

#pragma endregion

#pragma region Contents

	float	gravityFactor = 1.0f;

	float	grippableEnd = 0.f;
	bool	isGrippable = false;

	bool	isDead = false;

#pragma endregion

};



