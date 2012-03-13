#pragma once

#include "IGameState.h"
#include "../CGame.h"

// Included for Difficulty ENUM
#include "../CBeat.h"
#include "../CPlayer.h"
#include "../Managers/CAiManager.h"

// JC - I cleaned up the enum a bit because stuff like lives isn't needed for now.
enum {OPTIONSMENU_SFXVOL, OPTIONSMENU_MUSICVOL, OPTIONSMENU_PAN, OPTIONSMENU_AILEVEL,
	OPTIONSMENU_DIFFICULTY, OPTIONSMENU_WINDOWED,OPTIONSMENU_GAME, NUM_OPTIONSMENU_OPTIONS}; //OPTIONSMENU_LIVES,};
class COptionsState : public IGameState
{
	private:
	int m_nMenuSelection;
	int m_nCursorID;

	int m_nSFX;
	int m_nBGM;

	int	  m_nAiLevel;

	int   m_nBackgroundID;
	int	  m_nGameImageID;
	int   m_nOptionsID;

	float m_nFXVolume;
	float m_nMusicVolume;
	float m_nMusicPan;
	BeatDifficulty m_eDifficulty;  // Sets difficulty
	

	COptionsState( void );
	COptionsState( const COptionsState& );
	COptionsState& operator=( const COptionsState& );
	~COptionsState(void);

public:
	static COptionsState* GetInstance( void );

	void Enter(void);
	bool Input(void);
	void Update(void);
	void Render(void);
	void Exit(void);

	// game difficulty setters and getter
	void SetDifficulty(BeatDifficulty eDiff) {m_eDifficulty = eDiff;}
	BeatDifficulty GetDifficulty() {return m_eDifficulty;}
	// AI level setters and getter
	int GetAILevel() {return m_nAiLevel;}
	void SetAILevel(int nAILevel) {m_nAiLevel = nAILevel;}

	// volume setter and getter for saving and loading
	float GetFXVol() {return m_nFXVolume;}
	void SetFXVol(float nFXVolume) {m_nFXVolume = nFXVolume;}
	float GetMusicVol() {return m_nMusicVolume;}
	float SetMusicVol(float nMusicVolume) {m_nMusicVolume = nMusicVolume;}
};