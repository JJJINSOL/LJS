#pragma once
#include "Core.h"
#include "AsyncSelect.h"
#include "DxObject.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
class Sample : public Core
{
	//�������̽� �ʿ�
	//�ؽ��Ŀ� ���õ� �������̽�
	//view = �������α׷����� shader�� ���޵Ǵ� ���ҽ��̴�
	ID3D11ShaderResourceView* m_pSRV0; //�ؽ��ĸ� GPU�� �����Ҷ��� �̰� �Ǿߵ�
	ID3D11Resource* m_pTexture0; //�ؽ��Ĵ� �����ε� ������ �ε��ϸ� �̰� �������

	ID3D11ShaderResourceView* m_pSRV1;
	ID3D11Resource* m_pTexture1;

	//����
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
