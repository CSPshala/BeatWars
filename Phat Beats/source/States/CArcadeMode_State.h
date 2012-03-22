///////////////////////////////////////////////////////
// File Name	:	"CArcadeMode_State.h"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////
#ifndef _CArcadeMode_State_H_
#define _CArcadeMode_State_H_

#include <vector>
#include <string>
using std::vector;
using std::string;

#include "IGameState.h"
#include "../CGame.h"
#include "CGameplay_State.h"
#include "../Globals.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "..\SGD Wrappers\CSGD_DirectInput.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"
#include "../../CLevelManager.h"


// Forward Declarations
class Bitmap_Font;

enum echaracters{LUKE_SKYWALKER, DARTH_VADER, NUM_CHARACTERS};
enum eplayer{PLAYER_ONE, PLAYER_TWO, AI, NUM_PLAYERS};


class CArcadeMode_State : public IGameState
{
public:
	void Enter(void); // Enters the Game State
	bool Input(void); // Input
	void Update(void); // Update
	void Render(void); // Draw
	void Exit(void); // Leaves the Game State

	static CArcadeMode_State* GetInstance();	

	bool GetMode() {return bIsArcadeMode;}
	void SetMode(bool ArcadeMode) {bIsArcadeMode = ArcadeMode;}

private:
	// Proper singleton
	CArcadeMode_State(const CArcadeMode_State&);
	//		Assignment op
	CArcadeMode_State& operator=(const CArcadeMode_State&);
	//		Destructor
	~CArcadeMode_State();
	//		Constructor
	CArcadeMode_State();


	//*****MEMBERS*******//
	vector<string> m_vMenu;
	Bitmap_Font* m_bMenu_Font;
	int m_nMenuSelection;
	int m_nCharacterSelection;
	CAnimationManager		AnMan;

		// Asset IDs
	int m_nBackgroundID;
	int m_nCursorImageID;
	int m_nFontID;
	int m_nTitleID;
	int m_nBackSoundID;
	int m_nCursorSoundID;
	
	bool bIsArcadeMode;
};

#endif