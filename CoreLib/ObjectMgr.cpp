#include "ObjectMgr.h"
void  ObjectMgr::AddCollisionExecute(BaseObject* owner, CollisionFunction func)
{
	//collision id 등록
	owner->m_iCollisionID = m_iExcueteCollisionID++;
	//오브젝트 리스트 등록->키가 id
	m_ObjectList.insert(std::make_pair(owner->m_iCollisionID, owner));
	//그 키로 함수 등록
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
	//N:N 충돌처리
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