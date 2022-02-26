#include "TextureMgr.h"
bool Texture::Load(ID3D11Device* pd3dDevice, std::wstring filename)
{
	//파일에서 읽어들임
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		pd3dDevice,
		filename.c_str(),
		(ID3D11Resource**)&m_pTexture,
		&m_pSRV);
	if (FAILED(hr))
	{
		//dds파일 읽어들임
		hr = DirectX::CreateDDSTextureFromFile(
			pd3dDevice,
			filename.c_str(),
			(ID3D11Resource**)&m_pTexture,
			&m_pSRV);
		if (FAILED(hr))
		{
			return false;
		}
	}
	m_pTexture->GetDesc(&m_TextureDesc);
	return true;
}
bool Texture::Init()
{
	return true;
}
bool Texture::Frame()
{
	return true;
}
bool Texture::Render()
{
	return true;
}
bool Texture::Release()
{
	if (m_pTexture) m_pTexture->Release();
	if (m_pSRV) m_pSRV->Release();
	return true;
}
Texture::Texture()
{

}
Texture::~Texture()
{

}

