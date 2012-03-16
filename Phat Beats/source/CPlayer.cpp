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
#include "SGD Wrappers\CSGD_FModManager.h"
#include "Managers\CFXManager.h"
#include "Managers\CBeatManager.h"
#include "Managers\CAiManager.h"
#include "Managers\CEvent.h"
#include "Managers\CEventSystem.h"
#include "Managers\CFXManager.h"
#include "Managers\CAnimationManager.h"
#include "States\COptionsState.h"
#include "States\CBitmapFont.h"
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
	SetCurrentPowerup(0);
	SetMaxPowerup(140);
	SetTotalBeatsHit(0);
	SetCurrentStreak(0);
	SetCurrentScore(0);
	SetTotalScore(0);
	SetTakeDown(0);
	SetAttackModeTimer(0);
	// Defaults to easy
	SetPlayerDifficulty(EASY);
	// Setting aiming to upwards
	SetAimingDirection(UP);	

	Player1 = NULL;
	Player2 = NULL;

	// Putting player in attack mode
	SetAttackMode(false);
	m_vecAnimations.clear();

	// Setting player hit key to random letter
	cHitKey = 'g';
	
	switch(m_nType)
	{
	case OBJ_PLAYER1:
		SetBeatConeID(TEXTUREMAN->LoadTexture("resource/graphics/p1cone.png"));
		SetPosX(200.0f - 64.0f);  // Offsetting to get the base of the cone right on point
		SetPosY(300.0f - 128.0f);
		m_nHitBoxImage = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HitBoxJedi.png");
		Player1 = new CXBOXController(1);
		break;

	case OBJ_PLAYER2:
		SetBeatConeID(TEXTUREMAN->LoadTexture("resource/graphics/p2cone.png"));
		SetPosX(600.0f - 64.0f); // Ditto
		SetPosY(300.0f - 128.0f);
		m_nHitBoxImage = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HitBoxSith.png");
		Player2 = new CXBOXController(2);
		break;
	case OBJ_AI:
		SetBeatConeID(TEXTUREMAN->LoadTexture("resource/graphics/p2cone.png"));
		SetPosX(600.0f - 64.0f); // Ditto
		SetPosY(300.0f - 128.0f);
		m_nHitBoxImage = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HitBoxSith.png");
		break;
	}

	// Event system register		
	m_IbwriteShit = false;

	m_nCurrAnim = 0;
	m_bAnimationsEmpty = false;

	m_fStreakTextTimer = 0.0f;
}
CPlayer::~CPlayer()
{		
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHitBoxImage);
	
	delete Player1;
	if (Player2 != NULL)
	{
		delete Player2;

	}

	CAnimationManager AM;
	AM.UnloadAnimations(m_vecAnimations);
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
	// Timer for streak text
	if((GetCurrentStreak() % 5) == 0 && GetCurrentStreak() != 0)
		m_fStreakTextTimer += CGame::GetInstance()->GetTimer().GetDeltaTime();

	// flushing last hit key
	cHitKey = 'g';

	//UpdateAnimations
	if( m_vecAnimations.size() > 0 )
	m_vecAnimations[m_nCurrAnim]->Update(fElapsedTime);

	// Just splitting up input for both players so this dosen't get all huge and gross
	// (like your mom)
	// Also comment out P2's handling for debugging, because right now P1 and P2 have
	// same control scheme, so you'd prolly be moving both cones as one if you don't.
	if(!CBeatManager::GetInstance()->IsPaused())
	{

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

	// Attack mode cooldown
	if(GetAttackMode())
	{
		SetAttackModeTimer(GetAttackModeTimer() + fElapsedTime);

		if(GetAttackModeTimer() >= 20.0f)
		{
			SetAttackMode(false);		// Taking us outta attack mode and resetting timer
			SetAttackModeTimer(0.0f);


			// Changing particle effect on hilt back to defense blue
			if(GetType() == OBJ_PLAYER1)
			{
				CFXManager::GetInstance()->DequeueParticle("P1ATTACK");
				CFXManager::GetInstance()->QueueParticle("P1GUARD");
			}
			else
			{
				CFXManager::GetInstance()->DequeueParticle("P2ATTACK");
				CFXManager::GetInstance()->QueueParticle("P2GUARD");
			}
		}
	}

}
void CPlayer::Render()
{

	//Render Animations
	if( m_nType == OBJ_PLAYER1 )
	{
		if( m_vecAnimations.size() > 0 )
			m_vecAnimations[m_nCurrAnim]->Render(275,500,1.0);
	}
	else
	{
		if( m_vecAnimations.size() > 0 )
			m_vecAnimations[m_nCurrAnim]->Render(595,500,1.0);

	}

	//D3D->DrawRect(GetCollisionRect(),100,100,100);
	// Rendering cone
	TEXTUREMAN->Draw(m_nHitBoxImage, GetCollisionRect().left, GetCollisionRect().top);
	TEXTUREMAN->DrawF(GetBeatConeID(),GetPosX(),GetPosY(),1.0f,1.0f,NULL,65.0f,127.0f,D3DXToRadian(GetCurrentRotation()),D3DCOLOR_ARGB(255,255,255,255));	
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
	switch(pBase->GetType())
	{

	case OBJ_BEAT:
		{
			RECT rTemp;
			RECT beatCollisionRect;
			// Players will only ever collide with notes
			CBeat* pBeat = (CBeat*)pBase;

			beatCollisionRect = pBeat->GetCollisionRect();

			// Adjusting beat for Player2 (since all beats are really located at P1's position)
			if(GetType() == OBJ_PLAYER2 || GetType() == OBJ_AI)
			{
				beatCollisionRect.left += 400;
				beatCollisionRect.right += 400;				
			}

			if(IntersectRect(&rTemp,&GetCollisionRect(),&beatCollisionRect))
			{
				pBeat->SetHasCollided(true);
				return true;
			}
			else
				return false;
		}
		break;

	default:
		return false;
		break;
	}
}
void CPlayer::HandleEvent( CEvent* pEvent )
{
	
	
}
////////////////////////////////////////
//		PRIVATE UTILITY FUNCTIONS
////////////////////////////////////////
void CPlayer::P1InputHandling()
{
	if (!Player1->IsConnected())
	{
		if((DI->KeyDown(DIK_LEFT) && DI->KeyDown(DIK_UP)) || DI->KeyDown(DIK_NUMPAD7) || DI->JoystickGetLStickDirDown(DIR_LEFT, 0) && DI->JoystickGetLStickDirDown(DIR_UP, 0))
			SetAimingDirection(LEFTUP);
		else if((DI->KeyDown(DIK_LEFT) && DI->KeyDown(DIK_DOWN)) || DI->KeyDown(DIK_NUMPAD1) || DI->JoystickGetLStickDirDown(DIR_LEFT, 0) && DI->JoystickGetLStickDirDown(DIR_DOWN, 0))
			SetAimingDirection(LEFTDOWN);
		else if(DI->KeyDown(DIK_LEFT) || DI->KeyDown(DIK_NUMPAD4) || DI->JoystickGetLStickDirDown(DIR_LEFT, 0))
			SetAimingDirection(LEFT);
		else if((DI->KeyDown(DIK_RIGHT) && DI->KeyDown(DIK_UP)) || DI->KeyDown(DIK_NUMPAD9) || DI->JoystickGetLStickDirDown(DIR_RIGHT, 0) && DI->JoystickGetLStickDirDown(DIR_UP, 0))
			SetAimingDirection(RIGHTUP);
		else if((DI->KeyDown(DIK_RIGHT) && DI->KeyDown(DIK_DOWN)) || DI->KeyDown(DIK_NUMPAD3) || DI->JoystickGetLStickDirDown(DIR_RIGHT, 0) && DI->JoystickGetLStickDirDown(DIR_DOWN, 0))
			SetAimingDirection(RIGHTDOWN);
		else if(DI->KeyDown(DIK_RIGHT) || DI->KeyDown(DIK_NUMPAD6) || DI->JoystickGetLStickDirDown(DIR_RIGHT, 0))
			SetAimingDirection(RIGHT);
		else if(DI->KeyDown(DIK_UP) || DI->KeyDown(DIK_NUMPAD8) || DI->JoystickGetLStickDirDown(DIR_UP, 0))
			SetAimingDirection(UP);
		else if(DI->KeyDown(DIK_DOWN) || DI->KeyDown(DIK_NUMPAD2) || DI->JoystickGetLStickDirDown(DIR_DOWN, 0))
			SetAimingDirection(DOWN);

		// Checking for Stance change
		if(DI->KeyPressed(DIK_SPACE) || DI->JoystickButtonPressed(2, 0))
		{
			if(GetCurrentPowerup() >= GetMaxPowerup()) // Player now switches stances on full power bar (in lieu of specials for now)
			SetAttackMode(true); // Toggling attack/defense
			SetAttackModeTimer(0);
			// Setting particle effect on hilt to show mode
			if(GetAttackMode())
			{
				SetAttackMode(!GetAttackMode()); // Toggling attack/defense
				SetAttackModeTimer(0);
				// Setting particle effect on hilt to show mode
				if(GetAttackMode())
				{
					CFXManager::GetInstance()->DequeueParticle("P1GUARD");
					CFXManager::GetInstance()->QueueParticle("P1ATTACK");
				}
				else
				{
					CFXManager::GetInstance()->DequeueParticle("P1ATTACK");
					CFXManager::GetInstance()->QueueParticle("P1GUARD");
				}

				SetCurrentPowerup(0); // Setting Power back to 0 (since we activated)
			}
		}
	}

	if (Player1->IsConnected())
	{

		if(Player1->GetState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && Player1->GetState().Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(LEFTUP);
		else if(Player1->GetState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  && Player1->GetState().Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(LEFTDOWN);
		else if(Player1->GetState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(LEFT);
		else if(Player1->GetState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && Player1->GetState().Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(RIGHTUP);
		else if(Player1->GetState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && Player1->GetState().Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(RIGHTDOWN);
		else if(Player1->GetState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(RIGHT);
		else if(Player1->GetState().Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(UP);
		else if(Player1->GetState().Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(DOWN);

		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			if(GetCurrentPowerup() >= GetMaxPowerup()) // Player now switches stances on full power bar (in lieu of specials for now)
			{
				SetAttackMode(!GetAttackMode()); // Toggling attack/defense
				SetAttackModeTimer(0);
				// Setting particle effect on hilt to show mode
				if(GetAttackMode())
				{
					CFXManager::GetInstance()->DequeueParticle("P1GUARD");
					CFXManager::GetInstance()->QueueParticle("P1ATTACK");
				}
				else
				{
					CFXManager::GetInstance()->DequeueParticle("P1ATTACK");
					CFXManager::GetInstance()->QueueParticle("P1GUARD");
				}

				SetCurrentPowerup(0); // Setting Power back to 0 (since we activated)
			}
		}
	}

	if(FMODMAN->IsSoundPlaying(CBeatManager::GetInstance()->GetCurrentlyPlayingSong()->GetSongID()))
	{
		int nSongID = CBeatManager::GetInstance()->GetCurrentlyPlayingSong()->GetSongID();
		unsigned int nTime;
		FMODMAN->GetLatestChannel(nSongID)->getPosition(&nTime,FMOD_TIMEUNIT_MS);

		if (Player1->IsConnected())
		{
			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				cHitKey = 'w';
			}
			else if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				cHitKey = 'a';
			}
			else if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
			{
				cHitKey = 's';
			}
			else if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
			{
				cHitKey = 'd';
			}
		}

		if (!Player1->IsConnected())
		{
			if(DI->KeyPressed(DIK_W) || DI->JoystickButtonPressed(3, 0))
			{		
				cHitKey = 'w';
				//m_qKeyPresses.push(TBeatHit('w',nTime));
			}
			else if(DI->KeyPressed(DIK_A) || DI->JoystickButtonPressed(0, 0))
			{
				cHitKey = 'a';
				//m_qKeyPresses.push(TBeatHit('a',nTime));
			}
			else if(DI->KeyPressed(DIK_S) || DI->JoystickButtonPressed(4, 0))
			{
				cHitKey = 's';
				//m_qKeyPresses.push(TBeatHit('s',nTime));
			}
			else if(DI->KeyPressed(DIK_D) || DI->JoystickButtonPressed(1, 0))
			{
				cHitKey = 'd';
				//m_qKeyPresses.push(TBeatHit('d',nTime));
			}
		}
		

	}

	DI->ClearInput();
}
void CPlayer::P2InputHandling()
{
	// Don't ever have p2 input running same time as P1.  Control schemes are the same
	if (!Player2->IsConnected())
	{
		if((DI->KeyDown(DIK_LEFT) && DI->KeyDown(DIK_UP)) || DI->KeyDown(DIK_NUMPAD7) || DI->JoystickGetLStickDirDown(DIR_LEFT, 1) && DI->JoystickGetLStickDirDown(DIR_UP, 1))
			SetAimingDirection(LEFTUP);
		else if((DI->KeyDown(DIK_LEFT) && DI->KeyDown(DIK_DOWN)) || DI->KeyDown(DIK_NUMPAD1) || DI->JoystickGetLStickDirDown(DIR_LEFT, 1) && DI->JoystickGetLStickDirDown(DIR_DOWN, 1))
			SetAimingDirection(LEFTDOWN);
		else if(DI->KeyDown(DIK_LEFT) || DI->KeyDown(DIK_NUMPAD4) || DI->JoystickGetLStickDirDown(DIR_LEFT, 1))
			SetAimingDirection(LEFT);
		else if((DI->KeyDown(DIK_RIGHT) && DI->KeyDown(DIK_UP)) || DI->KeyDown(DIK_NUMPAD9) || DI->JoystickGetLStickDirDown(DIR_RIGHT, 1) && DI->JoystickGetLStickDirDown(DIR_UP, 1))
			SetAimingDirection(RIGHTUP);
		else if((DI->KeyDown(DIK_RIGHT) && DI->KeyDown(DIK_DOWN)) || DI->KeyDown(DIK_NUMPAD3) || DI->JoystickGetLStickDirDown(DIR_RIGHT, 1) && DI->JoystickGetLStickDirDown(DIR_DOWN, 1))
			SetAimingDirection(RIGHTDOWN);
		else if(DI->KeyDown(DIK_RIGHT) || DI->KeyDown(DIK_NUMPAD6) || DI->JoystickGetLStickDirDown(DIR_RIGHT, 1))
			SetAimingDirection(RIGHT);
		else if(DI->KeyDown(DIK_UP) || DI->KeyDown(DIK_NUMPAD8) || DI->JoystickGetLStickDirDown(DIR_UP, 1))
			SetAimingDirection(UP);
		else if(DI->KeyDown(DIK_DOWN) || DI->KeyDown(DIK_NUMPAD2) || DI->JoystickGetLStickDirDown(DIR_DOWN, 1))
			SetAimingDirection(DOWN);

		// Checking for Stance change
		if(DI->KeyDown(DIK_SPACE) || DI->JoystickButtonPressed(2, 1))
		{
			SetAttackMode(true); // Toggling attack/defense
			SetAttackModeTimer(0);

			if(GetAttackMode())
			{
				CFXManager::GetInstance()->DequeueParticle("P2GUARD");
				CFXManager::GetInstance()->QueueParticle("P2ATTACK");
			}
			else
			{
				CFXManager::GetInstance()->DequeueParticle("P2ATTACK");
				CFXManager::GetInstance()->QueueParticle("P2GUARD");
			}
		}
	}

	if(Player2->IsConnected())
	{

		if(Player2->GetState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && Player2->GetState().Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(LEFTUP);
		else if(Player2->GetState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  && Player2->GetState().Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(LEFTDOWN);
		else if(Player2->GetState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(LEFT);
		else if(Player2->GetState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && Player2->GetState().Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(RIGHTUP);
		else if(Player2->GetState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && Player2->GetState().Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(RIGHTDOWN);
		else if(Player2->GetState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(RIGHT);
		else if(Player2->GetState().Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(UP);
		else if(Player2->GetState().Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			SetAimingDirection(DOWN);

		if (Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			if(GetCurrentPowerup() >= GetMaxPowerup()) // Player now switches stances on full power bar (in lieu of specials for now)
			{
				SetAttackMode(!GetAttackMode()); // Toggling attack/defense
				SetAttackModeTimer(0);
				// Setting particle effect on hilt to show mode
				if(GetAttackMode())
				{
					CFXManager::GetInstance()->DequeueParticle("P1GUARD");
					CFXManager::GetInstance()->QueueParticle("P1ATTACK");
				}
				else
				{
					CFXManager::GetInstance()->DequeueParticle("P1ATTACK");
					CFXManager::GetInstance()->QueueParticle("P1GUARD");
				}

				SetCurrentPowerup(0); // Setting Power back to 0 (since we activated)
			}
		}
	}




	if(FMODMAN->IsSoundPlaying(CBeatManager::GetInstance()->GetCurrentlyPlayingSong()->GetSongID()))
	{
		int nSongID = CBeatManager::GetInstance()->GetCurrentlyPlayingSong()->GetSongID();
		unsigned int nTime;
		FMODMAN->GetLatestChannel(nSongID)->getPosition(&nTime,FMOD_TIMEUNIT_MS);

		if (Player2->IsConnected())
		{
			if (Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				cHitKey = 'w';
			}
			else if (Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				cHitKey = 'a';
			}
			else if (Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
			{
				cHitKey = 's';
			}
			else if (Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
			{
				cHitKey = 'd';
			}
		}

		if (!Player2->IsConnected())
		{
			if(DI->KeyPressed(DIK_W) || DI->JoystickButtonPressed(3, 1))
			{		
				cHitKey = 'w';
				//m_qKeyPresses.push(TBeatHit('w',nTime));
			}
			else if(DI->KeyPressed(DIK_A) || DI->JoystickButtonPressed(0, 1))
			{
				cHitKey = 'a';
				//m_qKeyPresses.push(TBeatHit('a',nTime));
			}
			else if(DI->KeyPressed(DIK_S) || DI->JoystickButtonPressed(4, 1))
			{
				cHitKey = 's';
				//m_qKeyPresses.push(TBeatHit('s',nTime));
			}
			else if(DI->KeyPressed(DIK_D) || DI->JoystickButtonPressed(1, 1))
			{
				cHitKey = 'd';
				//m_qKeyPresses.push(TBeatHit('d',nTime));
			}
		}
	}

	DI->ClearInput();
}
void CPlayer::AIHandling()
{
	// local variables for function
	CBeatManager* AIbeatDir;
	AIbeatDir = CBeatManager::GetInstance();
	CSong* AiSong;
	AiSong = AIbeatDir->GetCurrentlyPlayingSong();
	list<CBeat*>::iterator iter;
	
	// checking the random difficult
	// that returns a bool to set the hit to true or false


	for (iter = AiSong->GetActiveBeatList().begin(); iter != AiSong->GetActiveBeatList().end(); ++iter)
	{
		if ((*iter)->GetDirection() == LEFT && (*iter)->GetHasCollided() == true)
			SetAimingDirection(LEFT);		
		if ((*iter)->GetDirection()  == UP && (*iter)->GetHasCollided() == true)
			SetAimingDirection(UP);		
		if ((*iter)->GetDirection()  == RIGHT && (*iter)->GetHasCollided() == true)
			SetAimingDirection(RIGHT);		
		if ((*iter)->GetDirection()  == LEFTUP && (*iter)->GetHasCollided() == true)
			SetAimingDirection(LEFTUP);
		if ((*iter)->GetDirection() == RIGHTUP && (*iter)->GetHasCollided() == true)
			SetAimingDirection(RIGHTUP);
		if ((*iter)->GetDirection() == DOWN && (*iter)->GetHasCollided() == true)
			SetAimingDirection(DOWN);
		if ((*iter)->GetDirection() == LEFTDOWN && (*iter)->GetHasCollided() == true)
			SetAimingDirection(LEFTDOWN);
		if ((*iter)->GetDirection() == RIGHTDOWN && (*iter)->GetHasCollided() == true)
			SetAimingDirection(RIGHTDOWN);
	}

	// AI now will activate attack mode when power bar is maxed
	if(GetCurrentPowerup() >= GetMaxPowerup()) // AI now switches stances on full power bar (in lieu of specials for now)
	{
		SetAttackMode(true); // Toggling attack/defense
		SetAttackModeTimer(0);
		// Setting particle effect on hilt to show mode
		if(GetAttackMode())
		{
			CFXManager::GetInstance()->DequeueParticle("P2GUARD");
			CFXManager::GetInstance()->QueueParticle("P2ATTACK");
		}
		else
		{
			CFXManager::GetInstance()->DequeueParticle("P2ATTACK");
			CFXManager::GetInstance()->QueueParticle("P2GUARD");
		}

		SetCurrentPowerup(0); // Setting Power back to 0 (since we activated)
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
char CPlayer::GetMostRecentKeyPress()
{	
	return cHitKey;
}
void CPlayer::PlayAnimation()
{
	m_vecAnimations[m_nCurrAnim]->Play();
}
void CPlayer::StopAnimation()
{
	m_vecAnimations[m_nCurrAnim]->Stop();
}
void CPlayer::ResetAnimation()
{
	m_vecAnimations[m_nCurrAnim]->Reset();
}
void CPlayer::SetCurrAnimation(string szAnimName )
{
	std::vector<CAnimation*>::size_type i;
	for(i = 0; i < m_vecAnimations.size(); ++i)
	{
		if( m_vecAnimations[i]->GetName() == szAnimName )
		{
			m_nCurrAnim = i;
			m_vecAnimations[m_nCurrAnim]->Reset();
			break;
		}
	}
}
void CPlayer::SetSingleAnimation(CAnimation* pAnim)
{
	m_vecAnimations.push_back(pAnim);
}
////////////////////////////////////////
//	    PRIVATE ACCESSORS / MUTATORS
////////////////////////////////////////