#pragma once
#include "Core.h"
#include "AsyncSelect.h"
#include "DxObject.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
class Sample : public Core
{
	//인터페이스 필요
	//텍스쳐와 관련된 인터페이스
	//view = 응용프로그램에서 shader로 전달되는 리소스이다
	ID3D11ShaderResourceView* m_pSRV0; //텍스쳐를 GPU에 전달할때는 이게 되야됨
	ID3D11Resource* m_pTexture0; //텍스쳐는 파일인데 파일을 로드하면 이게 만들어짐

	ID3D11ShaderResourceView* m_pSRV1;
	ID3D11Resource* m_pTexture1;

	//투명도
	ID3D11BlendState* m_AlphaBlend;

public:
	std::vector<DxObject> m_ObjectList;
	AsyncSelect m_net;
	HWND m_edit;
	HWND m_button;
	HWND m_listbox;
	int m_chatcount;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};
