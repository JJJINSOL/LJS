#pragma once
#include "Object3D.h"
#include <fbxsdk.h>

struct PNCT
{
	T::TVector3 p;
	T::TVector3 n;
	T::TVector4 c;
	T::TVector2 t;
};
struct VertexIW
{
	float  i[4];
	float  w[4];
	VertexIW()
	{
		i[0] = i[1] = i[2] = i[3] = 0;
		w[0] = w[1] = w[2] = w[3] = 0.0f;
	}
};
// 멀티스트림  (VB[0], VB[1])
// 인풋레이아웃 ( PNCTIW ) -> 정점쉐이더 전달

struct Track
{
	UINT	iFrame;
	TMatrix matTrack;
	// SRT
	T::TVector3	   s;
	T::TQuaternion r;
	T::TVector3    t;
};
struct Weight
{
	std::vector<int>	Index;  // 영향을 미치는 행렬의 인덱스
	std::vector<float>  weight; // 가중치
	void InsertWeight(int iBoneIndex, float fBoneWeight)
	{
		for (DWORD i = 0; i < Index.size(); ++i)
		{
			if (fBoneWeight > weight[i])
			{
				for (DWORD j = (Index.size() - 1); j > i; --j)
				{
					Index[j] = Index[j - 1];
					weight[j] = weight[j - 1];
				}
				Index[i] = iBoneIndex;
				weight[i] = fBoneWeight;
				break;
			}
		}
	}
	Weight()
	{
		Index.resize(8);
		weight.resize(8);
	}
};
class FbxModel : public Object3D
{
public:	
	int			 m_iIndex = -1;
	bool		 m_bSkinned = false;
	TMatrix		 m_matLocal;
	TMatrix		 m_matAnim;
	FbxNode*	 m_pFbxParent = nullptr;
	FbxNode*	 m_pFbxNode = nullptr;
	FbxModel*	 m_pParentObj = nullptr;
	std::wstring m_szTexFileName;
	// submaterial
	std::vector<std::wstring>  m_szTexFileList;

	using SubVertex = std::vector<Vertex>;
	using SubVertexIW = std::vector<VertexIW>;

	std::vector<SubVertex>      m_pSubVertexList;
	std::vector<SubVertexIW>    m_pSubIWVertexList;

	std::vector<Weight>			m_WeightList;

	std::vector<ID3D11Buffer*>   m_pVBList;
	std::vector<ID3D11Buffer*>   m_pVBWeightList;
	std::vector<Texture*>		 m_pTextureList;

	std::vector<Track>					m_AnimTrack;
	std::map<std::wstring, TMatrix>		m_dxMatrixBindPoseMap;
	TMatrix aaa[255];
public:
	virtual bool    SetVertexData() override;	
	virtual bool	CreateVertexBuffer()override;
	virtual bool    SetIndexData() override;
	virtual bool	CreateInputLayout() override;
	virtual bool	PostRender() override;
	virtual bool    Release() override;
	
};
struct Scene
{
	UINT   iStart;
	UINT   iEnd;
	UINT   iFrameSpeed;
};
//FbxImporter 로 클래스 설정 안됨
class TFbxImporter : public Object3D
{
public:
	Scene		m_Scene;
	float		m_fDir = 1.0f;
	float		m_fTime = 0.0f;
	float		m_fSpeed = 1.0f;
	BoneWorld	m_matBoneArray;
public:
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
	FbxNode*		m_pRootNode;

	std::map<FbxNode*, int>				m_pFbxNodeMap;
	std::map<std::wstring, FbxModel*>	m_pFbxModelMap;

	std::vector<FbxModel*>  m_DrawList;
	std::vector<FbxModel*>  m_TreeList;
	ID3D11Buffer* m_pBoneCB = nullptr;
public:
	bool			Load(ID3D11Device* pd3dDevice, std::wstring filename);
	virtual bool	Load(std::string filename);
	virtual void    PreProcess(FbxNode* node, FbxModel* fbxParent=nullptr);
	virtual void	ParseMesh(FbxModel* pObject);
	std::string		ParseMaterial(FbxSurfaceMaterial* pMtrl);

	void ReadTextureCoord(	FbxMesh* pFbxMesh, 
							FbxLayerElementUV* pUVSet, 
							int vertexIndex, 
							int uvIndex, 
							FbxVector2& uv);
	
	FbxColor ReadColor(const FbxMesh* mesh,
						DWORD dwVertexColorCount,
						FbxLayerElementVertexColor* pVertexColorSet,
						DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh,int controlPointIndex,int vertexCounter);

	int		GetSubMaterialIndex(int iPlygon,FbxLayerElementMaterial* pMtrl);
	bool	ParseMeshSkinning(FbxMesh* pFbxMesh, FbxModel* pObject);

public:
	TMatrix     DxConvertMatrix(TMatrix m);
	TMatrix     ConvertMatrix(FbxMatrix& m);
	TMatrix     ConvertAMatrix(FbxAMatrix& m);
	void		ParseAnimation();
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
	virtual bool	CreateConstantBuffer(ID3D11Device* pDevice);
};

