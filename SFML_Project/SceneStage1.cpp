#include "SceneStage1.h"
#include "GameInstance.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"

#include "Camera.h"
#include "Player.h"
#include "Grunt.h"
#include "Block.h"
#include "Stage1_BackGround.h"
#include "Grippable.h"
#include "Fan.h"
#include "EndBlock.h"
#include "MouseCursor.h"
#include "SoundManager.h"

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
	resourceManager.LoadTexture("default", "resource/textures/default/default");
	resourceManager.LoadTexture("cursor", "resource/textures/cursor/cursor");

#pragma region Player

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

	// hit
	resourceManager.LoadTextureSequence(PLAYER_HIT_BEGIN, "resource/textures/player/hit_begin", 2);
	resourceManager.LoadTextureSequence(PLAYER_HIT_LOOP, "resource/textures/player/hit_loop", 4);
	resourceManager.LoadTextureSequence(PLAYER_HIT_GROUND, "resource/textures/player/hit_ground", 6);
	resourceManager.LoadTextureSequence(PLAYER_HIT_RECOVER, "resource/textures/player/hit_recover", 9);

	resourceManager.LoadTexture(HIT_LINE, "resource/textures/hitline/line");

	// blood
	resourceManager.LoadTextureSequence(BLOOD_0, "resource/textures/blood/blood_0", 6);
	resourceManager.LoadTextureSequence(BLOOD_1, "resource/textures/blood/blood_1", 6);
	resourceManager.LoadTextureSequence(BLOOD_2, "resource/textures/blood/blood_2", 6);

	resourceManager.LoadTexture(BLOOD_DECAL_0, "resource/textures/blood/blood_decal/blood_0");
	resourceManager.LoadTexture(BLOOD_DECAL_1, "resource/textures/blood/blood_decal/blood_1");
	resourceManager.LoadTexture(BLOOD_DECAL_2, "resource/textures/blood/blood_decal/blood_2");
	resourceManager.LoadTexture(BLOOD_DECAL_3, "resource/textures/blood/blood_decal/blood_3");
	resourceManager.LoadTexture(BLOOD_DECAL_4, "resource/textures/blood/blood_decal/blood_4");

	resourceManager.LoadTexture(BLOOD_DECAL_SMALL_0, "resource/textures/blood/blood_decal/small_blood_0");
	resourceManager.LoadTexture(BLOOD_DECAL_SMALL_1, "resource/textures/blood/blood_decal/small_blood_1");

	resourceManager.LoadTexture(BLOOD_DECAL_4, "resource/textures/blood/blood_decal/blood_4");
	resourceManager.LoadTexture(BLOOD_DECAL_4, "resource/textures/blood/blood_decal/blood_4");

	// dust
	resourceManager.LoadTextureSequence(SPRINT_DUST, "resource/textures/dust/sprint_dust", 10);
	resourceManager.LoadTextureSequence(JUMP_DUST, "resource/textures/dust/jump_dust", 4);
	resourceManager.LoadTextureSequence(LAND_DUST, "resource/textures/dust/land_dust", 7);
	resourceManager.LoadTextureSequence(WALLSLIDE_DUST, "resource/textures/dust/wallslide_dust", 7);

#pragma endregion

#pragma region Grunt

	resourceManager.LoadTextureSequence(GRUNT_IDLE, "resource/textures/grunt/Idle", 8);
	resourceManager.LoadTextureSequence(GRUNT_RUN, "resource/textures/grunt/run", 10);
	resourceManager.LoadTextureSequence(GRUNT_ATTACK, "resource/textures/grunt/attack", 8);
	resourceManager.LoadTextureSequence(GRUNT_HIT, "resource/textures/grunt/hit", 2);
	resourceManager.LoadTextureSequence(GRUNT_HIT_ROLL, "resource/textures/grunt/hit_roll", 13);
	resourceManager.LoadTextureSequence(GRUNT_HIT_GROUND, "resource/textures/grunt/hit_ground", 16);
	resourceManager.LoadTextureSequence(GRUNT_SLASH, "resource/textures/grunt/slash", 5);

#pragma endregion

#pragma region Fan

	resourceManager.LoadTexture("fan_fg", "resource/textures/fan/fan_fg");
	resourceManager.LoadTexture("fan_bg", "resource/textures/fan/fan_bg");
	resourceManager.LoadTextureSequence(FANBLADE, "resource/textures/fan/fanblade", 32);

#pragma endregion

#pragma region Smoke

	resourceManager.LoadTextureSequence(SMOKE_START, "resource/textures/smoke/start", 3);
	resourceManager.LoadTextureSequence(SMOKE_LOOP, "resource/textures/smoke/loop", 6);
	resourceManager.LoadTextureSequence(SMOKE_END, "resource/textures/smoke/end", 7);

