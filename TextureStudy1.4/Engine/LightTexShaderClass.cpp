#include "LightTexShaderClass.h"


LightTexShaderClass::LightTexShaderClass(void)
	{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_lightBuffer = 0;
	m_materialBuffer = 0;
	m_cameraBuffer = 0;

    m_sampleState = 0;

	}

LightTexShaderClass::LightTexShaderClass(const LightTexShaderClass & others)
	{

	}

LightTexShaderClass::~LightTexShaderClass(void)
	{
	}

bool LightTexShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
	{
	bool result;

	// 初始化vs和ps.
	result = InitializeShader(device, hwnd, (WCHAR*)L"lighttex.vs", (WCHAR*)L"lighttex.ps");
	if(!result)
		{
		return false;
		}

	return true;
	}

void LightTexShaderClass::Shutdown()
	{
	// 释放shader资源.
	ShutdownShader();

	return;
	}

bool LightTexShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, 
	XMMATRIX projectionMatrix, LightBufferType light, MaterialBufferType material, CameraBufferType camera,ID3D11ShaderResourceView* texture)
	{
	bool result;


	// 设置shader参数.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, light, material, camera,texture);
	if(!result)
		{
		HR(result);
		return false;
		}

	// 用指定shader渲染顶点缓冲.
	RenderShader(deviceContext, indexCount);

	return true;
	}

bool LightTexShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
	{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightmaterialBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;


	// 初始化指针为空.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// 编译vs代码.
	//增加D3D10_SHADER_DEBUG，是为了在perfstuido中查看hlsl代码
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS|D3D10_SHADER_DEBUG, 0, 
		&vertexShaderBuffer, &errorMessage);
	if(FAILED(result))
		{
		HR(result);
		// 如果vs编译失败，输出错误消息
		if(errorMessage)
			{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
			}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
			{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
			}

		return false;
		}

	//  编译ps代码.

	result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS|D3D10_SHADER_DEBUG, 0, 
		&pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
		{
		// 如果ps编译失败，输出错误信息.
		HR(result);
		if(errorMessage)
			{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
			}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
			{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
			}

		return false;
		}

	// 从缓冲创建vs shader.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
		{
		HR(result);
		return false;
		}

	// 从缓冲中创建pvs shader.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
		{
		HR(result);
		return false;
		}

	// 设置数据布局，以便在shader中使用.
	// 定义要和ModelClass中的顶点结构一致.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = 12;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = 24;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "DIFFUSE";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = 32;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "SPECULAR";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = 48;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;
	// 得到layout中的元素数量
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 创建顶点输入布局.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 
		&m_layout);
	if(FAILED(result))
		{
		HR(result);
		return false;
		}

	//释放顶点和像素缓冲.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	// 设置动态矩阵描述.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
		{
		HR(result);
		return false;
		}

	lightmaterialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightmaterialBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightmaterialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightmaterialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightmaterialBufferDesc.MiscFlags = 0;
	lightmaterialBufferDesc.StructureByteStride = 0;

	// 创建const buffer指针，以便访问shader常量.
	result = device->CreateBuffer(&lightmaterialBufferDesc, NULL, & m_lightBuffer);
	if(FAILED(result))
		{
		HR(result);
		return false;
		}

	lightmaterialBufferDesc.ByteWidth = sizeof(MaterialBufferType);
	result = device->CreateBuffer(&lightmaterialBufferDesc, NULL, & m_materialBuffer);
	if(FAILED(result))
		{
		HR(result);
		return false;
		}

	lightmaterialBufferDesc.ByteWidth = sizeof(CameraBufferType);
	result = device->CreateBuffer(&lightmaterialBufferDesc, NULL, & m_cameraBuffer);
	if(FAILED(result))
		{
		HR(result);
		return false;
		}
		
			// 创建纹理采样描述符
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1.0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1.0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// 创建纹理采样状态.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if(FAILED(result))
		{
		return false;
		}

	return true;
	}

