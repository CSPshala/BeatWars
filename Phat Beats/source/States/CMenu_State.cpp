///////////////////////////////////////////////////////
// File Name	:	"CMenu_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CMenu_State.h"
#include "CBitmapFont.h"

CMenu_State::CMenu_State()
{
	
	m_nMenuSelection = 0;

	// Asset IDs
	m_nBackgroundID = -1;
	m_nCursorImageID = -1;
	m_nTitleID = -1;
	m_nBackSoundID = -1;
	m_nCursorSoundID = -1;
}

CMenu_State::~CMenu_State()
{

}

void CMenu_State::Enter(void)
{	
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/star-gazing2.png");
	
}

bool CMenu_State::Input(void)
{

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		return false;

	return true;
}

void CMenu_State::Update(void)
{
	
}

void CMenu_State::Render(void)
{
	CSGD_Direct3D::GetInstance()->Clear(0,0,0);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID,0,0,1.6f,1.3f);
	CBitmapFont::GetInstance()->SetScale(3);
	CBitmapFont::GetInstance()->PrintText("BeatWars",244,25,D3DCOLOR_XRGB(255,255,255));
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	
	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->DeviceEnd();
	CSGD_Direct3D::GetInstance()->Present();
}

void CMenu_State::Exit(void)
{
	
}

CMenu_State* CMenu_State::GetInstance()
{
	// Lazy instantiation
	static CMenu_State instance; // Static allows passing back of address
	return &instance;	
}