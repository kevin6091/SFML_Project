#include "Bullet.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "SingleEffect.h"
#include "Camera.h"
#include "SoundManager.h"

Bullet::Bullet()
{
	eObjectTag = EObjectTag::EnemyAttack;
	eRenderLayer = ERenderLayer::Effect;
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
#pragma region Resource

	auto& resource = GameInstance::GetInstance().GetResourceManager();

	sprite.emplace(*resource.GetTexture(BULLET));
	auto size = sprite->getTexture().getSize();
	sprite->setOrigin({ (float)size.x * 0.5f, (float)size.y * 0.5f });
	sprite->setScale({ 0.3f,1.0f });

#pragma endregion

	uCollider = make_unique<Collider>(this);
	FloatRect rect = CreateRectFromCenter({ 5.f,1.f }, { 10.f, 2.f });
	ColliderDesc desc = { "", EColliderType::RectAttack, rect };
	uCollider->Initialize(desc);
	SetPosition(descStatus.vSpawnPoint);
}

void Bullet::Update(float deltaTime)
{
	Vector2f pos = GetPosition();
	
	if (GameInstance::GetInstance().GetIsSlow())
	{
		pos += velocity * deltaTime * 50.f;
		sprite->setScale({ (accTime += deltaTime) , 1.0f });
	}
	else
	{
		pos += velocity * deltaTime * 150.f;
	}
	SetPosition(pos);
}

void Bullet::LateUpdate(float deltaTime)
{
}

void Bullet::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*this, BlendAlpha);
}

void Bullet::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);

	RectangleShape debugBox(uCollider->GetBounds().size);
	debugBox.setPosition(uCollider->GetBounds().position);
	debugBox.setFillColor(Color::Transparent);
	debugBox.setOutlineThickness(1.0f);

	if (GameInstance::GetInstance().GetIsRenderDebug())
		target.draw(debugBox);
}

void Bullet::CollisionEvent(GameObject& other)
{
	if (other.GetTag() == EObjectTag::PlayerAttack && eObjectTag == EObjectTag::EnemyAttack)
	{
		// Parry
		velocity.x *= -1.f;
		sprite->setScale({ -1.f,1.f });
		eObjectTag = EObjectTag::PlayerAttack;

		GameInstance::GetInstance().GetCamera().Shake({ velocity.x, 0.5f }, 0.1f, 2.f);

		// Spark
		auto effect = make_unique<SingleEffect>();
		velocity.x >= 0.f ? effect->GetDesc().bFace = true : effect->GetDesc().bFace = false;
		effect->GetDesc().vSpawnPoint = GetPosition();
		effect->SetEffectType(ESingleEffectType::Bullet_Parry);
		GameInstance::GetInstance().GetObjectManager().AddObject(EObjectTag::Default, ERenderLayer::Effect, move(effect));

		SoundManager::GetInstance().PlaySFXWithReverb("parry", 50.f, 1.f, 3, 0.08f, true);
	}

	if (other.GetTag() == EObjectTag::Enemy && eObjectTag == EObjectTag::PlayerAttack || 
		other.GetTag() == EObjectTag::Player && eObjectTag == EObjectTag::EnemyAttack)
	{
		if(other.IsActive())
			Destroy();
	}
}

void Bullet::Release()
{
}