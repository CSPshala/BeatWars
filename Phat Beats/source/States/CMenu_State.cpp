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
#include "../Managers/CBeatManager.h"
#include "CLevelSelect_State.h"
#include "CArcadeMode_State.h"
#include "CLU_State.h"
#include "CLoad_State.h"
#include "../../CLevelManager.h"
#include "../JCMacros.h"
#include "CCredit_State.h"
#include "CHighScoreState.h"
#include "../SGD Wrappers/CSGD_FModManager.h"
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
	// KEEPS LOADING, NEVER RELEASING
	m_nMenuSelection = 0;
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MainMenuBG.png");
	m_nCursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/lightsaberCursor2.png");
	m_nTile = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/logo_beatWars_1024.png");
	m_nTile = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/logo_beatWars_1024.png");
	m_nBackSoundID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sound/Star_Wars_-_Main_Title_Theme.mp3",FMOD_LOOP_NORMAL);
}

bool CMenu_State::Input(void)
{
#pragma region KEYBOARD
	if (!CGame::GetInstance()->GetPlayerControl()->IsConnected())
	{
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 1) )
		{
			m_nMenuSelection -= 1;
			if( m_nMenuSelection == -1 )
			{
				m_nMenuSelection = MAINMENU_EXIT;
			}
			CGame::GetInstance()->PlayNavMenuSound();
		}

		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 1) )
		{
			m_nMenuSelection += 1;

			if( m_nMenuSelection == NUM_MAINMENU_OPTIONS )
			{
				m_nMenuSelection = 0;
			}
			CGame::GetInstance()->PlayNavMenuSound();
		}

		if( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(0, 0) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(0, 1) )
		{
			CGame::GetInstance()->PlayAccMenuSound();
			switch( m_nMenuSelection )
			{
			case MAINMENU_NEWGAME:
				{
					// Private function that wraps up CLU load calls to 
					// keep this area clean
					LoadGameplayStateAssets();
				}
				break;

			case MAINMENU_HIGHSCORE:
				{
					CHighScoreState::GetInstance()->SetChange(false);
					CGame::GetInstance()->ChangeState(CHighScoreState::GetInstance());
				}
				break;

			case MAINMENU_LOAD:	// GOES TO THE SKILLS TEST FOR NOW
				{
					CGame::GetInstance()->ChangeState(CLoad_State::GetInstance());
				}
				break;

			case MAINMENU_OPTIONS:
				{
					CGame::GetInstance()->ChangeState(COptionsState::GetInstance());
				}
				break;		
			case MAINMENU_CREDITS:
				{
					CHighScoreState::GetInstance()->SetChange(false);
					CGame::GetInstance()->ChangeState(CCredit_State::GetInstance());
				}
				break;
			case MAINMENU_ARCADE:
				{
					CGame::GetInstance()->ChangeState(CArcadeMode_State::GetInstance());
				}
				break;
			case MAINMENU_TUTORIAL:
				{
					loadTutorial();
				}
				break;


			case MAINMENU_EXIT:
				{
					return false;
				}
				break;
			}
		}
	}
#pragma endregion


#pragma region XBOX
	if (CGame::GetInstance()->GetPlayerControl()->IsConnected())
	{
		if(CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 1) )
		{
			m_nMenuSelection -= 1;
			if( m_nMenuSelection == -1 )
			{
				m_nMenuSelection = MAINMENU_EXIT;
			}

		}

		if(CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 0) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 1) )
		{
			m_nMenuSelection += 1;

			if( m_nMenuSelection == NUM_MAINMENU_OPTIONS )
			{
				m_nMenuSelection = 0;
			}
		}

		if(CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A && CGame::GetInstance()->GetButtonPressed() == false)
		{
			CGame::GetInstance()->SetButtonPressed(true);

			switch( m_nMenuSelection )
			{
				case MAINMENU_NEWGAME:
					{
						// Private function that wraps up CLU load calls to 
						// keep this area clean
						LoadGameplayStateAssets();
					}
					break;

				case MAINMENU_HIGHSCORE:
					{
						CGame::GetInstance()->ChangeState(CHighScoreState::GetInstance());
					}
					break;

				case MAINMENU_LOAD:	// GOES TO THE SKILLS TEST FOR NOW
					{
						CGame::GetInstance()->ChangeState(CLoad_State::GetInstance());
					}
					break;
				case MAINMENU_OPTIONS:
					{
						CGame::GetInstance()->ChangeState(COptionsState::GetInstance());
					}
					break;		
				case MAINMENU_CREDITS:
					{
						CGame::GetInstance()->ChangeState(CCredit_State::GetInstance());
					}
					break;

				case MAINMENU_ARCADE:
					{
						CGame::GetInstance()->ChangeState(CArcadeMode_State::GetInstance());
					}
					break;
			case MAINMENU_TUTORIAL:
				{
					loadTutorial();
				}
				break;


				case MAINMENU_EXIT:
					{
						return false;
					}
					break;
			}
		}
		else if (CGame::GetInstance()->GetPlayerControl()->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A && CGame::GetInstance()->GetButtonPressed() == true)
		{
			CGame::GetInstance()->SetButtonPressed(false);
		}
	}
	
#pragma endregion
	return true;
}

