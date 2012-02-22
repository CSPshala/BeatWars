//////////////////////////////////////////////////////////////////////////
//	File Name	:	"CFXManager.h"
//	
//	Purpose		:	To wrap up data for an effect
///////////////////////////////////////////////////////////////////////////

#ifndef C_FXManager_H
#define C_FXManager_H

#include "../CFX.h"

#include <list>
#include <string>
#include <map>
using std::string;

class CFXManager
{
public:
	
	~CFXManager();

	const void Render(void);
	const void Update(float fElapsedTime);
	const void AddFX(CFX* pEffect, string szKey);
	const void LoadFX(string szFileName, string szKey);
	const void UnloadFX(string szKey);
	const void UnloadAllFX(void);
	const void QueueParticle(string szKey);

	static CFXManager* GetInstance();

private:
	CFXManager();
	//*******MEMBERS*********//
	std::vector<CFX*> m_listActiveFX;
	std::map<std::string, CFX*> m_fxTable;
};

#endif