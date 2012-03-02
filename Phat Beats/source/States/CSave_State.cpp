///////////////////////////////////////////////////////
// File Name	:	"CSave_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CSave_State.h"
#include "CBitmapFont.h"
#include "../Globals.h"
#include "../Managers/CBeatManager.h"
#include "../CPlayer.h"
#include "CGameplay_State.h"
#include "COptionsState.h"
#include "CPause_State.h"
CSave_State::CSave_State()
{
	//m_bMenu_Font = NULL;
	m_nMenuSelection = 0;

	// Asset IDs
	m_nBackgroundID = -1;
	m_nCursorImageID = -1;
	m_nFontID = -1;
	m_nTitleID = -1;
	m_nBackSoundID = -1;
	m_nCursorSoundID = -1;
	m_nSlotNumber = 0;
}

CSave_State::~CSave_State()
{

}

void CSave_State::Enter(void)
{	
	m_nCursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/lightsaberCursor2.png");
}

bool CSave_State::Input(void)
{

	/*
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
				return false;
				*/

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());

	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP))
	{
		m_nMenuSelection -= 1;
		if (m_nMenuSelection == -1)
		{
			m_nMenuSelection = NUM_SAVEMENU_OPTIONS - 1;

		}
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) )
	{
		m_nMenuSelection += 1;

		if( m_nMenuSelection == NUM_SAVEMENU_OPTIONS )
		{
			m_nMenuSelection = 0;
		}

	}

	if( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) )
	{
		switch( m_nMenuSelection )
		{
		case SAVEMENU_SLOTONE:
			{
				m_nSlotNumber = 1;
				saveGame();
				CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
			}
			break;
		case SAVEMENU_SLOTTWO:
			{
				m_nSlotNumber = 2;
				saveGame();
				CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
			}
			break;
		case SAVEMENU_SLOTTHREE:
			{
				m_nSlotNumber = 3;
				saveGame();
				CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
			}
			break;

		}
	}
	return true;
}

void CSave_State::Update(void)
{

}

void CSave_State::Render(void)
{
	
	//CBitmapFont::GetInstance()->PrintText("slot one",215,175,D3DCOLOR_XRGB(255,255,255));
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	CBitmapFont::GetInstance()->SetScale(3.5f);
	RECT rTitle = {0, 40, CGame::GetInstance()->GetScreenWidth(), 80};
	CBitmapFont::GetInstance()->PrintInRect("save", &rTitle, ALIGN_CENTER,D3DCOLOR_XRGB(242,251,4));
	CBitmapFont::GetInstance()->SetScale(1.0f);
	RECT rMenuOptions = { 225, 177, CGame::GetInstance()->GetScreenWidth(), 180};
	CBitmapFont::GetInstance()->PrintInRect("slot one\n\nslot two\n\nslot three",
		&rMenuOptions, ALIGN_LEFT, D3DCOLOR_XRGB(225, 225, 225));
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	switch(m_nMenuSelection)
	{
	case SAVEMENU_SLOTONE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (SAVEMENU_SLOTONE * 40) );
		}
		break;

	case SAVEMENU_SLOTTWO:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (SAVEMENU_SLOTTWO * 40) );
		}
		break;

	case SAVEMENU_SLOTTHREE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 75, 140 + (SAVEMENU_SLOTTHREE * 40) );
		}
		break;

	}	
	
}

void CSave_State::Exit(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nCursorImageID);
}

CSave_State* CSave_State::GetInstance()
{
	// Lazy instantiation
	static CSave_State instance; // Static allows passing back of address
	return &instance;	
}

void CSave_State::saveGame()
{
	ofstream outFile("resource/saves/SaveGame.fu");

	if (outFile.is_open())
	{
		if (outFile.good())
		{
			outFile<<m_nSlotNumber;
			outFile<<"\n";
			outFile<<CBeatManager::GetInstance()->GetFileName()<<"\n";
			outFile<<CBeatManager::GetInstance()->GetCurrentlyPlayingSongName();
		}
		outFile.close();
	}
	

}
