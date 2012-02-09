///////////////////////////////////////////////////////
// File Name	:	"CAnimation.cpp"
//
// Purpose		:	To Contain all animation related data
//////////////////////////////////////////////////////

#include "CAnimation.h"
#include "CFrame.h"

CAnimation::CAnimation()
{
	m_bIsLooping = false;
	
	//Asset IDs
	m_nFrameID = -1;
	m_nImageID = -1;
}

CAnimation::~CAnimation()
{

}