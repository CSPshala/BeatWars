///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CEmitter.h"
//	
//	Purpose		:	To hold data for an emitter
///////////////////////////////////////////////////////////////////////////

#ifndef C_EMITTER_H
#define C_EMITTER_H

#include <list>
using std::list;

#include "CParticle.h"

class CEmitter
{
public:
	CEmitter();
	~CEmitter();

	void Reset();

private:

	//********MEMBERS**********//
	int		 m_nPosX;
	int		 m_nPosY;
	float	 m_fStartVelX;
	float	 m_fEndVelX;
	float	 m_fStartVelY;
	float	 m_fEndVelY;
	int		 m_nStartSize;
	int		 m_nEndSize;
	float	 m_fStartRotation;
	float	 m_fEndRotation;
	int		 m_nSpawnRate;
	float	 m_fParticleLife;
	int		 m_nMaxParticles;
	//D3DCOLOR m_nStartColor;
	//D3DCOLOR m_nEndColor;
	
	list<CParticle> m_listAliveParticles;
	list<CParticle> m_listDeadParticles;

	// Asset IDs
	int m_nImageID;

};

#endif