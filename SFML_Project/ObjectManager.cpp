#include "ObjectManager.h"
#include "GameObject.h"

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
                auto renderIter = find(renderObjects[(*it)->GetLayer()].begin(), renderObjects[(*it)->GetLayer()].begin(), (*it));
                renderObjects[(*it)->GetLayer()].erase(renderIter);
                it = tag.second.erase(it);
            }
            else
                it++;
        }
    }
}

void ObjectManager::Render(RenderWindow& window)
{
    for (auto& obj : renderObjects[ERenderLayer::Background])
        obj->Render();
}

void ObjectManager::AddObject(EObjectTag eTag, ERenderLayer eLayer, sptr<GameObject> obj)
{
    
}

void GameObject::Release()
{
}