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

}

void CObjectManager::RenderObjects()
{

}

void CObjectManager::CheckCollisions()
{

}

void CObjectManager::RemoveAllObjects()
{

}

void CObjectManager::AddObject(IBaseInterface* pObject)
{

}

void CObjectManager::RemoveObject(IBaseInterface* pObject)
{

}