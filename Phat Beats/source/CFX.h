///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CFX.h"
//	
//	Purpose		:	To wrap up data for an effect
///////////////////////////////////////////////////////////////////////////

#ifndef C_FX_H
#define C_FX_H

#include <list>
using std::list;

#include "CEmitter.h"

class CFX
{
public:
	CFX();
	~CFX();

private:

	//*****MEMBERS******//
	list<CEmitter> m_listEmitters;

	// Asset IDs
	int m_nSoundID;
	int m_nFXID;

};

#endif