///////////////////////////////////////////////////////
// File Name	:	"CGame.cpp"
//
// Author Name	:	JC Ricks 
//
// Purpose		:	To Contain all game related code
//////////////////////////////////////////////////////

#include "CGame.h"
#include <ctime>

CGame::CGame()
{
	m_pD3D  = NULL;
	m_pDI	= NULL;
	m_pTM	= NULL;
	m_pXA	= NULL;	
}

CGame::~CGame()
{

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
	m_pXA	= CSGD_XAudio2::GetInstance();

	// Init singletons:
	m_pD3D->InitDirect3D(hWnd,nScreenWidth,nScreenHeight,bIsWindowed,false);
	m_pTM->InitTextureManager(m_pD3D->GetDirect3DDevice(),m_pD3D->GetSprite());
	m_pDI->InitDirectInput(hWnd,hInstance, DI_KEYBOARD| DI_MOUSE);
	m_pXA->InitXAudio2();

	
	ChangeState(CGameplay_State::GetInstance());

	// Window parameters
	m_bWindowed = bIsWindowed;
	m_nWindowWidth = nScreenWidth;
	m_nWindowHeight = nScreenHeight;



	srand((unsigned int)time(0));
	
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

	// Fullscreen / Window Shift  (KeyDown used on alt to make it
	// so you don't hafta hit both keys exactly at the same time
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
	m_pCurState->Update();	// must be called or you will mess stuff up
}

void CGame::Render()
{
	m_pCurState->Render();
}

void CGame::Shutdown()
{
	
	// Shutdown in the opposite order
	if(m_pXA)
	{
		m_pXA->ShutdownXAudio2();
		m_pXA = NULL;
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