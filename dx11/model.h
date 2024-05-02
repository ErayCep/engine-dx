#pragma once

#ifndef _MODEL_CLASS_H_
#define _MODEL_CLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "texture.h"

using namespace DirectX;

class ModelClass {
public:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct VertexType2 {
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT3 normal;
	};

	struct ModelType {
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct ModelType2 {
		float x, y, z;
		float r, g, b, a;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11Buffer* GetVertexBuffer();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_texture;

	ModelType* m_model;
};

#endif