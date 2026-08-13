#include "Smoke_Throw.h"
#include "GameInstance.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "ObjectManager.h"
#include "RewindManager.h"
#include "RewindTracker.h"
#include "Camera.h"
#include "InputManager.h"
#include "Smoke.h"
#include "Player.h"

Smoke_Throw::Smoke_Throw()
{
	eObjectTag = EObjectTag::Smoke;
	eRenderLayer = ERenderLayer::Effect;
}

Smoke_Throw::~Smoke_Throw()
{
}

void Smoke_Throw::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();
	sprite.emplace(*resource.GetTexture(BULLET));

	sprite->setScale({ 0.2f, 1.f });

	uCollider = make_unique<Collider>(this);
	FloatRect rect = CreateRectFromCenter({ 2.f,2.f }, {4.f, 4.f });
	ColliderDesc desc = { "", EColliderType::RectAttack, rect };
	uCollider->Initialize(desc);
	SetPosition(descStatus.vSpawnPoint);
}

void Smoke_Throw::Update(float deltaTime)
{
	Vector2f pos = GetPosition();
	pos += velocity * deltaTime;
	SetPosition(pos);
}

void Smoke_Throw::LateUpdate(float deltaTime)
{
}

void Smoke_Throw::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect().draw(*sprite, BlendAlpha);
}

void Smoke_Throw::Release()
{
}

void Smoke_Throw::draw(RenderTarget& target, RenderStates states) const
{
}

void Smoke_Throw::CollisionEvent(GameObject& other)
{
	// Smoke터짐
   /* {
        auto& objManager = GameInstance::GetInstance().GetObjectManager();
        Vector2f hitPos = GetPosition();

        int smokeCount = 15;
        for (int i = 0; i < smokeCount; ++i)
        {
            auto smoke = make_unique<Smoke>();

			Vector2f offset = velocity.normalized();

			smoke->GetDesc().vSpawnPoint = hitPos;

            objManager.AddObject(EObjectTag::Smoke, ERenderLayer::UI, move(smoke));
        }

        Destroy();
    }*/

    auto& objManager = GameInstance::GetInstance().GetObjectManager();
    Vector2f hitPos = GetPosition();
    Vector2f throwVel = GetVelocity();

    // ★ 핵심 3: 스폰 위치 보정 (벽 안으로 파고들지 않게, 날아온 반대 방향으로 10픽셀 당겨줌)
    float length = sqrt(throwVel.x * throwVel.x + throwVel.y * throwVel.y);
    if (length > 0.f)
    {
        hitPos.x -= (throwVel.x / length) * 10.f;
        hitPos.y -= (throwVel.y / length) * 10.f;
    }

    int smokeCount = 10;
    float angleStep = 360.0f / smokeCount; // 15조각으로 균등 분할

    for (int i = 0; i < smokeCount; ++i)
    {
        auto smoke = make_shared<Smoke>();

        // 360도를 고르게 돌면서 약간의 흩어짐(랜덤 오차) 추가
        float angle = (angleStep * i + (rand() % 20 - 10)) * 3.141592f / 180.f;
        float speed = 250.f + (rand() % 150); // 폭발 속도 증가

        // 모든 연기가 완벽하게 똑같은 픽셀에서 시작하지 않도록 미세한 오프셋 부여
        Vector2f offset;
        offset.x = cos(angle) * (rand() % 8);
        offset.y = sin(angle) * (rand() % 8);

        smoke->GetDesc().vSpawnPoint = hitPos;

        // ★ Initialize가 끝난 직후에 계산된 방사형 속도를 주입!
        smoke->SetVelocity(Vector2f(cos(angle) * speed, sin(angle) * speed));

        objManager.AddObject(EObjectTag::Smoke, ERenderLayer::UI, move(smoke));
    }

    Destroy();
}
