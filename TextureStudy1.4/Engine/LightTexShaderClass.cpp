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

	// ��ʼ��vs��ps.
	result = InitializeShader(device, hwnd, (WCHAR*)L"lighttex.vs", (WCHAR*)L"lighttex.ps");
	if(!result)
		{
		return false;
		}

	return true;
	}

void LightTexShaderClass::Shutdown()
	{
	// �ͷ�shader��Դ.
	ShutdownShader();

	return;
	}

bool LightTexShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, 
	XMMATRIX projectionMatrix, LightBufferType light, MaterialBufferType material, CameraBufferType camera,ID3D11ShaderResourceView* texture)
	{
	bool result;


	// ����shader����.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, light, material, camera,texture);
	if(!result)
		{
		HR(result);
		return false;
		}

	// ��ָ��shader��Ⱦ���㻺��.
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


	// ��ʼ��ָ��Ϊ��.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// ����vs����.
	//����D3D10_SHADER_DEBUG����Ϊ����perfstuido�в鿴hlsl����
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS|D3D10_SHADER_DEBUG, 0, 
		&vertexShaderBuffer, &errorMessage);
	if(FAILED(result))
		{
		HR(result);
		// ���vs����ʧ�ܣ����������Ϣ
		if(errorMessage)
			{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
			}
		// ���û���κδ�����Ϣ��������shader�ļ���ʧ.
		else
			{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
			}

		return false;
		}

	//  ����ps����.

	result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS|D3D10_SHADER_DEBUG, 0, 
		&pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
		{
		// ���ps����ʧ�ܣ����������Ϣ.
		HR(result);
		if(errorMessage)
			{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
			}
		// ���û���κδ�����Ϣ��������shader�ļ���ʧ.
		else
			{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
			}

		return false;
		}

	// �ӻ��崴��vs shader.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
		{
		HR(result);
		return false;
		}

	// �ӻ����д���pvs shader.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
		{
		HR(result);
		return false;
		}

	// �������ݲ��֣��Ա���shader��ʹ��.
	// ����Ҫ��ModelClass�еĶ���ṹһ��.
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
	// �õ�layout�е�Ԫ������
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// �����������벼��.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 
		&m_layout);
	if(FAILED(result))
		{
		HR(result);
		return false;
		}

	//�ͷŶ�������ػ���.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	// ���ö�̬��������.
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

	// ����const bufferָ�룬�Ա����shader����.
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
		
			// �����������������
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

	// �����������״̬.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if(FAILED(result))
		{
		return false;
		}

	return true;
	}

void LightTexShaderClass::ShutdownShader()
	{

	// �ͷŲ���״̬
	if(m_sampleState)
		{
		m_sampleState->Release();
		m_sampleState = 0;
		}
	// �ͷŹ��ղ��ʻ���.
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
	//�ͷ����������
	if( m_cameraBuffer)
		{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
		}

	// �ͷų�������
	if(m_matrixBuffer)
		{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
		}

	//�ͷŶ��㲼��.
	if(m_layout)
		{
		m_layout->Release();
		m_layout = 0;
		}

	// �ͷ�ps .
	if(m_pixelShader)
		{
		m_pixelShader->Release();
		m_pixelShader = 0;
		}

	// �ͷ�vs
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


	// �õ�������Ϣbufferָ��.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// �õ�buffer�ĳ���.
	bufferSize = errorMessage->GetBufferSize();

	// ��һ���ļ�д������Ϣ.
	fout.open("shader-error.txt");

	// �����������.
	for(i=0; i<bufferSize; i++)
		{
		fout << compileErrors[i];
		}

	// �ر��ļ�
	fout.close();

	// �ͷŴ�����Ϣ.
	errorMessage->Release();
	errorMessage = 0;

	//����������ʾ.
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

	// ����shaderǰ��ȷ������ת�ã�����D3D11��Ҫ��.
	//XMMATRIXTranspose(&worldMatrix, &worldMatrix);
	//XMMATRIXTranspose(&viewMatrix, &viewMatrix);
	//XMMATRIXTranspose(&projectionMatrix, &projectionMatrix);
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	//  �����������壬�Ա��ܹ�д��.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		{
		return false;
		}

	// �õ�const bufferָ��.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ����world,view�Լ�projection����.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// ������������.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// ���ó�������λ��.
	bufferNumber = 0;

	// �ø��º��ֵ���ó�������.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);


	// �������ղ��ʻ���.
		result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		{
		return false;
		}

	//�õ����ճ�����������ָ��
	dataPtr2 = (LightBufferType*)mappedResource.pData;
	
	
	for(i =0 ; i < NUM_LIGHTS; i++)
		{
		// �������帳ֵ.
		dataPtr2->lightPosition[i] = light.lightPosition[i] ;
		dataPtr2->lightColor[i] = light.lightColor[i];
		dataPtr2->globalAmbient[i] = light.globalAmbient[i];
		dataPtr2->lightDirection[i] = light.lightDirection[i];
		dataPtr2->shininess[i] = light.shininess[i];
		dataPtr2->attenuation[i] = light.attenuation[i];
		dataPtr2->spotlight[i] = light.spotlight[i];

		}

	// ������������
	deviceContext->Unmap(m_lightBuffer, 0);

	// ���û�������Ϊ0����Ϊ����ps�еĵ�һ����������
	bufferNumber =0;

	// ���ù��ճ�������.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	// �������ʻ���.
	result = deviceContext->Map(m_materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		{
		return false;
		}
	//�õ�������������ָ��
	dataPtr3 = (MaterialBufferType*)mappedResource.pData;

	dataPtr3->Ka = material.Ka;
	dataPtr3->Ke = material.Ke;

	// ������������
	deviceContext->Unmap(m_materialBuffer, 0);
	// ���û�������Ϊ1����Ϊ����ps�еĵڶ�����������
	bufferNumber =1;

	// ���ù��ղ��ʳ�������.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_materialBuffer);

	// �������������.
	result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		{
		return false;
		}
	//�õ�������������ָ��
	dataPtr4 = (CameraBufferType*)mappedResource.pData;

	dataPtr4->cameraPosition 	= camera.cameraPosition;

	// ������������
	deviceContext->Unmap(m_cameraBuffer, 0);
	// ���û�������Ϊ2����Ϊ����ps�еĵ�������������
	bufferNumber = 2;

	// ���ù��ղ��ʳ�������.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
	
		// ����ps shader��Դ.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	return true;
	}

void LightTexShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
	{
	// �󶨶��㲼��.
	deviceContext->IASetInputLayout(m_layout);

	// ������Ⱦʹ��vs��ps.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	//���ò���״̬
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	// ��Ⱦ������
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
	}
