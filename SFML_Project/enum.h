#pragma once

enum class ERenderLayer 
{
    Background = 0, 
    Decal,          // 벽/바닥의 혈흔, 칼자국
    Prop,           // 부서지는 문, 등불, 소품
    Actor,          // 적 캐릭터, 시체
    Player,         // 플레이어 전용
    Projectile,     // 투사체, 총알, 던지는 소품, 칼날 궤적
    Effect,         // 잔상, 폭발, 스파크
    UI,              // 체력, 슬로모션 게이지, 대화창
    End
};

enum class EObjectTag 
{
    Default,
    Player,
    Enemy,
    Bullet,
    Slash,
    Wall,
    Hazard,
    End
};

enum class ESceneType
{
    Title,
    Stage1,
    End
};