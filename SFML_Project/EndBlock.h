#pragma once
#include "include.h"
#include "GameObject.h"

struct ColliderDesc;

class EndBlock : public GameObject
{
public:
	EndBlock(ColliderDesc desc);
	virtual ~EndBlock() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

	virtual void CollisionEvent(GameObject& other) override;
};

