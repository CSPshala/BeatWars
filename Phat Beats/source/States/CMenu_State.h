///////////////////////////////////////////////////////
// File Name	:	"CMenu_State.h"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////
#ifndef _CMENU_STATE_H_
#define _CMENU_STATE_H_

#include <vector>
#include <string>
using std::vector;
using std::string;

#include "IGameState.h"
#include "../CGame.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "..\SGD Wrappers\CSGD_DirectInput.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"
#include "../Timer.h"

// Forward Declarations

enum {MAINMENU_NEWGAME, MAINMENU_LOAD, MAINMENU_OPTIONS,  MAINMENU_CREDITS, MAINMENU_LEVEL, MAINMENU_ARCADE, MAINMENU_TUTORIAL, MAINMENU_EXIT, NUM_MAINMENU_OPTIONS};

class CMenu_State : public IGameState
{
public:
	void Enter(void); // Enters the Game State
	bool Input(void); // Input
	void Update(void); // Update
	void Render(void); // Draw
	void Exit(void); // Leaves the Game State

	static CMenu_State* GetInstance();	

	void LoadGameplayStateAssets();


private:
	// Proper singleton
	CMenu_State(const CMenu_State&);
	//		Assignment op
	CMenu_State& operator=(const CMenu_State&);
	//		Destructor
	~CMenu_State();
	//		Constructor
	CMenu_State();


	//*****MEMBERS*******//
	vector<string> m_vMenu;
	
	int m_nMenuSelection;

		// Asset IDs
	int m_nBackgroundID;
	int m_nCursorImageID;
	int m_nTitleID;
	int m_nBackSoundID;
	int m_nCursorSoundID;
	
	Timer GameTimer;
	int m_nTile;

	//*********PRIVATE UTILITY FUNCTIONS*************//

};

#endif