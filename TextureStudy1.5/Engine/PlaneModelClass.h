#pragma once
#include <d3d11.h>
#include "common.h"

class PlaneModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT3 normal; //法向
		XMFLOAT2 texture;//纹理坐标
		XMFLOAT4 Kd;  //材质漫反射系数
		XMFLOAT4 Ks;  //材质的高光系数
	};
public:
	PlaneModelClass(void);
	PlaneModelClass(const PlaneModelClass&);
	~PlaneModelClass(void);
	bool Initialize(ID3D11Device*, int, int, float);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
private:
	bool InitializeBuffers(ID3D11Device*, int, int, float);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	void ReleaseTexture();
	//顶点缓冲和顶点索引缓冲
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

