#include "model_loader.h"

ModelLoader::ModelLoader() :
    m_device(nullptr),
    m_deviceContext(nullptr),
    m_meshes(),
    directory(),
    texturesLoaded(),
    m_hwnd(nullptr) {}

ModelLoader::~ModelLoader() {}

bool ModelLoader::Load(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filename) {
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr) {
        return false;
    }

    this->directory = filename.substr(0, filename.find_last_of("/\\"));

    this->m_device = device;
    this->m_deviceContext = deviceContext;
    this->m_hwnd = hwnd;

    processNode(pScene->mRootNode, pScene);

    return true;
}

void ModelLoader::Draw(ID3D11DeviceContext* deviceContext) {
    for (size_t i = 0; i < m_meshes.size(); ++i) {
        m_meshes[i].Draw(deviceContext);
    }
}

Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<UINT> indices;
    std::vector<Texture> textures;

    for (UINT i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.x = mesh[i].mVertices->x;
        vertex.y = mesh[i].mVertices->y;
        vertex.z = mesh[i].mVertices->z;

        if (mesh->mTextureCoords[0]) {
            vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[i]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    }

    return Mesh(m_device, vertices, indices, textures);
}

std::vector<Texture> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene) {
    std::vector<Texture> textures;
    for (UINT i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (UINT j = 0; j < texturesLoaded.size(); j++) {
            if (std::strcmp(texturesLoaded[j].path.c_str(), str.C_Str()) == 0) {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) { // If texture hasn't been loaded, load it 
            HRESULT result;

            Texture texture;

            const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
            if (embeddedTexture != nullptr) {
                texture.texture = loadEmbeddedTexture(embeddedTexture);
            }
            else {
                std::string filename = std::string(str.C_Str());
                filename = directory + "/" + filename;
                std::wstring filenamews = std::wstring(filename.begin(), filename.end());
                result = CreateWICTextureFromFile(m_device, m_deviceContext, filenamews.c_str(), nullptr, &texture.texture);
                if (FAILED(result)) {
                    MessageBox(m_hwnd, L"Texture could not be loaded", L"Error", MB_ICONERROR | MB_OK);
                }
            }
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            this->texturesLoaded.push_back(texture);
        }
    }

    return textures;
}

void ModelLoader::Close() {
    for (auto& t : texturesLoaded) {
        t.Release();
    }

    for (size_t i = 0; i < m_meshes.size(); i++) {
        m_meshes[i].Close();
    }
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene) {
    for (UINT i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(this->processMesh(mesh, scene));
    }

    for (UINT i = 0; i < node->mNumChildren; i++) {
        this->processNode(node, scene);
    }
}

ID3D11ShaderResourceView* ModelLoader::loadEmbeddedTexture(const aiTexture* embeddedTexture) {
    HRESULT result;
    ID3D11ShaderResourceView* texture = nullptr;

    if (embeddedTexture->mWidth != 0) {
        D3D11_TEXTURE2D_DESC textureDesc;
        textureDesc.Width = embeddedTexture->mWidth;
        textureDesc.Height = embeddedTexture->mHeight;
        textureDesc.ArraySize = 1;
        textureDesc.MipLevels = 1;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA textureData;
        textureData.pSysMem = embeddedTexture->pcData;
        textureData.SysMemPitch = embeddedTexture->mWidth * 4;
        textureData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

        ID3D11Texture2D* texture2D = nullptr;
        result = m_device->CreateTexture2D(&textureDesc, &textureData, &texture2D);
        if (FAILED(result)) {
            MessageBox(m_hwnd, L"CreateTexture2D failed", L"Error!", MB_ICONERROR | MB_OK);
        }

        if (texture2D) {
            result = m_device->CreateShaderResourceView(texture2D, 0, &texture);
            if (FAILED(result)) {
                MessageBox(m_hwnd, L"CreateShaderResourceView failed", L"Error!", MB_ICONERROR | MB_OK);
            }
        }

        return texture;
    }

    const size_t size = embeddedTexture->mWidth;

    result = CreateWICTextureFromMemory(m_device, m_deviceContext, reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);
    if (FAILED(result)) {
        MessageBox(m_hwnd, L"CreateWICTextureFromMemory failed", L"Error!", MB_ICONERROR | MB_OK);
    }

    return texture;
}