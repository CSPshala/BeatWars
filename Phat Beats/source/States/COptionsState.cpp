#include "COptionsState.h"
#include "CMenu_State.h"
#include "../SGD Wrappers\CSGD_TextureManager.h"
#include "../SGD Wrappers\CSGD_Direct3D.h"
#include "../SGD Wrappers\CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_FModManager.h"
#include "CBitmapFont.h"
#include "../Globals.h"
#include "CGameplay_State.h"
#include "CPause_State.h"
#include "CLoad_State.h"
#include "CSave_State.h"

COptionsState::COptionsState( void )
{	
	CLoad_State::GetInstance()->loadGameSetting();
	SetDifficulty(CLoad_State::GetInstance()->GetPlayerDiff());
	SetAILevel(CLoad_State::GetInstance()->GetAILevel());

	// assign values to the local variables
	m_nMenuSelection = 0;
	m_nSFX = -1;
	m_nBackgroundID = -1;
	m_nGameImageID = -1;
	m_nOptionsID = -1;
	//setting the game volume
	SetFXVol(CGame::GetInstance()->GetSFXVolume());
	SetMusicVol(CGame::GetInstance()->GetMusicVolume());
	m_nMusicPan = CGame::GetInstance()->GetPanVolume();

	m_nMusicVolume = CLoad_State::GetInstance()->GetMusicVolume();
	m_nFXVolume = CLoad_State::GetInstance()->GetFXVolume();
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
	//setting the images in the texture manager
	m_nCursorID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/lightsaberCursor2.png" );	
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MainMenuBG.jpg");
	m_nGameImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/logo_beatWars_1024.png");
	m_nOptionsID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/options.png");
	//TODO:
	m_nSFX = CSGD_FModManager::GetInstance()->LoadSound("resource/light_saber.wav");
	m_nBGM = CSGD_FModManager::GetInstance()->LoadSound("resource/sound/cantina.mp3", FMOD_LOOP_NORMAL);

	// setting the volume for background music
	CSGD_FModManager::GetInstance()->SetVolume(m_nBGM,CGame::GetInstance()->GetMusicVolume());
	CSGD_FModManager::GetInstance()->SetPan(m_nBGM,CGame::GetInstance()->GetPanVolume());

	// music sound
	CSGD_FModManager::GetInstance()->PlaySoundA(m_nBGM);

}
bool COptionsState::Input(void)
{

#pragma region KEYBOARD

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_BACKSPACE))
		CGame::GetInstance()->GoBack();

	if (!CGame::GetInstance()->GetPlayerControl()->IsConnected())
	{
		// If you're at the top and press up, cycle the cursor to the bottom selection
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 1))
		{
			m_nMenuSelection -= 1;

			if( m_nMenuSelection < OPTIONSMENU_SFXVOL )
			{
				m_nMenuSelection = NUM_OPTIONSMENU_OPTIONS - 1;
			}

			CGame::GetInstance()->PlayNavMenuSound();
		}

		// If you're at the bottom and press down, cycle the cursor back to the top selection
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 1) )
		{
			m_nMenuSelection += 1;

			if( m_nMenuSelection >= NUM_OPTIONSMENU_OPTIONS)
			{
				m_nMenuSelection = OPTIONSMENU_SFXVOL;
			}

			CGame::GetInstance()->PlayNavMenuSound();
		}
		// input for the left key to control volume
		if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_LEFT) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_LEFT, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_LEFT, 1) )
		{
			switch (m_nMenuSelection)
			{
			case OPTIONSMENU_SFXVOL:
				m_nFXVolume -= 0.001f;
				SetFXVol(m_nFXVolume);
				if( m_nFXVolume <= 0.0f )
				{
					m_nFXVolume = 0.0f;
				}

				CSGD_FModManager::GetInstance()->SetVolume(m_nSFX, m_nFXVolume);
				break;
			case OPTIONSMENU_MUSICVOL:			
				m_nMusicVolume -= 0.001f;
				SetMusicVol(m_nMusicVolume);
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


		// input for the ai level and difficulty level
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_LEFT) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_LEFT, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_LEFT, 1))
		{
			switch (m_nMenuSelection)
			{
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
			case OPTIONSMENU_AILEVEL:
				{
					SetAILevel(((int)GetAILevel() - 1));

					if(GetAILevel() < AI_EASY)
						SetAILevel(AI_INSANE);
				}
				break;
			}

		}


		// Play the a sample sound when the user releases Left while changing the volume of the sound effects
		if(CSGD_DirectInput::GetInstance()->KeyReleased(DIK_LEFT) || CSGD_DirectInput::GetInstance()->JoystickGetLStickXAmount(0) == 0  )
		{
			switch (m_nMenuSelection)
			{
			case OPTIONSMENU_SFXVOL:
				if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(m_nSFX))
				{
					CSGD_FModManager::GetInstance()->PlaySound(m_nSFX);
				}							
				break;	
			}
		}

		// right key input for volume
		if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_RIGHT, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_RIGHT, 1) )
		{
			switch (m_nMenuSelection)
			{
			case OPTIONSMENU_SFXVOL:
				{			
					m_nFXVolume += 0.001f;
					SetFXVol(m_nFXVolume);
					if( m_nFXVolume >= 1.0f )
					{
						m_nFXVolume = 1.0f;
					}
					CSGD_FModManager::GetInstance()->SetVolume(m_nSFX, m_nFXVolume);			
				}
				break;
			case OPTIONSMENU_MUSICVOL:

				m_nMusicVolume += 0.001f;
				SetMusicVol(m_nMusicVolume);
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

		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(DIR_RIGHT) )
		{
			switch (m_nMenuSelection)
			{

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
			case OPTIONSMENU_AILEVEL:
				{
					SetAILevel(((int)GetAILevel() + 1));

					if(GetAILevel() > AI_INSANE)
						SetAILevel(AI_EASY);
				}
				break;
			}
		}


		// Play the a sample sound when the user releases Right while changing the volume of the sound effects
		if(CSGD_DirectInput::GetInstance()->KeyReleased(DIK_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickGetLStickXAmount(0) == 0  )
		{
			switch (m_nMenuSelection)
			{
			case OPTIONSMENU_SFXVOL:
				if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(m_nSFX))
				{
					CSGD_FModManager::GetInstance()->PlaySound(m_nSFX);
				}				
				break;	
			}
		}

		if( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(0))
		{
			CGame::GetInstance()->PlayAccMenuSound();
			switch( m_nMenuSelection )
			{
			case OPTIONSMENU_GAME:
				{
					if (CPause_State::GetInstance()->GetPauseState() == true)
					{
						CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
					}
					else
						CGame::GetInstance()->ChangeState( CMenu_State::GetInstance() );	

				}
				break;	

			case OPTIONSMENU_WINDOWED:
				{
					if(CGame::GetInstance()->GetIsWindowed())
					{
						CGame::GetInstance()->SetIsWindowed(false);
						CSGD_Direct3D::GetInstance()->ChangeDisplayParam(CGame::GetInstance()->GetScreenWidth(),
															CGame::GetInstance()->GetScreenHeight(),CGame::GetInstance()->GetIsWindowed());	
					}
					else
					{
						CGame::GetInstance()->SetIsWindowed(true);
						CSGD_Direct3D::GetInstance()->ChangeDisplayParam(CGame::GetInstance()->GetScreenWidth(),
															CGame::GetInstance()->GetScreenHeight(),CGame::GetInstance()->GetIsWindowed());	
					}
				}
				break;
			}
		}
	}

