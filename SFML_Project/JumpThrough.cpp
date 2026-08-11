#include "JumpThrough.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "GameInstance.h"

JumpThrough::JumpThrough(ColliderDesc desc)
{
	uCollider = make_unique<Collider>(this);
	uCollider->Initialize(desc);
}

JumpThrough::~JumpThrough()
{
}

void JumpThrough::Initialize()
{
	eRenderLayer = ERenderLayer::Background;
	eObjectTag = EObjectTag::Slope;
}

void JumpThrough::Update(float deltaTime)
{
}

void JumpThrough::LateUpdate(float deltaTime)
{
}

void JumpThrough::Render()
{
	GameInstance::GetInstance().GetRenderTarget_BG().draw(*this, RenderStates::Default);
}

void JumpThrough::Release()
{
}

void JumpThrough::draw(RenderTarget& target, RenderStates states) const
{
	RectangleShape debugBox(uCollider->GetBounds().size);
	debugBox.setPosition(uCollider->GetBounds().position);
	debugBox.setFillColor(Color(255, 0, 255, 30));

	debugBox.setOutlineThickness(1.0f);

	if (GameInstance::GetInstance().GetIsRenderDebug())
		target.draw(debugBox);
}
