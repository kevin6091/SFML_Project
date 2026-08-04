#include "SceneTitle.h"
#include "GameInstance.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Title.h"

void SceneTitle::Initialize()
{
	// 텍스쳐, 오브젝트 로딩
	// 배치
	// 플레이어 생성

	auto& gameInstance = GameInstance::GetInstance();
	auto& resourceManager = gameInstance.GetResourceManager();
	auto& objectManager = gameInstance.GetObjectManager();

#pragma region Zero

	resourceManager.LoadTexture("title_zero", "resource/textures/title/zero/zero");
	resourceManager.LoadTexture("title_zer", "resource/textures/title/zero/zer");
	resourceManager.LoadTexture("title_o", "resource/textures/title/zero/o");

#pragma endregion

#pragma region Katana

	resourceManager.LoadTexture("title_katana", "resource/textures/title/katana/katana");
	resourceManager.LoadTexture("title_katana2", "resource/textures/title/katana/katana2");

#pragma endregion

#pragma region BackGround

	resourceManager.LoadTexture("title_bg", "resource/textures/title/background/background");
	resourceManager.LoadTexture("title_fence", "resource/textures/title/background/fence");

#pragma endregion

#pragma region Plant

	resourceManager.LoadTexture("title_grass", "resource/textures/title/plant/grass");
	resourceManager.LoadTextureSequence("anim_title_plant", "resource/textures/title/plant", 10, "frame_");

#pragma endregion

	resourceManager.LoadTextureSequence("anim_zero_run", "resource/textures/zero/run", 10);

	auto title = make_unique<Title>();
	title->Initialize();
	gameInstance.GetCamera().SetTarget(title.get());
	objectManager.AddObject(EObjectTag::Default, ERenderLayer::Background, move(title));
}

void SceneTitle::Update(float deltaTime)
{
	if (accTime_Wait <= 2.0f)
		accTime_Wait += deltaTime;

	if (accTime_Wait > 2.0f)
		GameInstance::GetInstance().GetObjectManager().Update(deltaTime);
}

void SceneTitle::LateUpdate(float deltaTime)
{
	if (accTime_Wait > 2.0f)
		GameInstance::GetInstance().GetObjectManager().LateUpdate(deltaTime);
}

void SceneTitle::Render()
{
	if(accTime_Wait > 2.0f)
		GameInstance::GetInstance().GetObjectManager().Render();
}

void SceneTitle::RenderUI()
{
	if (accTime_Wait > 2.0f)
		GameInstance::GetInstance().GetObjectManager().RenderUI();
}

void SceneTitle::Release()
{
	// 텍스쳐, 오브젝트 삭제
	// 플레이어 제외
	// 오브젝트 매니저에 등록된 것들 삭제
	// FadeIn
}
