///////////////////////////////////////////////////////
// File Name	:	"CArcadeMode_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CArcadeMode_State.h"

CArcadeMode_State::CArcadeMode_State()
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

CArcadeMode_State::~CArcadeMode_State()
{

}

void CArcadeMode_State::Enter(void)
{	
	
}

bool CArcadeMode_State::Input(void)
{

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(6) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(6))
		return false;

	return true;
}

void CArcadeMode_State::Update(void)
{

}

void CArcadeMode_State::Render(void)
{
	CSGD_Direct3D::GetInstance()->Clear(0,0,0);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();	

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	
	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->DeviceEnd();
	CSGD_Direct3D::GetInstance()->Present();
}

void CArcadeMode_State::Exit(void)
{
	
}

CArcadeMode_State* CArcadeMode_State::GetInstance()
{
	// Lazy instantiation
	static CArcadeMode_State instance; // Static allows passing back of address
	return &instance;	
}