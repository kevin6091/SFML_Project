#include "MouseCursor.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "Camera.h"

MouseCursor::MouseCursor()
{
	eObjectTag = EObjectTag::NonDestroy;
	eRenderLayer = ERenderLayer::UI;
}

MouseCursor::~MouseCursor()
{
}

void MouseCursor::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();

	sprite.emplace(*resource.GetTexture("cursor"));
	auto size = sprite->getTexture().getSize();
	sprite->setOrigin({ (float)size.x * 0.5f, (float)size.y * 0.5f });
}

void MouseCursor::Update(float deltaTime)
{
	Vector2f pos = GameInstance::GetInstance().GetInputManager().GetMouseWorldPos();
	sprite->setPosition(pos);
}

void MouseCursor::LateUpdate(float deltaTime)
{
}

void MouseCursor::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*sprite, BlendAlpha);
}

void MouseCursor::Release()
{
}

void MouseCursor::draw(RenderTarget& target, RenderStates states) const
{
}
