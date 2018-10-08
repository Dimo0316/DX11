#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <vector>
#include "DDSTextureLoader.h"
#include "common.h"
using namespace DirectX;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TexManagerClass {
public:
	typedef vector<wchar_t*> StringVector;
	TexManagerClass();
	TexManagerClass(const TexManagerClass&);
	~TexManagerClass();
	void Shutdown();

	ID3D11ShaderResourceView* createTex(ID3D11Device*, wchar_t*);
	//LPCWSTR stringToLPCWSTR(string orig);
private:
	//void StringToWstring(wchar_t*, string);
	StringVector m_TextureNames;
	vector<ID3D11ShaderResourceView*> m_TextureRVs;
	ID3D11ShaderResourceView* m_RandomTexRV;
};
#endif