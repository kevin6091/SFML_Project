#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::UpdatePhysics(float deltaTime, list<sptr<GameObject>> dynamics, list<sptr<GameObject>> statics)
{
	for (auto& obj : dynamics)
    { 
        {
            // 1. 객체의 현재 위치와 이번 프레임 이동량(Velocity * dt) 획득
            Vector2f pos = obj->GetPosition();
            Vector2f vel = obj->GetVelocity();
            float speed = obj->GetSpeed();

            Vector2f moveDelta = vel * deltaTime;

            // X축 이동 및 충돌
            pos.x += moveDelta.x;
            obj->SetPosition(pos); // 임시 적용 후 BoundingBox 검사

            for (const auto& wall : statics)
            {
                FloatRect objBounds = obj->GetCollider().GetBounds();
                FloatRect wallBounds = wall->GetCollider().GetBounds();

                if (optional<FloatRect> overlap = objBounds.findIntersection(wallBounds))
                {
                    if (vel.x > 0.f) pos.x -= overlap->size.x;
                    else if (vel.x < 0.f) pos.x += overlap->size.x;
                    
                    vel.x = 0.f; // 벽에 막혔으니 속도 0
                    obj->SetPosition(pos); // 밀어낸 위치 즉시 갱신

                    if (wall->GetCollider().GetColliderType() == EColliderType::Grippable)
                        obj->CollisionEvent(*wall);
                }
            }

            // Y축 이동 및 충돌
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

                        if (objCenterY < wallCenterY) {
                            // 플레이어가 타일보다 위쪽에 있으면 바닥으로 취급
                            pos.y -= overlap->size.y;
                            isGrounded = true;
                        }
                        else {
                            // 플레이어가 타일보다 아래쪽에 있으면 천장으로 취급
                            pos.y += overlap->size.y;
                        }
                    }

                    vel.y = 0.f;
                    obj->SetPosition(pos);
                }
            }

            // 2. 최종 계산된 속도와 Grounded 상태를 객체에 다시 주입
            obj->SetVelocity(vel);
            obj->SetIsGrounded(isGrounded);
        }
    }
}
