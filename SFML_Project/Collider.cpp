#include "Collider.h"
#include "GameObject.h"
#include "ResourceManager.h"

Collider::Collider(GameObject* _pContext)
	: pContext(_pContext)
{
}

Collider::~Collider()
{
}

void Collider::Initialize(ColliderDesc collider)
{
	eColliderType = collider.type;
	bounds = collider.bounds;
}