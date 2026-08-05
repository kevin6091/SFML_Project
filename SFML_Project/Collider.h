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

private:
	EColliderType eColliderType = EColliderType::End;
	FloatRect bounds;
	GameObject* pContext;
};