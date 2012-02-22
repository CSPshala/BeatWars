///////////////////////////////////////////////////////
// File Name	:	"CGameplay_State.cpp"
//
// Purpose		:	To Contain all game related code
//////////////////////////////////////////////////////

#include "CGameplay_State.h"
#include "../CGame.h"
#include "../States/CMenu_State.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
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
}

CGameplay_State::~CGameplay_State()
{

}

void CGameplay_State::Enter(void)
{
	BeatManager.LoadSong("songtest1.xml");
AnimationManager.LoadAnimation("Anim.xml");
CMessageSystem::GetInstance()->InitMessageSystem(CGameplay_State::MessageProc);
}

bool CGameplay_State::Input(void)
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		CGame::GetInstance()->ChangeState( CMenu_State::GetInstance() );

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_O))
		BeatManager.Play();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_P))
		BeatManager.Pause();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_R))
		BeatManager.Reset();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_A) )
		AnimationManager.Play();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_S))
		AnimationManager.Stop();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_D))
		AnimationManager.Reset();
		
	return true;
}

void CGameplay_State::Update(void)
{
	// Updating audio
	CSGD_XAudio2::GetInstance()->Update();
	
	BeatManager.Update();
	AnimationManager.Update(CGame::GetInstance()->GetTimer().GetDeltaTime());
	
}

void CGameplay_State::Render(void)
{
	
		

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	
	BeatManager.Render(); 
	AnimationManager.Render();
	if (dickhead == true)
	{
		CSGD_Direct3D::GetInstance()->DrawTextA("this is a message test",320,340,255,0,0);
	}
	
}

void CGameplay_State::Exit(void)
{
	AnimationManager.UnloadAnimations();
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