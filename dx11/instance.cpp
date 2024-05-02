#include "instance.h"

Instance::Instance() {
	m_instanceBuffer = 0;
	m_instanceCount = 0;
	m_model = 0;
}

Instance::Instance(const Instance& other) {
	this->m_instanceBuffer = other.m_instanceBuffer;
	this->m_instanceCount = other.m_instanceCount;
	this->m_model = other.m_model;
}

Instance::~Instance() {}

bool Instance::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, char* modelFilename) {
	bool result;

	m_model = new ModelClass;

	result = m_model->Initialize(device, deviceContext, textureFilename, modelFilename);
	if (!result) {
		std::cout << "Failed to initialize instance model" << std::endl;
		return false;
	}

	result = InitializeBuffers(device);
	if (!result) {
		std::cout << "Failed to initialize instance buffers" << std::endl;
		return false;
	}

	return true;
}

bool Instance::Render(ID3D11DeviceContext* deviceContext) {
	bool result;

	m_model->Render(deviceContext);

	result = RenderBuffers(deviceContext);
	if (!result) {
		std::cout << "Failed to render instance buffer" << std::endl;
		return false;
	}

	return true;
}

void Instance::Shutdown() {
	ShutdownBuffers();

	return;
}

bool Instance::InitializeBuffers(ID3D11Device* device) {
	HRESULT result;
	const int n = 5;
	m_instanceCount = 5 * 5 * 5;
	InstancedData* instancedData;
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;

	instancedData = new InstancedData[n * n * n];

	float width = 50.0f;
	float height = 50.0f;
	float depth = 50.0f;
	float x = -0.5f * width;
	float y = -0.5f * height;
	float z = -0.5f * depth;
	float dx = width / (n - 1);
	float dy = height / (n - 1);
	float dz = depth / (n - 1);

	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				instancedData[k * n * n + i * n + j].worldMatrix = XMFLOAT4X4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					x + j * dx, y + i * dy, z + k * dz, 1.0f);
			}
		}
	}

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(InstancedData) * m_instanceCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = instancedData;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&bufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result)) {
		std::cout << "Failed to create instanced buffer" << std::endl;
		return false;
	}

	return true;
}

bool Instance::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	UINT stride[2] = { sizeof(ModelClass::VertexType), sizeof(InstancedData) };
	UINT offset[2] = { 0, 0 };

	ID3D11Buffer* vertexBuffers[2] = { m_model->GetVertexBuffer(), m_instanceBuffer };
	deviceContext->IASetVertexBuffers(0, 2, vertexBuffers, stride, offset);
	
	return true;
}

void Instance::ShutdownBuffers() {
	if (m_instanceBuffer) {
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}
}

UINT Instance::GetInstanceCount() {
	return m_instanceCount;
}

ModelClass* Instance::GetModel() {
	return m_model;
}