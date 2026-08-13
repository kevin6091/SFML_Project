#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"

class Smoke : public GameObject
{
public:
	Smoke();
	virtual ~Smoke() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

	virtual void CollisionEvent(GameObject& other) override;

private:
	Animator animator;
	float accTime = 0.f;
	float m_driftTime = 0.f;  // 좌우 흔들림을 위한 사인 곡선 타이머
	float m_friction = 0.92f; // 공기 저항 (1.0에 가까울수록 서서히 멈춤)
};

