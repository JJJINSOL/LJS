#pragma once
#include "TFbxImporter.h"

class TFbx :public TObject3D
{
public:
	TFbxImporter* m_pImporter=nullptr;
	float m_fDir = 1.0f;
	float m_fTime = 0.0f;
	float m_fSpeed = 1.0f;

	//2에서 이것 두개 추가됨
	TBoneWorld			m_matBoneArray; //상수버퍼
	std::vector<TFbx>	m_DrawList;

public:
	virtual bool	Init()override;
	virtual bool	Frame()override;
	virtual bool	Render()override;
	virtual bool    Release() override;
};
