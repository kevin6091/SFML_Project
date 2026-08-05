#include "Stage1_BackGround.h"
#include "GameInstance.h"
#include "ResourceManager.h"

Stage1_BackGround::Stage1_BackGround()
{
}

Stage1_BackGround::~Stage1_BackGround()
{
}

void Stage1_BackGround::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();

	sprite.emplace(*resource.GetTexture(MAP_STAGE1));
}

void Stage1_BackGround::Update(float deltaTime)
{
}

void Stage1_BackGround::LateUpdate(float deltaTime)
{
}

void Stage1_BackGround::Render()
{
	GameInstance::GetInstance().Draw(*this, RenderStates::Default);
}

void Stage1_BackGround::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}

void Stage1_BackGround::Release()
{
}
