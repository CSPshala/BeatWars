///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CParticle.h"
//	
//	Purpose		:	To hold data for a single particle
///////////////////////////////////////////////////////////////////////////

#ifndef C_PARTICLE_H
#define C_PARTICLE_H

#include "Timer.h"
#include "SGD_Math.h"
#include "../ColorF.h"
#include "SGD Wrappers/CSGD_TextureManager.h"

enum ColorValue {R = 0, G, B, A};

class CParticle
{
private:

	// Data Members (Current)
	D3DXVECTOR2 m_tPosition;
	D3DXVECTOR2 m_tVelocity;
	tColorF m_Overlay;
	float m_fScale;
	float m_fRotation;
	float m_fCurrentLife;
	float m_fDuration;
	int m_nImageID;

	// Data Members (Deltas)
	D3DXVECTOR2 m_tVelocityDelta;
	D3DXVECTOR2 m_tOrigin;
	float m_fRedDelta;
	float m_fGreenDelta;
	float m_fBlueDelta;
	float m_fAlphaDelta;
	float m_fScaleDelta;
	float m_fRotationDelta;

public:
	CParticle();
	~CParticle();

	void Update(const float fElapsedTime, const D3DXVECTOR2& tGravity, const float fGravitationalPull);
	void Render(const std::vector<int>& vTextures);

	// Accessors
	inline const D3DXVECTOR2 GetPosition(void) {return m_tPosition;}
	inline const D3DXVECTOR2 GetVelocity(void) {return m_tVelocity;}
	inline const tColorF& GetOverlay(void) {return m_Overlay;}
	inline const float GetScale(void) {return m_fScale;}
	inline const float GetRotation(void) {return m_fRotation;}
	inline const float GetCurrentLife(void) {return m_fCurrentLife;}
	inline const float GetDuration(void) {return m_fDuration;}
	inline const int GetImageID(void) {return m_nImageID;}

	inline const D3DXVECTOR2 GetVelocityDelta(void) {return m_tVelocityDelta;}
	inline const D3DXVECTOR2 GetOrigin(void) {return m_tOrigin;}
	inline const float GetScaleDelta(void) {return m_fScaleDelta;}
	inline const float GetRotationDelta(void) {return m_fRotationDelta;}
	const float GetColorDelta(const ColorValue nColorIndex);

	// Mutators
	inline const void SetPosition(const D3DXVECTOR2& tNewPos) {m_tPosition = tNewPos;}
	inline const void SetVelocity(const D3DXVECTOR2& tNewVel) {m_tVelocity = tNewVel;}
	inline const void SetColor(const tColorF& tNewColor) {m_Overlay = tNewColor;}
	inline const void SetScale(const float fNewScale) {m_fScale = fNewScale;}
	inline const void SetRotation(const float fNewRotation) {m_fRotation = fNewRotation;}
	inline const void SetCurrentLife(const float fNewLife) {m_fCurrentLife = fNewLife;}
	inline const void SetDuration(const float fNewDuration) {m_fDuration = fNewDuration;}
	inline const void SetImageID(const int nNewID) {m_nImageID = nNewID;}

	inline const void SetVelocityDelta(const D3DXVECTOR2& tNewVelDelta) {m_tVelocityDelta = tNewVelDelta;}
	inline const void SetOrigin(const D3DXVECTOR2& tNewOrigin) {m_tOrigin = tNewOrigin;}
	inline const void SetScaleDelta(const float fNewScaleDelta) {m_fScaleDelta = fNewScaleDelta;}
	inline const void SetRotationDelta(const float fNewRotationDelta) {m_fRotationDelta = fNewRotationDelta;}
	const void SetColorDelta(const ColorValue nColorIndex, const float fNewValue);
};

#endif