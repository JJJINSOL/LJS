#include "Student.h"
class StudentCollege : public Student
{
public:
	int m_graphic;
	int m_physics;
	int m_ai;

	virtual void Show() override;
	virtual void Save() override;
	virtual void Load() override;
	virtual void SetData(int iIndex) override;

	StudentCollege()
	{
		m_type = 3;
	}
	~StudentCollege()
	{

	}
};
