////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"
#include "AxisModelClass.h"
#include "CubeModelClass.h"
#include "PlaneModelClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"
#include "LightTexShaderClass.h"
#include "TexManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float);

	CameraClass* m_Camera;
	D3DClass* m_Direct3D;

private:
	bool Render(float);

private:

	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
	AxisModelClass* m_AxisModel;

	PlaneModelClass* m_PlaneModel;
	CubeModelClass* m_CubeModel;

	//LightClass* m_Light;
	LightShaderClass* m_LightShader;
	LightTexShaderClass* m_LightTexShader;
	TexManagerClass*  m_TexManager;
};

#endif