///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CParticle.h"
//	
//	Purpose		:	To hold data for a single particle
///////////////////////////////////////////////////////////////////////////

#ifndef C_PARTICLE_H
#define C_PARTICLE_H

#include "CBase.h"


class CParticle : public CBase
{
public:
	CParticle();
	~CParticle();

private:

	//********MEMBERS**********//
	float m_fDuration;
//	D3DCOLOR m_nCurrentColor;
	float m_fCurrentScale;
	float m_fCurrentRotation;

};

#endif