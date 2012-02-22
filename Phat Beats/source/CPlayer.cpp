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
#include "JCMacros.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "Managers\CAiManager.h"
#include "Managers\CEvent.h"
#include "Managers\CEventSystem.h"
////////////////////////////////////////
//				MISC
////////////////////////////////////////

///////////////////////////////////////////////
//  CONSTRUCTOR / DECONSTRUCT / OP OVERLOADS
///////////////////////////////////////////////
CPlayer::CPlayer(ObjType eType) : CBase()
{
	// Defaulting type to Player 1
	m_nType = eType;

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

	switch(m_nType)
	{
	case OBJ_PLAYER1:
		SetBeatConeID(TEXTUREMAN->LoadTexture("resource/graphics/p1cone.png"));
		SetPosX(200.0f - 64.0f);  // Offsetting to get the base of the cone right on point
		SetPosY(300.0f - 128.0f);
		break;

	case OBJ_PLAYER2:
		SetBeatConeID(TEXTUREMAN->LoadTexture("resource/graphics/p2cone.png"));
		SetPosX(600.0f - 64.0f); // Ditto
		SetPosY(300.0f - 128.0f);
		break;
	case OBJ_AI:
		SetBeatConeID(TEXTUREMAN->LoadTexture("resource/graphics/p1cone.png"));
		SetPosX(200.0f - 64.0f); // Ditto
		SetPosY(300.0f - 128.0f);
		break;
	}

	// Event system register
	CEventSystem::GetInstance()->RegisterClient("notecollision",this);
	m_IbwriteShit == false;
}

CPlayer::~CPlayer()
{
	CEventSystem::GetInstance()->UnregisterClient("notecollision",this);
}

////////////////////////////////////////
//		PUBLIC UTILITY FUNCTIONS
////////////////////////////////////////
void CPlayer::Input()
{
	//*******DONT USE THIS*********//
	// INPUT IS BEING HANDLED IN THE UPDATE//
}

void CPlayer::Update(float fElapsedTime)
{
	// Just splitting up input for both players so this dosen't get all huge and gross
	// (like your mom)
	switch(m_nType)
	{
	case OBJ_PLAYER1:
		P1InputHandling();
		break;

	case OBJ_PLAYER2:
		P2InputHandling();
		break;

	case OBJ_AI:
		AIHandling();
		break;
	}
}

void CPlayer::Render()
{
	
	D3D->DrawRect(GetCollisionRect(),100,100,100);
	// Rendering cone
	TEXTUREMAN->DrawF(GetBeatConeID(),GetPosX(),GetPosY(),1.0f,1.0f,NULL,65.0f,127.0f,D3DXToRadian(GetCurrentRotation()),D3DCOLOR_ARGB(255,255,255,255));
	if (m_IbwriteShit == true)
	{
		CSGD_Direct3D::GetInstance()->DrawTextA("This is a test of the Ai hit",200,24,255,0,0);
	}
	
}


RECT CPlayer::GetCollisionRect()
{
	RECT tRect;
	tRect.left = (LONG)GetPosX() +48;
	tRect.top = (LONG)GetPosY() +112;
	tRect.right = tRect.left + 32;
	tRect.bottom = tRect.top + 32;

	return tRect;
}

bool CPlayer::CheckCollision(IBaseInterface* pBase)
{
	RECT rTemp;
	// Players will only ever collide with notes
	CBeat* pBeat = (CBeat*)pBase;

	if(IntersectRect(&rTemp,&GetCollisionRect(),&pBeat->GetCollisionRect()))
	{
		return true;
	}
	else
		return false;
}


void CPlayer::HandleEvent( CEvent* pEvent )
{
	if(pEvent->GetEventID() == "notecollision" && GetType() == OBJ_AI)
	{
		bool found = false;
				
		for(int i = 0; i < GetAIBeats().size(); ++i)
			if(m_vAIBeats[i] == pEvent->GetParam())
				found = true;
		
		if(!found)
			m_vAIBeats.push_back((CBeat*)(pEvent->GetParam()));
	}
}



////////////////////////////////////////
//		PRIVATE UTILITY FUNCTIONS
////////////////////////////////////////
void CPlayer::P1InputHandling()
{
	if((DI->KeyDown(DIK_LEFT) && DI->KeyDown(DIK_UP)) || DI->KeyDown(DIK_NUMPAD7))
		SetAimingDirection(LEFTUP);
	else if((DI->KeyDown(DIK_LEFT) && DI->KeyDown(DIK_DOWN)) || DI->KeyDown(DIK_NUMPAD1))
		SetAimingDirection(LEFTDOWN);
	else if(DI->KeyDown(DIK_LEFT) || DI->KeyDown(DIK_NUMPAD4))
		SetAimingDirection(LEFT);
	else if((DI->KeyDown(DIK_RIGHT) && DI->KeyDown(DIK_UP)) || DI->KeyDown(DIK_NUMPAD9))
		SetAimingDirection(RIGHTUP);
	else if((DI->KeyDown(DIK_RIGHT) && DI->KeyDown(DIK_DOWN)) || DI->KeyDown(DIK_NUMPAD3))
		SetAimingDirection(RIGHTDOWN);
	else if(DI->KeyDown(DIK_RIGHT) || DI->KeyDown(DIK_NUMPAD6))
		SetAimingDirection(RIGHT);
	else if(DI->KeyDown(DIK_UP) || DI->KeyDown(DIK_NUMPAD8))
		SetAimingDirection(UP);
	else if(DI->KeyDown(DIK_DOWN) || DI->KeyDown(DIK_NUMPAD2))
		SetAimingDirection(DOWN);
}

void CPlayer::P2InputHandling()
{

}

void CPlayer::AIHandling()
{
	
	if(m_vAIBeats.size() != 0)
	{
				
		if (CAiManager::GetInsatance()->RandomDifficult(0) == true)
		{
			m_IbwriteShit = true;
			
		}
		else
			m_IbwriteShit = false;

		m_vAIBeats.pop_back();

	}
}

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