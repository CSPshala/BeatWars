///////////////////////////////////////////////////////
// File Name	:	"CLevelSelect_State.h"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////
#ifndef _CLevelSelect_State_H_
#define _CLevelSelect_State_H_

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
#include "../../source/StringHelper.h"


// Forward Declarations
class Bitmap_Font;
class CSong;
class CLevelSelect_State : public IGameState
{

private:
	// Proper singleton
	CLevelSelect_State(const CLevelSelect_State&);
	//		Assignment op
	CLevelSelect_State& operator=(const CLevelSelect_State&);
	//		Destructor
	~CLevelSelect_State();
	//		Constructor
	CLevelSelect_State();

	const void LoadLevels(void);

	struct LevelData {
		string szMenuName;
		string szFile;
		string szSongName;
		int szImage;
		int nSoundSample;
	};

	
	vector<int>& GetPlaylist() {return m_vPlaylist;}

	//*****MEMBERS*******//
	vector<LevelData*> m_vLevelData;
	vector<int> m_vPlaylist;
	int nBgID;
	char Selected;
	bool vsMode;
	FILE* LevelFile;

public:
	void Enter(void); // Enters the Game State
	bool Input(void); // Input
	void Update(void); // Update
	void Render(void); // Draw
	void Exit(void); // Leaves the Game State
	bool GetVsMode() {return vsMode;}
	void SetVsMode(bool vMode) {vsMode = vMode;}
	static CLevelSelect_State* GetInstance();	
	vector<LevelData*>& GetLevelData() {return m_vLevelData;}

};

#endif