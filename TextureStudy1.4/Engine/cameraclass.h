////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <directxmath.h>
using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	//XMFLOAT3 GetRotation();
	void SetPosition(float, float, float);
	//void SetRotation(float, float, float);
	void SetPosition(const XMFLOAT3&); 
	void GetPosition(XMFLOAT3*);

	XMFLOAT3 GetRight()const;
	XMFLOAT3 GetUp()const;
	XMFLOAT3 GetLook()const;
	void Reset();

	//��ȡ��������任����
	void GetViewMatrix(XMMATRIX&);

	//ͨ�����������ռ��λ�ã�Ŀ��㣬�Լ�����������������任����
	//void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);

	void Strafe(float units);//����
	void Fly(float units);//����
	void Walk(float units);//ǰ��

	void Pitch(float angle);//��תview����ϵright����
	void Yaw(float angle);//��up������ת
	void Roll(float angle);//��look������ת

private:
	XMMATRIX m_viewMatrix;
	XMFLOAT3 _right;
	XMFLOAT3 _up;
	XMFLOAT3 _look;
	XMFLOAT3 _pos;
};

#endif