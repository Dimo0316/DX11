#pragma once
#ifndef _COMMON_H_
#define _COMMON_H_
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#if defined(DEBUG) || defined(_DEBUG) 
#ifndef D3D_DEBUG_INFO 
#define D3D_DEBUG_INFO 
#endif 
#endif

#if defined(DEBUG) || defined(_DEBUG) 
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h> 
#endif

#include "dxerr.h" 
#include <cassert>
#include<atlconv.h>
//һ���򵥵Ĵ������
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                                \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			USES_CONVERSION;                                  \
			DXTrace(A2W(__FILE__), (DWORD)__LINE__, hr, (L#x), true); \
		}                                                     \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

//����һЩ������ɫ 
const XMFLOAT4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const XMFLOAT4 BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const XMFLOAT4 RED(1.0f, 0.0f, 0.0f, 1.0f);
const XMFLOAT4 GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const XMFLOAT4 BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const XMFLOAT4 YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const XMFLOAT4 CYAN(0.0f, 1.0f, 1.0f, 1.0f); //����ɫ 
const XMFLOAT4 MAGENTA(1.0f, 0.0f, 1.0f, 1.0f); //���ɫ

const XMFLOAT4 BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const XMFLOAT4 LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const XMFLOAT4 DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const XMFLOAT4 DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);

const bool INITIALIZE_SUCCESE = true;
const bool INITIALIZE_FAILED = false;

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float PI = 3.14159265358979323f;
#define NUM_LIGHTS 2
struct  LightBufferType
{
	XMFLOAT4 lightPosition[NUM_LIGHTS]; //��Դλ��
	XMFLOAT4 lightColor[NUM_LIGHTS];   //��Դ��ɫ
	XMFLOAT4 globalAmbient[NUM_LIGHTS]; //��Դ�Ļ����ⷴ��ϵ��
	XMFLOAT4 attenuation[NUM_LIGHTS]; //˥��ϵ����x,y,z��Ӧ���������ԺͶ���ϵ��
	XMFLOAT4 spotlight[NUM_LIGHTS];//����spotlight��x��y�ֱ�洢�ں����cosֵ
	XMFLOAT3 lightDirection[NUM_LIGHTS]; //ƽ�йⷽ��
	float shininess[NUM_LIGHTS]; //�߹�ָ��

};

struct  MaterialBufferType
{
	XMFLOAT4 Ke;  //���ʵ��Է���
	XMFLOAT4 Ka;  //���ʵĻ�����ϵ��
};

struct CameraBufferType
{
	XMFLOAT4 cameraPosition; //�������λ��
};

#endif // !_COMMON_H_