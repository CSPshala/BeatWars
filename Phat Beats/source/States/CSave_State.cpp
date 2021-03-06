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
#include "CLevelSelect_State.h"

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
	m_nGameImageID = -1;
	m_nSaveID = -1;
	m_nSaveImageID = -1;
	m_IsbSaveImage = false;
}

CSave_State::~CSave_State()
{

}

void CSave_State::Enter(void)
{	
	m_nCursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/lightsaberCursor2.png");
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MainMenuBG.jpg");
	m_nSaveID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/save.png");
	m_nGameImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/logo_beatWars_1024.png");
	m_nSaveImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/phatbeatsscreen2.png");
	
	
}

bool CSave_State::Input(void)
{

	/*
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
				return false;
				*/
#pragma region KEYBOARD
	if (!CGame::GetInstance()->GetPlayerControl()->IsConnected())
	{
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(6))
			CGame::GetInstance()->ChangeState(CMenu_State::GetInstance());

		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_BACKSPACE))
			CGame::GetInstance()->GoBack();

		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_UP, 1))
		{
			CGame::GetInstance()->PlayNavMenuSound();
			m_nMenuSelection -= 1;
			if (m_nMenuSelection == -1)
			{
				m_nMenuSelection = NUM_SAVEMENU_OPTIONS - 1;

			}
		}
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_DOWN, 1) )
		{
			CGame::GetInstance()->PlayNavMenuSound();
			m_nMenuSelection += 1;

			if( m_nMenuSelection == NUM_SAVEMENU_OPTIONS )
			{
				m_nMenuSelection = 0;
			}

		}

		if( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(0, 0) )
		{
			CGame::GetInstance()->PlayAccMenuSound();
			switch( m_nMenuSelection )
			{
			case SAVEMENU_SLOTONE:
				{
					m_nSlotNumber = 1;
					saveGame();
					CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
					m_IsbSaveImage = true;
					SetImageSave(m_IsbSaveImage);
				}
				break;
			case SAVEMENU_SLOTTWO:
				{
					m_nSlotNumber = 2;
					saveGame();
					CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
					m_IsbSaveImage = true;
					SetImageSave(m_IsbSaveImage);
				}
				break;
			case SAVEMENU_SLOTTHREE:
				{
					m_nSlotNumber = 3;
					saveGame();
					CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
					m_IsbSaveImage = true;
					SetImageSave(m_IsbSaveImage);
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
			CGame::GetInstance()->ChangeState(CMenu_State::GetInstance());

		if (CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_UP, 1))
		{
			m_nMenuSelection -= 1;
			if (m_nMenuSelection == -1)
			{
				m_nMenuSelection = NUM_SAVEMENU_OPTIONS - 1;

			}

			CGame::GetInstance()->PlayNavMenuSound();
		}
		if(CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_DOWN, 1) )
		{
			m_nMenuSelection += 1;

			if( m_nMenuSelection == NUM_SAVEMENU_OPTIONS )
			{
				m_nMenuSelection = 0;
			}

			CGame::GetInstance()->PlayNavMenuSound();
		}

		if( CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A && CGame::GetInstance()->GetButtonPressed() == false)
		{
			CGame::GetInstance()->SetButtonPressed(true);
			CGame::GetInstance()->PlayAccMenuSound();
			switch( m_nMenuSelection )
			{
			case SAVEMENU_SLOTONE:
				{
					m_nSlotNumber = 1;
					saveGame();
					CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
					m_IsbSaveImage = true;
					SetImageSave(m_IsbSaveImage);
				}
				break;
			case SAVEMENU_SLOTTWO:
				{
					m_nSlotNumber = 2;
					saveGame();
					CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
					m_IsbSaveImage = true;
					SetImageSave(m_IsbSaveImage);
				}
				break;
			case SAVEMENU_SLOTTHREE:
				{
					m_nSlotNumber = 3;
					saveGame();
					CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
					m_IsbSaveImage = true;
					SetImageSave(m_IsbSaveImage);
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

void CSave_State::Update(void)
{

}

void CSave_State::Render(void)
{
	
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID,0,0, 0.78125f, 0.5859375f);

	// rect for options title
	RECT gOptions = {0,0,1000,120};
	CSGD_TextureManager::GetInstance()->Draw(m_nSaveID,15,40,0.4f,0.9f,&gOptions);
	// rect for game image 
	RECT gImage = {0,350,290,550};
	CSGD_TextureManager::GetInstance()->Draw(m_nGameImageID,450,15,1.0f,1.0f,&gImage);

	CBitmapFont::GetInstance()->PrintText("press esc. to go back", 10, 10, D3DCOLOR_XRGB(225, 225, 225));

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	RECT gSaveImage = {0,0,225,125};
	if (GetImageSave() == true && m_nMenuSelection == SAVEMENU_SLOTONE)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nSaveImageID,50,160,1.0f,1.0f,&gSaveImage);
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
	if (GetImageSave() == true && m_nMenuSelection == SAVEMENU_SLOTTWO)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nSaveImageID,50,220,1.0f,1.0f,&gSaveImage);
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
	if (GetImageSave() == true && m_nMenuSelection == SAVEMENU_SLOTTHREE)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nSaveImageID,50,275,1.0f,1.0f,&gSaveImage);
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

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	switch(m_nMenuSelection)
	{
	case SAVEMENU_SLOTONE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 285 + (SAVEMENU_SLOTONE * 55) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 285  + (55* SAVEMENU_SLOTONE), -1.0f);
		}
		break;

	case SAVEMENU_SLOTTWO:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 285 + (SAVEMENU_SLOTTWO * 55) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 285  + (55* SAVEMENU_SLOTTWO), -1.0f);
		}
		break;

	case SAVEMENU_SLOTTHREE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 15, 285 + (SAVEMENU_SLOTTHREE * 55) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 785, 285  + (55* SAVEMENU_SLOTTHREE), -1.0f);
		}
		break;

	}	
	
}

void CSave_State::Exit(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nCursorImageID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nSaveID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nGameImageID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nBackgroundID);
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
			outFile<<CBeatManager::GetInstance()->GetCurrentlyPlayingSong()->GetCurrentFileName()<<"\n";
			outFile<<CBeatManager::GetInstance()->GetCurrentlyPlayingSongName();
			
		}
		outFile.close();
	}


}

void CSave_State::saveConfig()
{

	ofstream out("resource/saves/gameconfig.ass");

	m_nFXVolume = COptionsState::GetInstance()->GetFXVol();
	m_nMusicVolume = COptionsState::GetInstance()->GetMusicVol();
	Ailevel = COptionsState::GetInstance()->GetAILevel();
	playerDiff = COptionsState::GetInstance()->GetDifficulty();
	if (out.is_open())
	{
		if (out.good())
		{
			out<<m_nFXVolume<<'\n';
			out<<m_nMusicVolume<<'\n';
			out<<Ailevel<<'\n';
			out<<playerDiff;

		}
		out.close();
	}


}
