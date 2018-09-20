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


	// 初始化顶点缓冲和顶点索引缓冲.
	result = InitializeBuffers(device, m, n, dx);
	if (!result)
	{
		return false;
	}

	return true;
}

void PlaneModelClass::Shutdown()
{
	// 释放顶点和索引缓冲.
	ShutdownBuffers();

	return;
}

void PlaneModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// 把顶点和索引缓冲放入图形管线，准备渲染.
	RenderBuffers(deviceContext);

	return;
}

int PlaneModelClass::GetIndexCount()
{
	//返回索引顶点计数
	return m_indexCount;
}

bool PlaneModelClass::InitializeBuffers(ID3D11Device* device, int m, int n, float dx)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//计算得到顶点和索引顶点数目
		//首先得到三角形的数目，然后乘以3就是顶点索引数目
	m_vertexCount = m * n;
	m_indexCount = (m - 1)*(n - 1) * 2 * 3; 

	// 创建顶点临时缓冲.
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

	// 创建索引缓冲.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 迭代每个grid，计算得出索引.
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

			k += 6; //下一个grid
		}
	}


	// 设置顶点缓冲描述
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 指向保存顶点数据的临时缓冲.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 创建顶点缓冲.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// 设置索引缓冲描述.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 指向存临时索引缓冲.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 创建索引缓冲.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// 释放临时缓冲.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void PlaneModelClass::ShutdownBuffers()
{
	// 释放顶点缓冲.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 释放索引缓冲
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


	// 设置顶点缓冲跨度和偏移.
	stride = sizeof(VertexType);
	offset = 0;

	//在input assemberl阶段绑定顶点缓冲，以便能够被渲染
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//在input assemberl阶段绑定索引缓冲，以便能够被渲染
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 设置体元语义，渲染三角形列表.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

