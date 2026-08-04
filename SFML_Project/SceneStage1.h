#pragma once
#include "Scene.h"

class SceneStage1 : public Scene
{
public:
	SceneStage1() = default;
	~SceneStage1() = default;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void RenderUI() override;
	virtual void Release() override;
};

