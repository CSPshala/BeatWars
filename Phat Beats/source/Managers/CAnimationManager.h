///////////////////////////////////////////////////////
// File Name	:	"CAnimationManager.h"
//
// Purpose		:	To manage all loaded animations
//////////////////////////////////////////////////////

#ifndef C_ANIMATIONMANAGER_H
#define C_ANIMATIONMANAGER_H

#include <string>
#include <list>
using std::string;
using std::list;

//Forward Declarations
class CAnimation;

class CAnimationManager
{
public:
	CAnimationManager();
	~CAnimationManager();

	bool LoadAnimation(string szFileName);
	bool UnloadAnimations();

private:
	list<CAnimation*> m_listAnimations;

};

#endif