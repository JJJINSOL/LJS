#include "ObjectMgr.h"
void  ObjectMgr::AddCollisionExecute(BaseObject* owner, CollisionFunction func)
{
	//collision id ���
	owner->m_iCollisionID = m_iExcueteCollisionID++;
	//������Ʈ ����Ʈ ���->Ű�� id
	m_ObjectList.insert(std::make_pair(owner->m_iCollisionID, owner));
	//�� Ű�� �Լ� ���
	m_fnCollisionExecute.insert(std::make_pair(owner->m_iCollisionID, func));
}
void  ObjectMgr::DeleteExecute(BaseObject* owner)
{

}
bool  ObjectMgr::Init()
{
	return true;
}
bool  ObjectMgr::Frame()
{
	//N:N �浹ó��
	for (auto src : m_ObjectList)
	{
		BaseObject* pObjSrc = (BaseObject*)src.second;
		if (pObjSrc->m_dwCollisonType == CollisionType::Ignore) continue;
		DWORD dwState = CollisionType::Overlap;
		for (auto dest : m_ObjectList)
		{
			BaseObject* pObjDest = (BaseObject*)dest.second;
			if (pObjSrc == pObjDest) continue;
			if (Collision::ToRect(pObjSrc->m_rtCollision, pObjDest->m_rtCollision))
			{
				FuncionIterator colliter = m_fnCollisionExecute.find(pObjSrc->m_iCollisionID);
				if (colliter != m_fnCollisionExecute.end())
				{
					CollisionFunction call = colliter->second;
					call(pObjDest, dwState);
				}
			}
		}
	}
	return true;
}
bool  ObjectMgr::Releae()
{
	return true;
}