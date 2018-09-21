////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_AxisModel = 0;
	m_ColorShader = 0;

	m_PlaneModel = 0;
	m_CubeModel = 0;

	//m_Light = 0;
	m_LightShader = 0;
	m_LightTexShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//Clear all objects
	Shutdown();

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	// 设置摄像机位置.
	m_Camera->SetPosition(8.0f, 3.0f, -20.0f);
	//m_Camera->SetRotation(0.0f, 0.0f, 0.0f);


	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//初始化坐标轴模型
	m_AxisModel = new AxisModelClass();
	if (!m_AxisModel) {
		return false;
	}
	result = m_AxisModel->Initialize(m_Direct3D->GetDevice());
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the axis model object.", L"Error", MB_OK);
		return false;
	}
	//创建平面模型对象
	m_PlaneModel = new PlaneModelClass();
	if (!m_PlaneModel) {
		return false;
	}
	result = m_PlaneModel->Initialize(m_Direct3D->GetDevice(),300,300,1.0);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the plane model object.", L"Error", MB_OK);
		return false;
	}
	// 创建cube模型对象
	m_CubeModel = new CubeModelClass;
	if (!m_CubeModel)
	{
		return false;
	}
	// 初始化坐标cube模型对象.
	result = m_CubeModel->Initialize(m_Direct3D->GetDevice(), (char*)"../Engine/cube.txt",(WCHAR*)L"../Engine/checkboard.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the cube model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// 创建光照shader类
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	//初始化光照shader对象
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}


	// 创建光照纹理shader类
	m_LightTexShader = new LightTexShaderClass;
	if (!m_LightTexShader)
	{
		return false;
	}

	//初始化光照纹理shader对象
	result = m_LightTexShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light texture shader object.", L"Error", MB_OK);
		return false;
	}
	return true;
}


void GraphicsClass::Shutdown()
{
	// 释放光照纹理shader对象.
	if (m_LightTexShader)
	{
		m_LightTexShader->Shutdown();
		delete m_LightTexShader;
		m_LightTexShader = 0;
	}

	// 释放光照材质shader对象.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
	// 释放坐标轴模型对象.
	if (m_AxisModel)
	{
		m_AxisModel->Shutdown();
		delete m_AxisModel;
		m_AxisModel = 0;
	}

	//释放plane模型对象
	if (m_PlaneModel)
	{
		m_PlaneModel->Shutdown();
		delete m_PlaneModel;
		m_PlaneModel = 0;
	}

	//释放cube模型对象
	if (m_CubeModel)
	{
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(float dt)
{
	bool result;


	// Render the graphics scene.
	result = Render(dt);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float dt)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, worldMatrix1;
	bool result;

	worldMatrix1 = XMMatrixIdentity();
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position.
	//m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);


	m_AxisModel->Render(m_Direct3D->GetDeviceContext());
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_AxisModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	//执行平移操作，得到最终的模型世界矩阵
	worldMatrix1 = XMMatrixTranslation(4.0, 0.0, 0.0);
	// Render the model using the color shader.
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix1, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	worldMatrix1 = XMMatrixTranslation(8.0, 0.0, 0.0);
	// Render the model using the color shader.
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix1, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	//得到摄像机位置和一些材质光照系数
	XMFLOAT3 camerapos;

	m_Camera->GetPosition(&camerapos);

	LightBufferType light;
	MaterialBufferType material;
	CameraBufferType camera;
	LightClass mLight[NUM_LIGHTS];
	static float angle = 0;
	angle += dt;
	if (angle > 6.183)
		angle = 0;

	float x, z;
	x = 6 * cos(angle);
	z = 6 * sin(angle);
	camera.cameraPosition = XMFLOAT4(camerapos.x, camerapos.y, camerapos.z, 1.0);
	material.Ka = XMFLOAT4(0.0, 0.0, 0.0, 1.0);
	material.Ke = XMFLOAT4(0.0, 0.0, 0.0, 1.0);

	mLight[0].SetLightPosition(x, 5.0, z, 1.0);
	mLight[1].SetLightPosition(-3.0, 4.0, -5.0, 1.0);
	mLight[1].SetLightSpotLight(0.3, 0.2, 0.0, 0.0);
	mLight[1].SetLightColor(0.5, 0.3, 0.2, 1.0);

	light.lightPosition[0] = mLight[0].GetPosition();
	light.lightColor[0] = mLight[0].GetLightColor();
	light.globalAmbient[0] = mLight[0].GetGlobalAmbient();
	light.attenuation[0] = mLight[0].GetLightAttenuation();
	light.lightDirection[0] = mLight[0].GetDirection();
	light.shininess[0] = mLight[0].GetShininess();
	light.spotlight[0] = mLight[0].GetSpotLight();

	light.lightPosition[1] = mLight[1].GetPosition();
	light.lightColor[1] = mLight[1].GetLightColor();
	light.globalAmbient[1] = mLight[1].GetGlobalAmbient();
	light.attenuation[1] = mLight[1].GetLightAttenuation();
	light.lightDirection[1] = mLight[1].GetDirection();
	light.shininess[1] = mLight[1].GetShininess();
	light.spotlight[1] = mLight[1].GetSpotLight();

	//把cube顶点和索引数据放入缓冲区，准备渲染
	m_CubeModel->Render(m_Direct3D->GetDeviceContext());
	//用light shader渲染
	result = m_LightTexShader->Render(m_Direct3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		light, material, camera, m_CubeModel->GetTexture());
	if (!result)
	{
		return false;
	}

	//把plane顶点和索引数据放入缓冲区，准备渲染
	m_PlaneModel->Render(m_Direct3D->GetDeviceContext());
	//用light shader渲染
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneModel->GetIndexCount(), worldMatrix1, viewMatrix, projectionMatrix,
		light, material, camera);
	if (!result)
	{
		return false;
	}

	//把framebuffer中的图像present到屏幕上.
	m_Direct3D->EndScene();

	return true;
}