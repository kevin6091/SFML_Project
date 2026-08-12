#pragma once
#include "include.h"
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet();
	virtual ~Bullet() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

	virtual void CollisionEvent(GameObject& other) override;

private:
	float accTime = 0.2f;
};

