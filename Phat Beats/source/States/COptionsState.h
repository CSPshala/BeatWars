#pragma once

#include "IGameState.h"
#include "../CGame.h"

// Included for Difficulty ENUM
#include "../CBeat.h"

// JC - I cleaned up the enum a bit because stuff like lives isn't needed for now.
enum {OPTIONSMENU_SFXVOL, OPTIONSMENU_MUSICVOL, OPTIONSMENU_PAN, OPTIONSMENU_DIFFICULTY, OPTIONSMENU_WINDOWED, OPTIONSMENU_EXIT, OPTIONSMENU_GAME, NUM_OPTIONSMENU_OPTIONS}; //OPTIONSMENU_LIVES,};
class COptionsState : public IGameState
{
	private:
	int m_nMenuSelection;
	int m_nCursorID;

	int m_nSFX;
	int m_nBGM;
	
	
	float m_nFXVolume;
	float m_nMusicVolume;
	float m_nMusicPan;
	int	  m_nLives;
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

	void SetDifficulty(BeatDifficulty eDiff) {m_eDifficulty = eDiff;}
	BeatDifficulty GetDifficulty() {return m_eDifficulty;}
};