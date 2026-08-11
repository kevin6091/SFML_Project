#include "EndBlock.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "GameInstance.h"
#include "SceneManager.h"

EndBlock::EndBlock(ColliderDesc desc)
{
	uCollider = make_unique<Collider>(this);
	uCollider->Initialize(desc);
}

EndBlock::~EndBlock()
{
}

void EndBlock::Initialize()
{
	eRenderLayer = ERenderLayer::Background;
	eObjectTag = EObjectTag::Wall;
}

void EndBlock::Update(float deltaTime)
{
}

void EndBlock::LateUpdate(float deltaTime)
{
}

void EndBlock::Render()
{
	GameInstance::GetInstance().GetRenderTarget_BG().draw(*this, RenderStates::Default);
}

void EndBlock::Release()
{
}

void EndBlock::draw(RenderTarget& target, RenderStates states) const
{
	RectangleShape debugBox(uCollider->GetBounds().size);
	debugBox.setPosition(uCollider->GetBounds().position);
	debugBox.setFillColor(Color(0, 255, 0, 30));

	debugBox.setOutlineThickness(1.0f);

	if (GameInstance::GetInstance().GetIsRenderDebug())
		target.draw(debugBox);
}

void EndBlock::CollisionEvent(GameObject& other)
{
	if (other.GetTag() == EObjectTag::Player)
	{
		int sceneNum = (int) GameInstance::GetInstance().GetSceneManager().GetSceneType();
		sceneNum++;
		GameInstance::GetInstance().GetSceneManager().ChangeScene((ESceneType)sceneNum);
	}
}

