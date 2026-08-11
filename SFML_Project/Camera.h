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

    void Shake(Vector2f dir, float duration, float intensity);

    __forceinline const View& GetView() const { return view; }
    __forceinline Vector2f GetPosition() const { return view.getCenter(); }

private:
    View view;
    Vector2f vTargetPos{ 0.0f, 0.0f };
    const GameObject* followTarget = nullptr;

    // 카메라 추적 옵션
    float smoothSpeed = 8.0f; // 카메라 보간 속도 (Lerp)
    Vector2f vOffset{ 0.0f, -0.0f }; // 플레이어 살짝 위를 바라보도록 오프셋

    // 스테이지 경계 제한 (Bounding Rect)
    bool useBounds = false;
    FloatRect bounds;

    // Shake
    float shakeTimer = 0.f;           
    float shakeDuration = 0.f;        
    float shakeIntensity = 0.f;       

    Vector2f vShakeTargetOffset;      
    Vector2f vShakeOffset;
};