#pragma once
#include "include.h"
#include <filesystem>

using json = nlohmann::json;

// 지형 충돌체 데이터
struct ColliderDesc
{
    string objectName;
    EColliderType type = EColliderType::End;
    FloatRect bounds;
};

// 맵 내 스폰될 기타 객체 정보 (레이저, 환풍기, 플레이어 리스폰 등)
struct MapObjectInfo
{
    string objectName;
    string layerName;
    Vector2f position;
    Vector2f scale;
};

class ResourceManager 
{
public:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

public:
    void Initialzie();
    void Release();

    sptr<Texture> LoadTexture(const string& id, const string& filepath);
    sptr<Texture> GetTexture(const string& id) const;

    bool LoadTextureSequence(const string& sequenceId, const filesystem::path& directoryPath, int frameCount, const string& filenamePrefix = "frame_");
    const vector<sptr<Texture>>* GetTextureSequence(const string& sequenceId) const;

    bool LoadMap(const string& filePath,
        vector<ColliderDesc>& outColliders,
        vector<MapObjectInfo>& outObjects,
        Vector2f& outMapSize);

private:
    unordered_map<string, sptr<Texture>> textures;
    unordered_map<string, sptr<Font>> fonts;
    unordered_map<string, sptr<SoundBuffer>> sounds;
    unordered_map<string, vector<sptr<Texture>>> animSequences;
};