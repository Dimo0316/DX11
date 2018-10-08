#pragma once
#include <d3d11.h>
#include "common.h"

class PlaneModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT3 normal; //����
		XMFLOAT2 texture;//��������
		XMFLOAT4 Kd;  //����������ϵ��
		XMFLOAT4 Ks;  //���ʵĸ߹�ϵ��
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
	//���㻺��Ͷ�����������
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

