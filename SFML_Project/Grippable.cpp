#include "Grippable.h"
#include "ResourceManager.h"
#include "Collider.h"   
#include "GameInstance.h"

Grippable::Grippable(ColliderDesc desc)
{
    uCollider = make_unique<Collider>(this);
    uCollider->Initialize(desc);
}

Grippable::~Grippable()
{
}

void Grippable::Initialize()
{
    eRenderLayer = ERenderLayer::Background;
    eObjectTag = EObjectTag::Wall;
}

void Grippable::Update(float deltaTime)
{
}

void Grippable::LateUpdate(float deltaTime)
{
}

void Grippable::Render()
{
    GameInstance::GetInstance().GetRenderTarget_BG().draw(*this, RenderStates::Default);
}

void Grippable::Release()
{
}

void Grippable::draw(RenderTarget& target, RenderStates states) const
{
    RectangleShape debugBox(uCollider->GetBounds().size);
    debugBox.setPosition(uCollider->GetBounds().position);
    debugBox.setFillColor(Color(255, 0, 0, 30));

    debugBox.setOutlineThickness(1.0f);

    if(GameInstance::GetInstance().GetIsRenderDebug())
        target.draw(debugBox);
}
