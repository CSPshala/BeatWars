///////////////////////////////////////////////////////
// File Name	:	"CAnimationManager.h"
//
// Purpose		:	To manage all loaded animations
//////////////////////////////////////////////////////

#ifndef C_ANIMATIONMANAGER_H
#define C_ANIMATIONMANAGER_H

#include <string>
#include <vector>
using std::string;
using std::vector;


//Forward Declarations
class CAnimation;

class CAnimationManager
{
	public:
	
		vector<CAnimation*> LoadAnimation(string szFileName, string szImageName);
		bool UnloadAnimations(vector<CAnimation*> vecAnim);
			
		CAnimationManager();
	private:


};

#endif