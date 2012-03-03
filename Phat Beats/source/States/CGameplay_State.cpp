///////////////////////////////////////////////////////
// File Name	:	"CGameplay_State.cpp"
//
// Purpose		:	To Contain all game related code
//////////////////////////////////////////////////////

#include "CGameplay_State.h"
#include "../CGame.h"
#include "../States/CMenu_State.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "CMenu_State.h"
#include "../CGame.h"
#include "CPause_State.h"
#include "../Managers/CAiManager.h"
#include "../Managers/CFXManager.h"
#include "COptionsState.h"
#include "CLoad_State.h"
#include "CSave_State.h"
bool CGameplay_State::dickhead = false;
CGameplay_State::CGameplay_State()
{
	m_bMenu_Font = NULL;

	// Asset IDs
	m_nBackgroundID = -1;
	m_nBackSoundID = -1;
	m_nFontID = -1;
	m_nTitleID = -1;
	m_Player1 = NULL;
	m_Player2 = NULL;

	m_bPreviouslyPlaying = false;
}

CGameplay_State::~CGameplay_State()
{

}

void CGameplay_State::Enter(void)
{
	BeatManager = CBeatManager::GetInstance();

	BeatManager->LoadSong("cantina.xml");
	//BeatManager->LoadSong("noteeventtest.xml");
	CMessageSystem::GetInstance()->InitMessageSystem(CGameplay_State::MessageProc);
    m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/star_wars___battle_1182.jpg");
	m_nHudID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/bag_HUD.png");

	if (m_bPreviouslyPlaying == false)
	{
		BeatManager = CBeatManager::GetInstance();	
		if (CLoad_State::GetInstance()->GetLoadFlag() == true)
		{
			BeatManager->LoadSong(CLoad_State::GetInstance()->loadGame());
		}
		else
			BeatManager->LoadSong("cantina.xml");	

		//BeatManager->LoadSong("noteeventtest.xml");
		CMessageSystem::GetInstance()->InitMessageSystem(CGameplay_State::MessageProc);
		m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/star_wars___battle_1182.jpg");

		CFXManager::GetInstance()->LoadFX("GameBG.xml", "BACKGROUND");
		CFXManager::GetInstance()->QueueParticle("BACKGROUND");
		CFXManager::GetInstance()->LoadFX("Hit.xml", "P1_HIT");
		CFXManager::GetInstance()->LoadFX("Hit.xml", "P2_HIT");

		m_nHudID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/bag_HUD.png");
		// Setting up Players
		m_Player1 = new CPlayer(OBJ_PLAYER1);
		m_Player2 = new CPlayer(OBJ_AI);
		// Adding players to Object Manager
		CObjectManager::GetInstance()->AddObject(m_Player1);
		CObjectManager::GetInstance()->AddObject(m_Player2);
		
		if (CLoad_State::GetInstance()->GetLoadFlag() == true)
		{
			BeatManager->Play(CLoad_State::GetInstance()->GetSongName());
		}
		else
			BeatManager->Play("cantina");
		

		CFXManager::GetInstance()->MoveEffectTo("P1_HIT", D3DXVECTOR2((float)m_Player1->GetCollisionRect().left, (float)m_Player1->GetCollisionRect().top));
		CFXManager::GetInstance()->MoveEffectTo("P2_HIT", D3DXVECTOR2((float)m_Player2->GetCollisionRect().left, (float)m_Player2->GetCollisionRect().top));


		rLeftHandle.left = 20;
		rLeftHandle.top = 10;
		rLeftHandle.right = 67;
		rLeftHandle.bottom = 27;

		rRightHandle.left = 445;
		rRightHandle.top = 12;
		rRightHandle.right = 492;
		rRightHandle.bottom = 23;

		rLeftSaber.left = 20;
		rLeftSaber.top = 349;
		rLeftSaber.right = 227;
		rLeftSaber.bottom = 381;

		rRightSaber.left = 257;
		rRightSaber.top = 348;
		rRightSaber.right = 466;
		rRightSaber.bottom = 382;

		rRightPowerUpBar.left = 303;
		rRightPowerUpBar.top = 107;
		rRightPowerUpBar.right = 495;
		rRightPowerUpBar.bottom = 141;

		rLeftPowerUpBar.left = 22;
		rLeftPowerUpBar.top = 107;
		rLeftPowerUpBar.right = 214;
		rLeftPowerUpBar.bottom = 140;


		m_Player1->SetAnimations( AnimationManager.LoadAnimation("NewAnim.xml","nxc_bat_heihachi.PNG") );
		m_Player2->SetAnimations( AnimationManager.LoadAnimation("NewAnim.xml","nxc_bat_heihachi.PNG") );
		m_Player1->SetAnimationsIsEmpty(false);
		m_Player2->SetAnimationsIsEmpty(false);
		
		BeatManager->Play("cantina");
		m_bCheckAnimations = true;
	}
	else
	{
		if (CLoad_State::GetInstance()->GetLoadFlag() == true)
		{
			BeatManager->Play(CLoad_State::GetInstance()->GetSongName());
		}
		else
			BeatManager->Play("cantina");

	}

	m_bGameOver = false;

}

