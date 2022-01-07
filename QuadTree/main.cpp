#include "Quadtree.h"
#include "Vector2.h"
void main()
{
	//Quadtree tree;
	//Vector2 vec;
	//tree.Init(100, 100, 2);
	//for (int i = 0; i < 10; i++)
	//{
	//	int x = rand() % 100;
	//	int y = rand() % 100;
	//	tree.AddObject(x, y);
	//}
	//tree.AddObject(10, 20);
	//tree.AddObject(11, 19);
	//tree.PrintObjectList(tree.m_root);

	Vector2 a = Vector2(6.0, 2.0);
	//Vector2 b = Vector2(5.0, 10.0);

	float d = a.Length(a);
	//Vector2 ob = a.Normal(a);
	std::cout << d << std::endl;
	std::cout << a.Normal(a).x <<"  "<< a.Normal(a).y<<std::endl;
}