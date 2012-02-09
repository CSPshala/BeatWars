///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CEmitter.cpp"
//	
//	Purpose		:	To hold data for an emitter
///////////////////////////////////////////////////////////////////////////

#include "CEmitter.h"

CEmitter::CEmitter()
{
	 m_nPosX = 0;
	 m_nPosY = 0;
	 m_fStartVelX = 0.0f;
	 m_fEndVelX = 0.0f;
	 m_fStartVelY = 0.0f;
	 m_fEndVelY = 0.0f;
	 m_nStartSize = 0;
	 m_nEndSize = 0;
	 m_fStartRotation = 0.0f;
	 m_fEndRotation = 0.0f;
	 m_nSpawnRate = 0;
	 m_fParticleLife = 0.0f;
	 m_nMaxParticles = 0;
//	 m_nStartColor = 0;
	// m_nEndColor = 0;
	 
	 // Assets
	 m_nImageID = -1;
}

CEmitter::~CEmitter()
{

}

void CEmitter::Reset()
{

}