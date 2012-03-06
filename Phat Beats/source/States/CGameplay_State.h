///////////////////////////////////////////////////////
// File Name	:	"CGameplay_State.h"
//
// Purpose		:	To Contain all game related code
//////////////////////////////////////////////////////
#ifndef _CGameplay_State_H_
#define _CGameplay_State_H_

// Managers / Wrappers
#include "IGameState.h"
#include "../CGame.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "..\SGD Wrappers\CSGD_DirectInput.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"

#include "../Managers/CBeatManager.h"
#include "../Managers/CAnimationManager.h"
#include "../Managers/IListener.h"
#include "../Managers/CEventSystem.h"
#include "../Managers/CMessageSystem.h"

// Classes
#include "../CPlayer.h"
#include "CBitmapFont.h"

// STL
#include <vector>
#include <string>
using std::vector;
using std::string;

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

	bool GetPreviouslyPlaying() {return m_bPreviouslyPlaying;}
	void SetPreviouslyPlaying(bool prevplay) {m_bPreviouslyPlaying = prevplay;}
	void DrawARGB(string filename, DWORD argbColor);

	// Public Accessors
	void		SetIsTutorial(bool bSet) {m_bTutorial = bSet;}

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
	bool m_bPreviouslyPlaying;
	float m_SongTransitionAlpha;
	bool m_bStartTransition;
	// Managers
	CBeatManager* BeatManager;
	CAnimationManager AnimationManager;

	// Tutorial specific bool (used for pausing and displaying text)
	bool m_bTutorial;
	// Tutorial string counter
	int m_nTutorialTextIndex;
	// Vector containing tutorial strings
	vector<string> m_vTutorialText;
	// ID for tutorial box background
	int m_nTutorialBoxID;
	

	//*************PRIVATE UTILITY FUNCTIONS****************//
	void DrawTutorialText();

	//*************PRIVATE ACCESSORS************************//
	bool		GetIsTutorial() {return m_bTutorial;}

	//*************PRIVATE MUTATORS*************************//
	


	
};

#endif