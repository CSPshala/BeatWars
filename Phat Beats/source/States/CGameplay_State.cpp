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

bool CGameplay_State::dickhead = false;
CGameplay_State::CGameplay_State()
{
	m_bMenu_Font = NULL;

	// Start off defaulted in tutorial mode (make it so player can skip later)
//	SetIsTutorial(true);

	m_SongTransitionAlpha = 255;

	m_bStartTransition = true;
	
	m_bPreviouslyPlaying = false;
}

CGameplay_State::~CGameplay_State()
{

}

void CGameplay_State::Enter(void)
{

	
		CFXManager::GetInstance()->MoveEffectTo("P2ATTACK",D3DXVECTOR2((float)700,(float)12));
		CFXManager::GetInstance()->MoveEffectTo("P2GUARD",D3DXVECTOR2((float)700,(float)12));

		// Queueing effects to display		
		CFXManager::GetInstance()->QueueParticle("P1ATTACK");
		CFXManager::GetInstance()->QueueParticle("P2ATTACK");
 

	if(!GetPreviouslyPlaying()) {
		
		if (CLoad_State::GetInstance()->GetLoadFlag() == true)
		{
			CLU_State::GetInstance()->QueueLoadCommand(CLoad_State::GetInstance()->GetFileName(),"",Song);
			CBeatManager::GetInstance()->LoadSong(CLoad_State::GetInstance()->loadGame());
			CLevelManager::GetInstance()->QueueSong(CLoad_State::GetInstance()->GetSongName());
			CLU_State::GetInstance()->SetNewState(CGameplay_State::GetInstance());
		}
		else
			CLevelManager::GetInstance()->QueueSong("cantina");

		

		CFXManager::GetInstance()->MoveEffectTo("P1_HIT", D3DXVECTOR2((float)CLevelManager::GetInstance()->GetPlayer(PlayerOne)->GetCollisionRect().left, (float)CLevelManager::GetInstance()->GetPlayer(PlayerOne)->GetCollisionRect().top));
		CFXManager::GetInstance()->MoveEffectTo("P2_HIT", D3DXVECTOR2((float)CLevelManager::GetInstance()->GetPlayer(PlayerTwo)->GetCollisionRect().left, (float)CLevelManager::GetInstance()->GetPlayer(PlayerTwo)->GetCollisionRect().top));
	}

	CLevelManager::GetInstance()->EnterLevel();
}

bool CGameplay_State::Input(void)
{


	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_P)) {
		SetPreviouslyPlaying(true);
		CLevelManager::GetInstance()->LeaveLevel();
		CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE)) {
		CLevelManager::GetInstance()->LeaveLevel();
		CGame::GetInstance()->ChangeState(CMenu_State::GetInstance());
	}

	CLevelManager::GetInstance()->HandleLevelInput();

	return true;
}

void CGameplay_State::Update(void)
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
void CGameplay_State::Render(void)
{
	CLevelManager::GetInstance()->Render();
	
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

void CGameplay_State::DrawARGB(string filename, DWORD argbColor)
{
	int ImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/blackscreen.png");

	CSGD_TextureManager::GetInstance()->Draw(ImageID, 0, 0, 1.0f, 1.0f, 0, 800, 600, 0, argbColor);
}

