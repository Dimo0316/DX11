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

	//获取基础相机变换矩阵
	void GetViewMatrix(XMMATRIX&);

	//通过相机在世界空间的位置，目标点，以及上向量来定义相机变换矩阵
	//void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);

	void Strafe(float units);//左右
	void Fly(float units);//上下
	void Walk(float units);//前后

	void Pitch(float angle);//旋转view坐标系right向量
	void Yaw(float angle);//绕up向量旋转
	void Roll(float angle);//绕look向量旋转

private:
	XMMATRIX m_viewMatrix;
	XMFLOAT3 _right;
	XMFLOAT3 _up;
	XMFLOAT3 _look;
	XMFLOAT3 _pos;
};

#endif