#include "Sprite.h"
bool Sprite::Load(ID3D11Device* pd3dDevice, std::wstring filename)
{
	return true;
}
void Sprite::SetRectSouceArray(RECT rt)
{
	m_rtArray.push_back(rt);
}

bool Sprite::Init()
{
	return true;
}
bool Sprite::Frame()
{
	return true;
}
bool Sprite::Render()
{
	DxObject::Render();
	return true;
}
bool Sprite::Release()
{
	return true;
}
//스프라이트 txt 파일 로드=================================================================
Sprite* SpriteMgr::Load(std::wstring filename)
{
	TCHAR pBuffer[256] = { 0 };
	TCHAR pTemp[256] = { 0 };

	int iNumSprite = 0;
	float fAnimTimer;
	FILE* fp_src;
	//파일열기                           모드
	_wfopen_s(&fp_src, filename.c_str(), _T("rt"));
	if (fp_src == NULL) return nullptr;

	//파일에서 문자열 읽어오기
	//               256 - 1(마지막 null)
	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d"), pTemp, (unsigned int)_countof(pTemp),&iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		TCHAR name[256] = { 0 };
		Sprite* pSprite = new Sprite;
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s %d %f"), name, (unsigned int)_countof(name),
			&iNumFrame,
			&pSprite->m_fAnimTime);

		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, (unsigned int)_countof(pTemp),
				&rt.left, &rt.top, &rt.right, &rt.bottom);
			pSprite->m_rtArray.push_back(rt);
		}

		pSprite->m_csName = name;
		m_list.insert(make_pair(pSprite->m_csName, pSprite));
		m_iIndex++;
	}
	fclose(fp_src);

	Sprite* pSprite = new Sprite;
	TCHAR name[256] = {L"effect"};

	for (int iFrame = 0; iFrame < 6; iFrame++)
	{

		pSprite->m_rtArray.push_back({850,850,300,300});
	}

	pSprite->m_csName = name;
	pSprite->m_fAnimTime = 1.5;
	m_list.insert(make_pair(pSprite->m_csName, pSprite));
	m_iIndex++;

	return nullptr;
}