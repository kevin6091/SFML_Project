#pragma once
#include "include.h"
#include "GameObject.h"
class Stage3_BackGround : public GameObject
{
public:
	Stage3_BackGround();
	virtual ~Stage3_BackGround() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;
};