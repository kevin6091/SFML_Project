#include "GruntSlash.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "Collider.h"

GruntSlash::GruntSlash()
{
	eObjectTag = EObjectTag::EnemyAttack;
	eRenderLayer = ERenderLayer::Effect;
}

GruntSlash::~GruntSlash()
{
}

void GruntSlash::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();

	animator.AddClip(GRUNT_SLASH, 0.05f, false);
	sprite.emplace(*resource.GetTexture("default"));

	uCollider = make_unique<Collider>(this);
	FloatRect rect;
	if (descStatus.bFace)
		rect = CreateRectFromCenter({ 50.f, -35.f }, { 88.f, 50.f });
	else
		rect = CreateRectFromCenter({ -50.f, -35.f }, { 88.f, 50.f });

	ColliderDesc desc = { "", EColliderType::RectAttack, rect };
	uCollider->Initialize(desc);
	animator.UpdateSpriteTexture(*sprite, false);
	SetPosition(descStatus.vSpawnPoint + Vector2f(10.f,0.f));
	
	if (descStatus.bFace)
		sprite->setScale({ 1.5f,1.5f });
	else
		sprite->setScale({ -1.5f,1.5f });

	animator.Play(GRUNT_SLASH, true);
}

void GruntSlash::Update(float deltaTime)
{
	animator.Update(deltaTime, *sprite, true);
}

void GruntSlash::LateUpdate(float deltaTime)
{
	if (animator.IsFinished())
		Destroy();
}

void GruntSlash::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*this, BlendAlpha);
}

void GruntSlash::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);

	RectangleShape debugBox(uCollider->GetBounds().size);
	debugBox.setPosition(uCollider->GetBounds().position);
	debugBox.setFillColor(Color::Transparent);

	debugBox.setOutlineThickness(1.0f);

	if (GameInstance::GetInstance().GetIsRenderDebug())
		target.draw(debugBox);
}

void GruntSlash::RestartObject()
{
}

void GruntSlash::CollisionEvent(GameObject& other)
{
}

void GruntSlash::Release()
{
	animator.Release();
}
