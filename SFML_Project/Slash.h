#pragma once
#include "include.h"
#include "GameObject.h"
#include "Animator.h"
#include "RewindTracker.h"

class Player;

struct SlashSnapshot
{
	Vector2f position1;
	Vector2f position2;
	Vector2f scale1;
	Vector2f scale2;
	IntRect textureRect1;
	IntRect textureRect2;
	const Texture* texture1;
	const Texture* texture2;
	bool isRender;
};

class Slash : public GameObject
{
public:
	Slash(Player* _player);
	virtual ~Slash() override;

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void draw(RenderTarget& target, RenderStates states) const override;

	virtual void RestartObject();
public:
	void PlayerAttack();

private:
	Player* player;

	Animator animator_main;
	Animator animator_sub;
	optional<Sprite> sprite_sub;

	bool isPlayerAttack = false;

#pragma region Rewind

	RewindTracker<SlashSnapshot> rewinder;
	float rewindSpeed = 1.f;
	float rewinderTime = 0.f;

#pragma endregion
};

