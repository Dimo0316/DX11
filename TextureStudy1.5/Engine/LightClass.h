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
	XMFLOAT4 m_position; //光源位置
	XMFLOAT4 m_globalAmbient; //全局环境光系数
	XMFLOAT3 m_direction; //光源方向,平行光
	XMFLOAT4 m_lightColor; //光源颜色
	float m_shininess;  //高光系数
	XMFLOAT4 attenuation; //x, y,z分别保存常量、线性以及二次衰减系数
	XMFLOAT4 spotlight; //x,y分别保存内角和外角余弦值

};