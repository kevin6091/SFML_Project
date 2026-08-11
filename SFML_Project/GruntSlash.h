#pragma once
#include "include.h"	
#include "GameObject.h"
#include "Animator.h"

class GruntSlash : public GameObject
{
public:
	GruntSlash();
	virtual ~GruntSlash() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;
	virtual void RestartObject() override;

	virtual void CollisionEvent(GameObject& other) override;

private:
	Animator animator;
};