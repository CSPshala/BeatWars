///////////////////////////////////////////////////////
// File Name	:	"CMenu_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CMenu_State.h"
#include "CBitmapFont.h"
#include "CGameplay_State.h"

CMenu_State::CMenu_State()
{
	
	

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
	m_nMenuSelection = 0;
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/star-gazing2.png");
	m_nCursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/lightsaberCursor.png");
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
			}
			break;		

		case MAINMENU_CREDITS:
			{
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
	
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID,0,0,1.6f,1.3f);
	CBitmapFont::GetInstance()->SetScale(3);
	CBitmapFont::GetInstance()->PrintText("BeatWars",244,25,D3DCOLOR_XRGB(255,255,255));

	CBitmapFont::GetInstance()->SetScale(1.5f);
	CBitmapFont::GetInstance()->PrintText("PLAY", 225, 175,D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintText("OPTIONS", 225, 215,D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintText("SKILLS TEST", 225, 255,D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintText("CREDITS", 225, 295,D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintText("EXIT", 225, 335,D3DCOLOR_XRGB(255, 255, 255));

	int topSelection = 175;
	int spacing = 40;
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

	case MAINMENU_EXIT:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 100, topSelection  + (spacing * MAINMENU_EXIT) );
		}
		break;
	}
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