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
using std::list;
using std::string;

class CFXManager
{
public:
	CFXManager();
	~CFXManager();

	void Render();
	void Update(float fElapsedTime);
	void AddFX(int fxID);
	bool LoadFX(string szFileName);
	bool UnloadFX();

private:

	//*******MEMBERS*********//
	list<CFX> m_listActiveFX;
	list<CFX> m_listInactiveFX;

};

#endif