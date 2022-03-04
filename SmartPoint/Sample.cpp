#include <iostream>
#include <vector>
struct Node
{
	int iValue;
	Node(int i)
	{
		iValue = i;
	}
	Node()
	{
		iValue = 0;
	}
};
using T_Node = std::shared_ptr<Node>;
class Manager1
{
	std::vector<std::unique_ptr<Node>> list;
public:
	void Add(int data)
	{
		list.push_back(std::make_unique<Node>(data));
	};
	Node& GetPtr(int iData)
	{
		for (auto& data : list)
		{
			if (data->iValue == iData)
			{
				return *data;
			}
		}
	}
	Node* GetPtr3(int iData)
	{
		for (auto& data : list)
		{
			if (data->iValue == iData)
			{
				return data.get();
			}
		}
	}
	std::unique_ptr<Node>& GetPtr2(int iData)
	{
		//for (auto data : list)
		std::vector<std::unique_ptr<Node>>::iterator iter;
		for (iter = list.begin();
			iter != list.end();
			iter++)
		{
			if ((*iter)->iValue == iData)
			{
				return *iter;
			}
		}
	}
};
class Manager2
{
	std::vector<std::shared_ptr<Node>> list;
public:
	void Add(int data)
	{
		list.push_back(std::make_shared<Node>(data));
	};
	Node& GetPtr(int iData)
	{
		for (auto& data : list)
		{
			if (data->iValue == iData)
			{
				return *data;
			}
		}
	}
	std::shared_ptr<Node> GetPtr2(int iData)
	{
		for (auto data : list)
		{
			if (data->iValue == iData)
			{
				return data;
			}
		}
	}
};
void main()
{
	/*std::vector<T_Node>  list;
	list.push_back(T_Node(new TNode));
	TNode* pNode = list[0].get();
	auto data = list[0];
	std::cout << data.get()->iValue<<std::endl;
	std::cout << data->iValue << std::endl;*/

	//std::vector<std::shared_ptr<int>> data3;
	//data3.push_back(std::make_shared<int>(4));
	//std::vector<std::shared_ptr<int>> data4;
	//std::shared_ptr<int> aa = data3[0];
	////*data4[0].get() = 9;

	std::vector<std::unique_ptr<int>> data1;
	data1.push_back(std::make_unique<int>(4));
	std::vector<std::unique_ptr<int>> data2;
	//data2 = data1;
	//std::unique_ptr<int> copy = data1
	data2 = std::move(data1);
	//*data2[0].get() = 9;

	/*std::vector<std::unique_ptr<int>> data1(10);
	std::vector<std::unique_ptr<int>> data2;
	data2 = data1;*/



	Manager1 mgr1;
	for (int i = 0; i < 3; i++)
	{
		mgr1.Add(i);
	}

	Node* pnode3 = mgr1.GetPtr3(2);
	pnode3->iValue = 77;
	delete pnode3;

	Node& node1 = mgr1.GetPtr(2);
	node1.iValue = 10;

	std::unique_ptr<Node>& copy = mgr1.GetPtr2(1);
	copy.get()->iValue = 10;

	Manager2 mgr2;
	for (int i = 0; i < 3; i++)
	{
		mgr2.Add(i);
	}
	Node& node2 = mgr2.GetPtr(2);
	node2.iValue = 10;

	//  weak_ptr  포인터에 직접 접근 불가(멤버변수,함수 모두 사용 불가)
	//  유일한 사용방법은 lock()를 사용한다.
		//  shared_ptr을 반환 받는다.
	std::shared_ptr<int > w1(new int(3));
	std::weak_ptr<int> weak1 = w1;
	*w1.get() = 5;
	//*weak1.get() = 44;
	std::shared_ptr<int > obj = weak1.lock();
	*obj.get() = 44;

}