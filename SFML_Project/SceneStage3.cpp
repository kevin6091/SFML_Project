#include "SceneStage3.h"
#include "GameInstance.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"

#include "Camera.h"
#include "Player.h"
#include "Grunt.h"
#include "Gangster.h"
#include "Block.h"
#include "Grippable.h"
#include "Stage3_BackGround.h"
#include "EndBlock.h"
#include "Door.h"

void SceneStage3::Initialize()
{
	auto& gameInstance = GameInstance::GetInstance();
	auto& resourceManager = gameInstance.GetResourceManager();
	auto& objectManager = gameInstance.GetObjectManager();

	gameInstance.GetCamera().Initialize(Vector2f((float)WIDTH, (float)HEIGHT));
	gameInstance.GetCamera().SetOffSet(Vector2f(0.f, -60.f));

	// BackGround
	resourceManager.LoadTexture(MAP_STAGE3, "resource/textures/map/stage2");
	auto size = resourceManager.GetTexture(MAP_STAGE3)->getSize();
	gameInstance.GetCamera().SetBounds(FloatRect({ 0,0 }, { (float)size.x, (float)size.y }));

	// Resource

#pragma region MapInfo, Object

	auto stage3 = make_unique<Stage3_BackGround>();
	objectManager.AddObject(EObjectTag::Default, ERenderLayer::Background, move(stage3));

	vector<ColliderDesc> outColliders;
	vector<MapObjectInfo> outObjects;
	Vector2f outMapSize;

	resourceManager.LoadMap("resource/map_info/stage2_info.json", outColliders, outObjects, outMapSize);

	for (auto& col : outColliders)
	{
		if (col.type == EColliderType::Block)
		{
			auto block = make_unique<Block>(col);
			col.objectName == "obj_door" ? block->SetIsDoor(true) : block->SetIsDoor(false);
			if (col.objectName == "obj_door")
			{
				auto door = make_unique<Door>();
				door->GetDesc().vSpawnPoint = col.bounds.position;
				col.bounds.size.x >= 0 ? door->GetDesc().bFace = true : door->GetDesc().bFace = false;
				objectManager.AddObject(EObjectTag::Door, ERenderLayer::Background, move(door));
			}
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
			grunt->SetIsGrunt(true);
			objectManager.AddObject(EObjectTag::Enemy, ERenderLayer::Actor, move(grunt));
		}
		else if (obj.objectName == "obj_enemy_gangster")
		{
			auto gangster = make_unique<Gangster>();
			gangster->GetDesc().vSpawnPoint = obj.position;
			obj.scale.x >= 0 ? gangster->GetDesc().bFace = true : gangster->GetDesc().bFace = false;
			objectManager.AddObject(EObjectTag::Enemy, ERenderLayer::Actor, move(gangster));
		}
		else if (obj.objectName == "obj_enemy_pomp")
		{
			auto grunt = make_unique<Grunt>();
			grunt->GetDesc().vSpawnPoint = obj.position;
			obj.scale.x >= 0 ? grunt->GetDesc().bFace = true : grunt->GetDesc().bFace = false;
			grunt->SetIsGrunt(false);
			objectManager.AddObject(EObjectTag::Enemy, ERenderLayer::Actor, move(grunt));
		}
	}

#pragma endregion
}

void SceneStage3::Update(float deltaTime)
{
	GameInstance::GetInstance().GetObjectManager().Update(deltaTime);
}

void SceneStage3::LateUpdate(float deltaTime)
{
	GameInstance::GetInstance().GetObjectManager().LateUpdate(deltaTime);
}

void SceneStage3::Render()
{
	GameInstance::GetInstance().GetObjectManager().Render();
}

void SceneStage3::RenderUI()
{
	GameInstance::GetInstance().GetObjectManager().RenderUI();
}

void SceneStage3::Release()
{
	GameInstance::GetInstance().GetObjectManager().ReleaseScene();
}

void SceneStage3::RestartScene()
{
	GameInstance::GetInstance().GetObjectManager().RestartObject();
}