#pragma endregion

#pragma region XBOX
	if (CGame::GetInstance()->GetPlayerControl()->IsConnected())
	{

		if(CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 1))
		{
			m_nMenuSelection -= 1;

			if( m_nMenuSelection < OPTIONSMENU_SFXVOL )
			{
				m_nMenuSelection = NUM_OPTIONSMENU_OPTIONS - 1;
			}

			CGame::GetInstance()->PlayNavMenuSound();
		}

		// If you're at the bottom and press down, cycle the cursor back to the top selection
		if(CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 1) )
		{
			m_nMenuSelection += 1;

			if( m_nMenuSelection >= NUM_OPTIONSMENU_OPTIONS)
			{
				m_nMenuSelection = OPTIONSMENU_SFXVOL;
			}

			CGame::GetInstance()->PlayNavMenuSound();
		}
		// input for the left key to control volume
		if(CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			switch (m_nMenuSelection)
			{
			case OPTIONSMENU_SFXVOL:
				m_nFXVolume -= 0.001f;
				SetFXVol(m_nFXVolume);
				if( m_nFXVolume <= 0.0f )
				{
					m_nFXVolume = 0.0f;
				}

				CSGD_FModManager::GetInstance()->SetVolume(m_nSFX, m_nFXVolume);
				break;
			case OPTIONSMENU_MUSICVOL:			
				m_nMusicVolume -= 0.001f;
				SetMusicVol(m_nMusicVolume);
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


		// input for the ai level and difficulty level
		if(CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			switch (m_nMenuSelection)
			{
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
			case OPTIONSMENU_AILEVEL:
				{
					SetAILevel(((int)GetAILevel() + 1));

					if(GetAILevel() > AI_INSANE)
						SetAILevel(AI_EASY);
				}
				break;
			}

		}


		// Play the a sample sound when the user releases Left while changing the volume of the sound effects
		if(CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.sThumbLX == XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			switch (m_nMenuSelection)
			{
			case OPTIONSMENU_SFXVOL:
				if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(m_nSFX))
				{
					CSGD_FModManager::GetInstance()->PlaySound(m_nSFX);
				}							
				break;	
			}
		}

		// right key input for volume
		if(CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			switch (m_nMenuSelection)
			{
			case OPTIONSMENU_SFXVOL:
				{			
					m_nFXVolume += 0.001f;
					SetFXVol(m_nFXVolume);
					if( m_nFXVolume >= 1.0f )
					{
						m_nFXVolume = 1.0f;
					}
					CSGD_FModManager::GetInstance()->SetVolume(m_nSFX, m_nFXVolume);			
				}
				break;
			case OPTIONSMENU_MUSICVOL:

				m_nMusicVolume += 0.001f;
				SetMusicVol(m_nMusicVolume);
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

		if(CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			switch (m_nMenuSelection)
			{

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
			case OPTIONSMENU_AILEVEL:
				{
					SetAILevel(((int)GetAILevel() + 1));

					if(GetAILevel() > AI_INSANE)
						SetAILevel(AI_EASY);
				}
				break;
			}
		}


		// Play the a sample sound when the user releases Right while changing the volume of the sound effects
		if(CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.sThumbLX == XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			switch (m_nMenuSelection)
			{
			case OPTIONSMENU_SFXVOL:
				if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(m_nSFX))
				{
					CSGD_FModManager::GetInstance()->PlaySound(m_nSFX);
				}				
				break;	
			}
		}

		if(CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A && CGame::GetInstance()->GetButtonPressed() == false)
		{
			CGame::GetInstance()->SetButtonPressed(true);

			switch( m_nMenuSelection )
			{
			case OPTIONSMENU_GAME:
				{
					if (CPause_State::GetInstance()->GetPauseState() == true)
					{
						CGame::GetInstance()->ChangeState(CPause_State::GetInstance());
					}
					else
						CGame::GetInstance()->ChangeState( CMenu_State::GetInstance() );

				}
				break;	

			case OPTIONSMENU_WINDOWED:
			{
				if(CGame::GetInstance()->GetIsWindowed())
				{
					CGame::GetInstance()->SetIsWindowed(false);
					CSGD_Direct3D::GetInstance()->ChangeDisplayParam(CGame::GetInstance()->GetScreenWidth(),
														CGame::GetInstance()->GetScreenHeight(),CGame::GetInstance()->GetIsWindowed());	
				}
				else
				{
					CGame::GetInstance()->SetIsWindowed(true);
					CSGD_Direct3D::GetInstance()->ChangeDisplayParam(CGame::GetInstance()->GetScreenWidth(),
														CGame::GetInstance()->GetScreenHeight(),CGame::GetInstance()->GetIsWindowed());	
				}
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
void COptionsState::Update(void)
{

}
void COptionsState::Render(void)
{
	// setting the  buffer for the print statements
	char buffer[255];
	//rendering the background image
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID,0,0, 0.78125f, 0.5859375f);

	// rect for options title
	RECT gOptions = {0,0,1000,120};
	CSGD_TextureManager::GetInstance()->Draw(m_nOptionsID,15,40,0.4f,0.9f,&gOptions);
	// rect for game image 
	RECT gImage = {0,350,290,550};
	CSGD_TextureManager::GetInstance()->Draw(m_nGameImageID,450,15,1.0f,1.0f,&gImage);

	// setting the scale for the bitmap font
	CBitmapFont::GetInstance()->SetScale(1.0f);
	// the rect that will hold the title
	RECT rMenuOptions = { 260, 310, CGame::GetInstance()->GetScreenWidth(), 485};
	// printing the menu

	// Dynamically making the menu text
	string szMenuText;
	szMenuText = "SFx volume\n\nMusic volume\n\nMusic Pan\n\nai level\n\nDifficulty\n\n";
	
	if(!CGame::GetInstance()->GetIsWindowed())
		szMenuText += "Windowed Mode\n";
	else
		szMenuText += "Fullscreen Mode\n";

	if (CPause_State::GetInstance()->GetPauseState() == true)
		szMenuText += "\ngo back to pause";
	else
		szMenuText += "\nBack";
	
	// Printing menu text
	CBitmapFont::GetInstance()->PrintStrokedTextInRect(szMenuText,&rMenuOptions, ALIGN_LEFT, D3DCOLOR_XRGB(0, 0, 0),  D3DCOLOR_XRGB(225, 225, 225));
	

	// Printing the FX volume of the game
	sprintf_s( buffer, "%d", int( m_nFXVolume * 100));	
	CBitmapFont::GetInstance()->PrintText(buffer, 475, 265, D3DCOLOR_XRGB(225, 225, 225));
	// printing the music volume
	sprintf_s( buffer, "%d", int( m_nMusicVolume * 100));	
	CBitmapFont::GetInstance()->PrintText(buffer, 475, 305, D3DCOLOR_XRGB(225, 225, 225));	
	// printing the music panning for the left and right speaker
	sprintf_s( buffer, "L%d - R%d", int( 50 + (m_nMusicPan * -50)), int( 50 + (m_nMusicPan * 50)));	
	CBitmapFont::GetInstance()->PrintText(buffer, 475, 345, D3DCOLOR_XRGB(225, 225, 225));	

	// this is setting the game Ai level
	switch(GetAILevel())
	{
	case AI_EASY:
		sprintf_s( buffer,"easy");
		break;

	case AI_NORMAL:
		sprintf_s( buffer,"normal");
		break;

	case AI_HARD:
		sprintf_s( buffer,"hard");
		break;

	case AI_INSANE:
		sprintf_s( buffer,"insane");
		break;
	}
	// print out the level for the Ai	
	CBitmapFont::GetInstance()->PrintText(buffer, 475, 385, D3DCOLOR_XRGB(225, 225, 225));

	// setting the song level for the game
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
	// printing the song difficulty level
	CBitmapFont::GetInstance()->PrintText(buffer, 475, 425, D3DCOLOR_XRGB(225, 225, 225));

#pragma region cursorcontrol
	//draw the cursor at the current selection
	switch(m_nMenuSelection)
	{
	case OPTIONSMENU_SFXVOL:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 15, 265 + (OPTIONSMENU_SFXVOL * 40) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 768, 265  + (40* OPTIONSMENU_SFXVOL), -1.0f);
		}
		break;

	case OPTIONSMENU_MUSICVOL:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 15, 265 + (OPTIONSMENU_MUSICVOL * 40) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 768, 265  + (40* OPTIONSMENU_MUSICVOL), -1.0f);
		}
		break;

	case OPTIONSMENU_PAN:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 15, 265 + (OPTIONSMENU_PAN * 40) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 825, 265 + (40 *OPTIONSMENU_PAN), -1.0f);
		}
		break;
	case OPTIONSMENU_AILEVEL:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 15, 265 + (OPTIONSMENU_AILEVEL * 40) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 775, 265  + (40* OPTIONSMENU_AILEVEL), -1.0f);
		}
		break;

	case OPTIONSMENU_DIFFICULTY:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 15, 265 + (OPTIONSMENU_DIFFICULTY * 40) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 775, 265  + (40* OPTIONSMENU_DIFFICULTY), -1.0f);
		}
		break;

	case OPTIONSMENU_WINDOWED:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 15, 265 + (OPTIONSMENU_WINDOWED * 40) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 768, 265  + (40* OPTIONSMENU_WINDOWED), -1.0f);
		}
		break;

	case OPTIONSMENU_GAME:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 15, 265 + (OPTIONSMENU_GAME * 40) );
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorID, 768, 265  + (40* OPTIONSMENU_GAME), -1.0f);
		}
		break;
	}

#pragma endregion 

}
void COptionsState::Exit(void)
{
	CSave_State::GetInstance()->saveConfig();
	//TODO:
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nGameImageID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nCursorID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nBackgroundID);

	CSGD_FModManager::GetInstance()->StopSound(m_nSFX);
	CSGD_FModManager::GetInstance()->StopSound(m_nBGM);

	CSGD_FModManager::GetInstance()->UnloadSound(m_nSFX);
	CSGD_FModManager::GetInstance()->UnloadSound(m_nBGM);


	CGame::GetInstance()->SetMusicVolume(m_nMusicVolume);
	CGame::GetInstance()->SetPanVolume(m_nMusicPan);
	CGame::GetInstance()->SetSFXVolume(m_nFXVolume);	
}