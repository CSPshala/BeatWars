///////////////////////////////////////////////////////
// File Name	:	"CCharacterSelect_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CCharacterSelect_State.h"

CCharacterSelect_State::CCharacterSelect_State()
{
	Bitmap_Font* m_bMenu_Font = NULL;
	m_nMenuSelection = 0;

	// Asset IDs
	m_nBackgroundID = -1;
	m_nCursorImageID = -1;
	m_nFontID = -1;
	m_nTitleID = -1;
	m_nBackSoundID = -1;
	m_nCursorSoundID = -1;
}

CCharacterSelect_State::~CCharacterSelect_State()
{

}

void CCharacterSelect_State::Enter(void)
{	
	
}

bool CCharacterSelect_State::Input(void)
{

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		return false;

	return true;
}

void CCharacterSelect_State::Update(void)
{

}

void CCharacterSelect_State::Render(void)
{
	CSGD_Direct3D::GetInstance()->Clear(0,0,0);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();	

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	
	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->DeviceEnd();
	CSGD_Direct3D::GetInstance()->Present();
}

void CCharacterSelect_State::Exit(void)
{
	
}

CCharacterSelect_State* CCharacterSelect_State::GetInstance()
{
	// Lazy instantiation
	static CCharacterSelect_State instance; // Static allows passing back of address
	return &instance;	
}