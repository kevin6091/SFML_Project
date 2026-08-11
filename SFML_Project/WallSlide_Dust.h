#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"

class WallSlide_Dust : public GameObject
{
public:
	WallSlide_Dust();
	virtual ~WallSlide_Dust() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

private:
	Animator animator;

};

