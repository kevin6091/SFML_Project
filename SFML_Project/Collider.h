#pragma once
#include "include.h"

class GameObject;
struct ColliderDesc;

class Collider
{
public:
	Collider(GameObject* pContext);
	~Collider();

public:
	void Initialize(ColliderDesc collider);
	
	__forceinline EColliderType GetColliderType()	{ return eColliderType; }
	__forceinline FloatRect& GetBounds()			{ return bounds; }
	__forceinline GameObject* GetContext()			{ return pContext; }

	__forceinline void SetColliderType(EColliderType eType) { eColliderType = eType; }

private:
	EColliderType eColliderType = EColliderType::End;
	
	// LineCollider의 경우 Position을 시작, Scacle을 끝.
	FloatRect bounds;
	GameObject* pContext;
};