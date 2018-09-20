#include "CubeModelClass.h"


CubeModelClass::CubeModelClass(void)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_model = 0;

}

CubeModelClass::CubeModelClass(const CubeModelClass & others)
{
}
CubeModelClass::~CubeModelClass(void)
{
}

bool CubeModelClass::Initialize(ID3D11Device* device, char* modelFilename)
{
	bool result;

	// װ��ģ������,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}


	// ��ʼ�����㻺��Ͷ�����������.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void CubeModelClass::Shutdown()
{
	// �ͷŶ������������.
	ShutdownBuffers();
	// Release the model data.
	ReleaseModel();

	return;
}

void CubeModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// �Ѷ���������������ͼ�ι��ߣ�׼����Ⱦ.
	RenderBuffers(deviceContext);

	return;
}

int CubeModelClass::GetIndexCount()
{
	//���������������
	return m_indexCount;
}

bool CubeModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	//���ȣ����Ǵ���2����ʱ�����Ŷ�����������ݣ��Ա����ʹ�á�. 


	// ����������ʱ����.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}


	// ������������.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertices[i].Kd = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
		vertices[i].Ks = XMFLOAT4(0.2, 0.2, 0.2, 1.0);

		indices[i] = i;
	}


	// ���ö��㻺������
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ָ�򱣴涥�����ݵ���ʱ����.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// �������㻺��.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// ����������������.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ָ�����ʱ��������.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// ������������.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// �ͷ���ʱ����.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void CubeModelClass::ShutdownBuffers()
{
	// �ͷŶ��㻺��.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// �ͷ���������
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void CubeModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// ���ö��㻺���Ⱥ�ƫ��.
	stride = sizeof(VertexType);
	offset = 0;

	//��input assemberl�׶ΰ󶨶��㻺�壬�Ա��ܹ�����Ⱦ
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//��input assemberl�׶ΰ��������壬�Ա��ܹ�����Ⱦ
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ������Ԫ���壬��Ⱦ�������б�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool CubeModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	//��ģ���ļ�.
	fin.open(filename);

	// ���ܴ��ļ������˳�.
	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	//�õ���������.
	fin >> m_vertexCount;

	// ���ö��������Ͷ�������һ��.
	m_indexCount = m_vertexCount;

	// ����ģ��.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// �����������ݿ�ʼλ��.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// ��ȡ��������
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// �ر�ģ���ļ�
	fin.close();

	return true;
}

void CubeModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

