#include "Block.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "GameInstance.h"

Block::Block(ColliderDesc desc)
{
	uCollider = make_unique<Collider>(this);
	uCollider->Initialize(desc);
}

Block::~Block()
{
}

void Block::Initialize()
{
	eRenderLayer = ERenderLayer::Background;
	eObjectTag = EObjectTag::Wall;
}

void Block::Update(float deltaTime)
{
}

void Block::LateUpdate(float deltaTime)
{
}

void Block::Render()
{
	GameInstance::GetInstance().Draw(*this, RenderStates::Default);
}

void Block::draw(RenderTarget& target, RenderStates states) const
{
    RectangleShape debugBox(uCollider->GetBounds().size);
    debugBox.setPosition(uCollider->GetBounds().position);
    debugBox.setFillColor(Color(0,255,0,30));

    debugBox.setOutlineThickness(1.0f);

	if(GameInstance::GetInstance().GetIsRenderDebug())
		target.draw(debugBox);
}

void Block::Release()
{
}
