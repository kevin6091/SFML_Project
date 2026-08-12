#include "Stage3_BackGround.h"
#include "GameInstance.h"
#include "ResourceManager.h"

Stage3_BackGround::Stage3_BackGround()
{
	eObjectTag = EObjectTag::Default;
	eRenderLayer = ERenderLayer::Background;
}

Stage3_BackGround::~Stage3_BackGround()
{
}

void Stage3_BackGround::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();

	sprite.emplace(*resource.GetTexture(MAP_STAGE3));
}

void Stage3_BackGround::Update(float deltaTime)
{
}

void Stage3_BackGround::LateUpdate(float deltaTime)
{
}

void Stage3_BackGround::Render()
{
	GameInstance::GetInstance().GetRenderTarget_BG().draw(*this, BlendAlpha);
}

void Stage3_BackGround::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}

void Stage3_BackGround::Release()
{
}
