#pragma once
#include "Object2D.h"
#include "TFbxImporter.h"
//function�� ����ϸ� virtual ����� �۵�
//                                      <��ȯ��(���ڵ�)>
using CollisionFunction = std::function<void(BaseObject*, DWORD)>;//�浹ó��
using SelectFunction = std::function<void(BaseObject*, DWORD)>;//���콺�� ������Ʈ �����ϴ°�
//������Ʈ �浹ó��
class ObjectMgr : public BaseMgr<TFbxImporter, ObjectMgr>//��ü ������ �θ𿡼�
{
	friend class Singleton<TextureMgr>;
private:
	int	m_iExcueteCollisionID;
	int	m_iExcueteSelectID;
	std::map<int, BaseObject*> m_ObjectList;
	std::map<int, BaseObject*> m_SelectList;
public://�θ� �ڽ��� ���� ���� �յ��� friend�� ����
	friend Singleton< ObjectMgr>;

	typedef std::map<int, CollisionFunction>::iterator FuncionIterator;
	std::map<int, CollisionFunction> m_fnCollisionExecute;//�Լ������/������ ������Ʈ�� �ε����� �ش��ϴ� �Լ�
	typedef std::map<int, SelectFunction>::iterator FuncionIterator;
	std::map<int, SelectFunction> m_fnSelectExecute;

	//                        �ڱ� �ڽ�          ��ȯ�� �Լ�
	void  AddCollisionExecute(BaseObject* owner, CollisionFunction func);//�̰� �Ҷ����� m_iExcueteCollisionID�� +1
	void  DeleteCollisionExecute(BaseObject* owner);

	void  AddSelectExecute(BaseObject* owner, CollisionFunction func);
	void  DeleteSelectExecute(BaseObject* owner);

	bool  Init();
	bool  Frame();
	bool  Release();
	void  CallRecursive(BaseObject* pSrcObj, DWORD dwState);

private://�ܺο����� ����� ��ü ���� �Ұ���
	ObjectMgr();
public:
	virtual ~ObjectMgr();

};
#define I_ObjectMgr ObjectMgr::Get()//�׷��� ��ü �����ϰ� ������ �̰� ��ߴ