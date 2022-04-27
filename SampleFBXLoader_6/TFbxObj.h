#pragma once
#include "TFbxImporter.h"
#include "Camera.h"
class Fbx :public Object3D
{
public:
	Camera* m_pMainCamera;
	UINT     m_iShadowID = 0;
	TVector4 m_vShadowColor;
public:
	TFbxImporter* m_pMeshImp;
	TFbxImporter* m_pAnimImporter;
	float m_fDir = 1.0f;
	float m_fTime = 0.0f;
	float m_fSpeed = 1.0f;
	BoneWorld			m_matBoneArray;
	std::vector<Fbx>	m_DrawList;
public:
	virtual bool	Init()override;
	virtual bool	Frame()override;
	virtual bool	Render()override;
	virtual bool    Release() override;
	void			GenAABB()override;
public:
	bool	RenderShadow(Shader* pShader);
	T::TMatrix Interplate(TFbxImporter* pAnimImp,FbxModel* pModel, float fFrame);
};
