#include "COptionsState.h"
#include "CMenu_State.h"
#include "../SGD Wrappers\CSGD_TextureManager.h"
#include "../SGD Wrappers\CSGD_Direct3D.h"
#include "../SGD Wrappers\CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_FModManager.h"

#include "CBitmapFont.h"
//#include "CGameProfiler.h"
#include "../Globals.h"

#include "CGameplay_State.h"
#include "CPause_State.h"

COptionsState::COptionsState( void )
{
	SetDifficulty(HARD);
}
COptionsState::~COptionsState(void)
{

}
COptionsState* COptionsState::GetInstance( void )
{
	//	Lazy instantiation
	static COptionsState instance;
	return &instance;
}
void COptionsState::Enter(void)
{
	m_nMenuSelection = 0;
	m_nSFX = -1;

	//CGameProfiler::GetInstance()->LoadUserSettings("user settings.txt");

	// assign values to the local variables

	CGame::GetInstance()->GetSFXVolume();
	CGame::GetInstance()->GetMusicVolume();
	m_nMusicPan = CGame::GetInstance()->GetPanVolume();
	//m_nLives = CGame::GetInstance()->GetStartingLives();

	

	m_nCursorID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/lightsaberCursor.png" );	

	//TODO:
	m_nSFX = CSGD_FModManager::GetInstance()->LoadSound("resource/light_saber.wav");
	//m_nBGM = CSGD_FModManager::GetInstance()->LoadSound("resource/sound/cantina.mp3", FMOD_LOOP_NORMAL);




	CSGD_FModManager::GetInstance()->SetVolume(m_nBGM,CGame::GetInstance()->GetMusicVolume());
	CSGD_FModManager::GetInstance()->SetPan(m_nBGM,CGame::GetInstance()->GetPanVolume());

	//CSGD_FModManager::GetInstance()->PlaySound(m_nBGM);
}
bool COptionsState::Input(void)
{
	// If you're at the top and press up, cycle the cursor to the bottom selection
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_UP))
	{
		m_nMenuSelection -= 1;

		if( m_nMenuSelection < OPTIONSMENU_SFXVOL )
		{
			m_nMenuSelection = NUM_OPTIONSMENU_OPTIONS - 1;
		}

	}

	// If you're at the bottom and press down, cycle the cursor back to the top selection
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_DOWN) )
	{
		m_nMenuSelection += 1;

		if( m_nMenuSelection >= NUM_OPTIONSMENU_OPTIONS)
		{
			m_nMenuSelection = OPTIONSMENU_SFXVOL;
		}

	}

	if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_LEFT) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_LEFT) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_LEFT) )
	{
		switch (m_nMenuSelection)
		{
		case OPTIONSMENU_SFXVOL:
			m_nFXVolume -= 0.001f;
			if( m_nFXVolume <= 0.0f )
			{
				m_nFXVolume = 0.0f;
			}

			CSGD_FModManager::GetInstance()->SetVolume(m_nSFX, m_nFXVolume);
			break;
		case OPTIONSMENU_MUSICVOL:			
			m_nMusicVolume -= 0.001f;
			if( m_nMusicVolume <= 0.0f )
			{
				m_nMusicVolume = 0.0f;
			}
			CSGD_FModManager::GetInstance()->SetVolume(m_nBGM, m_nMusicVolume);

			break;
		case OPTIONSMENU_PAN:
			m_nMusicPan -= 0.002f;
			if( m_nMusicPan <= -1.0f )
			{
				m_nMusicPan = -1.0f;
			}
			CSGD_FModManager::GetInstance()->SetPan(m_nBGM, m_nMusicPan);
			CSGD_FModManager::GetInstance()->SetPan(m_nSFX, m_nMusicPan);
			break;
		
		}		
	}

	

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_LEFT) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_LEFT) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_LEFT))
	{
		switch (m_nMenuSelection)
		{
			// Not Needed
		//case OPTIONSMENU_LIVES:
			//m_nLives -= 1;
			//if( m_nLives <= 1 )
			//{
			//	m_nLives = 1;
			//}
			////CGame::GetInstance()->SetStartingLives(m_nLives);
			//break;
		case OPTIONSMENU_WINDOWED:
			{
				//CGame::GetInstance()->ChangeWindowMode();
			}
			break;

		case OPTIONSMENU_DIFFICULTY:
		{
			SetDifficulty(BeatDifficulty((int)GetDifficulty() - 1));

			if(GetDifficulty() < EASY)
				SetDifficulty(HARD);
		}
		break;

		}

	}


	// Play the a sample sound when the user releases Left while changing the volume of the sound effects
	if(CSGD_DirectInput::GetInstance()->KeyReleased(DIK_LEFT) || CSGD_DirectInput::GetInstance()->JoystickButtonReleased(DIR_LEFT) )
	{
		switch (m_nMenuSelection)
		{
		case OPTIONSMENU_SFXVOL:
			CSGD_FModManager::GetInstance()->PlaySound(m_nSFX);
			break;	
		}
	}
	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_P) || CSGD_DirectInput::GetInstance()->MouseButtonPressed(1))
	{
		CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
	}


	if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(DIR_RIGHT) )
	{
		switch (m_nMenuSelection)
		{
		case OPTIONSMENU_SFXVOL:
			{			
				m_nFXVolume += 0.001f;
				if( m_nFXVolume >= 1.0f )
				{
					m_nFXVolume = 1.0f;
				}
				CSGD_FModManager::GetInstance()->SetVolume(m_nSFX, m_nFXVolume);			
			}
			break;

		case OPTIONSMENU_MUSICVOL:

			m_nMusicVolume += 0.001f;
			if( m_nMusicVolume >= 1.0f )
			{
				m_nMusicVolume = 1.0f;
			}
			CSGD_FModManager::GetInstance()->SetVolume(m_nBGM, m_nMusicVolume);			
			break;

		case OPTIONSMENU_PAN:
			m_nMusicPan += 0.002f;
			if( m_nMusicPan >= 1.0f )
			{
				m_nMusicPan = 1.0f;
			}
			CSGD_FModManager::GetInstance()->SetPan(m_nBGM, m_nMusicPan);
			CSGD_FModManager::GetInstance()->SetPan(m_nSFX, m_nMusicPan);
			break;
		}
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickButtonReleased(DIR_RIGHT) )
	{
		switch (m_nMenuSelection)
		{
			// Not Needed
		//case OPTIONSMENU_LIVES:
			/*
			m_nLives += 1;
			if( m_nLives >= CGame::GetInstance()->GetMaxStartingLives() )
			{
			m_nLives = CGame::GetInstance()->GetMaxStartingLives();
			}*/

			//CGame::GetInstance()->SetStartingLives(m_nLives);
			break;

		case OPTIONSMENU_WINDOWED:
			{
				//CGame::GetInstance()->ChangeWindowMode();
			}
			break;

		case OPTIONSMENU_DIFFICULTY:
			{
			SetDifficulty(BeatDifficulty((int)GetDifficulty() + 1));

			if(GetDifficulty() > HARD)
				SetDifficulty(EASY);
			}
			break;
		}
	}


	// Play the a sample sound when the user releases Right while changing the volume of the sound effects
	if(CSGD_DirectInput::GetInstance()->KeyReleased(DIK_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickButtonReleased(DIR_RIGHT) )
	{
		switch (m_nMenuSelection)
		{
		case OPTIONSMENU_SFXVOL:
			CSGD_FModManager::GetInstance()->PlaySound(m_nSFX);
			break;	
		}
	}

	if( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(6) || CSGD_DirectInput::GetInstance()->JoystickGetRStickDirPressed(6) )
	{
		switch( m_nMenuSelection )
		{
		case OPTIONSMENU_GAME:
			{
				CGame::GetInstance()->ChangeState(CGameplay_State::GetInstance());
			}
			break;
		case OPTIONSMENU_EXIT:
			{
				CGame::GetInstance()->ChangeState( CMenu_State::GetInstance() );
			}
			break;

		}
	}

	return true;
}
void COptionsState::Update(void)
{

	//TODO:
	if( !CSGD_FModManager::GetInstance()->IsSoundPlaying(m_nBGM) )
		CSGD_FModManager::GetInstance()->PlaySound(m_nBGM);


}
void COptionsState::Render(void)
{

	char buffer[255];
	//CBitmapFont BF;

	CBitmapFont::GetInstance()->SetScale(4.5f);

	RECT rTitle = {0, 40, CGame::GetInstance()->GetScreenWidth(), 80};
	CBitmapFont::GetInstance()->PrintInRect("options", &rTitle, ALIGN_CENTER,D3DCOLOR_XRGB(242,251,4));
	//BF.PrintText("Options", 10, 10, 1, D3DCOLOR_XRGB(255, 255, 255), true);

	CBitmapFont::GetInstance()->SetScale(1.0f);
	RECT rMenuOptions = { 225, 175, CGame::GetInstance()->GetScreenWidth(), 378};
	CBitmapFont::GetInstance()->PrintInRect("SFx volume\n\nMusic volume\n\nMusic Pan\n\nDifficulty\n\nWindowed Mode\n\nBack\n\ngo back to game", &rMenuOptions, ALIGN_LEFT, D3DCOLOR_XRGB(225, 225, 225));
	sprintf_s( buffer, "%d", int( m_nFXVolume * 100));
	//BF.PrintText("SFX Volume", 100, 50, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintText(buffer, 450, 147, D3DCOLOR_XRGB(225, 225, 225));
	//BF.PrintText(buffer, 400, 50, 0.75f, D3DCOLOR_XRGB(255, 255, 255));


	sprintf_s( buffer, "%d", int( m_nMusicVolume * 100));
	//BF.PrintText("Music Volume", 100, 80, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintText(buffer, 450, 187, D3DCOLOR_XRGB(225, 225, 225));
	//BF.PrintText(buffer, 400, 80, 0.75f, D3DCOLOR_XRGB(255, 255, 255));

	sprintf_s( buffer, "L%d - R%d", int( 50 + (m_nMusicPan * -50)), int( 50 + (m_nMusicPan * 50)));
	//BF.PrintText("Music Pan", 100, 110, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
	CBitmapFont::GetInstance()->PrintText(buffer, 450, 227, D3DCOLOR_XRGB(225, 225, 225));
	//BF.PrintText(buffer, 400, 110, 0.75f, D3DCOLOR_XRGB(255, 255, 255));

	// Not needed right now, if not EVER  - JC
	//sprintf_s( buffer, "%i", m_nLives);
	//BF.PrintText("Starting Lives", 100, 140, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
	//CBitmapFont::GetInstance()->PrintText(buffer, 450, 267, D3DCOLOR_XRGB(225, 225, 225));
	//BF.PrintText(buffer, 420, 140, 0.75f, D3DCOLOR_XRGB(255, 255, 255));

	switch(GetDifficulty())
	{
	case EASY:
		sprintf_s( buffer,"easy");
		break;

	case NORMAL:
		sprintf_s( buffer,"normal");
		break;

	case HARD:
		sprintf_s( buffer,"hard");
		break;
	}

	CBitmapFont::GetInstance()->PrintText(buffer, 450, 267, D3DCOLOR_XRGB(225, 225, 225));

	//BF.PrintText("Back", 100, 170, 0.75f, D3DCOLOR_XRGB(255, 255, 255));

	//draw the cursor at the current selection
	switch(m_nMenuSelection)
	{
	case OPTIONSMENU_SFXVOL:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 75, 143 + (OPTIONSMENU_SFXVOL * 40) );
		}
		break;

	case OPTIONSMENU_MUSICVOL:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 75, 143 + (OPTIONSMENU_MUSICVOL * 40) );
		}
		break;

	case OPTIONSMENU_PAN:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 75, 143 + (OPTIONSMENU_PAN * 40) );
		}
		break;

	// Again commented out because not needed - JC
	/*case OPTIONSMENU_LIVES:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 75, 143 + (OPTIONSMENU_LIVES * 40) );
		}
		break;*/

	case OPTIONSMENU_DIFFICULTY:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 75, 143 + (OPTIONSMENU_DIFFICULTY * 40) );
		}
		break;

	case OPTIONSMENU_WINDOWED:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 75, 143 + (OPTIONSMENU_WINDOWED * 40) );
		}
		break;

	case OPTIONSMENU_EXIT:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 75, 143 + (OPTIONSMENU_EXIT * 40) );
		}
		break;
	case OPTIONSMENU_GAME:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 75, 143 + (OPTIONSMENU_GAME * 40));
		}
		break;
	}
}

void COptionsState::Exit(void)
{
	//TODO:
	CSGD_FModManager::GetInstance()->StopSound(m_nSFX);
	CSGD_FModManager::GetInstance()->StopSound(m_nBGM);

	CSGD_FModManager::GetInstance()->UnloadSound(m_nSFX);
	CSGD_FModManager::GetInstance()->UnloadSound(m_nBGM);


	CGame::GetInstance()->SetMusicVolume(m_nMusicVolume);
	CGame::GetInstance()->SetPanVolume(m_nMusicPan);
	CGame::GetInstance()->SetSFXVolume(m_nFXVolume);


	//CGameProfiler::GetInstance()->SaveUserSettings("user settings.txt");
}