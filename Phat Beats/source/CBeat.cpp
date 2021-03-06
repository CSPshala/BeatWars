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
#include "CPlayer.h"
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
	SetDifficulty(EASY);
	SetDirection(UP);
	m_nType = OBJ_BEAT;
	SetEvent("");	
	
	m_bCollision = false;
	m_bPlayer1Hit = false;
	m_bPlayer2Hit = false;
	m_bPlayer1Miss = false;
	m_bPlayer2Miss = false;

}

CBeat::~CBeat()
{

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
	m_bPlayer1Hit = theBeat.m_bPlayer1Hit;
	m_bPlayer2Hit = theBeat.m_bPlayer2Hit;	
	m_bPlayer1Miss = theBeat.m_bPlayer1Miss;
	m_bPlayer2Miss = theBeat.m_bPlayer2Miss;

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
	SetHasCollided(false);
	SetPlayer1Hit(false);
	SetPlayer2Hit(false);
	SetPlayer1Miss(false);
	SetPlayer2Miss(false);
}

void CBeat::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
	

	if (GetPosX() < 0 && GetPosY() < 0)
	{
		CMessageSystem::GetInstance()->SendMsg(new CCreateTestMsg());
	}
		


}

void CBeat::Render()
{
	// Hitbox rect for player1's notes for debugging purposes
	//D3D->DrawRect(GetCollisionRect(),0,120,0);

	// Only draws the object if it's actually on screen
	if(GetPosX() >= 0.0f && GetPosX() <= (float)CGame::GetInstance()->GetScreenWidth())
		if(GetPosY() >= 0.0f && GetPosY() <= (float)CGame::GetInstance()->GetScreenHeight())
		{
			// Drawing player1 notes if not already hit
			if((!GetPlayer1Hit() && !GetPlayer1Miss()) || !GetHasCollided())
				CSGD_TextureManager::GetInstance()->DrawF(GetImageID(),GetPosX() - 16,GetPosY() - 16,0.5f,0.5f);
			// Drawing player2 notes if not already hit. And if hasn't collided (for AI)
			if((!GetPlayer2Hit() && !GetPlayer2Miss()) || !GetHasCollided())
					CSGD_TextureManager::GetInstance()->DrawF(GetImageID(),GetPosX() + 400 -16,GetPosY() - 16,0.5f,0.5f);
			
		}

	// Drawing when player hits note
	if(GetPlayer1Hit())
		D3D->DrawText("P1 Hit",10,10,255,0,0);
		
}

RECT CBeat::GetCollisionRect()
{

	RECT rTemp;
	rTemp.left = (LONG)GetPosX() - 16;
	rTemp.top = (LONG)GetPosY() - 16;
	rTemp.right = rTemp.left + GetWidth();
	rTemp.bottom = rTemp.top + GetHeight();

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
	
}

////////////////////////////////////////
//	    PRIVATE ACCESSORS / MUTATORS
////////////////////////////////////////


