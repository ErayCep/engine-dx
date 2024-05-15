#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_model = 0;
	m_camera = 0;
	m_textureShader = 0;
	m_instance = 0;
	m_lightShader = 0;
	m_lights = 0;
	m_sprite = 0;
	m_timer = 0;
	m_FontShader = 0;
	m_Font = 0;
	m_TextString1 = 0;
	m_TextString2 = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	char testString1[32], testString2[32];
	char textureFilename[128];
	char modelFilename[128];
	char spriteFilename[128];

	// Create and initialize the Direct3D object.
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_model = new ModelClass;
	strcpy_s(textureFilename, "C:\\Users\\cepni\\source\\repos\\dx11\\assets\\stone01.tga");
	
	strcpy_s(modelFilename, "C:\\Users\\cepni\\source\\repos\\dx11\\assets\\models\\sphere.txt");

	result = m_model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), textureFilename, modelFilename);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the model class.", L"Error", MB_OK);
		return false;
	}

	strcpy_s(spriteFilename, "C:\\Users\\cepni\\source\\repos\\dx11\\assets\\sprite_data_01.txt");

	m_sprite = new Sprite;

	result = m_sprite->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, spriteFilename, 50, 50);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the sprite class.", L"Error", MB_OK);
		return false;
	}

	m_timer = new Timer;
	result = m_timer->Initialize();
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the timer class.", L"Error", MB_OK);
		return false;
	}

	m_instance = new Instance;
	result = m_instance->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), textureFilename, modelFilename);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the instance class.", L"Error", MB_OK);
		return false;
	}

	m_lightShader = new LightShader;

	result = m_lightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the light shader class.", L"Error", MB_OK);
		return false;
	}



	m_numLights = 4;

	m_lights = new LightClass[m_numLights];

	//lManuslly set the color and position of each light.
	m_lights[0].SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);  // Red
	m_lights[0].SetPosition(-3.0f, 1.0f, 3.0f);

	m_lights[1].SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);  // Green
	m_lights[1].SetPosition(3.0f, 1.0f, 3.0f);

	m_lights[2].SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);  // Blue
	m_lights[2].SetPosition(-3.0f, 1.0f, -3.0f);

	m_lights[3].SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);  // White
	m_lights[3].SetPosition(3.0f, 1.0f, -3.0f);

	// Create and initialize the texture shader object.
	m_textureShader = new TextureShader;

	result = m_textureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_camera = new CameraClass;
	m_camera->SetPosition(0.0f, 0.0f, -5.0f);

	m_FontShader = new FontShader;

	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	m_Font = new Font;

	result = m_Font->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 0);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font class class.", L"Error", MB_OK);
		return false;
	}

	strcpy_s(testString1, "Hello");
	strcpy_s(testString2, "Goodbye");

	m_TextString1 = new Text;

	result = m_TextString1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, testString1, 10, 10, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text string 1 class.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the second text object.
	m_TextString2 = new Text;

	result = m_TextString2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, testString2, 10, 50, 1.0f, 1.0f, 0.0f);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text string 2 class.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void ApplicationClass::Shutdown()
{
	if (m_TextString2)
	{
		m_TextString2->Shutdown();
		delete m_TextString2;
		m_TextString2 = 0;
	}

	if (m_TextString1)
	{
		m_TextString1->Shutdown();
		delete m_TextString1;
		m_TextString1 = 0;
	}

	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	if (m_timer) {
		delete m_timer;
		m_timer = 0;
	}

	if (m_sprite) {
		m_sprite->Shutdown();
		delete m_sprite;
		m_sprite = 0;
	}

	if (m_instance) {
		m_instance->Shutdown();
		m_instance = 0;
	}

	if (m_lights) {
		delete [] m_lights;
		m_lights = 0;
	}

	if (m_lightShader) {
		m_lightShader->Shutdown();
		delete m_lightShader;
		m_lightShader = 0;
	}

	if (m_model) {
		m_model->Shutdown();
		delete m_model;
		m_model = 0;
	}

	if (m_textureShader)
	{
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}

	if (m_camera) {
		delete m_camera;
		m_camera = 0;
	}

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result;
	float frameTime;
	static float rotation = 0.0f;

	m_timer->Frame();

	frameTime = m_timer->GetTime();

	m_sprite->Update(frameTime);

	rotation -= 0.0174532925f * 0.1f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	rotation = 0.0f;

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}


bool ApplicationClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projMatrix, rotateMatrix, translateMatrix, scaleMatrix, srMatrix, orthoMatrix;
	XMFLOAT4 diffuseColor[4], lightPosition[4];
	bool result;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	for (int i = 0; i < m_numLights; i++)
	{
		// Create the diffuse color array from the four light colors.
		diffuseColor[i] = m_lights[i].GetDiffuseColor();

		// Create the light position array from the four light positions.
		lightPosition[i] = m_lights[i].GetPosition();
	}

	rotateMatrix = XMMatrixRotationY(rotation);  // Build the rotation matrix.
	translateMatrix = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);  // Build the translation matrix.

	// Multiply them together to create the final world transformation matrix.
	worldMatrix = XMMatrixMultiply(rotateMatrix, translateMatrix);

	m_model->Render(m_Direct3D->GetDeviceContext());

	result = m_lightShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projMatrix, m_model->GetTexture(), diffuseColor, lightPosition);
	if (!result) {
		return false;
	}

	scaleMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);  // Build the scaling matrix.
	rotateMatrix = XMMatrixRotationY(rotation);  // Build the rotation matrix.
	translateMatrix = XMMatrixTranslation(2.0f, 0.0f, 0.0f);  // Build the translation matrix.

	// Multiply the scale, rotation, and translation matrices together to create the final world transformation matrix.
	srMatrix = XMMatrixMultiply(scaleMatrix, rotateMatrix);
	worldMatrix = XMMatrixMultiply(srMatrix, translateMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_model->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_lightShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projMatrix, m_model->GetTexture(),
		diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}
	
	result = m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projMatrix, m_model->GetTexture());
	if (!result)
	{
		return false;
	}

	result = m_instance->Render(m_Direct3D->GetDeviceContext());
	if (!result) {
		return false;
	}

	result = m_textureShader->RenderInstance(m_Direct3D->GetDeviceContext(), m_instance->GetModel()->GetIndexCount(), m_instance->GetInstanceCount(), worldMatrix, viewMatrix, projMatrix, m_instance->GetModel()->GetTexture());
	if (!result)
	{
		return false;
	}

	m_Direct3D->TurnZBufferOff();

	result = m_sprite->Render(m_Direct3D->GetDeviceContext());
	if (!result) {
		return false;
	}

	result = m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_sprite->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_sprite->GetTexture());
	if (!result) {
		return false;
	}

	m_Direct3D->TurnZBufferOn();

	m_Direct3D->TurnZBufferOff();
	m_Direct3D->EnableAlphaBlending();

	m_TextString1->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_TextString1->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_TextString1->GetPixelColor());
	if (!result)
	{
		return false;
	}

	m_TextString2->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_TextString2->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_TextString2->GetPixelColor());
	if (!result)
	{
		return false;
	}

	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

CameraClass* ApplicationClass::GetCamera() {
	return m_camera;
}