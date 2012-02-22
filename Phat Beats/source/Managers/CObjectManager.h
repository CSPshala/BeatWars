///////////////////////////////////////////////////////
// File Name	:	"CObjectManager.h"
//
// Purpose		:	To manage added Ibaseinterface objects
//////////////////////////////////////////////////////

#ifndef C_OBJECTMANAGER_H
#define C_OBJECTMANAGER_H

#include <vector>
using std::vector;

// Forward Declarations
class IBaseInterface;

class CObjectManager
{
public:
	// Singleton related functions
	static CObjectManager* GetInstance();
	static void DeleteInstance();

	// Public Utility Functions
	void UpdateObjects(float fElapsedTime);
	void RenderObjects();
	void CheckCollisions(IBaseInterface* pBase);
	void RemoveAllObjects();
	void AddObject(IBaseInterface* pObject);
	void RemoveObject(IBaseInterface* pObject);

private:
	// Singleton
	CObjectManager();
	~CObjectManager();
	CObjectManager& operator=(const CObjectManager&);

	// Singleton pointer (DeleteInstance must be called)
	static CObjectManager* sm_pInstance;

	vector<IBaseInterface*> m_vObjectList;
};

#endif