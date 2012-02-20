///////////////////////////////////////////////////////
// File Name	:	"CGame.h"
//
// Author Name	:	JC Ricks(@CSPshala)
//
// Purpose		:	To Contain all game related code
//////////////////////////////////////////////////////
#ifndef _CGAME_H_
#define _CGAME_H_

// SGD Singletons:
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_FModManager.h"
// Gamestates
#include "States\IGameState.h"
#include "States\CMenu_State.h"
#include "CBase.h"
#include "Timer.h"
// Messages and Event system by Dave Brown
#include "Managers/CEventSystem.h"
#include "Managers/CMessageSystem.h"
#include "Managers/CObjectManager.h"
#include <string>
using std::string;
class CBitmapFont;

class CGame
{
private:
	// Singleton pointers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;	
	CBitmapFont*			m_pBF;
	CEventSystem*			m_pES;
	CMessageSystem*			m_pMS;
	CObjectManager*			m_pOM;
	CSGD_FModManager*		m_pFM;

	// Game state Pointer
	IGameState* m_pCurState;

	// Asset IDs:
	//int						m_nImageID;
	//int						m_nSoundID;

	// Proper Singleton
	// Trilogy of Evil:
	//		Copy Constructor
	CGame(const CGame&);
	//		Assignment op
	CGame& operator=(const CGame&);
	//		Destructor
	~CGame();
	//		Constructor
	CGame();

	// Private functions:
	bool Input();	// Returns false if game should quit
	void Update();
	void Render();

	// Windowed bool
	bool m_bWindowed;
	// Window height and width
	int m_nWindowHeight;
	int m_nWindowWidth;

	// For Game timing:
	Timer cTimer;


public:
	// Singleton accessor
	static CGame* GetInstance();
	// 3 things a game does during LIFETIME:
	// Init
	void Init(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight,bool bIsWindowed);
	// Execute
	// NOTE: Returns false when game should quit
	bool Main();
	// Clean Up
	void Shutdown();
	// Game State
	void ChangeState(IGameState* pNewState);
	int GetScreenHeight(void) {return m_nWindowHeight;}
	int GetScreenWidth(void) {return m_nWindowWidth;}

	// Acessors
	Timer& GetTimer() {return cTimer;}

	// Mutators
	

};


#endif