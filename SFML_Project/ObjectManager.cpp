#include "ObjectManager.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "Player.h"

void ObjectManager::Initialize()
{

}

void ObjectManager::Update(float deltaTime)
{
    for (auto& tag : objects)
    {
        for (auto& obj : tag.second)
        {
            obj->Update(deltaTime);
        }
    }

    auto& collisionManager = GameInstance::GetInstance().GetCollisionManager();

    collisionManager.UpdatePhysics(deltaTime, objects[EObjectTag::Player], objects[EObjectTag::Wall]);

    collisionManager.UpdatePhysics(deltaTime, objects[EObjectTag::Enemy], objects[EObjectTag::Wall]);

    collisionManager.CollisionTest(objects[EObjectTag::PlayerAttack], objects[EObjectTag::Enemy]);
}

void ObjectManager::LateUpdate(float deltaTime)
{
    for (auto& tag : objects)
    {
        for (auto& obj : tag.second)
        {
            obj->LateUpdate(deltaTime);
        }
    }

    // »èÁ¦
    for (auto& tag : objects)
    {
        if (tag.first == EObjectTag::Player)
            continue;

        for (auto it = tag.second.begin(); it != tag.second.end();)
        {
            if ((*it)->GetIsDestroy())
            {
                (*it)->Release();

                auto renderIter = find(renderObjects[(*it)->GetLayer()].begin(), renderObjects[(*it)->GetLayer()].begin(), (*it));
                renderObjects[(*it)->GetLayer()].erase(renderIter);
                it = tag.second.erase(it);
            }
            else
                it++;
        }
    }
}

void ObjectManager::Render()
{
    for (int i = 0; i < (int)ERenderLayer::UI; i++)
    {
        for (auto& obj : renderObjects[(ERenderLayer)i])
        {
            obj->Render();
        }
    }
}

void ObjectManager::RenderUI()
{
    for (int i = (int)ERenderLayer::UI; i < (int)ERenderLayer::End; i++)
    {
        for (auto& obj : renderObjects[(ERenderLayer)i])
        {
            if (obj->IsActive())
                obj->Render();
            else
                continue;
        }
    }
}

void ObjectManager::AddObject(EObjectTag eTag, ERenderLayer eLayer, sptr<GameObject> uObj)
{
    objects[eTag].push_back(uObj);
    renderObjects[eLayer].push_back(uObj);
    uObj->Initialize();
}

void ObjectManager::ReleaseScene()
{
    for (int i = (int)EObjectTag::Default; i < (int)EObjectTag::End; i++)
    {
        if ((EObjectTag)i == EObjectTag::Player)
            continue;

        for (auto& obj : objects[(EObjectTag)i])
        {
            obj->Release();
        }
        objects[(EObjectTag)i].clear();
    }

    for (int i = (int)ERenderLayer::Background; i < (int)ERenderLayer::End; i++)
    {
        if ((ERenderLayer)i == ERenderLayer::Actor)
            continue;
        renderObjects[(ERenderLayer)i].clear();
    }
}

void ObjectManager::RestartObject()
{
    for (int i = (int)EObjectTag::Default; i < (int)EObjectTag::End; i++)
    {
        for (auto& obj : objects[(EObjectTag)i])
        {
            obj->RestartObject();
        }
    }
}

Player* ObjectManager::GetPlayer()
{
    const auto& iter = objects[EObjectTag::Player].begin();
    return static_cast<Player*>((*iter).get());
}

void ObjectManager::Release()
{
    objects.clear();
    renderObjects.clear();
}