///////////////////////////////////////////////////////
// File Name	:	"CLevelSelect_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CLevelSelect_State.h"
#include "CBitmapFont.h"
#include "../CSong.h"
#include "CMenu_State.h"
#include "../CGame.h"

CLevelSelect_State::CLevelSelect_State()
{
	CBitmapFont* m_bMenu_Font = NULL;
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
		CGame::GetInstance()->ChangeState(CMenu_State::GetInstance());

	return true;
}

void CLevelSelect_State::Update(void)
{

}

void CLevelSelect_State::Render(void)
{
	RECT rLevel1={25,25,45,45};
	CSGD_Direct3D::GetInstance()->DrawRect(rLevel1,255,0,0);
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	
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