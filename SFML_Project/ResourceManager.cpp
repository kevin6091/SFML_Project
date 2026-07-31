#include "ResourceManager.h"

shared_ptr<Texture> ResourceManager::LoadTexture(const string& id, const string& filepath)
{
    auto it = textures.find(id);
    if (it != textures.end())
        return it->second;

    auto tex = sptr<Texture>(new Texture());
    if (tex->loadFromFile(filepath))
    {
        textures[id] = tex;
        return tex;
    }

    return nullptr;
}
