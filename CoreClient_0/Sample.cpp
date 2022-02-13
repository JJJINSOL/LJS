#include "Sample.h"
LRESULT  Sample::MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		//WM_COMMAND = 명령 있으면 ex. 확인버튼 입력
	case WM_COMMAND:
	{
		switch (LOWORD(wparam))
		{
		case 200:
		{
			char buffer[MAX_PATH] = { 0, };
			SendMessageA(m_edit, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);
			Packet packet(PACKET_CHAT_MSG);
			packet << 999 << "이진솔" << 50 << buffer;
			m_net.SendMsg(m_net.m_sock, packet.m_upacket);

			SendMessageA(m_edit, WM_SETTEXT, 0, (LPARAM)"");
		}break;
		}
	}break;
	}
	return m_net.MsgProc(hwnd, msg, wparam, lparam);
}
bool Sample::Init()
{
	////윈도우창 스타일
	//DWORD style = WS_CHILD | WS_VISIBLE | ES_MULTILINE;
	//m_edit = CreateWindow(L"edit", NULL, style, 0, g_rtClient.bottom - 50, 300, 50, m_hWnd, (HMENU)100, m_hInsatance, NULL);
	//style = WS_CHILD | WS_VISIBLE;
	//m_button = CreateWindow(L"button", L"전송", style, 310, g_rtClient.bottom - 50, 50, 50, m_hWnd, (HMENU)200, m_hInsatance, NULL);
	//m_listbox = CreateWindow(L"listbox", NULL, style, 0, 0, 300, g_rtClient.bottom - 70, m_hWnd, (HMENU)300, m_hInsatance, NULL);

	//SendMessageA(m_listbox, LB_ADDSTRING, 0, (LPARAM)"채팅시작!");

	//for (int i = 0; i < 12; i++)
	//{
	//	DxObject obj;
	//	obj.Init();

	//	if (obj.Create(m_pd3dDevice, m_pImmediateContext, Vector2(-100 * i, i * 50), 400, 30))
	//	{
	//		m_ObjectList.push_back(obj);
	//	}
	//}

	DxObject obj;
	obj.Init();

	if (obj.Create(m_pd3dDevice, m_pImmediateContext, Vector2(400, 300), 400, 300))
	{
		m_ObjectList.push_back(obj);
	}

	HRESULT hr;
	//파일에서 읽어들임
	hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice,
		L"../../data/bitmap1.bmp", // 불투명알파 텍스처
		//L"../../data/bitmap1Alpha.bmp",// 투명알파 텍스처		
		//L"../../data/wireframe.dds",
		&m_pTexture0,
		&m_pSRV0);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			m_pd3dDevice,
			L"../../data/bitmap1.bmp", // 불투명알파 텍스처
			//L"../../data/bitmap1Alpha.bmp",// 투명알파 텍스처		
			//L"../../data/wireframe.dds",
			&m_pTexture0,
			&m_pSRV0);
	}
	hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice,
		L"../../data/bitmap2.bmp",
		&m_pTexture1,
		&m_pSRV1);

	// (소스컬러*D3D11_BLEND_SRC_ALPHA) 
	//                  + 
	// (대상컬러*D3D11_BLEND_INV_SRC_ALPHA)
	// 컬러   =  투명하길 원하는 부분컬러값 = (1,1,1,1)
	// 마스크 =  1.0 - 투명하길 원하는 부분컬러값 = (1,1,1,1)

	// FinalColor = SrcColor*SrcAlpha + DestColor*(1.0f- SrcAlpha) 	    
	// if SrcAlpha == 0 완전투명
	//           FinalColor() = SrcColor*0 + DestColor*(1-0)
	//                FinalColor = DestColor;
	// if SrcAlpha == 1 완전불투명
	//           FinalColor() = SrcColor*1 + DestColor*(1-1)
	//                FinalColor = SrcColor;
	// 혼합상태 = 소스(지금드로우객체 픽셀) (연산) 대상(백버퍼 객체:픽셀)
	// 혼합상태 = 픽셀쉐이더 출력 컬러  (연산:사칙연산) 출력버퍼의 컬러
	D3D11_BLEND_DESC  bd;  //투명도 속성 관련 구조체
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	/*bd.AlphaToCoverageEnable;
	bd.IndependentBlendEnable;*/
	bd.RenderTarget[0].BlendEnable = TRUE; //혼합 연산 할래 말래?
	//컬러(밑의 3개) RGB 연산 저장
	// 혼합상태 = 소스(지금드로우객체 픽셀) (연산) 대상(백버퍼 객체:픽셀)
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // 소스
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //대상 
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; //연산
	//// A 연산 저장
	//투명도(밑의 3개) 알파 연산 담당
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//일반적으로 add
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	//투명도 생성
	m_pd3dDevice->CreateBlendState(&bd, &m_AlphaBlend);



	m_net.Initnetwork();
	//"192.168.0.12"
	//"192.168.0.90"
	//"27.35.45.57"
	m_net.Connect(g_hwnd, SOCK_STREAM, 10000, "211.47.119.43");
	return true;
}
bool Sample::Frame()
{
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].Frame();
	}

	int count = m_net.m_playuser.m_packetpool.size();
	if (count > 0 && m_chatcount != count)
	{
		m_chatcount = count;

		SendMessage(m_listbox, LB_RESETCONTENT, 0, 0);
		list<Packet> ::iterator iter;
		if (m_net.m_playuser.m_packetpool.size() > 20)
		{
			m_net.m_playuser.m_packetpool.pop_front();
		}
		for (iter = m_net.m_playuser.m_packetpool.begin(); iter != m_net.m_playuser.m_packetpool.end(); iter++)
		{
			ChatMsg recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			(*iter) >> recvdata.index >> recvdata.name >> recvdata.damage >> recvdata.message;
			SendMessageA(m_listbox, LB_ADDSTRING, 0, (LPARAM)recvdata.message);
			//iter = m_Net.m_PlayerUser.m_packetPool.erase(iter);
			(*iter).Reset();
		}
	}
	return true;
}
bool Sample::Render()
{
	//픽셀 shader로 텍스쳐 전달 전달
	//                                        텍스쳐 레지스터 슬롯 번호   개수
	m_pImmediateContext->PSSetShaderResources(0,                          1, &m_pSRV0); //s가 붙으면 배열로 전달
	m_pImmediateContext->PSSetShaderResources(1,                          1, &m_pSRV1);
	
	m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);

	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].Render();
	}
	return true;
}
bool Sample::Release()
{
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].Release();
	}

	if (m_AlphaBlend) m_AlphaBlend->Release();
	if (m_pTexture0) m_pTexture0->Release();
	if (m_pSRV0) m_pSRV0->Release();
	if (m_pTexture1) m_pTexture1->Release();
	if (m_pSRV1) m_pSRV1->Release();
	m_AlphaBlend = nullptr;
	m_pTexture0 = nullptr;
	m_pSRV0 = nullptr;
	m_pTexture1 = nullptr;
	m_pSRV1 = nullptr;

	m_net.Closenetwork();
	return true;
}
Sample::Sample()
{

}
Sample:: ~Sample()
{

}
RUN();