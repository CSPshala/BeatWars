///////////////////////////////////////////////////////
// File Name	:	"CLoad_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CLoad_State.h"
#include "CBitmapFont.h"
#include "../Globals.h"
#include "../Managers/CBeatManager.h"
#include "../CPlayer.h"
#include "CGameplay_State.h"
#include "../CGame.h"

CLoad_State::CLoad_State()
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
	m_nLoadFlag = false;
}

CLoad_State::~CLoad_State()
{

}

void CLoad_State::Enter(void)
{	
	m_nCursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/lightsaberCursor2.png");
}

bool CLoad_State::Input(void)
{


	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());

	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP))
	{
		m_nMenuSelection -= 1;
		if (m_nMenuSelection == -1)
		{
			m_nMenuSelection = NUM_LOADMENU_OPTIONS - 1;

		}
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) )
	{
		m_nMenuSelection += 1;

		if( m_nMenuSelection == NUM_LOADMENU_OPTIONS )
		{
			m_nMenuSelection = 0;
		}

	}

	if( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) )
	{
		switch( m_nMenuSelection )
		{
		case LOADMENU_SLOTONE:
			{
				m_nLoadFlag = true;
				CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());
			}
			break;
		case LOADMENU_SLOTTWO:
			{
				m_nLoadFlag = true;				
			}
			break;
		case LOADMENU_SLOTTHREE:
			{
				m_nLoadFlag = true;				
			}
			break;

		}
	}
	return true;
}

void CLoad_State::Update(void)
{

}

void CLoad_State::Render(void)
{
	
	
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	CBitmapFont::GetInstance()->SetScale(3.5f);
	RECT rTitle = {0, 40, CGame::GetInstance()->GetScreenWidth(), 80};
	CBitmapFont::GetInstance()->PrintInRect("load", &rTitle, ALIGN_CENTER,D3DCOLOR_XRGB(242,251,4));
	CBitmapFont::GetInstance()->SetScale(1.0f);
	RECT rMenuOptions = { 225, 177, CGame::GetInstance()->GetScreenWidth(), 180};
	CBitmapFont::GetInstance()->PrintInRect("slot one\n\nslot two\n\nslot three",
		&rMenuOptions, ALIGN_LEFT, D3DCOLOR_XRGB(225, 225, 225));
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	switch(m_nMenuSelection)
	{
	case LOADMENU_SLOTONE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (LOADMENU_SLOTONE * 40) );
		}
		break;

	case LOADMENU_SLOTTWO:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (LOADMENU_SLOTTWO * 40) );
		}
		break;

	case LOADMENU_SLOTTHREE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (LOADMENU_SLOTTHREE * 40) );
		}
		break;

	}	
	
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

void CLoad_State::loadGame()
{
	ifstream inFile("resource/saves/SaveGame.fu");

	if (inFile.is_open())
	{
		if (inFile.good())
		{
			inFile>>m_nSlotNumber;
			inFile>>m_szSongFileName;
			CBeatManager::GetInstance()->LoadSong(m_szSongFileName);
		}
		inFile.close();
	}
}
