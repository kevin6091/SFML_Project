#include "SceneStage2.h"
#include "GameInstance.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"

#include "Camera.h"
#include "Player.h"
#include "Grunt.h"
#include "Block.h"
#include "Grippable.h"
#include "Stage2_BackGround.h"
#include "EndBlock.h"

void SceneStage2::Initialize()
{
	auto& gameInstance = GameInstance::GetInstance();
	auto& resourceManager = gameInstance.GetResourceManager();
	auto& objectManager = gameInstance.GetObjectManager();

	gameInstance.GetCamera().Initialize(Vector2f((float)WIDTH, (float)HEIGHT));
	gameInstance.GetCamera().SetOffSet(Vector2f(0.f, -60.f));

	// BackGround
	resourceManager.LoadTexture(MAP_STAGE2, "resource/textures/map/stage2");
	auto size = resourceManager.GetTexture(MAP_STAGE2)->getSize();
	gameInstance.GetCamera().SetBounds(FloatRect({ 0,0 }, { (float)size.x, (float)size.y }));

#pragma region MapInfo, Object

	 auto stage2 = make_unique<Stage2_BackGround>();
	 objectManager.AddObject(EObjectTag::Default, ERenderLayer::Background, move(stage2));

	vector<ColliderDesc> outColliders;
	vector<MapObjectInfo> outObjects;
	Vector2f outMapSize;

	resourceManager.LoadMap("resource/map_info/stage2_info.json", outColliders, outObjects, outMapSize);

	for (auto& col : outColliders)
	{
		if (col.type == EColliderType::Block)
		{
			auto block = make_unique<Block>(col);
			objectManager.AddObject(EObjectTag::Wall, ERenderLayer::Background, move(block));
		}
		else if (col.type == EColliderType::Grippable)
		{
			auto grippable = make_unique<Grippable>(col);
			objectManager.AddObject(EObjectTag::Wall, ERenderLayer::Background, move(grippable));
		}
		else if (col.type == EColliderType::EndBlock)
		{
			auto endBlock = make_unique<EndBlock>(col);
			objectManager.AddObject(EObjectTag::Wall, ERenderLayer::Background, move(endBlock));
		}
		else if (col.type == EColliderType::SlopBlock1 || col.type == EColliderType::SlopBlock2)
		{
			auto block = make_unique<Block>(col);
			objectManager.AddObject(EObjectTag::Wall, ERenderLayer::Background, move(block));
		}
		else if (col.type == EColliderType::JumpThrough)
		{
			auto block = make_unique<Block>(col);
			objectManager.AddObject(EObjectTag::Wall, ERenderLayer::Background, move(block));
		}
	}

	for (auto& obj : outObjects)
	{
		if (obj.layerName == "player")
		{
			GameInstance::GetInstance().GetPlayer()->GetDesc().vSpawnPoint = obj.position;
			GameInstance::GetInstance().GetPlayer()->RestartObject();
		}
		else if (obj.objectName == "obj_enemy_grunt")
		{
			auto grunt = make_unique<Grunt>();
			grunt->GetDesc().vSpawnPoint = obj.position;
			obj.scale.x >= 0 ? grunt->GetDesc().bFace = true : grunt->GetDesc().bFace = false;
			objectManager.AddObject(EObjectTag::Enemy, ERenderLayer::Actor, move(grunt));
		}
	}

#pragma endregion
}

void SceneStage2::Update(float deltaTime)
{
	GameInstance::GetInstance().GetObjectManager().Update(deltaTime);
}

void SceneStage2::LateUpdate(float deltaTime)
{
	GameInstance::GetInstance().GetObjectManager().LateUpdate(deltaTime);
}

void SceneStage2::Render()
{
	GameInstance::GetInstance().GetObjectManager().Render();
}

void SceneStage2::RenderUI()
{
	GameInstance::GetInstance().GetObjectManager().RenderUI();
}

void SceneStage2::Release()
{
	GameInstance::GetInstance().GetObjectManager().ReleaseScene();
}

void SceneStage2::RestartScene()
{
	GameInstance::GetInstance().GetObjectManager().RestartObject();
}
