#include "CollisionManager.h"
#include "InputManager.h"
#include "GameInstance.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::UpdatePhysics(float deltaTime, list<sptr<GameObject>> dynamics, list<sptr<GameObject>> statics)
{
    bool isDownPressed = GameInstance::GetInstance().GetInputManager().GetKeyPress(Keyboard::Key::S);

	for (auto& obj : dynamics)
    { 
        if (!obj->IsActive())
        {
            // 물리 계수 설정
            float bounciness = 0.5f; // 탄성
            float friction = 0.9f;   // 마찰

            Vector2f pos = obj->GetPosition();
            Vector2f vel = obj->GetVelocity();
            Vector2f moveDelta = vel * deltaTime;

            // 1. X축 이동 및 튕김
            pos.x += moveDelta.x;
            obj->SetPosition(pos);
            obj->SetVelocity(vel);
            
            for (const auto& wall : statics)
            {
                FloatRect objBounds = obj->GetCollider().GetBounds();
                FloatRect wallBounds = wall->GetCollider().GetBounds();

                if (optional<FloatRect> overlap = objBounds.findIntersection(wallBounds))
                {
                    if (vel.x > 0.f) pos.x -= overlap->size.x;
                    else if (vel.x < 0.f) pos.x += overlap->size.x;

                    // 벽에 부딪히면 X축 속도를 반전시키고 탄성 감소
                    vel.x = -vel.x * bounciness;
                    obj->SetVelocity(vel);
                    obj->SetPosition(pos);

                    obj->CollisionBounce();
                }
            }

            // 2. Y축 이동 및 튕김
            pos.y += moveDelta.y;
            obj->SetPosition(pos);
            bool isGrounded = false;

            for (const auto& wall : statics)
            {
                FloatRect objBounds = obj->GetCollider().GetBounds();
                FloatRect wallBounds = wall->GetCollider().GetBounds();

                if (optional<FloatRect> overlap = objBounds.findIntersection(wallBounds))
                {
                    if (vel.y > 0.f)
                    {
                        // 바닥에 닿았을 때만 X축 속도(구르는 속도)를 줄여 마찰 구현
                        pos.y -= overlap->size.y;
                        vel.x *= friction;
                        isGrounded = true;
                    }
                    else if (vel.y < 0.f)
                    {
                        pos.y += overlap->size.y;
                    }

                    // 천장이나 바닥에 부딪히면 Y축 속도를 반전시키고 탄성 감소
                    vel.y = -vel.y * bounciness;

                    // 중력(가속도)에 의해 생기는 속도보다 튕기는 속도가 작으면 강제로 멈춤
                    if (abs(vel.y) < 30.f) vel.y = 0.f;

                    obj->SetVelocity(vel);
                    obj->SetPosition(pos);

                    obj->CollisionBounce();
                    if (abs(vel.y) < 100.f && isGrounded)
                        obj->CollisionBounceEnd();
                }
            }

            obj->SetIsGrounded(isGrounded);
        }
        else
        {
            // 1. 객체의 현재 위치와 이번 프레임 이동량(Velocity * dt) 획득
            Vector2f pos = obj->GetPosition();
            Vector2f vel = obj->GetVelocity();
            float speed = obj->GetSpeed();

            Vector2f moveDelta = vel * deltaTime;

            bool wasGrounded = obj->GetIsGrounded();
            bool wasOnSlope = obj->GetIsOnSlope(); // 이전 프레임에 빗면을 탔는가?
            bool isOnSlopeThisFrame = false;       // 이번 프레임에 빗면을 타는가?

            // X축 이동 및 충돌
            pos.x += moveDelta.x;
            obj->SetPosition(pos); // 임시 적용 후 BoundingBox 검사

            for (const auto& wall : statics)
            {
                EColliderType wallType = wall->GetCollider().GetColliderType();

                if (wallType == EColliderType::SlopBlock1 || 
                    wallType == EColliderType::SlopBlock2 || 
                    wallType == EColliderType::JumpThrough)
                    continue;

                FloatRect objBounds = obj->GetCollider().GetBounds();
                FloatRect wallBounds = wall->GetCollider().GetBounds();

                if (optional<FloatRect> overlap = objBounds.findIntersection(wallBounds))
                {
                    float playerBottom = objBounds.position.y + objBounds.size.y;
                    float wallTop = wallBounds.position.y;
                    float stepTolerance = wasOnSlope ? 32.0f : 8.0f; // 타일 크기에 맞춰 조절(32 추천)

                    bool isFallingOrFlat = (vel.y >= 0.f);
                    if (wasGrounded && isFallingOrFlat && (playerBottom - wallTop) <= stepTolerance && (playerBottom - wallTop) > 0.f)
                    {
                        continue;
                    }

                    if (vel.x > 0.f) pos.x -= overlap->size.x;
                    else if (vel.x < 0.f) pos.x += overlap->size.x;
                    
                    vel.x = 0.f; // 벽에 막혔으니 속도 0
                    obj->SetPosition(pos); // 밀어낸 위치 즉시 갱신

                    if (wall->GetCollider().GetColliderType() == EColliderType::Grippable)
                        obj->CollisionEvent(*wall);
                    if (wall->GetCollider().GetColliderType() == EColliderType::EndBlock)
                        wall->CollisionEvent(*obj);
                }
            }

            // Y축 이동 및 충돌
            pos.y += moveDelta.y;
            obj->SetPosition(pos);
            bool isGrounded = false;

            for (const auto& wall : statics)
            {
                EColliderType wallType = wall->GetCollider().GetColliderType();
                FloatRect objBounds = obj->GetCollider().GetBounds();
                FloatRect wallBounds = wall->GetCollider().GetBounds();

                if (wallType == EColliderType::JumpThrough)
                {
                    // 이전 프레임의 발바닥 위치 역산 (현재 바닥 Y - 이번 프레임 이동량)
                    if (isDownPressed)
                        continue;

                    // 이전 프레임의 발바닥 위치 역산 (현재 바닥 Y - 이번 프레임 이동량)
                    float prevBottom = objBounds.position.y + objBounds.size.y - moveDelta.y;
                    float wallTop = wallBounds.position.y;

                    // 조건 1: 낙하 중일 것 (vel.y >= 0)
                    // 조건 2: 이전 프레임에 플레이어 발바닥이 블록의 상단(Top)보다 위(또는 살짝 오차 내)에 있었을 것
                    // (※ !isDownPressed 조건은 위에서 이미 걸렀으므로 삭제했습니다)
                    if (vel.y >= 0.f && prevBottom <= wallTop + 4.0f)
                    {
                        if (optional<FloatRect> overlap = objBounds.findIntersection(wallBounds))
                        {
                            pos.y -= overlap->size.y;
                            vel.y = 0.f;
                            isGrounded = true;
                            obj->SetPosition(pos);
                        }
                    }
                }
                else if (wallType == EColliderType::SlopBlock1 || wallType == EColliderType::SlopBlock2)
                {
                    bool isSlopeUp = (wallType == EColliderType::SlopBlock1);
                    float newBoundsTopY = 0.f;

                    // 빗면 충돌 판정 및 보정 위치 받아오기
                    if (SlopeCollision(objBounds, wallBounds, isSlopeUp, newBoundsTopY))
                    {
                        // Origin 설정에 상관없이, 이동해야 할 변화량(Delta)만큼 pos.y에 더해줍니다.
                        pos.y += (newBoundsTopY - objBounds.position.y);
                        vel.y = 0.f;
                        isGrounded = true;
                        isOnSlopeThisFrame = true;
                        obj->SetPosition(pos);
                    }
                }
                else
                {
                    if (optional<FloatRect> overlap = objBounds.findIntersection(wallBounds))
                    {
                        if (vel.y > 0.f)
                        {
                            pos.y -= overlap->size.y;
                            isGrounded = true; // 땅에 닿음 
                        }
                        else if (vel.y < 0.f)
                        {
                            pos.y += overlap->size.y;
                        }
                        else
                        {
                            // vel.y == 0.f 인데 충돌했을 경우 (무중력 대시 등)
                            // 객체의 중심과 벽의 중심 Y좌표를 비교하여 위/아래를 판단
                            float objCenterY = objBounds.position.y + objBounds.size.y * 0.5f;
                            float wallCenterY = wallBounds.position.y + wallBounds.size.y * 0.5f;

                            if (objCenterY < wallCenterY)
                            {
                                // 플레이어가 타일보다 위쪽에 있으면 바닥으로 취급
                                pos.y -= overlap->size.y;
                                isGrounded = true;
                            }
                            else
                            {
                                // 플레이어가 타일보다 아래쪽에 있으면 천장으로 취급
                                pos.y += overlap->size.y;
                            }
                        }

                        vel.y = 0.f;
                        obj->SetPosition(pos);
                    }
                }
            }

            // Slope Snap
            if (!isGrounded && wasGrounded && vel.y >= 0.f)
            {
                FloatRect objBounds = obj->GetCollider().GetBounds();
                float playerCenterX = objBounds.position.x + objBounds.size.x * 0.5f;
                float playerBottom = objBounds.position.y + objBounds.size.y;

                float snapDistance = 32.0f;

                float bestSurfaceY = 999999.f;
                bool foundSnap = false;

                for (const auto& wall : statics)
                {
                    EColliderType wallType = wall->GetCollider().GetColliderType();

                    if (isDownPressed && wallType == EColliderType::JumpThrough)
                        continue;
                    
                    if (isDownPressed && wallType == EColliderType::Block)
                    {
                        FloatRect wallBounds = wall->GetCollider().GetBounds();
                        if (wallBounds.position.y > playerBottom + 4.0f)
                            continue; // 허공을 떨어질 땐 억지로 블록에 스냅하지 않음
                    }

                    if (wallType != EColliderType::Block &&
                        wallType != EColliderType::SlopBlock1 &&
                        wallType != EColliderType::SlopBlock2 &&
                        wallType != EColliderType::JumpThrough)
                        continue;

                    FloatRect wallBounds = wall->GetCollider().GetBounds();

                    // 1. 벽이나 빗면의 가로(X) 구간 안에 플레이어의 '정중앙'이 속해 있는지 확인
                    if (playerCenterX >= wallBounds.position.x && playerCenterX <= wallBounds.position.x + wallBounds.size.x)
                    {
                        float surfaceY = wallBounds.position.y; // 일반 블록은 윗부분이 표면

                        // 빗면일 경우, X좌표에 따른 정확한 표면(Y) 높이를 계산
                        if (wallType == EColliderType::SlopBlock1 || wallType == EColliderType::SlopBlock2)
                        {
                            float localX = playerCenterX - wallBounds.position.x;
                            float ratio = localX / wallBounds.size.x;

                            if (wallType == EColliderType::SlopBlock1)
                                surfaceY = (wallBounds.position.y + wallBounds.size.y) - (ratio * wallBounds.size.y);
                            else
                                surfaceY = wallBounds.position.y + (ratio * wallBounds.size.y);
                        }

                        // 2. 찾아낸 표면이 플레이어의 발밑~스냅 사거리 내에 존재한다면?
                        if (surfaceY >= playerBottom - 2.0f && surfaceY <= playerBottom + snapDistance)
                        {
                            // 가장 높은 땅(Y값이 제일 작은 땅)을 최우선으로 잡음
                            if (surfaceY < bestSurfaceY)
                            {
                                bestSurfaceY = surfaceY;
                                foundSnap = true;
                            }
                        }
                    }
                }

                // 3. 바닥을 찾았다면 강제로 끌어당기기
                if (foundSnap)
                {
                    // ★ 4. pos.y에 덮어씌우지 않고, 바운딩 박스 기준의 '오차(Delta)값'만큼만 더해줍니다!
                    // 이렇게 해야 Origin(중심점) 세팅에 상관없이 발바닥이 땅에 정확히 붙습니다.
                    float targetTopY = bestSurfaceY - objBounds.size.y;
                    pos.y += (targetTopY - objBounds.position.y);

                    vel.y = 0.f;
                    isGrounded = true;
                    isOnSlopeThisFrame = true;
                    obj->SetPosition(pos);
                }
            }

            obj->SetVelocity(vel);
            obj->SetIsGrounded(isGrounded);
            obj->SetIsOnSlope(isOnSlopeThisFrame);
        }
    }
}

