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
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/star-gazing2.png");
	m_nCursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/lightsaberCursor.png");
	CFXManager::GetInstance()->LoadFX("Test.xml", "MENU_PARTICLE");
	CFXManager::GetInstance()->QueueParticle("MENU_PARTICLE");
	m_nTile = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/title.png");
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
				CGame::GetInstance()->ChangeState( CGameplay_State::GetInstance() );
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
	CFXManager::GetInstance()->Update(CGame::GetInstance()->GetTimer().GetDeltaTime());
}

void CMenu_State::Render(void)
{
	RECT rBody = {225, 200, CGame::GetInstance()->GetScreenWidth(), 400};
	RECT rTitle = {0,25,800,75};
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID,0,0,1.6f,1.3f);
	
	CBitmapFont::GetInstance()->SetScale(4.5f);
	//CBitmapFont::GetInstance()->PrintInRect("BeatWars",&rTitle,ALIGN_CENTER,D3DCOLOR_XRGB(242,251,4));
	CSGD_TextureManager::GetInstance()->Draw(m_nTile,145,25,2.0f,1.0f);
	CBitmapFont::GetInstance()->SetScale(3.0f);
	CBitmapFont::GetInstance()->PrintInRect("new game\nload\noptions\ncredits\nlevel select\nexit", &rBody, ALIGN_LEFT, D3DCOLOR_XRGB(225, 225, 225));
	/*
RECT rNewGame = {0,175,800,205};
	RECT rLoad = {0,205,800,235};
	RECT rOptions = {0,235,800,265};
	RECT rCredits = {0,265,800,295};
	RECT rExit = {0,295,800,330};
	CBitmapFont::GetInstance()->SetScale(1.0f);
	//CBitmapFont::GetInstance()->PrintText("new game", 225, 175,D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintInRect("new game", &rNewGame, ALIGN_CENTER, D3DCOLOR_XRGB(225, 225, 225));
	CBitmapFont::GetInstance()->SetScale(1.5f);
	//CBitmapFont::GetInstance()->PrintText("load", 250, 215,D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintInRect("load", &rLoad, ALIGN_CENTER, D3DCOLOR_XRGB(225, 225, 225));
	CBitmapFont::GetInstance()->SetScale(2.0f);
	//CBitmapFont::GetInstance()->PrintText("options", 300, 255,D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintInRect("options", &rOptions, ALIGN_CENTER, D3DCOLOR_XRGB(225, 225, 225));
	CBitmapFont::GetInstance()->SetScale(2.5f);
	//CBitmapFont::GetInstance()->PrintText("credits", 325, 295,D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintInRect("credits", &rCredits, ALIGN_CENTER, D3DCOLOR_XRGB(225, 225, 225));
	CBitmapFont::GetInstance()->SetScale(3.0f);
	//CBitmapFont::GetInstance()->PrintText("exit", 350, 335,D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintInRect("exit", &rExit, ALIGN_CENTER, D3DCOLOR_XRGB(225, 225, 225));
*/

	int topSelection = 175;
	int spacing = 63;
	switch(m_nMenuSelection)
	{
	case MAINMENU_NEWGAME:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 100, topSelection  + (spacing * MAINMENU_NEWGAME) );
		}
		break;

	case MAINMENU_LOAD: // skills test for now
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 100, topSelection  + (spacing * MAINMENU_LOAD) );
		}
		break;

	case MAINMENU_OPTIONS:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 100, topSelection  + (spacing * MAINMENU_OPTIONS) );
		}
		break;	

	case MAINMENU_CREDITS:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 100, topSelection  + (spacing * MAINMENU_CREDITS) );
		}
		break;

	case MAINMENU_LEVEL:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 100, topSelection  + (spacing * MAINMENU_LEVEL) );
		}
		break;

	case MAINMENU_EXIT:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 100, topSelection  + (spacing * MAINMENU_EXIT) );
		}
		break;
	}
	
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	CFXManager::GetInstance()->Render();
	
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