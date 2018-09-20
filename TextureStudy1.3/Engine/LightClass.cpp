#include "LightClass.h"


LightClass::LightClass(void)
{
	m_position = XMFLOAT4(5.0, 5.0, -3.0, 1.0);
	m_globalAmbient = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
	m_direction = XMFLOAT3(-1.0, -1.0, 1.0);
	m_lightColor = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
	m_shininess = 5.0f;
	
	attenuation = XMFLOAT4(1.0, 0.0, 0.0, 1.0);
	spotlight = XMFLOAT4(0.7, 0.6, 0.0, 1.0);
}

LightClass::LightClass(const LightClass & others)
{

}

LightClass::~LightClass(void)
{
}

void  LightClass::SetLightPosition(float x, float y, float z, float w)
{
	m_position = XMFLOAT4(x, y, z, w);
}

//����ȫ�ֻ�����ϵ��
void LightClass::SetGlobalAmbient(float red, float green, float blue, float alpha)
{
	m_globalAmbient = XMFLOAT4(red, green, blue, alpha);
	return;
}

//����ƽ�йⷽ��
void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

//���ù�Դ��ɫ
void LightClass::SetLightColor(float red, float green, float blue, float alpha)
{
	m_lightColor = XMFLOAT4(red, green, blue, alpha);
	return;
}
//���ù�Դ˥��ϵ��
void LightClass::SetLightAttenuation(float x, float y, float z, float w)
{
	attenuation = XMFLOAT4(x, y, z, w);
	return;
}

//����̽�յƹ�Դ�ڽ��������ֵ
void LightClass::SetLightSpotLight(float x, float y, float z, float w)
{
	spotlight = XMFLOAT4(x, y, z, w);
	return;
}

//����ȫ�ֻ�����ϵ��
void LightClass::SetShininess(float shininess)
{
	m_shininess = shininess;
	return;
}

//�õ���Դλ��
XMFLOAT4 LightClass::GetPosition()
{
	return m_position;
}

//�õ�ȫ�ֻ�����ϵ��
XMFLOAT4 LightClass::GetGlobalAmbient()
{
	return m_globalAmbient;
}

//�õ�ƽ�йⷽ��
XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

//�õ���Դ��ɫ
XMFLOAT4 LightClass::GetLightColor()
{
	return m_lightColor;
}

//�õ�ȫ�ֻ�����ϵ��
float LightClass::GetShininess()
{
	return m_shininess;
}

//�õ�˥��ϵ��
XMFLOAT4 LightClass::GetLightAttenuation()
{
	return attenuation;
}

//�õ�spotlight���������ֵ
XMFLOAT4 LightClass::GetSpotLight()
{
	return spotlight;
}