#pragma once

#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <iostream>

#include "model.h"

using namespace DirectX;

class Instance {
private:
	struct InstancedData{
		XMFLOAT4X4 worldMatrix;
	};

public:
	Instance();
	Instance(const Instance&);
	~Instance();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*,  char*, char*);
	bool Render(ID3D11DeviceContext*);
	void Shutdown();

	UINT GetInstanceCount();
	ModelClass* GetModel();

private:
	bool InitializeBuffers(ID3D11Device*);
	bool RenderBuffers(ID3D11DeviceContext*);
	void ShutdownBuffers();

private:
	ID3D11Buffer* m_instanceBuffer;
	UINT m_instanceCount;
	ModelClass* m_model;
};

#endif