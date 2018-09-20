////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"
TextureClass::TextureClass()
{
	m_textureView = 0;
}

TextureClass::TextureClass(const TextureClass& other)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, wchar_t* filename)
{
	HRESULT result;

	// 从一个文件创建纹理资源视图.
	//result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_textureView, NULL);
	result = CreateDDSTextureFromFile(device, filename, NULL, &m_textureView);
	if (FAILED(result))
	{
	    HR(result);
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture.
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}
