#include "SlopeBlock.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "GameInstance.h"

SlopeBlock::SlopeBlock(ColliderDesc desc)
{
	uCollider = make_unique<Collider>(this);
	uCollider->Initialize(desc);
}

SlopeBlock::~SlopeBlock()
{
}

void SlopeBlock::Initialize()
{
	eRenderLayer = ERenderLayer::Background;
	eObjectTag = EObjectTag::Slope;
}

void SlopeBlock::Update(float deltaTime)
{
}

void SlopeBlock::LateUpdate(float deltaTime)
{
}

void SlopeBlock::Render()
{
	GameInstance::GetInstance().GetRenderTarget_BG().draw(*this, RenderStates::Default);
}

void SlopeBlock::Release()
{
}

void SlopeBlock::draw(RenderTarget& target, RenderStates states) const
{
	RectangleShape debugBox(uCollider->GetBounds().size);
	debugBox.setPosition(uCollider->GetBounds().position);
	debugBox.setFillColor(Color(0, 0, 255, 30));

	debugBox.setOutlineThickness(1.0f);

	if (GameInstance::GetInstance().GetIsRenderDebug())
		target.draw(debugBox);
}
