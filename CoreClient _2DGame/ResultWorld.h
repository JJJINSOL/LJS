#include "World.h"
#include "UIObject.h"
#include "SoundMgr.h"
#include "Life2D.h"
#include "ShaderMgr.h"
#include "TextureMgr.h"
class ResultWorld : public World
{
public:
	Texture*		m_BGTex;
	Texture*		m_BGSubTex;
	Life2D			m_Gameover;
	Sound*			m_pBackGroundMusic;

	float			m_fGOTimer = 0.0f;
	float			m_fGOChangeTime = 0.3;
	int				m_CurrentIndex = 0;

	bool			CreateModelType();
	bool			Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

