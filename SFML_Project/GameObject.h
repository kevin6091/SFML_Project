#pragma once
#include "include.h"

class GameObject : public Drawable
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

public:
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

	virtual void draw(RenderTarget& target, RenderStates states) const override = 0;

 	__forceinline void Destroy() { isDestroy = true; }
	__forceinline bool IsDestroy() const { return isDestroy; }

	__forceinline void SetActive(bool active) { isActive = active; }
	__forceinline bool IsActive() const { return isActive; }

	__forceinline Vector2f GetPosition() const { return position; }
	__forceinline virtual void SetPosition(const Vector2f& pos) { position = pos; }

	__forceinline ERenderLayer GetLayer() const { return eRenderLayer; }
	__forceinline void SetLayer(ERenderLayer _layer) { eRenderLayer = _layer; }

	__forceinline EObjectTag GetTag() const { return eObjectTag; }
	__forceinline void SetTag(EObjectTag tag) { eObjectTag = tag; }

    // 충돌 판정용 AABB 바운딩 박스 (기본값: 위치 기준 크기 0)
	//__forceinline virtual FloatRect GetBoundingBox() const { return FloatRect(position, { 0.0f, 0.0f }); }

protected:
	Vector2f position{ 0.0f, 0.0f };
	Vector2f scale{ 1.0f, 1.0f };
	float rotation = 0.0f;

	ERenderLayer eRenderLayer = ERenderLayer::End;
	EObjectTag eObjectTag = EObjectTag::End;

	bool isActive = true;		// 그리거나 연산할지 말지
	bool isDestroy = false;		// Delete할지 말지
};

