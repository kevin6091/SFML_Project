#include "SceneStage1.h"
#include "GameInstance.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"

#include "Camera.h"
#include "Player.h"


void SceneStage1::Initialize()
{
	auto& gameInstance = GameInstance::GetInstance();
	auto& resourceManager = gameInstance.GetResourceManager();
	auto& objectManager = gameInstance.GetObjectManager();

	//************** Texture, Anim **************//
#pragma region Zero

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

	// attack
	resourceManager.LoadTextureSequence(PLAYER_ATTACK, "resource/textures/player/attack", 7);

#pragma endregion

	//************** Object **************//
#pragma region Player

	auto player = make_unique<Player>();
	player->Initialize();
	gameInstance.GetCamera().SetTarget(player.get());
	objectManager.AddObject(EObjectTag::Player, ERenderLayer::Player, move(player));

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
