#pragma once
#include "include.h"
#include <filesystem>

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
private:
    unordered_map<string, sptr<Texture>> textures;
    unordered_map<string, sptr<Font>> fonts;
    unordered_map<string, sptr<SoundBuffer>> sounds;
    unordered_map<string, vector<sptr<Texture>>> animSequences;
};