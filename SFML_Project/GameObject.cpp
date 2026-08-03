#include "GameObject.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "GameInstance.h"
#include "ResourceManager.h"

void GameObject::Init_Sprites(Animator& animator, optional<Sprite>& sprite, string name, float duration, bool loop)
{
    auto& resourceManager = GameInstance::GetInstance().GetResourceManager();

    if (auto runFrames = resourceManager.GetTextureSequence(name)) {
        animator.AddClip(name, AnimationClip(*runFrames, duration, loop));
        if (!runFrames->empty() && (*runFrames)[0])
            sprite.emplace(*(*runFrames)[0]);
    }
}

Sprite GameObject::CenterAlign_Sprite(Sprite sprite)
{
    Vector2u size = sprite.getTexture().getSize();
    sprite.setOrigin({ static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.5f });

    return sprite;
}
