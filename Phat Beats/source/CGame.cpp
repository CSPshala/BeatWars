///////////////////////////////////////////////////////
// File Name	:	"CGame.cpp"
//
// Author Name	:	JC Ricks(@CSPshala)
//
// Purpose		:	To Contain all game related code
//////////////////////////////////////////////////////

#include "CGame.h"
#include <ctime>
#include "States/CBitmapFont.h"
#include "Managers/CFXManager.h"
#include <iostream>
using std::cout;

CGame::CGame()
{
	m_pD3D  = NULL;
	m_pDI	= NULL;
	m_pTM	= NULL;		
	m_pBF	= NULL;
	m_pES	= NULL;
	m_pMS	= NULL;
	m_pOM	= NULL;
	m_pFM	= NULL;
	PlayerControl = new CXBOXController(1);

	SetCharacterSelection(true);
	SetCharacterSelection2(false);
}

CGame::~CGame()
{
	delete PlayerControl;
}

CGame* CGame::GetInstance()
{
	// Lazy instantiation
	static CGame instance; // Static allows passing back of address
	return &instance;	
}

void CGame::Init(HWND hWnd, HINSTANCE hInstance, int nScreenWidth,
					int nScreenHeight,bool bIsWindowed)
{
	// Get pointers to singletons:
	m_pD3D  = CSGD_Direct3D::GetInstance();
	m_pDI	= CSGD_DirectInput::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();
	m_pBF	= CBitmapFont::GetInstance();
	m_pES	= CEventSystem::GetInstance();
	m_pMS	= CMessageSystem::GetInstance();
	m_pOM	= CObjectManager::GetInstance();
	m_pFM	= CSGD_FModManager::GetInstance();

	// Init singletons:
	m_pD3D->InitDirect3D(hWnd,nScreenWidth,nScreenHeight,bIsWindowed,false);
	m_pTM->InitTextureManager(m_pD3D->GetDirect3DDevice(),m_pD3D->GetSprite());
	m_pDI->InitDirectInput(hWnd,hInstance, DI_KEYBOARD| DI_MOUSE | DI_JOYSTICKS);
	m_pFM->InitFModManager(hWnd);

	
	ChangeState(CMenu_State::GetInstance());

	// Window parameters
	m_bWindowed = bIsWindowed;
	m_nWindowWidth = nScreenWidth;
	m_nWindowHeight = nScreenHeight;

	// Initializing timer
	cTimer.Reset();

	srand((unsigned int)time(0));

	if (false == m_pBF->LoadXMLFont("starwarfont.fnt"))
	{
		cout << "Failure to load the starwar font in CGame's Initialize() \n";
	}
	else
	{
		// set the font to star war
		m_pBF->SetFont("starwarfont.fnt");	
		m_pBF->SetScale(1.0f);
	}

	//setting volume val
	m_nFXVolume = 1.0f;
	m_nMusicVolume = 1.0f;
	m_nMusicPan = 0.5f;
	
	
}

bool CGame::Main()
{	
	// 3 Things the game does during execution
	// Input
	if (Input() == false)
		return false;
	// Update
	Update();
	// Render
	Render();
	return true;
}

bool CGame::Input()
{
	m_pDI->ReadDevices(); // called ONCE a frame

	// Full screen / Window Shift  (KeyDown used on alt to make it
	// so you don't haft a hit both keys exactly at the same time
	if(m_pDI->KeyPressed(DIK_RETURN) && (m_pDI->KeyDown(DIK_RALT) || m_pDI->KeyDown(DIK_LALT)))
	{
		m_pDI->ReadDevices();
		if(m_bWindowed)
		{
			m_bWindowed = false;
			m_pD3D->ChangeDisplayParam(m_nWindowWidth,m_nWindowHeight,m_bWindowed);			
		}
		else
		{
			m_bWindowed = true;
			m_pD3D->ChangeDisplayParam(m_nWindowWidth,m_nWindowHeight,m_bWindowed);
		}
	}		

	if(!m_pCurState->Input())
		return false;	

	return true;
}

void CGame::Update()
{
	// Updating timer
	cTimer.Update();

	m_pCurState->Update();	// must be called or you will mess stuff up
	m_pES->ProcessEvents();
	m_pMS->ProcessMessages();	

	// Updating FMOD
	m_pFM->Update();
}

void CGame::Render()
{
	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	m_pCurState->Render();
	//CObjectManager::GetInstance()->RenderObjects();
	
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();

}

void CGame::Shutdown()
{
	CFXManager::GetInstance()->UnloadAllFX();
	if (m_pOM)
	{
		m_pOM->RemoveAllObjects();
		m_pOM->DeleteInstance();
	}
	if(m_pMS)
	{
		m_pMS->ShutdownMessageSystem();
		m_pMS = NULL;
	}
	m_pES->ShutdownEventSystem();
	// Shutdown in the opposite order
	if(m_pFM)
	{
		m_pFM->ShutdownFModManager();
		m_pFM = NULL;
	}

	if(m_pDI)
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = NULL;
	}

	if(m_pTM)
	{
		m_pTM->ShutdownTextureManager();
		m_pTM = NULL;
	}

	if(m_pD3D)
	{
		m_pD3D->ShutdownDirect3D();
		m_pD3D = NULL;
	}
}

void CGame::ChangeState(IGameState* pNewState)
{
	if(m_pCurState)
		m_pCurState->Exit();

	m_pCurState = pNewState;

	if(m_pCurState)
		m_pCurState->Enter();
}