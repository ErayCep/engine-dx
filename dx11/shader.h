#pragma once

#ifndef _SHADER_CLASS_H_
#define _SHADER_CLASS_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include <iostream>

using namespace DirectX;
using namespace std;

class ShaderClass {
public:
	ShaderClass();
	ShaderClass(const ShaderClass&);
	~ShaderClass();

	struct MatrixBuffer {
		XMMATRIX model;
		XMMATRIX view;
		XMMATRIX projection;
	};

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_inputLayout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif