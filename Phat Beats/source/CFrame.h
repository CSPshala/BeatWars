///////////////////////////////////////////////////////
// File Name	:	"CFrame.h"
//
// Purpose		:	To Contain a single animation frame's data
//////////////////////////////////////////////////////

#ifndef C_FRAME_H
#define C_FRAME_H

class CFrame
{
public:
	CFrame();
	~CFrame();

private:

	//**********MEMBERS**********//
	int m_nAnchorX;
	int m_nAnchorY;
	float m_fDuration;
	int m_nDrawX;
	int m_nDrawY;
};

#endif