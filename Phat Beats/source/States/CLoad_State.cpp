///////////////////////////////////////////////////////
// File Name	:	"CLoad_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CLoad_State.h"

CLoad_State::CLoad_State()
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

CLoad_State::~CLoad_State()
{

}

void CLoad_State::Enter(void)
{	
	
}

bool CLoad_State::Input(void)
{

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		return false;

	return true;
}

void CLoad_State::Update(void)
{

}

void CLoad_State::Render(void)
{
	CSGD_Direct3D::GetInstance()->Clear(0,0,0);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();	

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	
	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->DeviceEnd();
	CSGD_Direct3D::GetInstance()->Present();
}

void CLoad_State::Exit(void)
{
	
}

CLoad_State* CLoad_State::GetInstance()
{
	// Lazy instantiation
	static CLoad_State instance; // Static allows passing back of address
	return &instance;	
}