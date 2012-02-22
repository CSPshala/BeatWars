///////////////////////////////////////////////////////
// File Name	:	"CPause_State.h"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////
#ifndef _CPause_State_H_
#define _CPause_State_H_

#include <vector>
#include <string>
using std::vector;
using std::string;

#include "IGameState.h"
#include "../CGame.h"
#include "CGameplay_State.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "..\SGD Wrappers\CSGD_DirectInput.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"


// Forward Declarations
class CBitmapFont;
enum {PAUSEMENU_RESET, PAUSEMENU_EXIT, PAUSEMENU_MAINMENU, PAUSEMENU_LOAD, PAUSEMENU_SAVE,
		 PAUSEMENU_OPTIONSTATE, NUM_PAUSEMENU_OPTIONS};
class CPause_State : public IGameState
{
public:
	void Enter(void); // Enters the Game State
	bool Input(void); // Input
	void Update(void); // Update
	void Render(void); // Draw
	void Exit(void); // Leaves the Game State

	static CPause_State* GetInstance();	

private:
	// Proper singleton
	CPause_State(const CPause_State&);
	//		Assignment op
	CPause_State& operator=(const CPause_State&);
	//		Destructor
	~CPause_State();
	//		Constructor
	CPause_State();


	//*****MEMBERS*******//
	vector<string> m_vMenu;
	CBitmapFont* m_bMenu_Font;
	int m_nMenuSelection;

		// Asset IDs
	int m_nBackgroundID;
	int m_nCursorImageID;
	int m_nFontID;
	int m_nTitleID;
	int m_nBackSoundID;
	int m_nCursorSoundID;
	
};

#endif