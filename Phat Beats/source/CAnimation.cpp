///////////////////////////////////////////////////////
// File Name	:	"CAnimation.cpp"
//
// Purpose		:	To Contain all animation related data
//////////////////////////////////////////////////////

#include "CAnimation.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "CFrame.h"
#include "Managers/CEventSystem.h"

CAnimation::CAnimation()
{
	m_bIsLooping = true;

	//Asset IDs
	m_nFrameID = -1;
	m_nImageID = -1;


	m_bIsPlaying = true;
	m_PlayedAlready = false;
	m_nCurrFrame = 0;
	m_fTimeWaited = 0.0;
	m_nMaxFrame = 0;
}
CAnimation::~CAnimation()
{

}
void CAnimation::SetIsLooping(bool bLoop)
{
	m_bIsLooping = bLoop;
}
void CAnimation::SetName(string szName)
{
	m_szName = szName;
}
void CAnimation::SetImageID(int nImageID)
{
	m_nImageID = nImageID; 
}
void CAnimation::Play()						
{
	m_bIsPlaying = true;
	m_PlayedAlready = false;
}
void CAnimation::Stop()						
{
	m_bIsPlaying = false;
}
void CAnimation::Reset()						
{
	m_bIsPlaying = true;
	m_PlayedAlready = false;
	m_fTimeWaited = 0.0;
	m_nCurrFrame = 0;

}


void CAnimation::Update(float fElapsedTime)	
{
	if( m_nMaxFrame == 0 )
		m_nMaxFrame = m_vecFrames.size();

	if( !m_bIsPlaying )
		return;

	m_fTimeWaited += fElapsedTime;

	if( m_vecFrames.size() > 0)
	{
		if(m_vecFrames[m_nCurrFrame]->GetEvent() != "" )
		{
			CEventSystem::GetInstance()->SendEvent("Particle.Animation.Event");
		}

		if (m_fTimeWaited > m_vecFrames[m_nCurrFrame]->GetDuration())
		{
			m_fTimeWaited -= m_vecFrames[m_nCurrFrame]->GetDuration();
			m_nCurrFrame++;

			if( (unsigned)m_nCurrFrame >= m_vecFrames.size())
			{
				if( m_bIsLooping )
				{
					m_fTimeWaited = 0.0;
					m_nCurrFrame = 0;
				}
				else
				{
					Stop();
					m_nCurrFrame = m_vecFrames.size() - 1;
					m_PlayedAlready = true;
				}
			} 	
		}	
	}



}



void CAnimation::Render(int posX, int posY, float fScale)
{
	if( !m_bIsPlaying )
		return;

	if( m_vecFrames.size() > 0)
	{
		RECT DrawRect;

		DrawRect.left = m_vecFrames[m_nCurrFrame]->GetDrawX();
		DrawRect.top = m_vecFrames[m_nCurrFrame]->GetDrawY();
		DrawRect.right = m_vecFrames[m_nCurrFrame]->GetDrawX() + m_vecFrames[m_nCurrFrame]->GetWidth();
		DrawRect.bottom = m_vecFrames[m_nCurrFrame]->GetDrawY() + m_vecFrames[m_nCurrFrame]->GetHeight();
		
		CSGD_TextureManager::GetInstance()->Draw(GetImageID(),posX -( m_vecFrames[m_nCurrFrame]->GetAnchorX() ), posY - ( m_vecFrames[m_nCurrFrame]->GetAnchorY() ), fScale,1.0, &DrawRect );
	}
}

int CAnimation::GetImageID()
{
	return m_nImageID;
}

void CAnimation::SetIsPlaying(bool lPlay)
{
	m_bIsPlaying = lPlay;
}