///////////////////////////////////////////////////////
// File Name	:	"CCharacterSelect_State.h"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////
#ifndef _CCharacterSelect_State_H_
#define _CCharacterSelect_State_H_

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
class Bitmap_Font;

class CCharacterSelect_State : public IGameState
{
public:
	void Enter(void); // Enters the Game State
	bool Input(void); // Input
	void Update(void); // Update
	void Render(void); // Draw
	void Exit(void); // Leaves the Game State

	static CCharacterSelect_State* GetInstance();	

private:
	// Proper singleton
	CCharacterSelect_State(const CCharacterSelect_State&);
	//		Assignment op
	CCharacterSelect_State& operator=(const CCharacterSelect_State&);
	//		Destructor
	~CCharacterSelect_State();
	//		Constructor
	CCharacterSelect_State();


	//*****MEMBERS*******//
	vector<string> m_vMenu;
	Bitmap_Font* m_bMenu_Font;
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