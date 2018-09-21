#include "PlaneModelClass.h"


PlaneModelClass::PlaneModelClass(void)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

PlaneModelClass::PlaneModelClass(const PlaneModelClass& others)
{

}

PlaneModelClass::~PlaneModelClass(void)
{
}

bool PlaneModelClass::Initialize(ID3D11Device* device, int m, int n, float dx)
{
	bool result;


	// ��ʼ�����㻺��Ͷ�����������.
	result = InitializeBuffers(device, m, n, dx);
	if (!result)
	{
		return false;
	}

	return true;
}

void PlaneModelClass::Shutdown()
{
	// �ͷŶ������������.
	ShutdownBuffers();

	return;
}

void PlaneModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// �Ѷ���������������ͼ�ι��ߣ�׼����Ⱦ.
	RenderBuffers(deviceContext);

	return;
}

int PlaneModelClass::GetIndexCount()
{
	//���������������
	return m_indexCount;
}

bool PlaneModelClass::InitializeBuffers(ID3D11Device* device, int m, int n, float dx)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//����õ����������������Ŀ
		//���ȵõ������ε���Ŀ��Ȼ�����3���Ƕ���������Ŀ
	m_vertexCount = m * n;
	m_indexCount = (m - 1)*(n - 1) * 2 * 3; 

	// ����������ʱ����.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	float halfWidth = (n - 1)*dx*0.5f;
	float halfDepth = (m - 1)*dx*0.5f;

	for (int i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dx;
		for (int j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;

			float y = -3.0;

			vertices[i*n + j].position = XMFLOAT3(x, y, z);
			vertices[i*n + j].normal = XMFLOAT3(0.0, 1.0f, 0.0);
			vertices[i*n + j].Kd = XMFLOAT4(0.0, 1.0, 0.0, 1.0);;
			vertices[i*n + j].Ks = XMFLOAT4(0.2, 0.2, 0.2, 1.0);

		}
	}

	// ������������.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// ����ÿ��grid������ó�����.
	int k = 0;
	for (int i = 0; i < m - 1; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[k + 1] = i * n + j + 1;
			indices[k + 2] = (i + 1)*n + j;

			indices[k + 3] = (i + 1)*n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1)*n + j + 1;

			k += 6; //��һ��grid
		}
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

void PlaneModelClass::ShutdownBuffers()
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

void PlaneModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

