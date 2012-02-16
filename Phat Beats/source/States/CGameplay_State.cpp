///////////////////////////////////////////////////////
// File Name	:	"CGameplay_State.cpp"
//
// Purpose		:	To Contain all game related code
//////////////////////////////////////////////////////

#include "CGameplay_State.h"
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
}

CGameplay_State::~CGameplay_State()
{
}

void CGameplay_State::Enter(void)
{
	BeatManager.LoadSong("songtest1.xml");
	CMessageSystem::GetInstance()->InitMessageSystem(CGameplay_State::MessageProc);
}

bool CGameplay_State::Input(void)
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		return false;

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_O))
		BeatManager.Play();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_P))
		BeatManager.Pause();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_R))
		BeatManager.Reset();
		
	return true;
}

void CGameplay_State::Update(void)
{
	// Updating audio
	CSGD_XAudio2::GetInstance()->Update();
	
	BeatManager.Update();
	
}

void CGameplay_State::Render(void)
{
	
		

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	
	BeatManager.Render();
	if (dickhead == true)
	{
		CSGD_Direct3D::GetInstance()->DrawTextA("this is a message test",320,340,255,0,0);
	}
	
}

void CGameplay_State::Exit(void)
{
	
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
