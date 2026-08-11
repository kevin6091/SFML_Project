#include "Fan.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "Player.h"
#include "PlayerFSM.h"
#include "ObjectManager.h"

Fan::Fan()
{
	eObjectTag = EObjectTag::Default;
	eRenderLayer = ERenderLayer::Background;
}

Fan::~Fan()
{
}

void Fan::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();

	animator.AddClip(FANBLADE, 0.01f, true);
	sprite.emplace(*resource.GetTexture("default"));
	spriteBack.emplace(*resource.GetTexture("fan_bg"));
	spriteFront.emplace(*resource.GetTexture("fan_fg"));

	sprite->setPosition(descStatus.vSpawnPoint);
	spriteBack->setPosition(descStatus.vSpawnPoint);
	spriteFront->setPosition(descStatus.vSpawnPoint);

	Vector2u size = spriteBack->getTexture().getSize();
	spriteBack->setOrigin({ 0.f,0.f });
	size = spriteFront->getTexture().getSize();
	spriteFront->setOrigin({ 0.f,0.f });
	size = sprite->getTexture().getSize();
	sprite->setOrigin({ -18.f, 0.f });

	sprite->setScale({ 2.f,2.f });
	spriteFront->setScale({ 2.f ,2.f });
	spriteBack->setScale({ 2.f ,2.f });

	animator.Play(FANBLADE, true);
}

void Fan::Update(float deltaTime)
{
	animator.Update(deltaTime, *sprite, true);

	Vector2u size = sprite->getTexture().getSize();
	sprite->setOrigin({ -18.f, -16.f });

	auto& game = GameInstance::GetInstance();
	
	const auto& objList = game.GetObjectManager().GetObjects(EObjectTag::Enemy);
	for (auto& iter : objList)
	{
		float dis = abs(iter.get()->GetPosition().x - (descStatus.vSpawnPoint.x + 38.f));
		if (dis <= 30.f)
		{
			iter.get()->SetVelocity(Vector2f(-1.f, -1.f).normalized() * 600.f);
			iter.get()->FanHit();
		}
	}

	Vector2f playerPos = game.GetPlayer()->GetPosition();
	float dis = abs(playerPos.x - (descStatus.vSpawnPoint.x + 38.f));

	int frame = (int)animator.GetCurrentFrameIndex();
	bool isHitFrame = (frame <= 6 || (frame <= 31 && frame >= 27));

	if (isHitFrame && game.GetIsSlow())
		isRed = true;
	else
		isRed = false;	

	if (dis <= 20.f)
	{
		if (game.GetIsSlow() && !isHitFrame && game.GetPlayer()->GetState() == EPlayerState::Roll)
		{
			// Åë°ú
		}
		else
		{
			if (game.GetPlayer()->GetState() != EPlayerState::Hit_Begin &&
				game.GetPlayer()->GetState() != EPlayerState::Hit_Loop &&
				game.GetPlayer()->GetState() != EPlayerState::Hit_Ground &&
				game.GetPlayer()->GetState() != EPlayerState::Hit_Recover)
			{
				game.GetPlayer()->SetHitDir(Vector2f(-1.f, -1.f).normalized());
				game.GetPlayer()->SetIsDead(false);
				game.GetPlayer()->ForceChangeFSM(make_unique<Player_Hit_Begin>(*game.GetPlayer()));
			}
		}
	}
}

void Fan::LateUpdate(float deltaTime)
{
}

void Fan::Render()
{

	RenderStates states(&GameInstance::GetInstance().GetFanShader());
	GameInstance::GetInstance().GetFanShader().setUniform("isRed", isRed);
	GameInstance::GetInstance().GetBloodShader().setUniform("currentTexture", Shader::CurrentTexture);

	GameInstance::GetInstance().GetRenderTarget_BG().draw(*spriteBack, BlendAlpha);
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*sprite, states);
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*spriteFront, BlendAlpha);
}

void Fan::Release()
{
	animator.Release();
}

void Fan::draw(RenderTarget& target, RenderStates states) const
{
}
