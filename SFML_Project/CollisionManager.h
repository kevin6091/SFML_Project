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
	void CollisionTest(list<sptr<GameObject>> lObjs, list<sptr<GameObject>> rObjs);

private:
	bool LineCollisionTest(Vector2f start, Vector2f end, Vector2f pos, float thickness);
};