void CollisionManager::CollisionTest(list<sptr<GameObject>> lObjs, list<sptr<GameObject>> rObjs)
{
    for (auto& lObj : lObjs)
    {
        for (auto& rObj : rObjs)
        {
            FloatRect lBounds = lObj->GetCollider().GetBounds();
            FloatRect rBounds = rObj->GetCollider().GetBounds();
            if (lObj->GetCollider().GetColliderType() == EColliderType::End)
                continue;

            if (lObj->GetCollider().GetColliderType() == EColliderType::LineAttack)
            {
                Vector2f offset;
                if (rObj->GetIsDoor())
                {
                    offset = Vector2f{ 0.f, 64.f };
                }

                if (LineCollisionTest(lBounds.position, lBounds.size, rBounds.position + offset, 60.f))
                {
                    lObj->CollisionEvent(*rObj);
                    rObj->CollisionEvent(*lObj);
                }
            }
            else if (optional<FloatRect> overlap = lBounds.findIntersection(rBounds))
            {
                lObj->CollisionEvent(*rObj);
                rObj->CollisionEvent(*lObj);
            }
        }
    }
}

bool CollisionManager::LineCollisionTest(Vector2f A, Vector2f B, Vector2f P, float thickness)
{
    Vector2f LineAB = B - A;
    Vector2f LineAP = P - A;

    float lengthSq = LineAB.lengthSquared();

    if (lengthSq == 0.f)
    {
        return (P - A).length() <= thickness;
    }

    float t = LineAP.dot(LineAB) / lengthSq;

    t = clamp(t, 0.f, 1.f);

    Vector2f C = A + (LineAB * t);

    float distance = (P - C).length();

    return distance <= thickness;
}

