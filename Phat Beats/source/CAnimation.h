///////////////////////////////////////////////////////
// File Name	:	"CAnimation.h"
//
// Purpose		:	To Contain all animation related data
//////////////////////////////////////////////////////

#ifndef C_ANIMATION_H
#define C_ANIMATION_H

#include <list>
using std::list;

// Forward declarations
class CFrame;

class CAnimation
{
public:
	CAnimation();
	~CAnimation();

private:


	//*********MEMBERS**********//
	list<CFrame*> m_listFrames;
	bool m_bIsLooping;

	// Asset IDs
	int m_nFrameID;
	int m_nImageID;
};

#endif