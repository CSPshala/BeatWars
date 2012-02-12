///////////////////////////////////////////////////////
// File Name	:	"CGame.h"
//
// Author Name	:	JC Ricks
//
// Purpose		:	To Contain all game related code
//////////////////////////////////////////////////////
#ifndef _CGAME_H_
#define _CGAME_H_

// SGD Singletons:
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
// Gamestates
#include "States\IGameState.h"
#include "States\CMenu_State.h"
#include "CBase.h"


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
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pBF;

	// Gamestate Pointer
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
	DWORD					m_dwTimeStamp;
	float					m_fElapsedTime;		// delta time. Time (in seconds) between frames
	float					m_fGameTime;		// how long the game has been playing
	DWORD					m_dwPreviousTimeStamp;	// Time samp from the previous frame


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
	DWORD GetTimeStamp() {return m_dwTimeStamp;}
	float GetElapsedTime() {return m_fElapsedTime;}
	float GetGameTime() {return m_fGameTime;}
	DWORD GetPreviousTime() {return m_dwPreviousTimeStamp;}

	// Mutators
	void SetTimeStamp(DWORD dwTimeStamp) {m_dwTimeStamp = dwTimeStamp;}
	void SetElapsedTime(float fElapsedTime) {m_fElapsedTime = fElapsedTime;}
	void SetGameTime(float fGameTime) {m_fGameTime = fGameTime;}
	void SetPreviousTimeStamp(DWORD dwPreviousTimeStamp) {m_dwPreviousTimeStamp = dwPreviousTimeStamp;}

};


#endif