#pragma once
#include "include.h"

class SoundManager
{
private:
    SoundManager() = default;
    ~SoundManager() = default;

public:
    // 싱글톤 인스턴스 반환
    static SoundManager& GetInstance()
    {
        static SoundManager instance;
        return instance;
    }

    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    // 매 프레임 호출하여 재생이 끝난 소리나 에코 큐를 정리합니다.
    void Update(float deltaTime);

    // ==========================================
    // 리소스 로드
    // ==========================================
    bool LoadSFX(const string& key, const string& filepath);
    bool LoadBGM(const string& key, const string& filepath);

    // ==========================================
    // 재생 컨트롤
    // ==========================================
    // BGM 재생 (기존 BGM은 자동으로 멈춤)
    void PlayBGM(const string& key, float volume = 100.f, float pitch = 1.0f, bool loop = true, bool ignoreGlobalPitch = false);
    void StopBGM();

    // 단발성 효과음(SFX) 재생
    void PlaySFX(const string& key, float volume = 100.f, float pitch = 1.0f, bool ignoreGlobalPitch = false);

    // 야매(?) 리버브 효과음 재생 (원음 재생 후, 딜레이를 두고 볼륨이 줄어드는 에코 발생)
    void PlaySFXWithReverb(const string& key, float volume = 100.f, float pitch = 1.0f, int echoCount = 3, float echoDelay = 0.08f,  bool ignoreGlobalPitch = false);

    // ==========================================
    // 특수 효과 (카타나 제로 슬로우 모션 연동)
    // ==========================================
    // 전체 사운드의 피치(재생 속도 및 주파수)를 일괄 조절합니다.
    void SetGlobalPitch(float pitch);

private:
    // 리소스 컨테이너
    unordered_map<string, SoundBuffer> m_sfxBuffers;
    unordered_map<string, unique_ptr<Music>> m_bgms;

    // 현재 재생 중인 다중 효과음 관리 리스트
    list<Sound> m_activeSounds;
    list<Sound> m_unaffectedSounds;
    // 현재 재생 중인 BGM 포인터
    Music* m_currentBGM = nullptr;
    bool m_bgmIgnoreGlobalPitch = false;

    // 현재 게임의 전체 피치 (슬로우 모션 비율)
    float m_globalPitch = 1.0f;

    // 리버브(에코) 구현을 위한 딜레이 큐 구조체
    struct EchoTask
    {
        string key;
        float timeLeft;
        float volume;
        float pitch;
        bool ignoreGlobalPitch;
    };
    list<EchoTask> m_echoQueue;
};