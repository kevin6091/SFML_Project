#include "SceneStage1.h"
#include "GameInstance.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"

#include "Camera.h"
#include "Player.h"
#include "Block.h"
#include "Stage1_BackGround.h"
#include "Grippable.h"


void SceneStage1::Initialize()
{
	auto& gameInstance = GameInstance::GetInstance();
	auto& resourceManager = gameInstance.GetResourceManager();
	auto& objectManager = gameInstance.GetObjectManager();

	//gameInstance.GetCamera().Initialize(Vector2f((float)WIDTH * 5.5f, (float)HEIGHT * 5.5f));
	//gameInstance.GetCamera().Initialize(Vector2f((float)WIDTH / 1.5f, (float)HEIGHT / 1.5f));
	gameInstance.GetCamera().Initialize(Vector2f((float)WIDTH, (float)HEIGHT));

	gameInstance.GetCamera().SetOffSet(Vector2f(0.f, -60.f)); 

	//************** Texture, Anim **************//
#pragma region Texture. Anim

	// idle, moving
	resourceManager.LoadTextureSequence(PLAYER_IDLE, "resource/textures/player/idle", 11);
	resourceManager.LoadTextureSequence(PLAYER_RUN, "resource/textures/player/run", 10);
	resourceManager.LoadTextureSequence(PLAYER_RUN_TO_IDLE, "resource/textures/player/run_to_idle", 5);
	resourceManager.LoadTextureSequence(PLAYER_IDLE_TO_RUN, "resource/textures/player/idle_to_run", 4);

	resourceManager.LoadTextureSequence(PLAYER_CROUCH, "resource/textures/player/crouch", 2);
	resourceManager.LoadTextureSequence(PLAYER_CROUCH_TO_IDLE, "resource/textures/player/crouch", 2);

	resourceManager.LoadTextureSequence(PLAYER_ROLL, "resource/textures/player/roll", 7);
	resourceManager.LoadTextureSequence(PLAYER_JUMP, "resource/textures/player/jump", 4);
	resourceManager.LoadTextureSequence(PLAYER_FALL, "resource/textures/player/fall", 4);
	resourceManager.LoadTextureSequence(PLAYER_WALLSLIDE, "resource/textures/player/wallslide", 1);
	resourceManager.LoadTextureSequence(PLAYER_FLIP, "resource/textures/player/flip", 11);

	// attack
	resourceManager.LoadTextureSequence(PLAYER_ATTACK, "resource/textures/player/attack", 7);
	resourceManager.LoadTextureSequence(SLASH_MAIN, "resource/textures/player/slash/slash_main", 7);
	resourceManager.LoadTextureSequence(SLASH_SUB, "resource/textures/player/slash/slash_sub", 5);

	// BackGround
	resourceManager.LoadTexture(MAP_STAGE1, "resource/textures/map/stage1");

#pragma endregion



	//************** Object **************//
#pragma region MapInfo, Object

	auto stage1 = make_unique<Stage1_BackGround>();
	objectManager.AddObject(EObjectTag::Default, ERenderLayer::Background, move(stage1));

	vector<ColliderDesc> outColliders;
	vector<MapObjectInfo> outObjects;
	Vector2f outMapSize;
	
	resourceManager.LoadMap("resource/map_info/stage1_info.json", outColliders, outObjects, outMapSize);

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
	}

	for (auto& obj : outObjects) 
	{
		if (obj.layerName == "player")
		{
			auto player = make_unique<Player>();
			gameInstance.GetCamera().SetTarget(player.get());
			player->GetDesc().vSpawnPoint = obj.position;
			objectManager.AddObject(EObjectTag::Player, ERenderLayer::Actor, move(player));
		}
	}

#pragma endregion
}

void SceneStage1::Update(float deltaTime)
{
	GameInstance::GetInstance().GetObjectManager().Update(deltaTime);
}

void SceneStage1::LateUpdate(float deltaTime)
{
	GameInstance::GetInstance().GetObjectManager().LateUpdate(deltaTime);
}

void SceneStage1::Render()
{
	GameInstance::GetInstance().GetObjectManager().Render();
}

void SceneStage1::RenderUI()
{
	GameInstance::GetInstance().GetObjectManager().RenderUI();
}

void SceneStage1::Release()
{
	GameInstance::GetInstance().GetObjectManager().ReleaseScene();
}

void SceneStage1::RestartScene()
{
	GameInstance::GetInstance().GetObjectManager().RestartObject();
}
