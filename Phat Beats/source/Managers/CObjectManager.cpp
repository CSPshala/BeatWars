///////////////////////////////////////////////////////
// File Name	:	"CObjectManager.cpp"
//
// Purpose		:	To manage added Ibaseinterface objects
//////////////////////////////////////////////////////

#include "CObjectManager.h"
#include "../IBaseInterface.h"

//////////////////////////////////////
//	Singleton pointer instantiation
//////////////////////////////////////
CObjectManager* CObjectManager::sm_pInstance = NULL;

CObjectManager::CObjectManager()
{
	
}

CObjectManager::~CObjectManager()
{

}

CObjectManager* CObjectManager::GetInstance()
{
	if(sm_pInstance == NULL)
		sm_pInstance = new CObjectManager();	

	return sm_pInstance;
}

void CObjectManager::DeleteInstance()
{
	if(sm_pInstance)
	{
		delete sm_pInstance;
		sm_pInstance = NULL;
	}
}

void CObjectManager::UpdateObjects(float fElapsedTime)
{
	vector<IBaseInterface*>::iterator iter = m_vObjectList.begin();
	for (;iter != m_vObjectList.end(); ++iter)
	{
		(*iter)->Update(fElapsedTime);
	}
}

void CObjectManager::RenderObjects()
{
	vector<IBaseInterface*>::iterator iter = m_vObjectList.begin();
	for (;iter != m_vObjectList.end(); ++iter)
	{
		(*iter)->Render();
	}
}

void CObjectManager::CheckCollisions(IBaseInterface* pBase)
{
	
	for (vector<IBaseInterface*>::size_type i = 0; i < m_vObjectList.size(); ++i)
	{
		// Making sure we're not checking with ourselves
		// List is actually checking notes against passed in pointer (which IS a player)
		// And even then it only checks against active beats, and not the whole list
		if(pBase != m_vObjectList[i])
			m_vObjectList[i]->CheckCollision(pBase);
	}
}

void CObjectManager::RemoveAllObjects()
{
	vector<IBaseInterface*>::iterator iter = m_vObjectList.begin();
	for (;iter != m_vObjectList.end(); ++iter)
	{
		(*iter)->Release();
	}
	m_vObjectList.clear();
}

void CObjectManager::AddObject(IBaseInterface* pObject)
{
	m_vObjectList.push_back(pObject);
	pObject->AddRef();
}

void CObjectManager::RemoveObject(IBaseInterface* pObject)
{
	vector<IBaseInterface*>::iterator iter = m_vObjectList.begin();
	for (;iter != m_vObjectList.end(); ++iter)
	{
		if ((*iter) == pObject)
		{
			(*iter)->Release();
			m_vObjectList.erase(iter);
			break;
		}
	}
}