///////////////////////////////////////////////////////
// File Name	:	"CGameplay_State.h"
//
// Purpose		:	To Contain all game related code
//////////////////////////////////////////////////////
#ifndef _CGameplay_State_H_
#define _CGameplay_State_H_


#include "IGameState.h"
#include "../CGame.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "..\SGD Wrappers\CSGD_DirectInput.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"
#include "..\SGD Wrappers\CSGD_XAudio2.h"
#include "../Managers/CBeatManager.h"
#include "../Managers/CAnimationManager.h"
#include "../Managers/IListener.h"
#include "../Managers/CEventSystem.h"
#include "../Managers/CMessageSystem.h"

// Forward Declarations
class Bitmap_Font;

class CGameplay_State : public IGameState, IListener
{
public:
	void Enter(void); // Enters the Game State
	bool Input(void); // Input
	void Update(); // Update
	void Render(void); // Draw
	void Exit(void); // Leaves the Game State	
	void HandleEvent(CEvent* pEvent);
	static CGameplay_State* GetInstance();			
	static void MessageProc(CBaseMessage* pMsg);// access to messageproc
private:
	// Proper singleton
	CGameplay_State(const CGameplay_State&);
	//		Assignment op
	CGameplay_State& operator=(const CGameplay_State&);
	//		Destructor
	~CGameplay_State();
	//		Constructor
	CGameplay_State();

	//*******MEMBERS**********//
	Bitmap_Font* m_bMenu_Font;
	
		// Asset IDs
	int m_nBackgroundID;
	int m_nBackSoundID;
	int m_nFontID;
	int m_nTitleID;

	bool m_bPlayAnimation;

		// Managers
	CBeatManager BeatManager;
CAnimationManager AnimationManager;
		
static bool dickhead;
};

#endif