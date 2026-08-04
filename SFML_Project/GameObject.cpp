#include "GameObject.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "GameInstance.h"
#include "ResourceManager.h"

void GameObject::SetFace(bool face)
{
    if (descStatus.bFace != face)
    {
        descStatus.bFace = face;
        Vector2f originScale = (*sprite).getScale();
        (*sprite).setScale(Vector2f(originScale.x * -1.0f, originScale.y * 1.0f));
    }
}

Sprite GameObject::CenterAlign_Sprite(Sprite sprite)
{
    Vector2u size = sprite.getTexture().getSize();
    sprite.setOrigin({ static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.5f });

    return sprite;
}
