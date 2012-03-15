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
#include "../../CLevelManager.h"
#include "CLU_State.h"


CGameplay_State::CGameplay_State()
{
	m_bMenu_Font = NULL;

	// Start off defaulted in tutorial mode (make it so player can skip later)
//	SetIsTutorial(true);

	m_SongTransitionAlpha = 255;

	m_bStartTransition = true;
	
	m_bPreviouslyPlaying = false;

	// Setting tutorial bool (note events will throw this in tutorial track)
	SetIsTutorial(false);

	// Setting text index
	m_nTutorialTextIndex = 0;
	// Setting tut box
	m_nTutorialBoxID = -1;
}

CGameplay_State::~CGameplay_State()
{

}

void CGameplay_State::Enter(void)
{
	BeatManager = CBeatManager::GetInstance();
	
	if(!GetPreviouslyPlaying()) 
	{
		
		if (CLoad_State::GetInstance()->GetLoadFlag() == true)
		{
			CLU_State::GetInstance()->QueueLoadCommand(CLoad_State::GetInstance()->GetFileName(),"",Song);
			CBeatManager::GetInstance()->LoadSongBIN(CLoad_State::GetInstance()->loadGame());
			CLevelManager::GetInstance()->QueueSong(CLoad_State::GetInstance()->GetSongName());
			CLU_State::GetInstance()->SetNewState(CGameplay_State::GetInstance());
		}

		/*
		CSGD_FModManager::GetInstance()->SetVolume(CBeatManager::GetInstance()->GetCurrentlyPlayingSong()->GetSongID()
		,COptionsState::GetInstance()->GetMusicVol());
		*/

	
		CFXManager::GetInstance()->MoveEffectTo("P2ATTACK",D3DXVECTOR2((float)700,(float)12));
		CFXManager::GetInstance()->MoveEffectTo("P2GUARD",D3DXVECTOR2((float)700,(float)12));
		CFXManager::GetInstance()->MoveEffectTo("P1_HIT", D3DXVECTOR2((float)CLevelManager::GetInstance()->GetPlayer(PlayerOne)->GetCollisionRect().left, (float)CLevelManager::GetInstance()->GetPlayer(PlayerOne)->GetCollisionRect().top));
		CFXManager::GetInstance()->MoveEffectTo("P2_HIT", D3DXVECTOR2((float)CLevelManager::GetInstance()->GetPlayer(PlayerTwo)->GetCollisionRect().left, (float)CLevelManager::GetInstance()->GetPlayer(PlayerTwo)->GetCollisionRect().top));

		CFXManager::GetInstance()->MoveEffectTo("P1_GUARD",D3DXVECTOR2(400.0f, 300.0f));
		CFXManager::GetInstance()->MoveEffectTo("P1_PBAR",D3DXVECTOR2(32.0f, 32.0f));
		CFXManager::GetInstance()->MoveEffectTo("P2_PBAR",D3DXVECTOR2(600.0f, 32.0f));

		// Queueing effects to display		
		CFXManager::GetInstance()->QueueParticle("P1GUARD");
		CFXManager::GetInstance()->QueueParticle("P2GUARD");	

		

		// Registering events for tutorial
		CEventSystem::GetInstance()->RegisterClient("tutorialpause",this); // Pauses tutorial so player can read intro text

		if(GetIsTutorial())
		{
			// This is so gross, if I get time I'll clean this up - JC
			// If we're in tutorial, setting tutorial strings for player output
			m_vTutorialText.push_back("Welcome to BeatWars\nDon't worry if you miss a note, this is practice.");
			m_vTutorialText.push_back("This is a note\nRed notes are imperial notes.\nPress the A key with the beat to hit it.");
			m_vTutorialText.push_back("Got it?  Now try another note.\nBlue notes are Republic Notes.\nPress the D key with the beat to hit it.");
			m_vTutorialText.push_back("Now a Mandelorian note. (That is the skull.)\nPress the W key with the beat to hit it.");
			m_vTutorialText.push_back("Now a Sun note, they're yellow.\nPress the D key with the beat to hit it.");
			m_vTutorialText.push_back("Next things are about to get hard.\nPress an arrow key (or numpad key)\nto aim at a note.\nHit the right key to hit it.");
			m_vTutorialText.push_back("Remember!  If you do not aim at a note,\neven if you hit the right key,\nyou will miss.");
			m_vTutorialText.push_back("Next up is health.  Try to hit these notes.");
			m_vTutorialText.push_back("Your opponent's health just went down.\nHit more notes than your opponent and you\nwill kill them.  This is how you win.");			
			m_vTutorialText.push_back("See that blue glow on your lightsaber\nat the top?\nThat means you are in defense mode.\nYou take less damage in this mode.");
			m_vTutorialText.push_back("Press spacebar to change to attack mode.\nYou will do more damage in this mode but\nwill take more damage as well.");
			m_vTutorialText.push_back("Watch out there are also diagonal directions.\nPress two arrow keys or numpad\nto aim at them.");
			m_vTutorialText.push_back("Try to empty your opponent's life bar\nas much as possible. Whoever has the most\ntakedowns at the end of the song wins.");
			m_vTutorialText.push_back("Now, practice by hitting notes\nbeat your opponent up by hitting notes.\nDon't worry your opponent is easy for now.");
			m_vTutorialText.push_back("Great job.\nNow lets head to your first song.");
			
			m_nTutorialBoxID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/tutorialbox.png");			
		}

		m_nTutorialTextIndex = 0;

		
	}	

	CLevelManager::GetInstance()->EnterLevel();

	if(GetIsTutorial())
		BeatManager->Pause();
}

