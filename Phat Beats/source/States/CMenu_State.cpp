///////////////////////////////////////////////////////
// File Name	:	"CMenu_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CMenu_State.h"
#include "CBitmapFont.h"
#include "CGameplay_State.h"
#include "COptionsState.h"
#include "../Globals.h"
#include "../Managers/CFXManager.h"
#include "CLevelSelect_State.h"
#include "CLU_State.h"

CMenu_State::CMenu_State()
{
	// Asset IDs
	m_nBackgroundID = -1;
	m_nCursorImageID = -1;
	m_nTitleID = -1;
	m_nBackSoundID = -1;
	m_nCursorSoundID = -1;
	m_nTile = -1;
}

CMenu_State::~CMenu_State()
{

}

void CMenu_State::Enter(void)
{	
	m_nMenuSelection = 0;
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MainMenuBG.jpg");
	m_nCursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/lightsaberCursor2.png");
	m_nTile = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/logo_beatWars_1024.png");
	m_nTile = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/logo_beatWars_1024.png");
}

bool CMenu_State::Input(void)
{

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		return false;
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP) )
	{
		m_nMenuSelection -= 1;
		if( m_nMenuSelection == -1 )
		{
			m_nMenuSelection = MAINMENU_EXIT;
		}

	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) )
	{
		m_nMenuSelection += 1;

		if( m_nMenuSelection == NUM_MAINMENU_OPTIONS )
		{
			m_nMenuSelection = 0;
		}
	}

	if( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) )
	{
		switch( m_nMenuSelection )
		{
		case MAINMENU_NEWGAME:
			{
				CLU_State::GetInstance()->SetNewState(CGameplay_State::GetInstance());
				CGame::GetInstance()->ChangeState(CLU_State::GetInstance());
			}
			break;

		case MAINMENU_LOAD:	// GOES TO THE SKILLS TEST FOR NOW
			{
			}
			break;

		case MAINMENU_OPTIONS:
			{
				CGame::GetInstance()->ChangeState(COptionsState::GetInstance());
			}
			break;		
		case MAINMENU_CREDITS:
			{
				//CGame::GetInstance()->ChangeState()
			}
			break;
		case MAINMENU_LEVEL:
			{
				CGame::GetInstance()->ChangeState(CLevelSelect_State::GetInstance());
			}
			break;


		case MAINMENU_EXIT:
			{
				return false;
			}
			break;

		}
	}

	return true;
}

void CMenu_State::Update(void)
{
}

void CMenu_State::Render(void)
{
	RECT rBody = {0, 286, CGame::GetInstance()->GetScreenWidth(), 635};
	RECT rTitle = {0,0,540,349};
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID,0,0, 0.78125f, 0.5859375f);
	
	CBitmapFont::GetInstance()->SetScale(4.5f);
	CSGD_TextureManager::GetInstance()->Draw(m_nTile,128,1,1.0f,1.0f,&rTitle);

	int topSelection = 380;
	int spacing = 30;
	switch(m_nMenuSelection)
	{
	case MAINMENU_NEWGAME:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 32, topSelection  + (spacing * MAINMENU_NEWGAME));
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 768, topSelection  + (spacing * MAINMENU_NEWGAME), -1.0f);
		}
		break;

	case MAINMENU_LOAD: // skills test for now
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 32, topSelection  + (spacing * MAINMENU_LOAD));
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 768, topSelection  + (spacing * MAINMENU_LOAD), -1.0f);
		}
		break;

	case MAINMENU_OPTIONS:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 32, topSelection  + (spacing * MAINMENU_OPTIONS));
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 768, topSelection  + (spacing * MAINMENU_OPTIONS), -1.0f);
		}
		break;	

	case MAINMENU_CREDITS:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 32, topSelection  + (spacing * MAINMENU_CREDITS));
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 768, topSelection  + (spacing * MAINMENU_CREDITS), -1.0f);
		}
		break;

	case MAINMENU_LEVEL:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 32, topSelection  + (spacing * MAINMENU_LEVEL));
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 768, topSelection  + (spacing * MAINMENU_LEVEL), -1.0f);
		}
		break;

	case MAINMENU_EXIT:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 32, topSelection  + (spacing * MAINMENU_EXIT));
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 768, topSelection  + (spacing * MAINMENU_EXIT), -1.0f);
		}
		break;
	}
	
	CBitmapFont::GetInstance()->SetScale(1.5f);
	CBitmapFont::GetInstance()->PrintStrokedTextInRect("new game\nload\noptions\ncredits\nlevel select\nexit", &rBody, ALIGN_CENTER, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
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