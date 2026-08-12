#include "GameObject.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Door.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::SetFace(bool face)
{
    descStatus.bFace = face;
    if (face)
    {
        Vector2f originScale = (*sprite).getScale();
        if(originScale.x < 0)
            sprite->setScale(Vector2f(originScale.x * -1.0f, originScale.y * 1.0f));
    }
    else
    {
        Vector2f originScale = (*sprite).getScale();
        if (originScale.x > 0)
            sprite->setScale(Vector2f(originScale.x * -1.0f, originScale.y * 1.0f));
    }
}

bool GameObject::IsInDoor()
{
    float playerX = GameInstance::GetInstance().GetPlayer()->GetPosition().x;
    float playerY = GameInstance::GetInstance().GetPlayer()->GetPosition().y;
    float myX = position.x;
    float myY = position.y;

    auto list = GameInstance::GetInstance().GetObjectManager().GetObjects(EObjectTag::Door);
    for (auto& door : list)
    {
        float doorX = door->GetPosition().x;
        float doorY = door->GetPosition().y + 64;
        
        if (abs(myY - doorY) >= 100.f || abs(playerY - doorY) >= 100.f)
            return true;

        if (doorX >= min(playerX, myX) && doorX <= max(playerX, myX)) 
        {
            if (isDoorOpen)
                continue;
            return true;
        }
    }

    return false;
}

Sprite GameObject::CenterAlign_Sprite(Sprite sprite)
{
    Vector2u size = sprite.getTexture().getSize();
    sprite.setOrigin({ static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.5f });

    return sprite;
}

void GameObject::SetPosition(const Vector2f& pos)
{
    prePosition = position;
    Vector2f offset = { 0.f, 0.f };
    if (uCollider) 
    {
        // 기존 오프셋(거리) 저장
        offset = uCollider->GetBounds().position - position;
    }

    position = pos;
    if (sprite) 
        sprite->setPosition(pos);

    if (uCollider) 
    {
        // 새로운 위치에 오프셋을 더해서 덮어쓰기 방지
        uCollider->GetBounds().position = pos + offset;
    }
}
