#pragma once
#ifndef  _AXISMODELCLASS_H_
#define _AXISMODELCLASS_H_
#include "common.h"
class AxisModelClass {
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	AxisModelClass();
	AxisModelClass(const AxisModelClass&);
	~AxisModelClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	//¶¥µã»º³åºÍ¶¥µãË÷Òý»º³å
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

};

#endif // ! _AXISMODELCLASS_H_

