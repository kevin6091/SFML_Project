#pragma once
#include "include.h"
#include "GameObject.h"

class HitLine : public GameObject
{
public:
	HitLine();
	virtual ~HitLine() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

	__forceinline void SetDir(Vector2f v) { dir = v; }
	__forceinline Vector2f GetDir() { return dir; }

private:
	Vector2f dir;
	float accTime = 0.f; 
	float accLifeTime = 0.f; 
};

