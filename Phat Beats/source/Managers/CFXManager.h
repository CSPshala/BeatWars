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
#include "../SGD_Math.h"
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
	const void DequeueParticle(string szKey);
	const void MoveEffectTo(std::string szID, const D3DXVECTOR2 tNewPos);

	static CFXManager* GetInstance();

private:
	CFXManager();
	//*******MEMBERS*********//
	std::vector<CFX*> m_listActiveFX;
	std::map<std::string, CFX*> m_fxTable;
};

#endif