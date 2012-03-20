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
#include "../../CLevelManager.h"
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
	m_nGameImageID = -1;
	m_nPauseID = -1;
	m_IsbGameState = false;
}

CPause_State::~CPause_State()
{

}

void CPause_State::Enter(void)
{	
	m_nGameImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/logo_beatWars_1024.png");
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MainMenuBG.jpg");
	m_nCursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/lightsaberCursor2.png");
	m_nPauseID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/pause.png");
}

bool CPause_State::Input(void)
{
#pragma region KEYBOARD
	if (!CGame::GetInstance()->GetPlayerControl()->IsConnected())
	{

		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(6))
			CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());

		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_BACKSPACE))
			CGame::GetInstance()->GoBack();

		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 1))
		{
			CGame::GetInstance()->PlayNavMenuSound();
			m_nMenuSelection -= 1;
			if (m_nMenuSelection == -1)
			{
				m_nMenuSelection = NUM_PAUSEMENU_OPTIONS - 1;

			}
		}
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 1) )
		{
			CGame::GetInstance()->PlayNavMenuSound();
			m_nMenuSelection += 1;

			if( m_nMenuSelection == NUM_PAUSEMENU_OPTIONS)
			{
				m_nMenuSelection = PAUSEMENU_EXIT;
			}

		}

		if( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(0, 0) )
		{
			CGame::GetInstance()->PlayAccMenuSound();
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
					CLevelManager::GetInstance()->EmptySongQueue();
					CGame::GetInstance()->ChangeState( CMenu_State::GetInstance() );
					m_IsbGameState = false;
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
					m_IsbGameState = true;
					SetPauseState(m_IsbGameState);
				}
				break;
			}
		}
	}

#pragma endregion

#pragma region XBOX

	if (CGame::GetInstance()->GetPlayerControl()->IsConnected())
	{
		if(CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
			CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());

		if (CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 1))
		{
			CGame::GetInstance()->PlayNavMenuSound();
			m_nMenuSelection -= 1;
			if (m_nMenuSelection == -1)
			{
				m_nMenuSelection = NUM_PAUSEMENU_OPTIONS - 1;
			}
		}
		if(CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 1) )
		{
			m_nMenuSelection += 1;

			CGame::GetInstance()->PlayNavMenuSound();
			if( m_nMenuSelection == NUM_PAUSEMENU_OPTIONS)
			{
				m_nMenuSelection = PAUSEMENU_EXIT;
			}

		}

		if(CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A && CGame::GetInstance()->GetButtonPressed() == false)
		{
			CGame::GetInstance()->SetButtonPressed(true);
			CGame::GetInstance()->PlayAccMenuSound();
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
					CLevelManager::GetInstance()->EmptySongQueue();
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
					m_IsbGameState = true;
					SetPauseState(m_IsbGameState);
				}
				break;
			}
		}
		else if ( CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A && CGame::GetInstance()->GetButtonPressed() == true )
		{
			CGame::GetInstance()->SetButtonPressed(false);
		}
	}

#pragma endregion

	return true;
}

void CPause_State::Update(void)
{

}

void CPause_State::Render(void)
{
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID,0,0, 0.78125f, 0.5859375f);
	
	// rect for options title
	RECT gOptions = {0,0,1000,120};
	CSGD_TextureManager::GetInstance()->Draw(m_nPauseID,15,40,0.4f,0.9f,&gOptions);
	// rect for game image 
	RECT gImage = {0,350,290,550};
	CSGD_TextureManager::GetInstance()->Draw(m_nGameImageID,450,15,1.0f,1.0f,&gImage);

	CBitmapFont::GetInstance()->SetScale(1.5f);
	RECT rMenuOptions = { 15, 250, CGame::GetInstance()->GetScreenWidth(), 450};
	CBitmapFont::GetInstance()->PrintStrokedTextInRect("return to game\n\nmain menu\n\nload\n\nsave\n\noptions menu",
		&rMenuOptions, ALIGN_CENTER,D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(225, 225, 225));
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up

	switch(m_nMenuSelection)
	{
	case PAUSEMENU_EXIT:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 235 + (PAUSEMENU_EXIT * 53) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 235  + (53* PAUSEMENU_EXIT), -1.0f);
		}
		break;

	case PAUSEMENU_MAINMENU:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 235 + (PAUSEMENU_MAINMENU * 53) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 235  + (53* PAUSEMENU_MAINMENU), -1.0f);
		}
		break;

	case PAUSEMENU_LOAD:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 235 + (PAUSEMENU_LOAD * 53) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 235  + (53* PAUSEMENU_LOAD), -1.0f);
		}
		break;

	case PAUSEMENU_SAVE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 235 + (PAUSEMENU_SAVE * 53) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 235  + (53* PAUSEMENU_SAVE), -1.0f);
		}
		break;

	case PAUSEMENU_OPTIONSTATE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 235 + (PAUSEMENU_OPTIONSTATE * 53) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 235  + (53* PAUSEMENU_OPTIONSTATE), -1.0f);
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