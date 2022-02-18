#pragma once
#include "Device.h"
#include "DxObject.h"
#include "Input.h"
#include "Timer.h"
#include "WriteFont.h"
class Core : public Device
{
public:
	Timer m_GameTimer;
	WriteFont  m_dxWrite;
	ID3D11SamplerState* m_pSamplerState = nullptr;
private:
	bool CoreInit();
	bool CoreFrame();
	bool CoreRender();
	bool CoreRelease();
public:
	bool GameRun();


public:
	virtual bool Init() { return true; };
	virtual bool Frame() { return true; };
	virtual bool Render() { return true; };
	virtual bool Release() { return true; };

	Core() {}
	virtual ~Core() {}
};