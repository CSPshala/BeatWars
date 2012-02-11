///////////////////////////////////////////////////////
// File Name	:	"CGameplay_State.cpp"
//
// Purpose		:	To Contain all game related code
//////////////////////////////////////////////////////

#include "CGameplay_State.h"

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
}

bool CGameplay_State::Input(void)
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		return false;


		
	return true;
}

void CGameplay_State::Update(void)
{
	
	
	
}

void CGameplay_State::Render(void)
{
	CSGD_Direct3D::GetInstance()->Clear(0,0,0);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();
		

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	
	BeatManager.Play();
	
	
	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->DeviceEnd();
	CSGD_Direct3D::GetInstance()->Present();
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