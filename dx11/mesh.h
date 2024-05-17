#pragma once

#ifndef _MESH_H_
#define _MESH_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <d3d11_1.h>
#include <DirectXMath.h>

#include "safe_release.h"

using namespace DirectX;

struct Vertex {
	FLOAT x, y, z;
	XMFLOAT2 texcoord;
};

struct Texture {
	std::string type;
	std::string path;
	ID3D11ShaderResourceView* texture;

	void Release() {
		texture->Release();
	}
};

class Mesh {
public:
	std::vector<Vertex> m_vertices;
	std::vector<UINT> m_indices;
	std::vector<Texture> m_textures;
	ID3D11Device* m_device;

	Mesh(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices, const std::vector<Texture>& textures) : m_device(device), m_vertices(vertices), m_indices(indices), m_textures(textures), vertexBuffer(nullptr), indexBuffer(nullptr) {
		this->setupMesh(device);
	}

	void Draw(ID3D11DeviceContext* deviceContext) {
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		deviceContext->PSSetShaderResources(0, 1, &m_textures[0].texture);

		deviceContext->DrawIndexed(static_cast<UINT>(m_indices.size()), 0, 0);
	}

	void Close() {
		SafeRelease(vertexBuffer);
		SafeRelease(indexBuffer);
	}

private:
	ID3D11Buffer* vertexBuffer, * indexBuffer;

	void setupMesh(ID3D11Device* device) {
		HRESULT result;

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = static_cast<UINT>(sizeof(Vertex) * m_vertices.size());
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = &m_vertices[0];

		result = device->CreateBuffer(&vbd, &vertexData, &vertexBuffer);
		if (FAILED(result)) {
			throw std::runtime_error("Failed to create model vertex buffer");
		}

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = static_cast<UINT>(sizeof(UINT) * m_indices.size());
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = &m_indices[0];

		result = device->CreateBuffer(&ibd, &indexData, &indexBuffer);
		if (FAILED(result)) {
			throw std::runtime_error("Failed to create model index buffer");
		}
	}
};

#endif