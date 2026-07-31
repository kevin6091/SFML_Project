#pragma once
#include "include.h"

class ResourceManager {
private:
    unordered_map<string, shared_ptr<Texture>> textures;
    unordered_map<string, shared_ptr<Font>> fonts;
    unordered_map<string, shared_ptr<SoundBuffer>> sounds;

public:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    shared_ptr<Texture> LoadTexture(const string& id, const string& filepath);

    shared_ptr<Texture> GetTexture(const string& id) const 
    {
        auto it = textures.find(id);
        return (it != textures.end()) ? it->second : nullptr;
    }

    void Clear() 
    {
        textures.clear();
        fonts.clear();
        sounds.clear();
    }
};