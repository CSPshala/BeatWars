///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CParticle.cpp"
//	
//	Purpose		:	To hold data for a single particle
///////////////////////////////////////////////////////////////////////////

#include "CParticle.h"

CParticle::CParticle()
{

}

CParticle::~CParticle()
{

}

void CParticle::Update(const float fElapsedTime, const D3DXVECTOR2& tGravity, const float fGravitationalPull)
{
	// Update Color
	m_Overlay.vValues.x += m_fRedDelta * fElapsedTime;
	m_Overlay.vValues.y += m_fGreenDelta * fElapsedTime;
	m_Overlay.vValues.z += m_fBlueDelta * fElapsedTime;
	m_Overlay.vValues.w += m_fAlphaDelta * fElapsedTime;

	// Update Velocity
	m_tVelocity.x += m_tVelocityDelta.x * fElapsedTime;
	m_tVelocity.y += m_tVelocityDelta.y * fElapsedTime;

	D3DXVECTOR2 tDeltaPosWell = tGravity - m_tPosition;
	D3DXVec2Normalize(&tDeltaPosWell, &tDeltaPosWell);
	tDeltaPosWell = tDeltaPosWell * (fGravitationalPull * fElapsedTime);
	m_tVelocity = m_tVelocity + tDeltaPosWell;

	// Update Position
	m_tPosition = m_tPosition + (m_tVelocity * fElapsedTime);

	// Update Scale & Rotation
	m_fScale += m_fScaleDelta * fElapsedTime;
	m_fRotation += m_fRotationDelta * fElapsedTime;

	// Update Life
	m_fCurrentLife += fElapsedTime;
}

void CParticle::Render(const std::vector<int>& vTextures)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	if(m_nImageID >= 0)
	{
		m_tOrigin.x = ((float)(pTM->GetTextureWidth(m_nImageID) >> 1)) * GetScale();
		m_tOrigin.y = ((float)(pTM->GetTextureHeight(m_nImageID) >> 1))  * GetScale();
		pTM->DrawF(m_nImageID, m_tPosition.x - m_tOrigin.x, m_tPosition.y - m_tOrigin.y, GetScale(), GetScale(), nullptr, m_tOrigin.x, m_tOrigin.y, GetRotation(), m_Overlay.TOD3DCOLOR());
	}
}

const float const CParticle::GetColorDelta(const ColorValue nColorIndex)
{
	switch(nColorIndex)
	{
	case R:
		return m_fRedDelta;
		break;

	case G:
		return m_fGreenDelta;
		break;

	case B:
		return m_fBlueDelta;
		break;

	case A:
		return m_fAlphaDelta;
		break;
	}
}
const void CParticle::SetColorDelta(const ColorValue nColorIndex, const float fNewValue)
{
	switch(nColorIndex)
	{
	case R:
		m_fRedDelta = fNewValue;
		break;

	case G:
		m_fGreenDelta = fNewValue;
		break;

	case B:
		m_fBlueDelta = fNewValue;
		break;

	case A:
		m_fAlphaDelta = fNewValue;
		break;
	}
}