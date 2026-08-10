#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"

class Blood : public GameObject
{
public:
	Blood();
	virtual ~Blood() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

	__forceinline void SetDir(Vector2f v) { dir = v; }

private:
	vector<uptr<Animator>> vecAnimator;
	vector<optional<Sprite>> vecSprites;
	Vector2f	dir;
};

