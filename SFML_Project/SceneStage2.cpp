#include "SceneStage2.h"
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
#include "Stage2_BackGround.h"
#include "EndBlock.h"
#include "Door.h"

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

	// Resource
#pragma region Bullet

	resourceManager.LoadTexture(BULLET, "resource/textures/bullet/bullet");
	resourceManager.LoadTextureSequence(BULLET_PARRY, "resource/textures/bullet/bullet_parry", 5);
	resourceManager.LoadTextureSequence(BULLET_SPARK, "resource/textures/bullet/bullet_spark", 8);
	resourceManager.LoadTextureSequence(BULLET_SMOKE, "resource/textures/bullet/bullet_smoke", 10);
	
#pragma endregion

#pragma region Gangster

	resourceManager.LoadTextureSequence(GANGSTER_IDLE, "resource/textures/gangster/idle", 8);
	resourceManager.LoadTextureSequence(GANGSTER_RUN, "resource/textures/gangster/run", 10);
	resourceManager.LoadTextureSequence(GANGSTER_HIT, "resource/textures/gangster/hit", 2);
	resourceManager.LoadTextureSequence(GANGSTER_HIT_ROLL, "resource/textures/gangster/hit_roll", 12);
	resourceManager.LoadTextureSequence(GANGSTER_HIT_GROUND, "resource/textures/gangster/hit_ground", 14);
	resourceManager.LoadTextureSequence(GANGSTER_ATTACK, "resource/textures/gangster/attack", 8);

#pragma endregion

#pragma region Pomp

	resourceManager.LoadTextureSequence(POMP_IDLE, "resource/textures/pomp/idle", 8);
	resourceManager.LoadTextureSequence(POMP_RUN, "resource/textures/pomp/run", 10);
	resourceManager.LoadTextureSequence(POMP_ATTACK, "resource/textures/pomp/attack", 6);
	resourceManager.LoadTextureSequence(POMP_HIT, "resource/textures/pomp/hit", 2);
	resourceManager.LoadTextureSequence(POMP_HIT_ROLL, "resource/textures/pomp/hit_roll", 12);
	resourceManager.LoadTextureSequence(POMP_HIT_GROUND, "resource/textures/pomp/hit_ground", 15);

#pragma endregion

#pragma region Door

	resourceManager.LoadTextureSequence(DOOR_OPEN, "resource/textures/door", 20);

#pragma endregion


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
			grunt->SetGruntType(EGruntType::Grunt1);
			objectManager.AddObject(EObjectTag::Enemy, ERenderLayer::Actor, move(grunt));
		}
		else if (obj.objectName == "obj_enemy_gangster")
		{
			auto gangster = make_unique<Gangster>();
			gangster->GetDesc().vSpawnPoint = obj.position;
			gangster->SetIsGangster(true);
			obj.scale.x >= 0 ? gangster->GetDesc().bFace = true : gangster->GetDesc().bFace = false;
			objectManager.AddObject(EObjectTag::Enemy, ERenderLayer::Actor, move(gangster));
		}
		else if (obj.objectName == "obj_enemy_pomp")
		{
			auto grunt = make_unique<Grunt>();
			grunt->GetDesc().vSpawnPoint = obj.position;
			obj.scale.x >= 0 ? grunt->GetDesc().bFace = true : grunt->GetDesc().bFace = false;
			grunt->SetGruntType(EGruntType::Pomp);
			objectManager.AddObject(EObjectTag::Enemy, ERenderLayer::Actor, move(grunt));
		}
	}

#pragma endregion
}

void SceneStage2::Update(float deltaTime)
{
	if (GameInstance::GetInstance().GetInputManager().GetKeyDown(Keyboard::Key::Enter))
		GameInstance::GetInstance().GetSceneManager().ChangeScene(ESceneType::Stage3);
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
