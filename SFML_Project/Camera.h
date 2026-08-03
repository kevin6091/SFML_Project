#pragma once
#include "include.h"
#include "GameObject.h"

class Camera 
{
public:
    Camera();
    ~Camera();

    void Initialize(Vector2f viewSize);
    void Update(float deltaTime);

    __forceinline void SetTarget(const GameObject* target) { followTarget = target; }
    __forceinline void SetBounds(const FloatRect& _bounds) { bounds = _bounds; useBounds = true; }
    __forceinline void SetOffSet(Vector2f vector) { vOffset = vector; }


    void SetZoom(float factor);

    //duration 흔들릴 시간(초)
    //intensity 흔들림 강도 (픽셀 단위)
    void Shake(float duration, float intensity);


    __forceinline const View& GetView() const { return view; }
    __forceinline Vector2f GetPosition() const { return view.getCenter(); }

private:
    View view;
    Vector2f vTargetPos{ 0.0f, 0.0f };
    const GameObject* followTarget = nullptr; // 단일 소유권 원칙에 따른 Raw Pointer 관찰

    // 1. 카메라 추적 옵션
    float smoothSpeed = 8.0f; // 카메라 보간 속도 (Lerp)
    Vector2f vOffset{ 0.0f, -0.0f }; // 플레이어 살짝 위를 바라보도록 오프셋

    // 2. 스테이지 경계 제한 (Bounding Rect)
    bool useBounds = false;
    FloatRect bounds;

    float shakeDuration = 0.0f;
    float shakeIntensity = 0.0f;
    Vector2f vShakeOffset{ 0.0f, 0.0f };
};