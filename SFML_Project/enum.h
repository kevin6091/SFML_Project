#pragma once

enum class ERenderLayer 
{
    Background = 0, 
    Decal,          // 벽/바닥의 혈흔, 칼자국
    Prop,           // 부서지는 문, 등불, 소품
    Actor,          // 사람
    Effect,         // 잔상, 폭발, 스파크, 피튀김
    UI,              // 체력, 슬로모션 게이지, 대화창
    End
};

enum class EObjectTag 
{
    Default,
    Player,
    Enemy,
    EnemyAttack,
    PlayerAttack,
    Wall,
    Slope,
    Hazard,
    End
};

enum class ESceneType
{
    Title,
    Stage1,
    Stage2,
    End
};

enum class EColliderType
{
    Block,          // 일반 이동 불가 벽/바닥 (obj_block)
    SlopBlock1,      // 비탈길. 오른쪽 위
    SlopBlock2,      // 비탈길. 오른쪽 아래
    Grippable,      // 벽 타기/달리기 가능한 수직 벽 (obj_grippable)
    JumpThrough,    // 일방통행(위로 뚫고 점프 가능) 발판 (obj_jumpthrough)
    EndBlock,       // 스테이지 클리어/이동 구역 (obj_end_block)
    LineAttack,     // 직선 공격
    RectAttack,     // 사각형 공격
    DynamicObject,  // 플레이어, 몬스터, 총알 등 움직이는 충돌체
    End
};

enum class EMapObjectInfo
{
    PlayerRespawn,
    Laser,
    FanBlade,
    End
};