#pragma once

#include "CEvent.h"
// NO NO:
/*
-50000000000000000000000000000000000000000 pts
CBase : public IBaseInterface, public IListener

// DO THIS:

CBase : public IBaseInterface

CBullet : public CBase, public IListener

*/

class IListener
{
public:
	IListener(void) {}
	virtual ~IListener(void) {}

	//	Blue print function - MUST be defined in the derived class.
	//	Mini-proc function for whenever this object receives and event
	virtual void HandleEvent(CEvent* pEvent) = 0;
};