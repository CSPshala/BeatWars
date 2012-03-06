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
#include "../Managers/CBeatManager.h"
#include "CLU_State.h"
#include "CSave_State.h"
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
	m_nGameImageID = -1;
	m_nLoadImageID = -1;
	m_nLoadID = -1;
}

CLoad_State::~CLoad_State()
{

}

void CLoad_State::Enter(void)
{	
	m_nCursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/lightsaberCursor2.png");
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MainMenuBG.jpg");
	m_nLoadID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/load.png");
	m_nGameImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/logo_beatWars_1024.png");
	m_nLoadImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/phatbeatsscreen2.png");
}

bool CLoad_State::Input(void)
{


	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->MouseButtonPressed(1))
		CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());

	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 0) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_UP, 1))
	{
		m_nMenuSelection -= 1;
		if (m_nMenuSelection == -1)
		{
			m_nMenuSelection = NUM_LOADMENU_OPTIONS - 1;

		}
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 0) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_DOWN, 1) )
	{
		m_nMenuSelection += 1;

		if( m_nMenuSelection == NUM_LOADMENU_OPTIONS )
		{
			m_nMenuSelection = 0;
		}

	}

	if( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(0, 0))
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
				CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());
			}
			break;
		case LOADMENU_SLOTTHREE:
			{
				m_nLoadFlag = true;		
				CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());
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
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID,0,0, 0.78125f, 0.5859375f);

	// rect for options title
	RECT gOptions = {0,0,1000,120};
	CSGD_TextureManager::GetInstance()->Draw(m_nLoadID,15,40,0.4f,0.9f,&gOptions);
	// rect for game image 
	RECT gImage = {0,350,290,550};
	CSGD_TextureManager::GetInstance()->Draw(m_nGameImageID,450,15,1.0f,1.0f,&gImage);

	
	//CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	 Draw everything now that is queued up
	/*
	CBitmapFont::GetInstance()->SetScale(3.5f);
		RECT rTitle = {0, 40, CGame::GetInstance()->GetScreenWidth(), 80};
		CBitmapFont::GetInstance()->PrintInRect("load", &rTitle, ALIGN_CENTER,D3DCOLOR_XRGB(242,251,4));*/
	RECT gSaveImage = {0,0,225,125};
	if (CSave_State::GetInstance()->GetImageSave() == true && m_nMenuSelection == LOADMENU_SLOTONE)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nLoadImageID,50,160,1.0f,1.0f,&gSaveImage);
		CBitmapFont::GetInstance()->SetScale(1.5f);
		RECT rMenuOptions = { 25, 295, CGame::GetInstance()->GetScreenWidth()-500, 450};
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("level one\n\n\n\n",
			&rMenuOptions, ALIGN_LEFT,D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(225, 225, 225));
	}
	else
	{
		CBitmapFont::GetInstance()->SetScale(1.5f);
		RECT rMenuOptions = { 15, 250, CGame::GetInstance()->GetScreenWidth(), 450};
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("\n\nslot two\n\nslot three",
			&rMenuOptions, ALIGN_CENTER,D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(225, 225, 225));

	}
	if (CSave_State::GetInstance()->GetImageSave() == true && m_nMenuSelection == LOADMENU_SLOTTWO)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nLoadImageID,50,220,1.0f,1.0f,&gSaveImage);
		CBitmapFont::GetInstance()->SetScale(1.5f);
		RECT rMenuOptions = { 25, 295, CGame::GetInstance()->GetScreenWidth()-500, 450};
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("\n\nlevel two\n\n",
			&rMenuOptions, ALIGN_LEFT,D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(225, 225, 225));
	}
	else
	{
		CBitmapFont::GetInstance()->SetScale(1.5f);
		RECT rMenuOptions = { 15, 250, CGame::GetInstance()->GetScreenWidth(), 450};
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("slot one\n\nslot two\n\nslot three",
			&rMenuOptions, ALIGN_CENTER,D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(225, 225, 225));

	}
	if (CSave_State::GetInstance()->GetImageSave() == true && m_nMenuSelection == LOADMENU_SLOTTHREE)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nLoadImageID,50,275,1.0f,1.0f,&gSaveImage);
		CBitmapFont::GetInstance()->SetScale(1.5f);
		RECT rMenuOptions = { 25, 295, CGame::GetInstance()->GetScreenWidth()-500, 450};
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("\n\n\n\nlevel three",
			&rMenuOptions, ALIGN_LEFT,D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(225, 225, 225));
	}
	else
	{
		CBitmapFont::GetInstance()->SetScale(1.5f);
		RECT rMenuOptions = { 15, 250, CGame::GetInstance()->GetScreenWidth(), 450};
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("slot one\n\nslot two\n\nslot three",
			&rMenuOptions, ALIGN_CENTER,D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(225, 225, 225));

	}

	
	/*
	CBitmapFont::GetInstance()->SetScale(1.5f);
	RECT rMenuOptions = { 15, 250, CGame::GetInstance()->GetScreenWidth(), 450};
	CBitmapFont::GetInstance()->PrintStrokedTextInRect("slot one\n\nslot two\n\nslot three",
	&rMenuOptions, ALIGN_CENTER,D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(225, 225, 225));
	*/
		
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	switch(m_nMenuSelection)
	{
	case LOADMENU_SLOTONE:
		{			
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 285 + (LOADMENU_SLOTONE * 55) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 285  + (55* LOADMENU_SLOTONE), -1.0f);
		}
		break;

	case LOADMENU_SLOTTWO:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 285 + (LOADMENU_SLOTTWO * 55) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 285  + (55* LOADMENU_SLOTTWO), -1.0f);
		}
		break;

	case LOADMENU_SLOTTHREE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 285 + (LOADMENU_SLOTTHREE * 55) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 285  + (55* LOADMENU_SLOTTHREE), -1.0f);
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

string CLoad_State::loadGame()
{
	ifstream inFile("resource/saves/SaveGame.fu");
	
	if (inFile.is_open())
	{
		if (inFile.good())
		{
			inFile>>m_nSlotNumber;
			inFile>>m_szSongFileName;
			inFile>>m_szSongName;
			SetSongName(m_szSongName);
			SetFileName(m_szSongFileName);
		}
		inFile.close();
	}
	return GetFileName();
}
