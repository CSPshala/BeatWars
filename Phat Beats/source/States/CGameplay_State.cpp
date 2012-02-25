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

bool CGameplay_State::dickhead = false;
CGameplay_State::CGameplay_State()
{
	m_bMenu_Font = NULL;
	
		// Asset IDs
	m_nBackgroundID = -1;
	m_nBackSoundID = -1;
	m_nFontID = -1;
	m_nTitleID = -1;
	m_bPlayAnimation = false;
	m_Player1 = NULL;
	m_Player2 = NULL;
	
}

CGameplay_State::~CGameplay_State()
{

}

void CGameplay_State::Enter(void)
{
	BeatManager = CBeatManager::GetInstance();

	BeatManager->LoadSong("noteeventtest.xml");
	AnimationManager.LoadAnimation("Anim.xml","nxc_bat_heihachi.PNG");
	CMessageSystem::GetInstance()->InitMessageSystem(CGameplay_State::MessageProc);

	// Setting up Players
	m_Player1 = new CPlayer(OBJ_PLAYER1);
	m_Player2 = new CPlayer(OBJ_PLAYER2);

	// Adding players to Object Manager
	CObjectManager::GetInstance()->AddObject(m_Player1);
	CObjectManager::GetInstance()->AddObject(m_Player2);
}

bool CGameplay_State::Input(void)
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		CGame::GetInstance()->ChangeState(CMenu_State::GetInstance());

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_O))
		BeatManager->Play("Avicii");

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_P))
	{
		BeatManager->Pause();
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_R))
		BeatManager->Reset();

	/*if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_A) )
		AnimationManager.Play();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_S))
		AnimationManager.Stop();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_D))
		AnimationManager.Reset();*/
	
	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_I))
	{
		CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
	}
	

	return true;
}

void CGameplay_State::Update(void)
{
	
	// Updating Objects (if beatmanager isn't paused)
	CFXManager::GetInstance()->Update(CGame::GetInstance()->GetTimer().GetDeltaTime());
	if(!BeatManager->IsPaused())
	{
		// Updating song
		CObjectManager::GetInstance()->UpdateObjects(CGame::GetInstance()->GetTimer().GetDeltaTime());	
		// Updating animations
		AnimationManager.Update(CGame::GetInstance()->GetTimer().GetDeltaTime());
		// Checking collisions
		CObjectManager::GetInstance()->CheckCollisions(m_Player1);
		//CObjectManager::GetInstance()->CheckCollisions(m_Player2);

		// Taking care of player input
		BeatManager->CheckPlayerInput(m_Player1);

	}

	
}

void CGameplay_State::Render(void)
{
	// Drawing everything before this
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
		
	AnimationManager.Render();
	CFXManager::GetInstance()->Render();

	// You know what's up
	CObjectManager::GetInstance()->RenderObjects();

	if (dickhead == false)
	{
		CSGD_Direct3D::GetInstance()->DrawTextA("this is a test",320,340,255,0,0);
	}
	


}

void CGameplay_State::Exit(void)
{
	AnimationManager.UnloadAnimations();

	// Removing references to players on the way out so they'll get cleaned up
	if(m_Player1)
		m_Player1->Release();

	if(m_Player2)
		m_Player2->Release();
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