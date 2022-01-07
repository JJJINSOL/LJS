#include "DxObject.h"
//#include "../CoreLib/Window.cpp"

void DxObject:: SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	m_pd3dDevice = pd3dDevice;
	m_pContext = pContext;
}

// 화면좌표 위치를 중점으로 NDC 변환
// 삼각형 두개 그래서 사각형 만들기, 점은 시계 방향으로 돌리자 / 0,1,2   3,4,5
void DxObject:: Convert(Vector2 center, float w, float h, std::vector<SimpleVertex>& rectlist)
{
	std::vector<SimpleVertex> list(6);
	float hw = w / 2.0f;
	float hh = h / 2.0f;

	list[0].v = { center.x - hw, center.y - hh };
	list[1].v = { center.x + hw, center.y - hh };
	list[2].v = { center.x - hw, center.y + hh };
	list[3].v = { center.x - hw, center.y + hh };
	list[4].v = { center.x + hw, center.y - hh };
	list[5].v = { center.x + hw, center.y + hh };

	Convert(list, rectlist);
}

//화면 좌표계를 NDC 변환
void DxObject:: Convert(std::vector<SimpleVertex>& list, std::vector<SimpleVertex>& rectlist)
{
	rectlist.resize(list.size());
	for (int i = 0; i < list.size(); i++)
	{
		// 0 ~ 800 -> 0 ~ 1 -> -1 ~ +1
		rectlist[i].v.x = list[i].v.x / g_rtClient.right;
		rectlist[i].v.y = list[i].v.y / g_rtClient.bottom;

		// 0 ~ 1 -> -1 ~ +1 :::: -1 ~ +1 -> 0 ~ 1
		// x = x * 2 + -1;  ::::  x= x * 0.5f + 0.5f;
		rectlist[i].v.x = rectlist[i].v.x * 2.0f - 1.0f;
		rectlist[i].v.y = -1.0f * (rectlist[i].v.y * 2.0f - 1.0f);
		
	}
}


bool DxObject:: Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, Vector2 pos, float w, float h)
{
	SetDevice(pd3dDevice, pContext);

	HRESULT hr;

	m_vPos = pos;
	Convert(m_vPos, w, h, m_VertexList);
	//GPU 메모리에 버퍼 할당(원하는 크기 할당)
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(SimpleVertex) * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);

	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer)))
	{
		return false;
	}

	// 새항목->유틸리티->txt파일 작성
	// 쉐이더 컴파일->오브젝트 파일을 통해서 쉐이더객체 생성 
	
	//vertex Shader 컴파일된 결과 들어옴===================================================================================================
	ID3DBlob* pVSCodeResult = nullptr;
	//에러 메시지
	ID3DBlob* pErrorMsgs = nullptr;
	//D3DCompileFromFile = 이 함수를 통해 shader를 컴파일할 수 있음 / vs_5_0 = 컴파일러 지정
	hr = D3DCompileFromFile(L"VertexShader.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, &pVSCodeResult, &pErrorMsgs);

	if (FAILED(hr))
	{
		//GetBufferPointer = 시작 주소이며 반환형이 lpvoid이므로 메세지 반환받기 위기 (char *) 문자열 강제로 바꿈
		MessageBoxA(NULL, (char*)pErrorMsgs->GetBufferPointer(), "ERROR", MB_OK);
		return false;
	}

	hr = m_pd3dDevice->CreateVertexShader(pVSCodeResult->GetBufferPointer(), pVSCodeResult->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{
		return false;
	}

	//pixel Shader 컴파일된 결과 들어옴====================================================================================================
	ID3DBlob* pPSCodeResult = nullptr;

	hr = D3DCompileFromFile(L"PixelShader.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSCodeResult, &pErrorMsgs);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)pErrorMsgs->GetBufferPointer(), "ERROR", MB_OK);
		return false;
	}

	hr = m_pd3dDevice->CreatePixelShader(pPSCodeResult->GetBufferPointer(), pPSCodeResult->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(hr))
	{
		return false;
	}
	// 정점쉐이더의 결과를 통해서 정점레이아웃을 생성한다.	
	// 정점버퍼의 각 정점의 어떤 성분을 정점쉐이더에 전달할 거냐

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//"POSITION" = 쉐이더 입력 서명에서 이 요소에 연결된 HLSL 의미
		//DXGI_FORMAT_R32G32_FLOAT = 요소 데이터 형식
		{"POSITION",0, DXGI_FORMAT_R32G32_FLOAT, 0,0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//입력 요소의 배열의 입력 데이터 형식의 수
	UINT NumElements = sizeof(layout) / sizeof(layout[0]);

	hr = m_pd3dDevice->CreateInputLayout( layout, NumElements, pVSCodeResult->GetBufferPointer(), pVSCodeResult->GetBufferSize(), &m_pVertexLayout);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool DxObject:: Init()
{
	return true;
}
bool DxObject:: Frame()
{
	m_VertexList[0].v.x += m_fSpeed;
	m_VertexList[1].v.x += m_fSpeed;
	m_VertexList[2].v.x += m_fSpeed;
	m_VertexList[3].v.x += m_fSpeed;
	m_VertexList[4].v.x += m_fSpeed;
	m_VertexList[5].v.x += m_fSpeed;
	m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	return true;
}
bool DxObject:: Render()
{
	m_pContext->IASetInputLayout(m_pVertexLayout);
	m_pContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext->PSSetShader(m_pPixelShader, NULL, 0);

	UINT StartSlot;
	UINT NumBuffers;
	UINT Strides = sizeof(SimpleVertex);
	UINT Offsets = 0;

	m_pContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &Strides, &Offsets);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	m_pContext->Draw(m_VertexList.size(), 0);
	return true;
}
bool DxObject:: Release()
{
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	m_pVertexBuffer = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	return true;
}