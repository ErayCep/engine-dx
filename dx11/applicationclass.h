#pragma once

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

#include <windows.h>

#include "d3dclass.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "texture_shader.h"
#include "light.h"
#include "light_shader.h"
#include "instance.h"
#include "sprite.h"
#include "timer.h"
#include "font.h"
#include "font_shader.h"
#include "text.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

	CameraClass* GetCamera();

private:
	bool Render(float);

private:
	D3DClass* m_Direct3D;
	ModelClass* m_model;
	CameraClass* m_camera;
	TextureShader* m_textureShader;
	Instance* m_instance;
	LightShader* m_lightShader;
	LightClass* m_lights;
	Sprite* m_sprite;
	Timer* m_timer;
	int m_numLights;
	FontShader* m_FontShader;
	Font* m_Font;
	Text* m_TextString1, * m_TextString2;
};

#endif
