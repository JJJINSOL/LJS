#pragma once
#include "Core.h"
#include "DxObject.h"
class Sample : public Core
{
	std::vector<DxObject> m_ObjectList;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	Sample();
	~Sample();
};

