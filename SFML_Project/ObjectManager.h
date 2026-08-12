#pragma once
#include "include.h"
#include "GameObject.h"

class Player;

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

    Player* GetPlayer();
    list<sptr<GameObject>>  GetObjects(EObjectTag tag);
    
    bool IsNoneEnemy();

private:
    unordered_map<EObjectTag, list<sptr<GameObject>>> objects;
    unordered_map<ERenderLayer, list<sptr<GameObject>>> renderObjects;
};