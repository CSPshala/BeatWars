//////////////////////////////////////////////////////////////////////////////////////////////////
//	File Name	:	"CPlayer.cpp"
//	
//	Author Name	:	JC Ricks(@CSPshala)
//	
//	Purpose		:	Hold Player info and Manage player input / rendering / beat hit detection
/////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include "CPlayer.h"
////////////////////////////////////////
//				MISC
////////////////////////////////////////

///////////////////////////////////////////////
//  CONSTRUCTOR / DECONSTRUCT / OP OVERLOADS
///////////////////////////////////////////////
CPlayer::CPlayer() : CBase()
{
	// Defaulting type to Player 1
	m_nType = OBJ_PLAYER1;

	SetMaxHP(100);
	SetCurrentHP(GetMaxHP());
	SetCurrentPower(0);
	SetMaxPower(100);
	SetTotalBeatsHit(0);
	SetCurrentStreak(0);
	SetCurrentScore(0);
	SetTotalScore(0);
	// Defaults to easy
	SetPlayerDifficulty(EASY);
	// Setting aiming to upwards
	SetAimingDirection(UP);
}

CPlayer::~CPlayer()
{
}

////////////////////////////////////////
//		PUBLIC UTILITY FUNCTIONS
////////////////////////////////////////
void CPlayer::Input()
{

}

void CPlayer::Update(float fElapsedTime)
{

}

void CPlayer::Render()
{

}


////////////////////////////////////////
//		PRIVATE UTILITY FUNCTIONS
////////////////////////////////////////

////////////////////////////////////////
//	    PUBLIC ACCESSORS / MUTATORS
////////////////////////////////////////
void CPlayer::SetAimingDirection(BeatDirection eAimingDirection)
{
	m_eAimingDirection = eAimingDirection;

	switch(m_eAimingDirection)
	{
	case LEFT:
		SetCurrentRotation(270.0f);
		break;
		
	case UP:
		SetCurrentRotation(0.0f);
		break;

	case RIGHT:
		SetCurrentRotation(90.0f);
		break;

	case DOWN:
		SetCurrentRotation(180.0f);
		break;

	case LEFTUP:
		SetCurrentRotation(314.0f);
		break;

	case RIGHTUP:
		SetCurrentRotation(45.0f);
		break;

	case RIGHTDOWN:
		SetCurrentRotation(135.0f);
		break;

	case LEFTDOWN:
		SetCurrentRotation(225.0f);
		break;

	}
}
////////////////////////////////////////
//	    PRIVATE ACCESSORS / MUTATORS
////////////////////////////////////////