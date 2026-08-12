#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"

enum ESingleEffectType
{
	Bullet_Smoke,
	Bullet_Parry,
	Bullet_Spark,
	End
};

class SingleEffect : public GameObject
{
public:
	SingleEffect();
	virtual ~SingleEffect() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

	void SetEffectType(ESingleEffectType eType) { eEffectType = eType; }
	ESingleEffectType GetEffectType() { return eEffectType; }

private:
	Animator animator;
	ESingleEffectType eEffectType = ESingleEffectType::End;
};

