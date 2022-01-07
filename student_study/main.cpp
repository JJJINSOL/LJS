#include "student.h"
void main()
{
	student a;
	int number;
	while (1)
	{
		printf("Create(0),Save(1),Delete(2),Load(3),Draw(4),Quit(9) : ");
		scanf("%d", &number);
		
		if (number == 9)break;
		switch (number)
		{
		case 0: 
			{
				a.Create();
			}break;
		case 1:
			{
				a.FileSave();
			}break;
		case 2:
			{
				a.AllDel();
			}break;
		case 3:
			{
				a.AllDel();
				a.Load();
			}break;
		case 4:
			{
				a.Draw();
			}break;
		}
	}
}