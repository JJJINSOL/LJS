#include "Student.h"

void main()
{
	Student a;
	int i = 0;
	while(1)
	{
		printf("\n->Create(0),Save(1),Delete(2),Load(3),Draw(4),Quit:9 : ");
		scanf_s("%d", &i);
		if (i == 9) break;
		switch (i)
		{
		case 0: 
			a.Create();
			break;
		case 1:
			a.FileSave();
			break;
		case 2:
			a.DelAll();
			break;
		case 3:
		{
			a.DelAll();
			a.Load();
		}break;
		case 4 :
			a.Draw();
			break;
		}
	}

}