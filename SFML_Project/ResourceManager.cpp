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

    cerr << "텍스쳐 로딩안됨!! : " << filepath << endl;

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

    vector<sptr<Texture>> textures;
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

bool ResourceManager::LoadMap(const string& filePath, 
    vector<ColliderDesc>& outColliders,
    vector<MapObjectInfo>& outObjects,
    Vector2f& outMapSize)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Map Info Json 읽기 실패!! : " << filePath << endl;
        return false;
    }

    json mapJson;
    file >> mapJson;

    // 룸 크기 정보 추출
    float roomWidth = mapJson.value("width", 0.0f);
    float roomHeight = mapJson.value("height", 0.0f);
    outMapSize = Vector2f(roomWidth, roomHeight);

    // 기본 충돌 블록 1개의 원본 크기 (GameMaker 기본 16x16 그리드)
    constexpr float BASE_TILE_SIZE = 32.0f;

    // 레이어 순회
    for (const auto& layer : mapJson["layers"])
    {
        string layerName = layer.value("layer_name", "");

        for (const auto& inst : layer["instances"])
        {
            string objName = inst.value("object_name", "");
            float x = inst.value("x", 0.0f);
            float y = inst.value("y", 0.0f);
            float scaleX = inst.value("scale_x", 1.0f);
            float scaleY = inst.value("scale_y", 1.0f);

            if (layerName == "blocks")
            {
                ColliderDesc collider;
                collider.objectName = objName;

                // AABB Bounds 계산 (GameMaker scale 변환 적용)
                float width = round(scaleX * BASE_TILE_SIZE);
                float height = round(scaleY * BASE_TILE_SIZE);

                collider.bounds = FloatRect({ x, y }, { width, height });

                if (objName == "obj_block")             collider.type = EColliderType::Block;
                else if (objName == "obj_grippable")    collider.type = EColliderType::Grippable;
                else if (objName == "obj_jumpthrough")  collider.type = EColliderType::JumpThrough;
                else if (objName == "obj_end_block")    collider.type = EColliderType::EndBlock;
                else if (objName == "obj_slope")        collider.type = EColliderType::SlopBlock1;
                else if (objName == "obj_slope2")       collider.type = EColliderType::SlopBlock2;
                outColliders.push_back(collider);
            }
            else if(layerName == "objects")
            {
                MapObjectInfo objInfo;
                objInfo.objectName = objName;
                objInfo.layerName = layerName;
                objInfo.position = Vector2f(x, y);
                objInfo.scale = Vector2f(scaleX, scaleY);
                
                outObjects.push_back(objInfo);
            }
            else if (layerName == "player")
            {
                // Player 리스폰위치
                MapObjectInfo objInfo;
                objInfo.objectName = objName;
                objInfo.layerName = layerName;
                objInfo.position = Vector2f(x, y);
                objInfo.scale = Vector2f(scaleX, scaleY);
                
                outObjects.push_back(objInfo);
            }
        }
    }

    return true;
}

void ResourceManager::Release()
{
    textures.clear();
    fonts.clear();
    sounds.clear();
    animSequences.clear();
}
