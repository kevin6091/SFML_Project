#pragma once
#include "include.h"	
#include "Collider.h"
#include "GameObject.h"

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

public:
	void UpdatePhysics(float deltaTime, list<sptr<GameObject>> dynamics, list<sptr<GameObject>> statics);
};