#pragma once
#include "include.h"	
#include "GameObject.h"
#include "Animator.h"

class SlashImpact : public GameObject
{
public:
	SlashImpact();
	virtual ~SlashImpact() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

public:
	__forceinline void SetImpactNum(int i) { impactNum = i; }
	__forceinline void SetHitDir(Vector2f dir) { hitDir = dir; }

private:
	Animator animator;
	int impactNum;
	Vector2f hitDir;
};

