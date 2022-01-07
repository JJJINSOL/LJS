#pragma once
#include "Core.h"
class Sample : public Core
{
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	Sample();
	~Sample();
};

