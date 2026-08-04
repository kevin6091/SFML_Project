#include "ObjectManager.h"
#include "GameObject.h"
#include "ResourceManager.h"

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
            if ((*it)->IsDestroy())
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
            if (obj->IsActive())
                obj->Render();
            else
                continue;
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
    uObj->Initialize();
    objects[eTag].push_back(uObj);
    renderObjects[eLayer].push_back(uObj);
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
        if ((ERenderLayer)i == ERenderLayer::Player)
            continue;
        renderObjects[(ERenderLayer)i].clear();
    }
}

void ObjectManager::Release()
{
    objects.clear();
    renderObjects.clear();
}