void LightTexShaderClass::ShutdownShader()
	{

	// 释放采样状态
	if(m_sampleState)
		{
		m_sampleState->Release();
		m_sampleState = 0;
		}
	// 释放光照材质缓冲.
	if( m_lightBuffer)
		{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
		}
	if( m_materialBuffer)
		{
		m_materialBuffer->Release();
		m_materialBuffer = 0;
		}
	//释放摄像机缓冲
	if( m_cameraBuffer)
		{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
		}

	// 释放常量缓冲
	if(m_matrixBuffer)
		{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
		}

	//释放顶点布局.
	if(m_layout)
		{
		m_layout->Release();
		m_layout = 0;
		}

	// 释放ps .
	if(m_pixelShader)
		{
		m_pixelShader->Release();
		m_pixelShader = 0;
		}

	// 释放vs
	if(m_vertexShader)
		{
		m_vertexShader->Release();
		m_vertexShader = 0;
		}
	return;
	}

void LightTexShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
	{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// 得到错误信息buffer指针.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// 得到buffer的长度.
	bufferSize = errorMessage->GetBufferSize();

	// 打开一个文件写错误信息.
	fout.open("shader-error.txt");

	// 输出错误信心.
	for(i=0; i<bufferSize; i++)
		{
		fout << compileErrors[i];
		}

	// 关闭文件
	fout.close();

	// 释放错误消息.
	errorMessage->Release();
	errorMessage = 0;

	//弹出错误提示.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
	}

bool LightTexShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, 
	XMMATRIX projectionMatrix, LightBufferType light, MaterialBufferType material, CameraBufferType camera,ID3D11ShaderResourceView* texture)
	{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	MaterialBufferType* dataPtr3;
	CameraBufferType* dataPtr4;
	int i;

	// 传入shader前，确保矩阵转置，这是D3D11的要求.
	//XMMATRIXTranspose(&worldMatrix, &worldMatrix);
	//XMMATRIXTranspose(&viewMatrix, &viewMatrix);
	//XMMATRIXTranspose(&projectionMatrix, &projectionMatrix);
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	//  锁定常量缓冲，以便能够写入.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		{
		return false;
		}

	// 得到const buffer指针.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// 设置world,view以及projection矩阵.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// 解锁常量缓冲.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// 设置常量缓冲位置.
	bufferNumber = 0;

	// 用更新后的值设置常量缓冲.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);


	// 锁定光照材质缓冲.
		result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		{
		return false;
		}

	//得到光照常量缓冲数据指针
	dataPtr2 = (LightBufferType*)mappedResource.pData;
	
	
	for(i =0 ; i < NUM_LIGHTS; i++)
		{
		// 常量缓冲赋值.
		dataPtr2->lightPosition[i] = light.lightPosition[i] ;
		dataPtr2->lightColor[i] = light.lightColor[i];
		dataPtr2->globalAmbient[i] = light.globalAmbient[i];
		dataPtr2->lightDirection[i] = light.lightDirection[i];
		dataPtr2->shininess[i] = light.shininess[i];
		dataPtr2->attenuation[i] = light.attenuation[i];
		dataPtr2->spotlight[i] = light.spotlight[i];

		}

	// 解锁常量缓冲
	deviceContext->Unmap(m_lightBuffer, 0);

	// 设置缓冲索引为0，因为这是ps中的第一个常量缓冲
	bufferNumber =0;

	// 设置光照常量缓冲.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	// 锁定材质缓冲.
	result = deviceContext->Map(m_materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		{
		return false;
		}
	//得到常量缓冲数据指针
	dataPtr3 = (MaterialBufferType*)mappedResource.pData;

	dataPtr3->Ka = material.Ka;
	dataPtr3->Ke = material.Ke;

	// 解锁常量缓冲
	deviceContext->Unmap(m_materialBuffer, 0);
	// 设置缓冲索引为1，因为这是ps中的第二个常量缓冲
	bufferNumber =1;

	// 设置光照材质常量缓冲.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_materialBuffer);

	// 锁定摄像机缓冲.
	result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		{
		return false;
		}
	//得到常量缓冲数据指针
	dataPtr4 = (CameraBufferType*)mappedResource.pData;

	dataPtr4->cameraPosition 	= camera.cameraPosition;

	// 解锁常量缓冲
	deviceContext->Unmap(m_cameraBuffer, 0);
	// 设置缓冲索引为2，因为这是ps中的第三个常量缓冲
	bufferNumber = 2;

	// 设置光照材质常量缓冲.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
	
		// 设置ps shader资源.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	return true;
	}

void LightTexShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
	{
	// 绑定顶点布局.
	deviceContext->IASetInputLayout(m_layout);

	// 设置渲染使用vs和ps.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	//设置采样状态
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	// 渲染三角形
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
	}
