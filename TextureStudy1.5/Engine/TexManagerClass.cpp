#include "TexManagerClass.h"

TexManagerClass::TexManagerClass(void)
{
	m_RandomTexRV = 0;
}

TexManagerClass::TexManagerClass(const TexManagerClass& others)
{
}

TexManagerClass::~TexManagerClass(void)
{
}

void TexManagerClass::Shutdown()
{
	for (int i = 0; i < m_TextureRVs.size(); ++i)
	{
		if (m_TextureRVs[i])
		{
			m_TextureRVs[i]->Release();
			m_TextureRVs[i] = 0;
		}
	}

	if (m_RandomTexRV)
	{
		m_RandomTexRV->Release();
		m_RandomTexRV = 0;
	}
}


ID3D11ShaderResourceView* TexManagerClass::createTex(ID3D11Device* device, wchar_t* filename)
{
	HRESULT result;
	// 如果纹理资源已经存在，则返回，否则创建
	for (int i = 0; i < m_TextureRVs.size(); ++i)
		if (m_TextureNames[i] == filename)
			return m_TextureRVs[i];

	// 从一个文件创建纹理资源视图.
	result = CreateDDSTextureFromFile( device, filename, NULL, &m_RandomTexRV);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	m_TextureNames.push_back(filename);
	m_TextureRVs.push_back(m_RandomTexRV);

	return m_RandomTexRV;
}