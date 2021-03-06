///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CBase.cpp"
//	
//	Purpose		:	To contain all related data and
//					functionality for our game objects.
///////////////////////////////////////////////////////////////////////////
#include "CBase.h"
#include "CGame.h"
#include "SGD Wrappers/CSGD_TextureManager.h"	//	Include where used

CBase::CBase(void)
{
	m_uiRefCount = 1;	//	 start with a ref to yourself
	SetPosX(0);
	SetPosY(0);
	SetVelX(0);
	SetVelY(0);
	SetHeight(0);
	SetWidth(0);
	SetIsActive(false);
	SetImageID(-1);
}

CBase::~CBase(void)
{

}

CBase::CBase(const CBase& aBase)
{
	*this = aBase;
}

CBase& CBase::operator=(const CBase& aBase)
{
	m_uiRefCount = aBase.m_uiRefCount;
	m_nPosX = aBase.m_nPosX;
	m_nPosY = aBase.m_nPosY;
	m_nVelX = aBase.m_nVelX;
	m_nVelY = aBase.m_nVelY;
	m_nWidth = aBase.m_nWidth;
	m_nHeight = aBase.m_nHeight;
	m_nImageID = aBase.m_nImageID;
	m_bIsActive = aBase.m_bIsActive;	
	m_nType = aBase.m_nType;

	return *this;
}

void CBase::Update(float fElapsedTime)
{
	//	pos += vel		<--	frame based
	//	pos += vel * dt	<--	time based
	SetPosX(GetPosX()+GetVelX() * fElapsedTime);
	SetPosY(GetPosY()+GetVelY() * fElapsedTime);
}

void CBase::Render(void)
{
	// Only draws the object if it's actually on screen
	if(GetPosX() >= 0.0f && GetPosX() <= (float)CGame::GetInstance()->GetScreenWidth())
		if(GetPosY() >= 0.0f && GetPosY() <= (float)CGame::GetInstance()->GetScreenHeight())
			CSGD_TextureManager::GetInstance()->DrawF(GetImageID(),GetPosX(),GetPosY());
}

RECT CBase::GetCollisionRect()
{
	RECT rTemp;
	rTemp.left = (LONG)GetPosX();
	rTemp.top = (LONG)GetPosY();
	rTemp.right = (LONG)GetPosX() + GetWidth();
	rTemp.bottom = (LONG)GetPosY() + GetHeight();

	return rTemp;
}

bool CBase::CheckCollision(IBaseInterface* pBase)
{
	RECT rTemp;
	if(IntersectRect(&rTemp,&GetCollisionRect(),&pBase->GetCollisionRect()))
	{
		return true;
	}
	else
		return false;
}
