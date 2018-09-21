#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "common.h"
#include "LightClass.h"

using namespace std;

class LightTexShaderClass
	{
	private:
	//和shader中const buffer结构一致，主要用来给这些shader参数赋值。
	struct MatrixBufferType
		{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		};

	public:
		LightTexShaderClass(void);
		LightTexShaderClass(const LightTexShaderClass &);
		~LightTexShaderClass(void);
		bool Initialize(ID3D11Device*, HWND);
		void Shutdown();
		bool Render(ID3D11DeviceContext* , int , XMMATRIX , XMMATRIX , XMMATRIX ,  LightBufferType, MaterialBufferType, CameraBufferType,ID3D11ShaderResourceView*);
		bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
		void ShutdownShader();
		void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

		bool SetShaderParameters(ID3D11DeviceContext* , XMMATRIX , XMMATRIX , XMMATRIX, LightBufferType, MaterialBufferType, CameraBufferType,ID3D11ShaderResourceView*);
		void RenderShader(ID3D11DeviceContext*, int);

	private:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
		ID3D11Buffer* m_matrixBuffer;
		ID3D11Buffer* m_lightBuffer; //材质光照参数buffer
		ID3D11Buffer* m_materialBuffer;
		ID3D11Buffer* m_cameraBuffer; //摄像机buffer
		
				//采样状态
		ID3D11SamplerState* m_sampleState;
	};

