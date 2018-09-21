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

//设置全局环境光系数
void LightClass::SetGlobalAmbient(float red, float green, float blue, float alpha)
{
	m_globalAmbient = XMFLOAT4(red, green, blue, alpha);
	return;
}

//设置平行光方向
void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

//设置光源颜色
void LightClass::SetLightColor(float red, float green, float blue, float alpha)
{
	m_lightColor = XMFLOAT4(red, green, blue, alpha);
	return;
}
//设置光源衰减系数
void LightClass::SetLightAttenuation(float x, float y, float z, float w)
{
	attenuation = XMFLOAT4(x, y, z, w);
	return;
}

//设置探照灯光源内角外角余弦值
void LightClass::SetLightSpotLight(float x, float y, float z, float w)
{
	spotlight = XMFLOAT4(x, y, z, w);
	return;
}

//设置全局环境光系数
void LightClass::SetShininess(float shininess)
{
	m_shininess = shininess;
	return;
}

//得到光源位置
XMFLOAT4 LightClass::GetPosition()
{
	return m_position;
}

//得到全局环境光系数
XMFLOAT4 LightClass::GetGlobalAmbient()
{
	return m_globalAmbient;
}

//得到平行光方向
XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

//得到光源颜色
XMFLOAT4 LightClass::GetLightColor()
{
	return m_lightColor;
}

//得到全局环境光系数
float LightClass::GetShininess()
{
	return m_shininess;
}

//得到衰减系数
XMFLOAT4 LightClass::GetLightAttenuation()
{
	return attenuation;
}

//得到spotlight内外角余弦值
XMFLOAT4 LightClass::GetSpotLight()
{
	return spotlight;
}