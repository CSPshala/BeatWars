///////////////////////////////////////////////////////
// File Name	:	"CBeat.h"
//
// Purpose		:	To contain beat / note info
//////////////////////////////////////////////////////
#ifndef C_BEAT_H
#define C_BEAT_H

#include "CBase.h"

class CBeat : public CBase
{
public:
	CBeat();
	~CBeat();

private:	

	//******MEMBERS*********//
	float m_fTimeofBeat;
	int m_nDifficulty;

};

#endif