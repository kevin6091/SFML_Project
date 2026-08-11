#pragma once
#include "Scene.h"

class SceneStage2 : public Scene
{
public:
	SceneStage2() = default;
	~SceneStage2() = default;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void RenderUI() override;
	virtual void Release() override;

	virtual void RestartScene() override;

};

