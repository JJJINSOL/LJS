#include <iostream>
#include <Windows.h>
#include "Octree.h"
using namespace std;

void main()
{
	Octree tree;
	tree.Init(100, 100, 100, 2);
	int obindex = 0;
	int num = 0;
	cout << "정적 객체 (1) / 동적 객체 (2) : ";
	cin >> num;

	if (num == 1)
	{
		//정적 객체
		for (int i = 0; i < 10; i++)
		{
			Object* staticob = new Object();
			staticob->SetPos((float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100));

			float hw = 3;
			float hh = 3;
			float hd = 3;

			Vector vMin;

			vMin.x = staticob->m_vPos.x - hw;
			vMin.y = staticob->m_vPos.y - hh;
			vMin.z = staticob->m_vPos.z - hd;
			Box bx(vMin, hw * 2.0f, hh * 2.0f, hd * 2.0f);
			staticob->SetBox(bx);
			tree.AddObject(staticob, i);
		}
		tree.PrintfObjectList(tree.m_RootNode);
	}
	else if (num == 2)
	{
		//동적 개체
		Object* dynamicob[10];

		for (int i = 0; i < 10; i++)
		{
			dynamicob[i] = new Object();
			dynamicob[i]->SetPos((float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100));

			Vector vMin;

			float hw = 3;
			float hh = 3;
			float hd = 3;

			vMin.x = dynamicob[i]->m_vPos.x - hw;
			vMin.y = dynamicob[i]->m_vPos.y - hh;
			vMin.z = dynamicob[i]->m_vPos.z - hd;

			Box bx(vMin, hw * 2.0f, hh * 2.0f, hd * 2.0f);

			dynamicob[i]->SetBox(bx);
			tree.AddObject(dynamicob[i], i);
		}
		tree.PrintfObjectList(tree.m_RootNode);

		int count = 20;

		while (count > 0)
		{
			tree.DeleteObject(tree.m_RootNode);
			for (int i = 0; i < 10; i++)
			{
				Vector position = dynamicob[i]->m_vPos;
				position = position + Vector(1, 1, 1) * 2;
				dynamicob[i]->SetPos(position.x, position.y, position.z);

				Vector vMin;
				vMin.x = dynamicob[i]->m_vPos.x - dynamicob[i]->m_box.vSize.x;
				vMin.y = dynamicob[i]->m_vPos.y - dynamicob[i]->m_box.vSize.y;
				vMin.z = dynamicob[i]->m_vPos.z - dynamicob[i]->m_box.vSize.z;

				Box bx(vMin, dynamicob[i]->m_box.vSize.x, dynamicob[i]->m_box.vSize.y, dynamicob[i]->m_box.vSize.z);

				dynamicob[i]->SetBox(bx);
				tree.AddObject(dynamicob[i], i);
			}
			system("cls");
			tree.PrintfObjectList(tree.m_RootNode);
			Sleep(1000);
			count--;
		}

		for (int i = 0; i < 10; i++)
		{
			delete dynamicob[i];
		}
	}
	else
		cout << "잘못입력하셨습니다.";
}