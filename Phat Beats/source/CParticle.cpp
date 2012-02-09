///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CParticle.cpp"
//	
//	Purpose		:	To hold data for a single particle
///////////////////////////////////////////////////////////////////////////

#include "CParticle.h"

CParticle::CParticle() : CBase()
{
	m_fDuration = 0.0f;
//	m_nCurrentColor = D3DCOLOR_XRGB(0,0,0);
	m_fCurrentScale = 1.0f;
	m_fCurrentRotation = 0.0f;
}

CParticle::~CParticle()
{

}