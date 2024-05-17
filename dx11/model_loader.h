#pragma once

#ifndef _MODEL_LOADER_H_
#define _MODEL_LOADER_H_

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

#include <assimp/Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "texture_loader.h"
#include "mesh.h"

using namespace DirectX;

class ModelLoader {
public:
	ModelLoader();
	~ModelLoader();

	bool Load(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filename);
	void Draw(ID3D11DeviceContext* deviceContext);

	void Close();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	std::vector<Mesh> m_meshes;
	std::string directory;
	std::vector<Texture> texturesLoaded;
	HWND m_hwnd;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	ID3D11ShaderResourceView* loadEmbeddedTexture(const aiTexture* embeddedTexture);
};

#endif