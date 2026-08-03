#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"

class Title : public GameObject
{
public:
	Title();
	virtual ~Title() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

private:
	optional<Sprite> sprite_ZERO;
	optional<Sprite> sprite_ZER;
	optional<Sprite> sprite_O;

	optional<Sprite> sprite_Katana;
	optional<Sprite> sprite_Katana2;

	optional<Sprite> sprite_BG;
	optional<Sprite> sprite_Fence;

	optional<Sprite> sprite_run;
	Animator animator_run;

private:
	float accTargetMove = 0.0f;
	float accZER = 0.0f;
	float accZER_2 = 0.0f;
	float accO = 0.0f;
	float accO_2 = 0.0f;

	bool isZER = false;
	bool isO = false;
};

