////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"

CameraClass::CameraClass()
{
	_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	_look = XMFLOAT3(0.0f, 0.0f, 1.0f);
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	_pos = XMFLOAT3(x, y, z);
}

void CameraClass::SetPosition(const XMFLOAT3& v)
{
	_pos = v;
}

void CameraClass::GetPosition(XMFLOAT3* position)
{
	*position = _pos;
}

XMFLOAT3 CameraClass::GetUp()const {
	return _up;
}

XMFLOAT3 CameraClass::GetLook()const {
	return _look;
}

XMFLOAT3 CameraClass::GetRight()const {
	return _right;
}

void CameraClass::Reset() {
	_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	_look = XMFLOAT3(0.0f, 0.0f, 1.0f);
}

//���ߣ�����������۲췽���ƶ�
void CameraClass::Walk(float units) {
	//_pos += _look * units;
	XMVECTOR s = XMVectorReplicate(units);
	XMVECTOR l = XMLoadFloat3(&_look);
	XMVECTOR p = XMLoadFloat3(&_pos);

	//XMVectorMultiplyAdd(v1,v2,v3)=v1*v2+v3
	XMStoreFloat3(&_pos, XMVectorMultiplyAdd(s, l, p));
	
}

//ɨ�ӣ�ָ���ַ��򲻱䣬��������right�ķ����һ��ƽ�Ƶ���һ��
void CameraClass::Strafe(float units) {
	//_pos += _right * units;
	XMVECTOR s = XMVectorReplicate(units);
	XMVECTOR r = XMLoadFloat3(&_right);
	XMVECTOR p = XMLoadFloat3(&_pos);

	//XMVectorMultiplyAdd(v1,v2,v3)=v1*v2+v3
	XMStoreFloat3(&_pos, XMVectorMultiplyAdd(s, r, p));
}

//����ģʽ��������ָ��������up������ƶ� 
void CameraClass::Fly(float units)
{
	//_pos += _up * units;
	XMFLOAT3 _up2 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMVECTOR s = XMVectorReplicate(units);
	XMVECTOR u = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR p = XMLoadFloat3(&_pos);
	
	//XMVectorMultiplyAdd(v1,v2,v3)=v1*v2+v3
	XMStoreFloat3(&_pos, XMVectorMultiplyAdd(s, u, p));
}

//Look��Up�������������ϵ��right����������ת����ʱlook��up�����ı䣬��right��poisiton���ı�
void CameraClass::Pitch(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&_right), angle);
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), R));
	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), R));
}

void CameraClass::Yaw(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&_up), angle);
	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), R));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), R));
}

void CameraClass::Roll(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&_look), angle);
	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), R));
	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), R));
}

//void CameraClass::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp) {
//	XMVECTOR Look = XMVectorSubtract(target, pos);
//	XMVECTOR Up = worldUp;
//	XMVECTOR Right = XMVector3Cross(Up, Look);
//	Up = XMVector3Cross(Look, Right);
//
//	//�����������
//	XMVector3Normalize(Look);
//	XMVector3Normalize(Up);
//	XMVector3Normalize(Right);
//
//	//����������Camera����
//	XMStoreFloat3(&_pos, pos);
//	XMStoreFloat3(&_look, Look);
//	XMStoreFloat3(&_up, Up);
//	XMStoreFloat3(&_right, Right);
//}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix )
{
	XMVECTOR R = XMLoadFloat3(&_right);
	XMVECTOR U = XMLoadFloat3(&_up);
	XMVECTOR L = XMLoadFloat3(&_look);
	XMVECTOR P = XMLoadFloat3(&_pos);

	L = XMVector3Normalize(L);

	//U=look(X)right
	U = XMVector3Normalize(XMVector3Cross(L, R));

	//R=up(X)look
	R = XMVector3Normalize(XMVector3Cross(U, L));

	//�������任����ĳЩ��������ĵ��
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMFLOAT4X4 mViewMatrix;
	XMStoreFloat4x4(&mViewMatrix, viewMatrix);
	//��һ��
	mViewMatrix(0, 0) = _right.x;
	mViewMatrix(1, 0) = _right.y;
	mViewMatrix(2, 0) = _right.z;
	mViewMatrix(3, 0) = x;

	//�ڶ���
	mViewMatrix(0, 1) = _up.x;
	mViewMatrix(1, 1) = _up.y;
	mViewMatrix(2, 1) = _up.z;
	mViewMatrix(3, 1) = y;

	//������
	mViewMatrix(0, 2) = _look.x;
	mViewMatrix(1, 2) = _look.y;
	mViewMatrix(2, 2) = _look.z;
	mViewMatrix(3, 2) = z;

	//������
	mViewMatrix(0, 3) = 0;
	mViewMatrix(1, 3) = 0;
	mViewMatrix(2, 3) = 0;
	mViewMatrix(3, 3) = 1.0f;

	viewMatrix = XMLoadFloat4x4(&mViewMatrix);
}