bool CollisionManager::SlopeCollision(const FloatRect& objBounds, const FloatRect& slopeBounds, bool isSlopeUp, float& outNewY)
{
    if (!objBounds.findIntersection(slopeBounds).has_value())
        return false;

    float objCenterX = objBounds.position.x + objBounds.size.x * 0.5f;
    float objBottomY = objBounds.position.y + objBounds.size.y;

    float localX = objCenterX - slopeBounds.position.x;

    // ★ [수정된 부분] clamp를 지우고, 중심축이 빗면 범위를 벗어나면 아예 무시합니다! ★
    // 이렇게 해야 인접한 두 빗면에 동시에 걸쳤을 때 충돌 연산이 꼬이지 않습니다.
    if (localX < 0.f || localX > slopeBounds.size.x)
    {
        return false;
    }

    float ratio = localX / slopeBounds.size.x;

    float surfaceY = 0.f;
    if (isSlopeUp) {
        surfaceY = (slopeBounds.position.y + slopeBounds.size.y) - (ratio * slopeBounds.size.y);
    }
    else {
        surfaceY = slopeBounds.position.y + (ratio * slopeBounds.size.y);
    }

    if (objBottomY >= surfaceY)
    {
        outNewY = surfaceY - objBounds.size.y;
        return true;
    }

    return false;
}
