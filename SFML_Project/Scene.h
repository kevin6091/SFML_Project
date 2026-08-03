#pragma once
#include "include.h"

class ObjectManager;

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

public:
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void RenderUI() = 0;
	virtual void Release() = 0;

protected:
	ESceneType eSceneType = ESceneType::End;
};