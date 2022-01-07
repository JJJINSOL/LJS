#pragma once
#include "StudentMiddle.h"
#include "StudentHigh.h"
#include "StudentCollege.h"

class NodeBox
{
public:
	Student* m_Data;
	Student* NewData(int index);

	NodeBox* m_next;

	NodeBox();
	NodeBox(int index);
	~NodeBox();
};

