#include "SoundManager.h"

void SoundManager::Update(float deltaTime)
{
    // 1. 재생이 완전히 끝난(Stopped) 효과음 객체를 리스트에서 메모리 해제
    m_activeSounds.remove_if([](const Sound& sound) {
        return sound.getStatus() == Sound::Status::Stopped;
        });

    // ★ [추가] 무시 리스트도 똑같이 메모리 관리
    m_unaffectedSounds.remove_if([](const Sound& sound) {
        return sound.getStatus() == Sound::Status::Stopped;
        });

    // 2. 가짜 리버브(에코) 딜레이 타이머 처리
    for (auto it = m_echoQueue.begin(); it != m_echoQueue.end(); )
    {
        it->timeLeft -= deltaTime;
        if (it->timeLeft <= 0.f)
        {
            // ★ [추가] 딜레이가 끝나면 무시 여부(ignoreGlobalPitch)도 함께 전달
            PlaySFX(it->key, it->volume, it->pitch, it->ignoreGlobalPitch);
            it = m_echoQueue.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool SoundManager::LoadSFX(const string& key, const string& filepath)
{
    if (m_sfxBuffers.find(key) != m_sfxBuffers.end())
        return true;

    SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath))
    {
        cerr << "SFX 로드 실패: " << filepath << endl;
        return false;
    }

    m_sfxBuffers[key] = std::move(buffer);
    return true;
}

bool SoundManager::LoadBGM(const string& key, const string& filepath)
{
    if (m_bgms.find(key) != m_bgms.end())
        return true;

    auto music = make_unique<Music>();
    if (!music->openFromFile(filepath))
    {
        cerr << "BGM 로드 실패: " << filepath << endl;
        return false;
    }

    m_bgms[key] = std::move(music);
    return true;
}

void SoundManager::PlayBGM(const string& key, float volume, float pitch, bool loop, bool ignoreGlobalPitch)
{
    auto it = m_bgms.find(key);
    if (it == m_bgms.end()) return;

    if (m_currentBGM)
        m_currentBGM->stop();

    m_currentBGM = it->second.get();
    m_bgmIgnoreGlobalPitch = ignoreGlobalPitch; // ★ 상태 기억

    m_currentBGM->setVolume(volume);

    // ★ 무시 여부에 따라 피치 결정
    float finalPitch = ignoreGlobalPitch ? pitch : (m_globalPitch * pitch);
    m_currentBGM->setPitch(finalPitch);

    m_currentBGM->setLooping(loop);
    m_currentBGM->play();
}

void SoundManager::StopBGM()
{
    if (m_currentBGM)
    {
        m_currentBGM->stop();
        m_currentBGM = nullptr;
    }
}

void SoundManager::PlaySFX(const string& key, float volume, float pitch, bool ignoreGlobalPitch)
{
    auto it = m_sfxBuffers.find(key);
    if (it == m_sfxBuffers.end()) return;

    // ★ [수정] 무시 여부에 따라 담을 바구니(리스트)를 다르게 선택합니다!
    if (ignoreGlobalPitch)
    {
        m_unaffectedSounds.emplace_back(it->second);
        Sound& newSound = m_unaffectedSounds.back();
        newSound.setVolume(volume);
        newSound.setPitch(pitch); // 글로벌 피치 안 섞음
        newSound.play();
    }
    else
    {
        m_activeSounds.emplace_back(it->second);
        Sound& newSound = m_activeSounds.back();
        newSound.setVolume(volume);
        newSound.setPitch(m_globalPitch * pitch); // 글로벌 피치 섞음
        newSound.play();
    }
}

void SoundManager::PlaySFXWithReverb(const string& key, float volume, float pitch, int echoCount, float echoDelay, bool ignoreGlobalPitch)
{
    // 원음 즉시 재생 (무시 여부 전달)
    PlaySFX(key, volume, pitch, ignoreGlobalPitch);

    // 잔향(에코) 생성 (원본 계산식 0.4f 유지)
    float currentVolume = volume;
    for (int i = 1; i <= echoCount; ++i)
    {
        currentVolume *= 0.4f;
        if (currentVolume < 1.0f) break;

        // 큐에 넣을 때 무시 여부도 같이 넣어줌
        m_echoQueue.push_back({ key, echoDelay * i, currentVolume, pitch, ignoreGlobalPitch });
    }
}

void SoundManager::SetGlobalPitch(float pitch)
{
    m_globalPitch = pitch;

    // 현재 재생 중인 BGM 속도 변경 (무시 상태가 아닐 때만)
    if (m_currentBGM && !m_bgmIgnoreGlobalPitch)
    {
        m_currentBGM->setPitch(pitch);
    }

    // ★ 날아가고 있는 총알 소리 등은 m_activeSounds에만 들어있으므로,
    // 이 루프를 돌려도 UI 소리(m_unaffectedSounds)는 절대 느려지지 않습니다!
    for (auto& sound : m_activeSounds)
    {
        sound.setPitch(pitch);
    }
}