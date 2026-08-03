#pragma once
#include "include.h"
#include "Scene.h"

class SceneTitle : public Scene
{
public:
	SceneTitle() = default;
	~SceneTitle() = default;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void RenderUI() override;
	virtual void Release() override;

private:
	float accTime_Wait = 0.0f;
};

