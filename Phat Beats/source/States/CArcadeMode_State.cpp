///////////////////////////////////////////////////////
// File Name	:	"CArcadeMode_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CArcadeMode_State.h"

CArcadeMode_State::CArcadeMode_State()
{
	Bitmap_Font* m_bMenu_Font = NULL;
	m_nMenuSelection = 0;

	// Asset IDs
	m_nBackgroundID = -1;
	m_nCursorImageID = -1;
	m_nFontID = -1;
	m_nTitleID = -1;
	m_nBackSoundID = -1;
	m_nCursorSoundID = -1;
}

CArcadeMode_State::~CArcadeMode_State()
{

}

void CArcadeMode_State::Enter(void)
{	
	m_nMenuSelection = 0;
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/sprites_luke_001.png");
	m_nTitleID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/sprites_vader_001.png");
}

bool CArcadeMode_State::Input(void)
{

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(6) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(6, 1))
		return false;

	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP) )
	{
		m_nMenuSelection -= 1;
		if (m_nMenuSelection == -1)
		{
			m_nMenuSelection = DARTH_VADER;
		}
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_W) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_UP, 1))
	{
		m_nCharacterSelection -=1;
		if (m_nCharacterSelection== -1)
		{
			m_nCharacterSelection = DARTH_VADER;
		}
	}

	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN) )
	{
		m_nMenuSelection += 1;
		if (m_nMenuSelection == NUM_CHARACTERS)
		{
			m_nMenuSelection = 0;
		}
	}

	
	if (  CSGD_DirectInput::GetInstance()->KeyPressed(DIK_S) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_DOWN, 1))
	{
		m_nCharacterSelection +=1;
		if (m_nCharacterSelection == NUM_CHARACTERS)
		{
			m_nCharacterSelection = 0;
		}
	}

	if (m_nMenuSelection == 0 )
	{
		CGame::GetInstance()->SetCharacterSelection(true);
	}
	if (m_nMenuSelection == 1 )
	{
		CGame::GetInstance()->SetCharacterSelection(false);
	}

	if (m_nCharacterSelection == 0 )
	{
		CGame::GetInstance()->SetCharacterSelection2(true);
	}
	if (m_nCharacterSelection == 1 )
	{
		CGame::GetInstance()->SetCharacterSelection2(false);
	}

	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(0))
	{
		CMenu_State::GetInstance()->LoadGameplayStateAssets();
	}

	return true;
}

void CArcadeMode_State::Update(void)
{

}

void CArcadeMode_State::Render(void)
{
	CSGD_Direct3D::GetInstance()->Clear(0,0,0);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();

	RECT rSkywalker = {6, 8, 195, 312};
	RECT rVader = {3, 5, 294, 270};
	RECT rBody = {0, 0, CGame::GetInstance()->GetScreenWidth(), 100};
	RECT rBody2 = {0, 0, CGame::GetInstance()->GetScreenWidth(), 300};


	CBitmapFont::GetInstance()->SetScale(2.0f);
	CBitmapFont::GetInstance()->PrintStrokedTextInRect("Arcade Mode", &rBody, ALIGN_CENTER, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 0));
	if (m_nMenuSelection == 0)
	{
		CBitmapFont::GetInstance()->SetScale(1.0f);
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("Luke SkyWalker", &rBody2, ALIGN_LEFT, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));		
		CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID, 10, 200, 1.0f, 1.0f, &rSkywalker);

	}
	if (m_nMenuSelection == 1)
	{
		CBitmapFont::GetInstance()->SetScale(1.0f);
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("Darth vader", &rBody2, ALIGN_LEFT, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));		

		CSGD_TextureManager::GetInstance()->Draw(m_nTitleID, 10, 200, 1.0f, 1.0f, &rVader);
	}

	if (m_nCharacterSelection == 0)
	{
		CBitmapFont::GetInstance()->SetScale(1.0f);
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("Luke SkyWalker", &rBody2, ALIGN_RIGHT, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));		

		CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID, 510, 200, 1.0f, 1.0f, &rSkywalker);
	}
	if (m_nCharacterSelection == 1)
	{
		CBitmapFont::GetInstance()->SetScale(1.0f);
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("Darth vader", &rBody2, ALIGN_RIGHT, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));		

		CSGD_TextureManager::GetInstance()->Draw(m_nTitleID, 510, 200, 1.0f, 1.0f, &rVader);
	}

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();	// Draw everything now that is queued up
	
	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->DeviceEnd();
	CSGD_Direct3D::GetInstance()->Present();
}

void CArcadeMode_State::Exit(void)
{
	
}

CArcadeMode_State* CArcadeMode_State::GetInstance()
{
	// Lazy instantiation
	static CArcadeMode_State instance; // Static allows passing back of address
	return &instance;	
}