void CMenu_State::Update(void)
{
	if( !CSGD_FModManager::GetInstance()->IsSoundPlaying(m_nBackSoundID))
		CSGD_FModManager::GetInstance()->PlaySoundA(m_nBackSoundID);
}

void CMenu_State::Render(void)
{
	RECT rBody = {0, 289, CGame::GetInstance()->GetScreenWidth(), 635};
	RECT rTitle = {0,0,540,349};
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID,0,0, 0.78125f, 0.5859375f);
	CSGD_TextureManager::GetInstance()->Draw(m_nTile,128,1,1.0f,1.0f,&rTitle);

	int topSelection = 370;
	int spacing = 23;
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

	case MAINMENU_ARCADE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 32, topSelection  + (spacing * MAINMENU_ARCADE));
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 768, topSelection  + (spacing * MAINMENU_ARCADE), -1.0f);
		}
		break;

	case MAINMENU_TUTORIAL:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 32, topSelection  + (spacing * MAINMENU_TUTORIAL));
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 768, topSelection  + (spacing * MAINMENU_TUTORIAL), -1.0f);
		}
		break;

	case MAINMENU_HIGHSCORE:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 32, topSelection  + (spacing * MAINMENU_HIGHSCORE));
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 768, topSelection  + (spacing * MAINMENU_HIGHSCORE), -1.0f);
		}
		break;

	case MAINMENU_EXIT:
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 32, topSelection  + (spacing * MAINMENU_EXIT));
			CSGD_TextureManager::GetInstance()->Draw(m_nCursorImageID, 768, topSelection  + (spacing * MAINMENU_EXIT), -1.0f);
		}
		break;
	}
	
	CBitmapFont::GetInstance()->SetScale(1.2f);

	CBitmapFont::GetInstance()->PrintStrokedTextInRect("new game\nload\noptions\ncredits\narcade mode\nhow to play\nhigh scores\nexit", &rBody, ALIGN_CENTER, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
}

void CMenu_State::Exit(void)
{
	CSGD_FModManager::GetInstance()->UnloadSound(m_nBackSoundID);
}

CMenu_State* CMenu_State::GetInstance()
{
	// Lazy instantiation
	static CMenu_State instance; // Static allows passing back of address
	return &instance;	
}

void CMenu_State::LoadGameplayStateAssets()
{
	// Cleaning crap out (if there's anything)
	CFXManager::GetInstance()->UnloadAllFX();
	CLevelManager::GetInstance()->EmptySongQueue();
	BEATMAN->UnloadSongs();	

	// Using defines from JCMacros.h
	CLU->SetNewState(CGameplay_State::GetInstance());

	// Setting GameplayState to tutorial mode
	CGameplay_State::GetInstance()->SetIsTutorial(true);

	// Loading Effects
	CLU->QueueLoadCommand("resource/Hit.xml","P1_HIT",Effect);
	CLU->QueueLoadCommand("resource/Hit.xml","P2_HIT",Effect);
	CLU->QueueLoadCommand("resource/P1PBAR.xml", "P1_PBAR", Effect);
	CLU->QueueLoadCommand("resource/P2PBAR.xml", "P2_PBAR", Effect);

	// Loading up BeatManager specific stuff

	CLU->QueueLoadCommand("tutorial.beat","",Song);
	CLU->QueueLoadCommand("cantina.beat","",Song);
	CLU->QueueLoadCommand("darksidedub.beat","",Song);
	CLU->QueueLoadCommand("DueloftheFates.beat","",Song);
	CLU->QueueLoadCommand("ImperialMarch.beat","",Song);
	CLU->QueueLoadCommand("oldrepublic.beat", "", Song);	
	CLU->QueueLoadCommand("noteeventtest.beat", "", Song);	

	CLevelManager::GetInstance()->QueueSong("jeditheme");
	CLevelManager::GetInstance()->QueueSong("cantina");
	CLevelManager::GetInstance()->QueueSong("DarkSideDub");
	CLevelManager::GetInstance()->QueueSong("dualofthefates");
	CLevelManager::GetInstance()->QueueSong("ImperialMarch");
	CLevelManager::GetInstance()->QueueSong("oldrepublic");
	CLevelManager::GetInstance()->QueueSong("Avicii");


	GAME->ChangeState(CLU_State::GetInstance());
}

void CMenu_State::loadTutorial()
{
	// Cleaning crap out (if there's anything)
	CFXManager::GetInstance()->UnloadAllFX();
	CLevelManager::GetInstance()->EmptySongQueue();
	BEATMAN->UnloadSongs();	

	// Using defines from JCMacros.h
	CLU->SetNewState(CGameplay_State::GetInstance());

	// Setting GameplayState to tutorial mode
	CGameplay_State::GetInstance()->SetIsTutorial(true);

	// Loading Effects	
	CLU->QueueLoadCommand("resource/Hit.xml","P1_HIT",Effect);
	CLU->QueueLoadCommand("resource/Hit.xml","P2_HIT",Effect);
	CLU->QueueLoadCommand("resource/P1PBAR.xml", "P1_PBAR", Effect);
	CLU->QueueLoadCommand("resource/P2PBAR.xml", "P2_PBAR", Effect);

	// Loading up BeatManager specific stuff

	CLU->QueueLoadCommand("tutorial.beat","",Song);

	CLevelManager::GetInstance()->QueueSong("jeditheme");

	GAME->ChangeState(CLU_State::GetInstance());
}