bool CGameplay_State::Input(void)
{
	

	if (CSGD_DirectInput::GetInstance()->MouseButtonPressed(0))
	{
		SetPreviouslyPlaying(true);
		CLevelManager::GetInstance()->LeaveLevel();
		CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
	}
	
	// Stopping regular play input when tutorial note is thrown
	// (because we are in a tutorial and it's waiting for player to read something
	if(!GetIsTutorial())
	{
		CLevelManager::GetInstance()->HandleLevelInput();

		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_P) || CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->KeyPressed(DIK_LALT) && CSGD_DirectInput::GetInstance()->KeyPressed(DIK_TAB)) 
		{
			SetPreviouslyPlaying(true);
			CLevelManager::GetInstance()->LeaveLevel();			
			CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
		}
	
	}
	else
	{
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(0, 0))
		{
			if(m_nTutorialTextIndex < m_vTutorialText.size() - 1)
			{
				SetIsTutorial(false);

				++m_nTutorialTextIndex;

				// Unpausing song
				BeatManager->Pause();
			}
			else
			{
				SetIsTutorial(false);
				CLevelManager::GetInstance()->SkipLevel();
			}
		}

		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->MouseButtonPressed(6))
		{
			SetIsTutorial(false);
			CLevelManager::GetInstance()->SkipLevel();
		}
	}



	CLevelManager::GetInstance()->HandleLevelInput();

	return true;
}

void CGameplay_State::Update(void)
{

	// Skpping update if is tutorial
	if(!GetIsTutorial())
	{
		CLevelManager::GetInstance()->Update(CGame::GetInstance()->GetTimer().GetDeltaTime());
		if (m_bStartTransition)
		{
			m_SongTransitionAlpha -= 0.25f;
			if (m_SongTransitionAlpha <= 1)
			{
				m_bStartTransition = false;
				m_SongTransitionAlpha = 255;
			}
		}
	}


	
}
void CGameplay_State::Render(void)
{
	CLevelManager::GetInstance()->Render();

	// Only draws tutorial text if we're in tutorial
	DrawTutorialText();


		if (m_bStartTransition)
		{
			DrawARGB("blackscreen.png", D3DCOLOR_ARGB((int)m_SongTransitionAlpha, 0, 0, 0));

		}
}

void CGameplay_State::Exit(void)
{
	if (!GetPreviouslyPlaying())
	{
		CLevelManager::GetInstance()->Exit();
		// Removing references to players on the way out so they'll get cleaned up

		CBeatManager::GetInstance()->Stop();
		CBeatManager::GetInstance()->UnloadSongs();

		CFXManager::GetInstance()->UnloadFX("P1ATTACK");
		CFXManager::GetInstance()->UnloadFX("P1GUARD");
		CFXManager::GetInstance()->UnloadFX("P2ATTACK");
		CFXManager::GetInstance()->UnloadFX("P2GUARD");
		CFXManager::GetInstance()->UnloadFX("P1_HIT");
		CFXManager::GetInstance()->UnloadFX("P2_HIT");
		CFXManager::GetInstance()->UnloadFX("P1_PBAR");
		CFXManager::GetInstance()->UnloadFX("P2_PBAR");

		// Cleaning up tutorial event
		CEventSystem::GetInstance()->UnregisterClient("tutorialpause",this);
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
	if(pEvent->GetEventID() == "tutorialpause")
	{		
		SetIsTutorial(true);
		BeatManager->Pause();
	}
}

void CGameplay_State::MessageProc( CBaseMessage* pMsg )
{
	
}

void CGameplay_State::DrawARGB(string filename, DWORD argbColor)
{
	int ImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/blackscreen.png");

	CSGD_TextureManager::GetInstance()->Draw(ImageID, 0, 0, 1.0f, 1.0f, 0, 800, 600, 0, argbColor);
}

void CGameplay_State::DrawTutorialText()
{
	if(GetIsTutorial())
	{
		RECT tRect = {200,400,600,600};

		CSGD_TextureManager::GetInstance()->Draw(m_nTutorialBoxID,90,450,2.5f,0.8f);
		CBitmapFont::GetInstance()->PrintStrokedTextInRect(m_vTutorialText[m_nTutorialTextIndex],&tRect,
														2,D3DCOLOR_XRGB(0,0,0),D3DCOLOR_XRGB(255,255,255));
	}
}

