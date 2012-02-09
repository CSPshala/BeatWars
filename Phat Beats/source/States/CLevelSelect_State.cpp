///////////////////////////////////////////////////////
// File Name	:	"CLevelSelect_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CLevelSelect_State.h"

CLevelSelect_State::CLevelSelect_State()
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

CLevelSelect_State::~CLevelSelect_State()
{

}

void CLevelSelect_State::Enter(void)
{	
	
}

bool CLevelSelect_State::Input(void)
{

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		return false;

	return true;
}

void CLevelSelect_State::Update(void)
{

}

void CLevelSelect_State::Render(void)
{
	CSGD_Direct3D::GetInstance()->Clear(0,0,0);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();	

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	
	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->DeviceEnd();
	CSGD_Direct3D::GetInstance()->Present();
}

void CLevelSelect_State::Exit(void)
{
	
}

CLevelSelect_State* CLevelSelect_State::GetInstance()
{
	// Lazy instantiation
	static CLevelSelect_State instance; // Static allows passing back of address
	return &instance;	
}