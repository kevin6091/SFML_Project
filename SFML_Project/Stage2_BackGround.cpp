#include "Stage2_BackGround.h"
#include "GameInstance.h"
#include "ResourceManager.h"

Stage2_BackGround::Stage2_BackGround()
{
	eObjectTag = EObjectTag::Default;
	eRenderLayer = ERenderLayer::Background;
}

Stage2_BackGround::~Stage2_BackGround()
{
}

void Stage2_BackGround::Initialize()
{

	auto& resource = GameInstance::GetInstance().GetResourceManager();

	sprite.emplace(*resource.GetTexture(MAP_STAGE2));
}

void Stage2_BackGround::Update(float deltaTime)
{
}

void Stage2_BackGround::LateUpdate(float deltaTime)
{
}

void Stage2_BackGround::Render()
{
	GameInstance::GetInstance().GetRenderTarget_BG().draw(*this, BlendAlpha);
}

void Stage2_BackGround::Release()
{
}

void Stage2_BackGround::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}
