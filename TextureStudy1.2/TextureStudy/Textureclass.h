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
#include "DDSTextureLoader.h"
#include "common.h"
using namespace DirectX;
////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass {
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, wchar_t*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_textureView;
};
#endif