bool CGameplay_State::Input(void)
{
	if(!BeatManager->IsPaused())
	{

		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
			CGame::GetInstance()->ChangeState(CMenu_State::GetInstance());

		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_O))
			BeatManager->Play("cantina");

		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_P))
		{
			m_bPreviouslyPlaying = true;
			BeatManager->Pause();
			CSave_State::GetInstance()->saveGame();
			CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
		}
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_R))
		{
			BeatManager->Reset();
			m_bGameOver = false;

			m_Player1->SetCurrentHP(100);
			m_Player2->SetCurrentHP(100);
		}

		if( m_bCheckAnimations) 
		{

			if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_B) )
			{
				m_Player1->SetCurrAnimation("Idle");
				m_Player1->PlayAnimation();
			}

			if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_N))
			{
				m_Player1->SetCurrAnimation("Attack");
				m_Player1->PlayAnimation();
			}

			if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_M))
			{
				m_Player1->SetCurrAnimation("Block");
				m_Player1->PlayAnimation();
			}

			if( m_Player1->GetCurrAnim()->GetPlayedAlready() )
			{
				m_Player1->SetCurrAnimation("Idle");
				m_Player1->PlayAnimation();
			}

		}

		if( m_bCheckAnimations)
		{
			if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_J) )
			{
				m_Player2->SetCurrAnimation("Idle");
				m_Player2->PlayAnimation();
			}

			if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_K))
			{
				m_Player2->SetCurrAnimation("Attack");
				m_Player2->PlayAnimation();
			}

			if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_L))
			{
				m_Player2->SetCurrAnimation("Block");
				m_Player2->PlayAnimation();
			}

			if( m_Player2->GetCurrAnim()->GetPlayedAlready() )
			{
				m_Player2->SetCurrAnimation("Idle");
				m_Player2->PlayAnimation();
			}

		}

		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_I))
		{

		}

	}

	return true;
}

void CGameplay_State::Update(void)
{

	if(!BeatManager->IsPaused() && !m_bGameOver)
	{
		// Updating Objects (if beatmanager isn't paused)
		CFXManager::GetInstance()->Update(CGame::GetInstance()->GetTimer().GetDeltaTime());
		// Updating song
		CObjectManager::GetInstance()->UpdateObjects(CGame::GetInstance()->GetTimer().GetDeltaTime());	
		// Checking collisions
		CObjectManager::GetInstance()->CheckCollisions(m_Player1);
		CObjectManager::GetInstance()->CheckCollisions(m_Player2);

		// Taking care of player input
		BeatManager->CheckPlayerInput(m_Player1);
		//BeatManager->CheckPlayerInput(m_Player2);

		// Updating beatmanager (handles current streak counting and player dmg)
		BeatManager->Update();

		if(m_Player1->GetCurrentHP() <= 0 || m_Player2->GetCurrentHP() <= 0)
			m_bGameOver = true;
	}

}

