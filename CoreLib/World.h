#pragma once
#include "Object2D.h"
class World
{
public:
	enum WorldID { T_LOADING, T_LOGIN, T_LOBBY, T_ZONE, T_RESULT, };
	ID3D11Device*		m_pd3dDevice;	// ����̽� ��ü
	ID3D11DeviceContext* m_pContext;// �ٺ��̽� ���ؽ�Ʈ ��ü
	bool m_bLoadZone = false;
public:
	static World* m_pWorld;
	std::vector<std::shared_ptr<Object2D>> m_UIObj;
	std::vector<std::shared_ptr<Object2D>> m_ItemObj;
	std::vector<std::shared_ptr<Object2D>> m_NpcObj;
	std::vector<std::shared_ptr<Object2D>> m_MapObj;
	using m_mapIter = std::vector<std::shared_ptr<Object2D>>::iterator;
public:
	virtual bool	Load(std::wstring saveFile);
	virtual bool	Init() ;
	virtual bool	Frame() ;
	virtual bool	Render() ;
	virtual bool	Release();
public:
	World();
	virtual ~World();
};