#pragma endregion
	resourceManager.LoadTexture(BULLET, "resource/textures/bullet/bullet");


	// BackGround
	resourceManager.LoadTexture(MAP_STAGE1, "resource/textures/map/stage1");
	auto size = resourceManager.GetTexture(MAP_STAGE1)->getSize();
	gameInstance.GetCamera().SetBounds(FloatRect({ 0,0 }, { (float)size .x, (float)size.y }));

#pragma endregion

	//************** Object **************//
#pragma region MapInfo, Object

	auto cursor = make_unique<MouseCursor>();
	objectManager.AddObject(EObjectTag::NonDestroy, ERenderLayer::UI, move(cursor));

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
		else if (col.type == EColliderType::EndBlock)
		{
			auto endBlock = make_unique<EndBlock>(col);
			objectManager.AddObject(EObjectTag::Wall, ERenderLayer::Background, move(endBlock));
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
		else if (obj.objectName == "obj_enemy_grunt")
		{
			auto grunt = make_unique<Grunt>();
			grunt->GetDesc().vSpawnPoint = obj.position;
			obj.scale.x >= 0 ? grunt->GetDesc().bFace = true : grunt->GetDesc().bFace = false;
			grunt->SetGruntType(EGruntType::Grunt1);
			objectManager.AddObject(EObjectTag::Enemy, ERenderLayer::Actor, move(grunt));
		}
		else if (obj.objectName == "obj_fanblade")
		{
			auto fan = make_unique<Fan>();
			fan->GetDesc().vSpawnPoint = obj.position;
			objectManager.AddObject(EObjectTag::Default, ERenderLayer::Background, move(fan));
		}
	}

#pragma endregion

	//************** Sound **************//
	SoundManager::GetInstance().LoadBGM("Stage1", "resource/sounds/BGM/stage1.ogg");
	SoundManager::GetInstance().LoadBGM("Stage3", "resource/sounds/BGM/stage3.ogg");
	SoundManager::GetInstance().LoadBGM("Stage3_3", "resource/sounds/BGM/stage3_3.ogg");
	SoundManager::GetInstance().LoadSFX(S_PLAYER_SLASH1, "resource/sounds/player/slash1.wav");
	SoundManager::GetInstance().LoadSFX(S_PLAYER_SLASH2, "resource/sounds/player/slash2.wav");
	SoundManager::GetInstance().LoadSFX(S_PLAYER_SLASH3, "resource/sounds/player/slash3.wav");
	
	SoundManager::GetInstance().LoadSFX(S_PLAYER_JUMP, "resource/sounds/player/jump.wav");
	SoundManager::GetInstance().LoadSFX(S_PLAYER_LAND, "resource/sounds/player/land.wav");
	SoundManager::GetInstance().LoadSFX(S_PLAYER_ROLL, "resource/sounds/player/roll.wav");
	SoundManager::GetInstance().LoadSFX(S_PLAYER_DIE, "resource/sounds/player/die.wav");
	SoundManager::GetInstance().LoadSFX(S_PLAYER_RUN_START, "resource/sounds/player/run_start.wav");
	SoundManager::GetInstance().LoadSFX(S_PLAYER_WALLSLIDE, "resource/sounds/player/wallslide.wav");
	
	SoundManager::GetInstance().LoadSFX(S_BLOOD1, "resource/sounds/blood/blood1.wav");
	SoundManager::GetInstance().LoadSFX(S_BLOOD2, "resource/sounds/blood/blood2.wav");
	SoundManager::GetInstance().LoadSFX(S_BLOOD3, "resource/sounds/blood/blood3.wav");
	SoundManager::GetInstance().LoadSFX(S_BLOOD4, "resource/sounds/blood/blood4.wav");

	SoundManager::GetInstance().LoadSFX(S_HIT, "resource/sounds/hit.wav");
	SoundManager::GetInstance().LoadSFX(S_DOOR_OPEN, "resource/sounds/hit.wav");

	SoundManager::GetInstance().LoadSFX("punch", "resource/sounds/punch.wav");
	SoundManager::GetInstance().LoadSFX("gun", "resource/sounds/gun.wav");
	SoundManager::GetInstance().LoadSFX("parry", "resource/sounds/parry.wav");
	SoundManager::GetInstance().LoadSFX("rewind", "resource/sounds/rewind.wav");

	SoundManager::GetInstance().PlayBGM("Stage1", 50.f);
}

void SceneStage1::Update(float deltaTime)
{
	if (GameInstance::GetInstance().GetInputManager().GetKeyDown(Keyboard::Key::Enter))
		GameInstance::GetInstance().GetSceneManager().ChangeScene(ESceneType::Stage2);
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
