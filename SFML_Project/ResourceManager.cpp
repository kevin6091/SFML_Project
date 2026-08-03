#include "ResourceManager.h"

void ResourceManager::Initialzie()
{
}

sptr<Texture> ResourceManager::LoadTexture(const string& id, const string& filepath)
{
    auto it = textures.find(id);
    if (it != textures.end())
        return it->second;

    auto tex = sptr<Texture>(new Texture());
    if (tex->loadFromFile(filepath + ".png"))
    {
        textures[id] = tex;
        return tex;
    }

    return nullptr;
}

sptr<Texture> ResourceManager::GetTexture(const string& id) const
{
    auto it = textures.find(id);
    return (it != textures.end()) ? it->second : nullptr;
}

bool ResourceManager::LoadTextureSequence(const string& sequenceId, const filesystem::path& directoryPath, int frameCount, const string& filenamePrefix)
{
    // 이미 로드된 시퀀스면 통과
    if (animSequences.find(sequenceId) != animSequences.end()) 
    {
        return true;
    }

    vector<sptr<sf::Texture>> textures;
    textures.reserve(frameCount);

    for (int i = 0; i < frameCount; ++i)
    {
        // 파일명 조합 (예: assets/zero_run/frame_0.png)
        string filename = filenamePrefix + to_string(i);
        filesystem::path fullPath = directoryPath / filename;

        string frameId = sequenceId + "_" + to_string(i);
        auto tex = LoadTexture(frameId, fullPath.string());

        if (!tex) 
        {
            return false;
        }
        textures.push_back(tex);
    }

    animSequences[sequenceId] = move(textures);
    return true;
}

const vector<sptr<Texture>>* ResourceManager::GetTextureSequence(const string& sequenceId) const
{
    auto it = animSequences.find(sequenceId);
    if (it != animSequences.end()) 
        return &(it->second);

    return nullptr;
}

void ResourceManager::Release()
{
    textures.clear();
    fonts.clear();
    sounds.clear();
    animSequences.clear();
}
