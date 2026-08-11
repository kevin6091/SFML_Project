#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"

class BloodDecal : public GameObject
{
public:
	BloodDecal();
	virtual ~BloodDecal() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

	__forceinline void	SetBloodNum(int n) { bloodNum = n; }
	__forceinline void	SetAttackDir(Vector2f dir) { attackDir = dir; }
	__forceinline void	SetIsSingle(bool b) { isSingle = b; }

private:
	int			bloodNum = 0;
	optional<Sprite> sprite2;

	Vector2f	attackDir;

	bool	isSingle = false;
	bool	isRewinding = false;

	float accTime = 0.f;
};

