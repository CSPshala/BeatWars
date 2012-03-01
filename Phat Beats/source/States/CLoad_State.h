///////////////////////////////////////////////////////
// File Name	:	"CLoad_State.h"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////
#ifndef _CLoad_State_H_
#define _CLoad_State_H_

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
#include <fstream>
#include <iostream>
using namespace std;

// Forward Declarations
class CBitmapFont;
enum {LOADMENU_SLOTONE, LOADMENU_SLOTTWO, LOADMENU_SLOTTHREE, NUM_LOADMENU_OPTIONS};
class CLoad_State : public IGameState
{
public:
	void Enter(void); // Enters the Game State
	bool Input(void); // Input
	void Update(void); // Update
	void Render(void); // Draw
	void Exit(void); // Leaves the Game State
	string loadGame();
	static CLoad_State* GetInstance();	

	// Accessors
	int GetSlotNumber() {return m_nSlotNumber;}
	string GetFileName() {return m_szSongFileName;}
	bool GetLoadFlag() {return m_nLoadFlag;}
	string GetSongName() {return m_szSongName;}
	// Mutators
	void SetFileName(string szFileName) {m_szSongFileName = szFileName;}
	void SetSlotNumber(int nSlotNumber) {m_nSlotNumber = nSlotNumber;}
	void SetSongName(string szSongName) {m_szSongName = szSongName;}
private:
	// Proper singleton
	CLoad_State(const CLoad_State&);
	//		Assignment op
	CLoad_State& operator=(const CLoad_State&);
	//		Destructor
	~CLoad_State();
	//		Constructor
	CLoad_State();

	
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
	int m_nSlotNumber;
	string m_szSongFileName;
	bool m_nLoadFlag;
	string m_szSongName;

	
};

#endif