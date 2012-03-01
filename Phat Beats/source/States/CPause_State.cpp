///////////////////////////////////////////////////////
// File Name	:	"CPause_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CPause_State.h"
#include "../CGame.h"
#include "../Globals.h"
#include "CGameplay_State.h"
#include "CLoad_State.h"
#include "CMenu_State.h"
#include "CSave_State.h"
#include "COptionsState.h"
#include "CBitmapFont.h"
CPause_State::CPause_State()
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

CPause_State::~CPause_State()
{

}

void CPause_State::Enter(void)
{	
	m_nCursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/lightsaberCursor.png");
}

bool CPause_State::Input(void)
{

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());

	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIK_UP) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIK_UP))
	{
		m_nMenuSelection -= 1;
		if (m_nMenuSelection == -1)
		{
			m_nMenuSelection = NUM_PAUSEMENU_OPTIONS - 1;

		}
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIK_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIK_DOWN) )
	{
		m_nMenuSelection += 1;

		if( m_nMenuSelection == NUM_PAUSEMENU_OPTIONS )
		{
			m_nMenuSelection = 0;
		}

	}

	if( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || CSGD_DirectInput::GetInstance()->MouseButtonPressed(0) )
	{
		switch( m_nMenuSelection )
		{
		case PAUSEMENU_EXIT:
			{
				CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());
			}
			break;
		case PAUSEMENU_MAINMENU:
			{
				CGameplay_State::GetInstance()->SetPreviouslyPlaying(false);
				CGameplay_State::GetInstance()->Exit();
				CGame::GetInstance()->ChangeState( CMenu_State::GetInstance() );
			}
			break;
		case PAUSEMENU_LOAD:
			{
				CGame::GetInstance()->ChangeState( CLoad_State::GetInstance() );
			}
			break;
		case PAUSEMENU_SAVE:
			{
				CGame::GetInstance()->ChangeState(CSave_State::GetInstance() );
			}
			break;
		case PAUSEMENU_OPTIONSTATE:
			{
				CGame::GetInstance()->ChangeState(COptionsState::GetInstance() );
			}
			break;
		}
	}
	return true;
}

void CPause_State::Update(void)
{

}

void CPause_State::Render(void)
{
	CBitmapFont::GetInstance()->SetScale(3.5f);
	RECT rTitle = {0, 40, CGame::GetInstance()->GetScreenWidth(), 80};
	CBitmapFont::GetInstance()->PrintInRect("pause", &rTitle, ALIGN_CENTER,D3DCOLOR_XRGB(242,251,4));
	CBitmapFont::GetInstance()->SetScale(1.0f);
	RECT rMenuOptions = { 225, 177, CGame::GetInstance()->GetScreenWidth(), 380};
	CBitmapFont::GetInstance()->PrintInRect("return to game\n\nmain menu\n\nload\n\nsave\n\noptions menu",
		&rMenuOptions, ALIGN_LEFT, D3DCOLOR_XRGB(225, 225, 225));
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	switch(m_nMenuSelection)
	{
	case PAUSEMENU_EXIT:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (PAUSEMENU_EXIT * 40) );
		}
		break;

	case PAUSEMENU_MAINMENU:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (PAUSEMENU_MAINMENU * 40) );
		}
		break;

	case PAUSEMENU_LOAD:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (PAUSEMENU_LOAD * 40) );
		}
		break;

	case PAUSEMENU_SAVE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (PAUSEMENU_SAVE * 40) );
		}
		break;

	case PAUSEMENU_OPTIONSTATE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (PAUSEMENU_OPTIONSTATE * 40) );
		}
		break;
	}	
}

void CPause_State::Exit(void)
{
	
}

CPause_State* CPause_State::GetInstance()
{
	// Lazy instantiation
	static CPause_State instance; // Static allows passing back of address
	return &instance;	
}