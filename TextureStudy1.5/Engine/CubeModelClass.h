#pragma once
#include <d3d11.h>
#include "common.h"
#include <fstream>
using namespace std;
class CubeModelClass
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
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
public:
	CubeModelClass(void);
	CubeModelClass(const CubeModelClass &);
	~CubeModelClass(void);
	bool Initialize(ID3D11Device*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadModel(char*);
	void ReleaseModel();
	//���㻺��Ͷ�����������
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	ModelType* m_model;
};