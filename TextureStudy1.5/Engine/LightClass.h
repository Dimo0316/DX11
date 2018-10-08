#pragma once
#include <d3d11.h>
#include "common.h"

class LightClass
{
public:
	LightClass(void);
	LightClass(const LightClass &);
	~LightClass(void);

	void SetLightPosition(float, float, float, float);
	void SetGlobalAmbient(float, float, float, float);
	void SetDirection(float, float, float);
	void SetLightColor(float, float, float, float);
	void SetLightAttenuation(float, float, float, float);
	void SetLightSpotLight(float, float, float, float);
	void SetShininess(float);

	XMFLOAT4 GetPosition();
	XMFLOAT4 GetGlobalAmbient();
	XMFLOAT3 GetDirection();
	XMFLOAT4 GetLightColor();
	XMFLOAT4 GetSpotLight();
	XMFLOAT4 GetLightAttenuation();
	float GetShininess();

private:
	XMFLOAT4 m_position; //��Դλ��
	XMFLOAT4 m_globalAmbient; //ȫ�ֻ�����ϵ��
	XMFLOAT3 m_direction; //��Դ����,ƽ�й�
	XMFLOAT4 m_lightColor; //��Դ��ɫ
	float m_shininess;  //�߹�ϵ��
	XMFLOAT4 attenuation; //x, y,z�ֱ𱣴泣���������Լ�����˥��ϵ��
	XMFLOAT4 spotlight; //x,y�ֱ𱣴��ڽǺ��������ֵ

};