void CGameplay_State::Render(void)
{
	if(m_bGameOver)
	{
		CSGD_Direct3D::GetInstance()->DrawText("Game Over", 100, 100, 255, 40, 40);
		if(m_Player1->GetCurrentHP()== 0)
			CSGD_Direct3D::GetInstance()->DrawText("Player 2 Won!", 100, 120, 255, 40, 40);
		else if(m_Player2->GetCurrentHP()== 0)
			CSGD_Direct3D::GetInstance()->DrawText("Player 1 Won!", 100, 120, 255, 40, 40);

		CSGD_Direct3D::GetInstance()->DrawText("Press 'R' to Restart or Escape to exit!", 100, 140, 255, 40, 40);
	}

	if(!BeatManager->IsPaused() && !m_bGameOver)
	{

		CSGD_TextureManager::GetInstance()->Draw(m_nHudID,59,10,1.0,1.0,&rLeftSaber);
		CSGD_TextureManager::GetInstance()->Draw(m_nHudID,513,10,1.0,1.0,&rRightSaber);
		CSGD_TextureManager::GetInstance()->Draw(m_nHudID,20,17,1.0,1.0,&rLeftHandle);
		CSGD_TextureManager::GetInstance()->Draw(m_nHudID,722,21,1.0,1.0,&rRightHandle);
		CSGD_TextureManager::GetInstance()->Draw(m_nHudID,59,45,1.0,1.0,&rLeftPowerUpBar);
		CSGD_TextureManager::GetInstance()->Draw(m_nHudID,529,45,1.0,1.0,&rRightPowerUpBar);


		// Drawing everything before this
		CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
		CFXManager::GetInstance()->Render();


		// You know what's up
		CObjectManager::GetInstance()->RenderObjects();

		char p1hp[50];
		char p2hp[50];

		itoa(m_Player1->GetCurrentHP(),p1hp,10);
		itoa(m_Player2->GetCurrentHP(),p2hp,10);

		CSGD_Direct3D::GetInstance()->DrawText(p1hp,0,0,255,0,0);
		CSGD_Direct3D::GetInstance()->DrawText(p2hp,100,0,255,0,0);

		if (dickhead == false)
		{
			CSGD_Direct3D::GetInstance()->DrawTextA("this is a test",320,340,255,0,0);
		}
	}
	



}

void CGameplay_State::Exit(void)
{
	if (m_bPreviouslyPlaying == false)
	{
		// Removing references to players on the way out so they'll get cleaned up
		if(m_Player1)
		{
			m_Player1->SetAnimationsIsEmpty(true);
			AnimationManager.UnloadAnimations(m_Player1->GetAnimations() );
			m_Player1->Release();
		}

		if(m_Player2)
		{
			
			m_Player2->SetAnimationsIsEmpty(true);
			AnimationManager.UnloadAnimations(m_Player2->GetAnimations() );
			m_Player2->Release();
		}

		m_bCheckAnimations = false;

		CBeatManager::GetInstance()->Stop();
		CBeatManager::GetInstance()->UnloadSongs();

		CFXManager::GetInstance()->UnloadFX("BACKGROUND");
		CFXManager::GetInstance()->UnloadFX("P1_HIT");
		CFXManager::GetInstance()->UnloadFX("P2_HIT");

	}
}

CGameplay_State* CGameplay_State::GetInstance()
{
	// Lazy instantiation
	static CGameplay_State instance; // Static allows passing back of address
	return &instance;	
}

void CGameplay_State::HandleEvent( CEvent* pEvent )
{

}

void CGameplay_State::MessageProc( CBaseMessage* pMsg )
{
	switch (pMsg->GetMsgID())
	{
	case MSG_TEST:
		dickhead = true;
		break;


	}
}


