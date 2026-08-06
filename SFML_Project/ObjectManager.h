#pragma once
#include "include.h"
#include "GameObject.h"

class ObjectManager
{
public:
    ObjectManager() = default;
    ~ObjectManager() = default;

public:
    void Initialize();
    void Update(float deltaTime);
    void LateUpdate(float deltaTime);
    void Render();
    void RenderUI();
    void Release();

    void AddObject(EObjectTag eTag, ERenderLayer eLayer, sptr<GameObject> obj);
    void ReleaseScene();

    void RestartObject();

private:
    unordered_map<EObjectTag, list<sptr<GameObject>>> objects;
    unordered_map<ERenderLayer, list<sptr<GameObject>>> renderObjects;
};