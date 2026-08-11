#include "BloodDecal.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "RewindManager.h"

BloodDecal::BloodDecal()
{
	eObjectTag = EObjectTag::Default;
	eRenderLayer = ERenderLayer::Decal;
}

BloodDecal::~BloodDecal()
{
}

void BloodDecal::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();
	
    int randomInt1 = 0;
    int randomInt2 = 0;

    while (randomInt1 == randomInt2)
    {
        randomInt1 = RandomInt(0, 4);
        randomInt2 = RandomInt(0, 4);
    }

	switch (randomInt1)
	{
	case 0:
		sprite.emplace(*resource.GetTexture(BLOOD_DECAL_0));
		break;
	case 1:
		sprite.emplace(*resource.GetTexture(BLOOD_DECAL_1));
		break;
	case 2:
		sprite.emplace(*resource.GetTexture(BLOOD_DECAL_2));
		break;
	case 3:
		sprite.emplace(*resource.GetTexture(BLOOD_DECAL_3));
		break;
	case 4:
		sprite.emplace(*resource.GetTexture(BLOOD_DECAL_4));
		break;
	default:
		break;
	}

	if (isSingle)
	{
		if (RandomInt(0, 1))
			sprite2.emplace(*resource.GetTexture(BLOOD_DECAL_SMALL_0));
		else
			sprite2.emplace(*resource.GetTexture(BLOOD_DECAL_SMALL_1));
	}
	else
	{
		switch (randomInt2)
	{
	case 0:
		sprite2.emplace(*resource.GetTexture(BLOOD_DECAL_0));
		break;
	case 1:
		sprite2.emplace(*resource.GetTexture(BLOOD_DECAL_1));
		break;
	case 2:
		sprite2.emplace(*resource.GetTexture(BLOOD_DECAL_2));
		break;
	case 3:
		sprite2.emplace(*resource.GetTexture(BLOOD_DECAL_3));
		break;
	case 4:
		sprite2.emplace(*resource.GetTexture(BLOOD_DECAL_4));
		break;
	default:
		break;
	}
	}
	
	Angle attackAngle = radians(std::atan2(attackDir.y, attackDir.x));

	Angle baseOffset = degrees(45.f);

	// ====================================================
	// [첫 번째 피]: 위쪽 대각선으로 튀는 피 (Top-Diagonal)
	// ====================================================
	int index1 = rand() % 6;
	FloatRect bounds1 = sprite->getLocalBounds();
	sprite->setOrigin({ bounds1.position.x + bounds1.size.x * 0.5f,
					   bounds1.position.y + bounds1.size.y * 0.5f });

	// 위쪽으로 15~35도 비틀기
	Angle topOffset = degrees(15.f + (rand() % 20));

	// Angle끼리 더하고 빼면 자동으로 내부 변환이 끝납니다!
	sprite->setRotation(attackAngle - topOffset + baseOffset);

	float scale1 = 0.8f + (rand() % 40) / 100.f;
	sprite->setScale({ scale1, scale1 });
	sprite->setPosition(descStatus.vSpawnPoint + (attackDir * (float)RandomInt(15, 20)));

	// ====================================================
	// [두 번째 피]: 정면 ~ 아래쪽 대각선으로 튀는 피 (Bottom-Diagonal)
	// ====================================================
	int index2 = rand() % 6;
	while (index2 == index1) { index2 = rand() % 6; }

	FloatRect bounds2 = sprite2->getLocalBounds();
	sprite2->setOrigin({ bounds2.position.x + bounds2.size.x * 0.5f,
					   bounds2.position.y + bounds2.size.y * 0.5f });

	// 아래쪽으로 5~25도 비틀기
	Angle bottomOffset = degrees(5.f + (rand() % 20));

	// 여기도 직관적인 덧셈!
	sprite2->setRotation(attackAngle + bottomOffset + baseOffset);

	float scale2 = 0.7f + (rand() % 40) / 100.f;
	sprite2->setScale({ scale2, -scale2 }); // Y축 뒤집기로 우하단 연출
	sprite2->setPosition(descStatus.vSpawnPoint + (attackDir * (float)RandomInt(30, 40)));

	// TODO: 렌더 타겟에 그리기
}

void BloodDecal::Update(float deltaTime)
{
	if (RewindManager::GetInstance().IsRewinding())
		isRewinding = true;
	if (isRewinding)
	{
		if (!RewindManager::GetInstance().IsRewinding())
			Destroy();
	}

	if ((accTime += (deltaTime * 0.15f)) >= 1.f)
	{
		Destroy();
	}
}

void BloodDecal::LateUpdate(float deltaTime)
{
}

void BloodDecal::Render()
{
	GameInstance::GetInstance().GetYShader().setUniform("currentTexture", Shader::CurrentTexture);
	GameInstance::GetInstance().GetYShader().setUniform("alpha", 1.f - accTime);
	RenderStates states(&GameInstance::GetInstance().GetYShader());

	GameInstance::GetInstance().GetRenderTarget_BG().draw(*this, states);
}

void BloodDecal::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
	if (isSingle) return;
	target.draw(*sprite2, states);
}

void BloodDecal::Release()
{
}