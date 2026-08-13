#include "Smoke.h"
#include "GameInstance.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "ObjectManager.h"
#include "RewindManager.h"
#include "RewindTracker.h"
#include "Camera.h"
#include "InputManager.h"

Smoke::Smoke()
{
	eObjectTag = EObjectTag::Smoke;
	eRenderLayer = ERenderLayer::UI;
}

Smoke::~Smoke()
{
}

void Smoke::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();
	animator.AddClip(SMOKE_START, 0.05f, false);
	animator.AddClip(SMOKE_LOOP, 0.1f, true);
	animator.AddClip(SMOKE_END, 0.1f, false);

	sprite.emplace(*resource.GetTexture("default"));

	animator.Play(SMOKE_START, true);
	auto size = sprite->getTexture().getSize();
	sprite->setOrigin({ size.x * 0.5f, size.y * 0.5f });
	sprite->setScale({ 1.5f,1.5f });
	SetPosition(descStatus.vSpawnPoint);

	m_driftTime = (rand() % 100) / 100.f * 3.14f;

	uCollider = make_unique<Collider>(this);
	FloatRect rect = CreateRectFromCenter({ size.x * 0.5f, size.y * 0.5f }, { 5.f, 5.f });
	ColliderDesc desc = { "", EColliderType::End, rect };
	uCollider->Initialize(desc);
}

void Smoke::Update(float deltaTime)
{
	animator.Update(deltaTime, *sprite, true);
	auto size = sprite->getTexture().getSize();
	sprite->setOrigin({ size.x * 0.5f, size.y * 0.5f });

	if (animator.GetCurrentClipName() == SMOKE_START && animator.IsFinished())
		animator.Play(SMOKE_LOOP, true);
	if (animator.GetCurrentClipName() == SMOKE_END && animator.IsFinished())
		Destroy();
	if (animator.GetCurrentClipName() == SMOKE_LOOP && (accTime += deltaTime) >= 15.f)
		animator.Play(SMOKE_END, true);

	Vector2f currentVel = GetVelocity();

	// ★ 핵심 2: 마찰력을 강하게 줘서 초반에 '촥!' 뻗어나간 뒤 바로 멈추게 만듭니다.
	// m_friction 변수가 따로 없다면 아래처럼 직접 0.85f를 곱해주셔도 됩니다.
	currentVel.x *= pow(0.85f, deltaTime * 60.f);
	currentVel.y *= pow(0.85f, deltaTime * 60.f);

	m_driftTime += deltaTime * 5.f;

	// 공중에 둥둥 떠다니는 기류 느낌
	currentVel.x += sin(m_driftTime) * 15.f * deltaTime;
	currentVel.y -= 20.f * deltaTime;

	SetVelocity(currentVel);
}

void Smoke::LateUpdate(float deltaTime)
{
}

void Smoke::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*this, BlendAlpha);
}

void Smoke::Release()
{
	animator.Release();
}

void Smoke::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);

	RectangleShape debugBox(uCollider->GetBounds().size);
	debugBox.setPosition(uCollider->GetBounds().position);
	debugBox.setFillColor(Color(0, 0, 255, 10));

	debugBox.setOutlineThickness(1.0f);

	if (GameInstance::GetInstance().GetIsRenderDebug())
		target.draw(debugBox);
}

void Smoke::CollisionEvent(GameObject& other)
{
}
