#include "model.h"

ModelClass::ModelClass() {
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_vertexCount = 0;
	m_indexCount = 0;
	m_texture = 0;
	m_model = 0;
}

ModelClass::ModelClass(const ModelClass& other) {}

ModelClass::~ModelClass() {}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, char* modelFilename) {
	bool result;

	result = LoadModel(modelFilename);
	if (!result) {
		return false;
	}
	
	result = InitializeBuffers(device);
	if (!result) {
		return false;
	}

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result) {
		return false;
		}

	return true;
}

void ModelClass::Shutdown() {
	ReleaseTexture();

	ShutdownBuffers();

	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext) {
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount() {
	return m_indexCount;
}

ID3D11Buffer* ModelClass::GetVertexBuffer() {
	return m_vertexBuffer;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device) {
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new VertexType[m_vertexCount];
	indices = new unsigned long[m_indexCount];

	for (int i = 0; i < m_vertexCount; i++) {
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)) {
		std::cout << "Failed to create vertex buffer" << std::endl;
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)) {
		std::cout << "Failed to create index buffer" << std::endl;
		return false;
	}

	delete [] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers() {
	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	if (m_indexBuffer) {
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	// Create and initialize the texture object.
	m_texture = new TextureClass;

	result = m_texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(char* modelFilename) {
	std::ifstream fin(modelFilename);
	char input;
	int i;

	if (!fin)
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void ModelClass::ReleaseModel() {
	if (m_model) {
		delete[] m_model;
		m_model = 0;
	}

	return;
}