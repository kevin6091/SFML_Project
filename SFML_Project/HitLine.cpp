#include "HitLine.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "Camera.h"

HitLine::HitLine()
{
}

HitLine::~HitLine()
{
}

void HitLine::Initialize()
{
	auto& resource = GameInstance::GetInstance().GetResourceManager();
	sprite.emplace(*resource.GetTexture(HIT_LINE));

	Angle targetAngle = radians(atan2(dir.y, dir.x));
	Vector2u size = sprite->getTexture().getSize();

	sprite->setOrigin({ (float)size.x * 0.5f, (float)size.y * 0.5f });
	sprite->setRotation(targetAngle);
	sprite->setPosition(descStatus.vSpawnPoint + Vector2f(0.f,-20.f) + dir * -30.f);
	RandomInt(0,1) ? sprite->getColor() = Color::Cyan : sprite->getColor() = Color::Magenta;
	GameInstance::GetInstance().GetCamera().Shake(dir, 0.2f, 15.0f);
}

void HitLine::Update(float deltaTime)
{
	Vector2f pos = sprite->getPosition() + dir * deltaTime * 6000.f;
	sprite->setPosition(pos);

	if ((accTime += deltaTime) >= 0.02f)
	{
		accTime = 0.f;
		if (sprite->getColor() == Color::Cyan)
			sprite->setColor(Color::Magenta);
		else
			sprite->setColor(Color::Cyan);
	}
}

void HitLine::LateUpdate(float deltaTime)
{
}

void HitLine::Render()
{
	GameInstance::GetInstance().GetRenderTarget_Effect_Glow().draw(*this, BlendAdd);
}

void HitLine::Release()
{
}

void HitLine::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(*sprite, states);
}
