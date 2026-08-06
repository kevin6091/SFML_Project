#pragma once
#include "include.h"

class Animator;
class Collider;

class GameObject : public Drawable
{
public:
	struct StatusDesc
	{
		Vector2f vSpawnPoint;
		bool bFace = true; // false = 왼쪽, true = 오른쪽
	};

public:
	GameObject();
	virtual ~GameObject();

public:
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void draw(RenderTarget& target, RenderStates states) const override = 0;

	virtual void RestartObject() {};

 	__forceinline void Destroy()				{ isDestroy = true; }
	__forceinline bool GetIsDestroy() const		{ return isDestroy; }

	__forceinline void SetActive(bool active)	{ isActive = active; }
	__forceinline bool IsActive() const			{ return isActive; }

	__forceinline Vector2f GetPosition() const					{ return position; }
	virtual void SetPosition(const Vector2f& pos);

	__forceinline ERenderLayer GetLayer() const					{ return eRenderLayer; }
	__forceinline void SetLayer(ERenderLayer _layer)			{ eRenderLayer = _layer; }

	__forceinline EObjectTag GetTag() const						{ return eObjectTag; }
	__forceinline void SetTag(EObjectTag tag)					{ eObjectTag = tag; }

	__forceinline StatusDesc& GetDesc()							{ return descStatus; }
	__forceinline Collider& GetCollider()						{ return *uCollider; }

	__forceinline Vector2f GetVelocity()						{ return velocity; }
	__forceinline void SetVelocity(Vector2f _velocity)			{ velocity = _velocity; }

	__forceinline float GetSpeed()								{ return speed; }
	__forceinline void SetSpeed(float _speed)					{ speed = _speed; }

	_forceinline bool GetIsGrounded()							{ return isGrounded; }
	_forceinline void SetIsGrounded(bool boolean)				{ isGrounded = boolean; }

	void SetFace(bool face);
    
public:
	virtual void CollisionEvent(GameObject& other) {}

protected:
	Sprite CenterAlign_Sprite(Sprite sprite);

protected:
	Vector2f position{ 0.0f, 0.0f };
	Vector2f scale{ 1.0f, 1.0f };

	Vector2f velocity{ 0.0f,0.0f };
	float	speed{ 0.0f };

	ERenderLayer eRenderLayer = ERenderLayer::End;
	EObjectTag eObjectTag = EObjectTag::End;

	bool isActive = true;		// 그리거나 연산할지 말지
	bool isDestroy = false;		// Delete할지 말지
	bool isGrounded = false;	// 땅에 있는지

	optional<Sprite> sprite;
	StatusDesc descStatus;

	uptr<Collider>		uCollider;
};

