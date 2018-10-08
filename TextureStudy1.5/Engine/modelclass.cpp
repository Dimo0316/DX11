////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
} 


bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

float ModelClass::getHeight(float x,float z) const{
	return 0.3f*(z*sinf(0.1f*x) + x * cosf(0.1f*z));
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 8;
	//m_vertexCount = m*n;

	// Set the number of indices in the index array.
	m_indexCount = 36;
	//m_indexCount = (m-1)*(n-1)*2*3;

	// Create the vertex array.创建临时缓冲
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}
	//float halfWidth = (n - 1)*dx*0.5f;
	//float halfDepth = (m - 1)*dx*0.5f;

	//for (int i = 0; i < m; ++i)
	//{
	//	float z = halfDepth - i * dx;
	//	for (int j = 0; j < n; ++j)
	//	{
	//		float x = -halfWidth + j * dx;

	//		// 计算得到z值. 
	//		float y = getHeight(x, z);

	//		vertices[i*n + j].position = XMFLOAT3(x, y, z);

	//		// 根据高度来定义颜色 
	//		if (y < -10.0f)
	//			vertices[i*n + j].color = BEACH_SAND;
	//		else if (y < 5.0f)
	//			vertices[i*n + j].color = LIGHT_YELLOW_GREEN;
	//		else if (y < 12.0f)
	//			vertices[i*n + j].color = DARK_YELLOW_GREEN;
	//		else if (y < 20.0f)
	//			vertices[i*n + j].color = DARKBROWN;
	//		else
	//			vertices[i*n + j].color = WHITE;
	//	}
	//}
	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}
	// 迭代每个grid，计算得出索引. 
	//int k = 0;
	//for (int i = 0; i < m - 1; ++i)
	//{
	//	for (int j = 0; j < n - 1; ++j)
	//	{
	//		indices[k] = i * n + j;
	//		indices[k + 1] = i * n + j + 1;
	//		indices[k + 2] = (i + 1)*n + j;

	//		indices[k + 3] = (i + 1)*n + j;
	//		indices[k + 4] = i * n + j + 1;
	//		indices[k + 5] = (i + 1)*n + j + 1;

	//		k += 6; //下一个grid 
	//	}
	//}
	//创建顺时针方向的三角形，左手规则 
	// 设置顶点数据. 
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	vertices[0].color = WHITE;

	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	vertices[1].color = BLACK;

	vertices[2].position = XMFLOAT3(1.0f, 1.0f, -1.0f);
	vertices[2].color = RED;

	vertices[3].position = XMFLOAT3(1.0f, -1.0f, -1.0f);
	vertices[3].color = GREEN;

	vertices[4].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	vertices[4].color = BLUE;

	vertices[5].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	vertices[5].color = YELLOW;

	vertices[6].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vertices[6].color = CYAN;

	vertices[7].position = XMFLOAT3(1.0f, -1.0f, 1.0f);
	vertices[7].color = MAGENTA;

	// 设置索引缓冲数据. 
	indices[0] = 0;  // 前面 
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	indices[6] = 4;  // 后面 
	indices[7] = 6;
	indices[8] = 5;
	indices[9] = 4;
	indices[10] = 7;
	indices[11] = 6;

	indices[12] = 4;  // 左面 
	indices[13] = 5;
	indices[14] = 1;
	indices[15] = 4;
	indices[16] = 1;
	indices[17] = 0;

	indices[18] = 3;  //右面 
	indices[19] = 2;
	indices[20] = 6;
	indices[21] = 3;
	indices[22] = 6;
	indices[23] = 7;

	indices[24] = 1;  // 上面 
	indices[25] = 5;
	indices[26] = 6;
	indices[27] = 1;
	indices[28] = 6;
	indices[29] = 2;

	indices[30] = 4; // 下面 
	indices[31] = 0;
	indices[32] = 3;
	indices[33] = 4;
	indices[34] = 3;
	indices[35] = 7;



	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		HR(result);
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}