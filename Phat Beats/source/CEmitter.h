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
#include "../ColorF.h"

class CEmitter
{

private:
	//********MEMBERS**********//
	std::vector<CParticle*> m_ListAliveParticles;
	std::vector<CParticle*> m_ListDeadParticles;
	std::vector<int> m_vTextureList;

	// Data members
	std::string m_szName;
	RECT m_rectRange;
	D3DXVECTOR2 m_tGravityPos;
	float m_fGravitationalPull;
	float m_fParticleDurationMin;
	float m_fPatricleDurationMax;
	float m_fLifeSpan;
	float m_fSpawnRate;
	short m_nMaxParticles;
	short m_nNumParticlesToSpit;

	// Options
	bool m_bRandStartX;
	bool m_bRandStartY;
	bool m_bRandEndX;
	bool m_bRandEndY;

	// Start & Ends
	tColorF m_tStartColor;
	tColorF m_tEndColor;
	D3DBLEND m_d3dSource;
	D3DBLEND m_d3dDestination;
	D3DXVECTOR2 m_vStartVelocity;
	D3DXVECTOR2 m_vEndVelocity;
	float m_fStartScale;
	float m_fEndScale;
	float m_fStartRotation;
	float m_fEndRotation;

	// Data
	float m_fCurLife;
	float m_fUpdate;

public:
	CEmitter();
	~CEmitter();

	void RecycleParticle();
	void Update(float fElapsedTime);
	void Render();

	// Accessors
	inline std::vector<int>* GetTextureList(void) {return &m_vTextureList;}
	inline const std::string& GetName(void) {return m_szName;}
	inline const RECT& GetRange(void) {return m_rectRange;}
	inline const D3DXVECTOR2& GetGravityPosition(void) {return m_tGravityPos;}
	inline const float GetGravitationalPull(void) {return m_fGravitationalPull;}
	inline const float GetParticleDurationMin(void) {return m_fParticleDurationMin;}
	inline const float GetParticleDurationMax(void) {return m_fPatricleDurationMax;}
	inline const float GetLifeSpan(void) {return m_fLifeSpan;}
	inline const float GetSpawnRate(void) {return m_fSpawnRate;}
	inline const float GetCurrentLife(void) {return m_fCurLife;}
	inline const short GetMaxParticles(void) {return m_nMaxParticles;}
	inline const short GetNumToSpit(void) {return m_nNumParticlesToSpit;}
	inline const bool GetRandStartX(void) {return m_bRandStartX;}
	inline const bool GetRandStartY(void) {return m_bRandStartY;}
	inline const bool GetRandEndX(void) {return m_bRandEndX;}
	inline const bool GetRandEndY(void) {return m_bRandEndY;}
	inline const tColorF& GetStartColor(void) { return m_tStartColor;}
	inline const tColorF& GetEndColor(void) { return m_tEndColor;}
	inline const D3DBLEND GetSourceBlend(void) {return m_d3dSource;}
	inline const D3DBLEND GetDestinationBlend(void) {return m_d3dDestination;}
	inline const D3DXVECTOR2& GetStartVelocity(void) { return m_vStartVelocity;}
	inline const D3DXVECTOR2& GetEndVelocity(void) { return m_vEndVelocity;}
	inline const float GetStartScale(void) { return m_fStartScale;}
	inline const float GetEndScale(void) { return m_fEndScale;}
	inline const float GetStartRotation(void) { return m_fStartRotation;}
	inline const float GetEndRotation(void) { return m_fEndRotation;}

	// Mutators
	inline const void SetName(const std::string szNewName) {m_szName = szNewName;}
	inline const void SetRange(const RECT& rectNewRange) {m_rectRange = rectNewRange;}
	inline const void SetGravityPosition(const D3DXVECTOR2& vNewPos){m_tGravityPos = D3DXVECTOR2(GetRange().left + vNewPos.x, GetRange().top + vNewPos.y);}
	inline const void SetGravitationalPull(const float fNewPull) {m_fGravitationalPull = fNewPull;}
	inline const void SetParticleDurationMin(const float fNewDurationMin) {m_fParticleDurationMin = fNewDurationMin;}
	inline const void SetParticleDurationMax(const float fNewDurationMax) {m_fPatricleDurationMax = fNewDurationMax;}
	inline const void SetLifeSpawn(const float fNewLife) {m_fLifeSpan = fNewLife;}
	inline const void SetSpawnRate(const float fNewSpawnRate) {m_fSpawnRate = fNewSpawnRate;}
	inline const void SetCurrentLife(const float fNewLife) {m_fCurLife = fNewLife;}
	const void SetMaxParticles(const short nNewMaxParticles);
	inline const void SetNumParticlesToSpit(const short nNewNumToSpit) {m_nNumParticlesToSpit = nNewNumToSpit;}
	inline const void SetRandStartX(const bool bNewRandStartX) {m_bRandStartX = bNewRandStartX;}
	inline const void SetRandStartY(const bool bNewRandStartY) {m_bRandStartY = bNewRandStartY;}
	inline const void SetRandEndX(const bool bNewRandEndX) {m_bRandEndX = bNewRandEndX;}
	inline const void SetRandEndY(const bool bNewRandEndY) {m_bRandEndY = bNewRandEndY;}
	inline const void SetStartColor(const tColorF& tNewStartColor) {m_tStartColor = tNewStartColor;}
	inline const void SetEndColor(const tColorF& tNewEndColor) {m_tEndColor = tNewEndColor;}
	inline const void SetSourceBlend(const D3DBLEND nNewBlend) {m_d3dSource = nNewBlend;}
	inline const void SetDestinationBlend(const D3DBLEND nNewBlend) {m_d3dDestination = nNewBlend;}
	inline const void SetStartVelocity(const D3DXVECTOR2& vNewStartVelocity) {m_vStartVelocity = vNewStartVelocity;}
	inline const void SetEndVelocity(const D3DXVECTOR2& vNewEndVelocity) {m_vEndVelocity = vNewEndVelocity;}
	inline const void SetStartScale(const float fNewStartScale) {m_fStartScale = fNewStartScale;}
	inline const void SetEndScale(const float fNewEndScale) {m_fEndScale = fNewEndScale;}
	inline const void SetStartRotation(const float fNewStartRotation) {m_fStartRotation = fNewStartRotation;}
	inline const void SetEndRotation(const float fNewEndRotation) {m_fEndRotation = fNewEndRotation;}
	inline const void SetTick(const float fTick) {m_fUpdate = fTick;}
};

#endif