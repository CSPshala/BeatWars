///////////////////////////////////////////////////////
// File Name	:	"CBeat.cpp"
//
// Author		:	JC Ricks(@CSPshala)
//
// Purpose		:	To Contain a single beat's info
//////////////////////////////////////////////////////

////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include "CBeat.h"
#include "JCMacros.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "CGame.h"

////////////////////////////////////////
//				MISC
////////////////////////////////////////

///////////////////////////////////////////////
//  CONSTRUCTOR / DECONSTRUCT / OP OVERLOADS
///////////////////////////////////////////////

CBeat::CBeat() : CBase()
{
	SetTimeOfBeat(0);
	SetDifficulty(0);
	SetDirection(UP);
	m_nType = OBJ_BEAT;
	SetEvent("");
	CEventSystem::GetInstance()->RegisterClient("test.event",this);
	CEventSystem::GetInstance()->RegisterClient("notecollision",this);
	m_bCollision = false;

}

CBeat::~CBeat()
{
	CEventSystem::GetInstance()->UnregisterClient("test.event",this);
	CEventSystem::GetInstance()->UnregisterClient("notecollision",this);
}

CBeat::CBeat(const CBeat& theBeat)
{
	*this = theBeat;
}

CBeat& CBeat::operator=(const CBeat& theBeat)
{
	CBase::operator=(theBeat);
	
	m_nTimeofBeat = theBeat.m_nTimeofBeat;
	m_nDifficulty = theBeat.m_nDifficulty;
	m_cKeyToPress = theBeat.m_cKeyToPress;
	m_eDirection = theBeat.m_eDirection;
	m_szEvent = theBeat.m_szEvent;

	m_fOriginalXPos = theBeat.m_fOriginalXPos;
	m_fOriginalYPos = theBeat.m_fOriginalYPos;

	m_bCollision = theBeat.m_bCollision;

	CEventSystem::GetInstance()->RegisterClient("test.event",this);
	CEventSystem::GetInstance()->RegisterClient("notecollision",this);

	return *this;
}

////////////////////////////////////////
//		PUBLIC UTILITY FUNCTIONS
////////////////////////////////////////
void CBeat::ResetBeat()
{
	SetPosX(GetOriginalXPos());
	SetPosY(GetOriginalYPos());
	SetIsActive(false);
}

void CBeat::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);

	if((GetPosX() > 200 && GetPosY() > 200))
		CEventSystem::GetInstance()->SendEvent("test.event",this);

	if (GetPosX() < 0 && GetPosY() < 0)
	{
		CMessageSystem::GetInstance()->SendMsg(new CCreateTestMsg());
	}
		

}

void CBeat::Render()
{
	CBase::Render();

	D3D->DrawRect(GetCollisionRect(),0,120,0);

	if(m_bCollision == true)
		D3D->DrawTextA("Note is colliding",400,100,255,0,0);	
}

RECT CBeat::GetCollisionRect()
{

	RECT rTemp;
	rTemp.left = (LONG)GetPosX();
	rTemp.top = (LONG)GetPosY();
	rTemp.right = (LONG)GetPosX() + GetWidth();
	rTemp.bottom = (LONG)GetPosY() + GetHeight();

	return rTemp;

}

////////////////////////////////////////
//		PRIVATE UTILITY FUNCTIONS
////////////////////////////////////////

////////////////////////////////////////
//	    PUBLIC ACCESSORS / MUTATORS
////////////////////////////////////////
void CBeat::SetDirection(BeatDirection dir)
{
	m_eDirection = dir;

	switch(dir)
	{
	case LEFT:
		SetPosX(0.0f);
		SetPosY(300.0f);
		SetVelX(201.0f);
		SetVelY(0.0f);
		break;

	case UP:
		SetPosX(200.0f);
		SetPosY(100.0f);
		SetVelX(0.0f);
		SetVelY(201.0f);
		break;

	case RIGHT:
		SetPosX(400.0f);
		SetPosY(300.0f);
		SetVelX(-201.0f);
		SetVelY(0.0f);
		break;

	case DOWN:
		SetPosX(200.0f);
		SetPosY(500.0f);
		SetVelX(0.0f);
		SetVelY(-201.0f);
		break;

	case LEFTUP:
		SetPosX(0.0f);
		SetPosY(100.0f);
		SetVelX(201.0f);
		SetVelY(201.0f);
		break;

	case RIGHTUP:
		SetPosX(400.0f);
		SetPosY(100.0f);
		SetVelX(-201.0f);
		SetVelY(201.0f);
		break;

	case RIGHTDOWN:
		SetPosX(400.0f);
		SetPosY(500.0f);
		SetVelX(-201.0f);
		SetVelY(-201.0f);
		break;

	case LEFTDOWN:
		SetPosX(0.0f);
		SetPosY(500.0f);
		SetVelX(201.0f);
		SetVelY(-201.0f);
		break;

	default:
		break;

	}

	// Now adjusting to put the center of the note
	// dead on the point instead of top left
	SetPosX(GetPosX() - (GetWidth() >> 1));
	SetPosY(GetPosY() - (GetHeight() >>1));

	// Setting original position
	SetOriginalXPos(GetPosX());
	SetOriginalYPos(GetPosY());


}

void CBeat::HandleEvent( CEvent* pEvent )
{
	if(pEvent->GetEventID() == "notecollision")
	{
		if(pEvent->GetParam() == this)
			m_bCollision = true;		
	}
}

////////////////////////////////////////
//	    PRIVATE ACCESSORS / MUTATORS
////////////////////////////////////////


