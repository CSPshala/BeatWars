///////////////////////////////////////////////////////
// File Name	:	"CBeat.cpp"
//
// Author		:	JC Ricks
//
// Purpose		:	To Contain a single beat's info
//////////////////////////////////////////////////////

////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include "CBeat.h"

////////////////////////////////////////
//				MISC
////////////////////////////////////////

///////////////////////////////////////////////
//  CONSTRUCTOR / DECONSTRUCT / OP OVERLOADS
///////////////////////////////////////////////
CBeat::CBeat() : CBase()
{
	SetTimeOfBeat(0.0f);
	SetDifficulty(0);
	SetDirection(UP);
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

	m_fTimeofBeat = theBeat.m_fTimeofBeat;
	m_nDifficulty = theBeat.m_nDifficulty;

	return *this;
}

////////////////////////////////////////
//		PUBLIC UTILITY FUNCTIONS
////////////////////////////////////////

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
		SetPosY(200.0f);
		SetVelX(10.0f);
		SetVelY(0.0f);
		break;

	case UP:
		SetPosX(200.0f);
		SetPosY(0.0f);
		SetVelX(0.0f);
		SetVelY(10.0f);
		break;

	case RIGHT:
		SetPosX(400.0f);
		SetPosY(200.0f);
		SetVelX(-10.0f);
		SetVelY(0.0f);
		break;

	case DOWN:
		SetPosX(200.0f);
		SetPosY(400.0f);
		SetVelX(0.0f);
		SetVelY(-10.0f);
		break;

	case LEFTUP:
		SetPosX(0.0f);
		SetPosY(0.0f);
		SetVelX(10.0f);
		SetVelY(10.0f);
		break;

	case RIGHTUP:
		SetPosX(400.0f);
		SetPosY(0.0f);
		SetVelX(-10.0f);
		SetVelY(10.0f);
		break;

	case RIGHTDOWN:
		SetPosX(400.0f);
		SetPosY(400.0f);
		SetVelX(-10.0f);
		SetVelY(-10.0f);
		break;

	case LEFTDOWN:
		SetPosX(0.0f);
		SetPosY(400.0f);
		SetVelX(10.0f);
		SetVelY(-10.0f);
		break;

	default:
		break;

	}
}
////////////////////////////////////////
//	    PRIVATE ACCESSORS / MUTATORS
////////////////////////////////////////


