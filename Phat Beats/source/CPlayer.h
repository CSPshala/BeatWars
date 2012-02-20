/////////////////////////////////////////////////////////////////////////////////////////////////
//	File Name	:	"CPlayer.h"
//	
//	Author Name	:	JC Ricks(@CSPshala)
//	
//	Purpose		:	Hold Player info and Manage player input / rendering / beat hit detection
/////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _C_PLAYER_H
#define _C_PLAYER_H

////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include "CBase.h"
#include "CBeat.h"
////////////////////////////////////////
//		   FORWARD DECLARATIONS
////////////////////////////////////////

////////////////////////////////////////
//				MISC
////////////////////////////////////////
class CPlayer : public CBase
{
public:
	/********** Construct / Deconstruct / OP Overloads ************/
	CPlayer();
	~CPlayer();
	/********** Public Utility Functions ************/
	void Input();
	void Update(float fElapsedTime);
	void Render();
	/********** Public Accessors ************/
	int				GetCurrentHP() {return m_nHP;}
	int				GetMaxHP() {return m_nMaxHP;}
	int				GetCurrentPower() {return m_nPower;}
	int				GetMaxPower() {return m_nMaxPower;}
	int				GetTotalBeatsHit() {return m_nBeatsHitTotal;}
	int				GetCurrentStreak() {return m_nCurrentStreak;}
	int				GetCurrentScore() {return m_nCurrentScore;}
	int				GetTotalScore() {return m_nTotalScore;}
	BeatDifficulty	GetPlayerDifficulty() {return m_eDifficulty;}
	float			GetCurrentRotation() {return m_fRotation;}
	BeatDirection	GetAimingDirection() {return m_eAimingDirection;}
	/********** Public Mutators  ************/	
	void			SetCurrentHP(int nHP) {m_nHP = nHP;}
	void			SetMaxHP(int nMaxHP) {m_nMaxHP = nMaxHP;}
	void			SetCurrentPower(int nPower) {m_nPower = nPower;}
	void			SetMaxPower(int nMaxPower) {m_nMaxPower = nMaxPower;}
	void			SetTotalBeatsHit(int nBeatsHitTotal) {m_nBeatsHitTotal = nBeatsHitTotal;}
	void			SetCurrentStreak(int nCurrentStreak) {m_nCurrentStreak = nCurrentStreak;}
	void			SetCurrentScore(int nCurrentScore) {m_nCurrentScore = nCurrentScore;}
	void			SetTotalScore(int nTotalScore) {m_nTotalScore = nTotalScore;}
	void			SetPlayerDifficulty(BeatDifficulty eDifficulty) {m_eDifficulty = eDifficulty;}
	void			SetCurrentRotation(float fRotation) {m_fRotation = fRotation;}
	void			SetAimingDirection(BeatDirection eAimingDirection);


	
private:
	/********** Private Members ************/
		// Player Attributes
		int m_nHP; // Current HP
		int m_nMaxHP; // Maximum HP
		int m_nPower; // Current Power
		int m_nMaxPower; // Maximum Power
		int m_nBeatsHitTotal; // Total Beats hit in song
		int m_nCurrentStreak; // Current Beat hit streak
		int m_nCurrentScore; // Current Level score (if we implement it)
		int m_nTotalScore; // Current playthrough score (ditto)
		BeatDifficulty m_eDifficulty; // Player's set difficulty (enum from CBeat.h)

		// Aim Specific stuff
		float m_fRotation; // Rotation of cone (in degrees)

		// Beat Specifics
		BeatDirection m_eAimingDirection; // Current direction player is aiming (enum from CBeat.h)


	/********** Private Accessors ************/

	/********** Private Mutators ************/

	/********** Private Utility Functions ************/

};
#endif