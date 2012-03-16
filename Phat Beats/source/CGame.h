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
#include "Managers/CXBOXManager.h"
#include <string>
#include <queue>
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
	CXBOXController*		PlayerControl;

	// Game state Pointer
	IGameState* m_pCurState;

	// Asset IDs:
	//int						m_nImageID;
	//int						m_nSoundID;
	std::queue<IGameState*> m_qStateHistory;


	// Volume 
	float m_nFXVolume;
	float m_nMusicVolume;
	float m_nMusicPan;
	bool Player1selection;
	bool Player2selection;

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

	// Accessors
	Timer& GetTimer() {return cTimer;}
	float GetMusicVolume() const		{ return m_nMusicVolume; }
	float GetSFXVolume() const			{ return m_nFXVolume; }
	float GetPanVolume() const			{ return m_nMusicPan; }
	bool GetCharacterSelection() {return Player1selection;}
	bool GetCharacterSelection2() {return Player2selection;}
	CXBOXController*	GetPlayerControl()	{return PlayerControl;}
	
	// Mutators
	void SetMusicVolume(float val)		{ m_nMusicVolume = val; }
	void SetSFXVolume(float val)		{ m_nFXVolume = val; }
	void SetPanVolume(float val)		{ m_nMusicPan = val; }
	void SetCharacterSelection(bool selection) {Player1selection = selection;} 
	void SetCharacterSelection2(bool selection) {Player2selection = selection;}
	void GoBack(void);
};


#endif