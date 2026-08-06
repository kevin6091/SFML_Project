#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Initialize(Vector2f viewSize)
{
    view.setSize(viewSize);
    view.setCenter(viewSize * 0.5f);
}

void Camera::Update(float deltaTime)
{
    if (followTarget) 
    {
        vTargetPos = followTarget->GetPosition() + vOffset;
    }

    // 2. 부드러운 카메라 이동
    Vector2f currentCenter = view.getCenter(); // 순수 중심점
    Vector2f newCenter = currentCenter + (vTargetPos - currentCenter) * (smoothSpeed * deltaTime);

    // 3. 스테이지 밖으로 카메라가 나가지 못하도록 Clamp
    if (useBounds) 
    {
        Vector2f halfSize = view.getSize() * 0.5f;

        float minX = bounds.position.x + halfSize.x;
        float maxX = bounds.position.x + bounds.size.x - halfSize.x;
        float minY = bounds.position.y + halfSize.y;
        float maxY = bounds.position.y + bounds.size.y - halfSize.y;

        // 스테이지 크기가 뷰보다 작은 예외 처리
        if (minX < maxX) newCenter.x = clamp(newCenter.x, minX, maxX);
        if (minY < maxY) newCenter.y = clamp(newCenter.y, minY, maxY);
    }

    // 4. Camera Shake 계산
    if (shakeTimer > 0.f)
    {
        shakeTimer -= deltaTime;

        if (shakeTimer <= 0.f)
        {
            shakeTimer = 0.f;
            vShakeOffset = { 0.f, 0.f };
        }
        else
        {
            float time = shakeTimer / shakeDuration;
            float ease = time * time * time;

            vShakeOffset = vShakeTargetOffset * ease;
        }
    }

    // 5. 최종 뷰 중심점 적용 (흔들림 오프셋 포함)
    view.setCenter(newCenter + vShakeOffset);
}

void Camera::SetZoom(float factor)
{
    view.zoom(factor);
}

void Camera::Shake(Vector2f dir, float duration, float intensity)
{
    shakeDuration = duration;
    shakeTimer = duration;

    vShakeTargetOffset = dir * intensity;
}
