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
#include <vector>
#include <queue>
using std::vector;
using std::queue;

#include "CBase.h"
#include "CBeat.h"
#include "IBaseInterface.h"
#include "Managers/IListener.h"
////////////////////////////////////////
//		   FORWARD DECLARATIONS
////////////////////////////////////////

////////////////////////////////////////
//				MISC
////////////////////////////////////////
struct TBeatHit
{
	TBeatHit(char note,int time) {cHitNote = note; nTime = time;}

	char cHitNote;
	int  nTime;
};


class CPlayer : public CBase , public IListener
{
public:
	/********** Construct / Deconstruct / OP Overloads ************/
	CPlayer(ObjType eType = OBJ_PLAYER1);
	~CPlayer();
	/********** Public Utility Functions ************/
	void Input();
	void Update(float fElapsedTime);
	void Render();
	RECT GetCollisionRect();  // Gonna override the CBase collision rect
	bool CheckCollision(IBaseInterface* pBase);
	void HandleEvent(CEvent* pEvent);
	/********** Public Accessors ************/
	int					GetCurrentHP() {return m_nHP;}
	int					GetMaxHP() {return m_nMaxHP;}
	int					GetCurrentPower() {return m_nPower;}
	int					GetMaxPower() {return m_nMaxPower;}
	int					GetTotalBeatsHit() {return m_nBeatsHitTotal;}
	int					GetCurrentStreak() {return m_nCurrentStreak;}
	int					GetCurrentScore() {return m_nCurrentScore;}
	int					GetTotalScore() {return m_nTotalScore;}
	BeatDifficulty		GetPlayerDifficulty() {return m_eDifficulty;}
	float				GetCurrentRotation() {return m_fRotation;}
	BeatDirection		GetAimingDirection() {return m_eAimingDirection;}
	vector<CBeat*>		GetAIBeats() {return m_vAIBeats;}
	queue<TBeatHit>&	GetPlayerHitQueue() {return m_qKeyPresses;}
	TBeatHit&			GetMostRecentKeyPress();
	/********** Public Mutators  ************/	
	void				SetCurrentHP(int nHP) {m_nHP = nHP;}
	void				SetMaxHP(int nMaxHP) {m_nMaxHP = nMaxHP;}
	void				SetCurrentPower(int nPower) {m_nPower = nPower;}
	void				SetMaxPower(int nMaxPower) {m_nMaxPower = nMaxPower;}
	void				SetTotalBeatsHit(int nBeatsHitTotal) {m_nBeatsHitTotal = nBeatsHitTotal;}
	void				SetCurrentStreak(int nCurrentStreak) {m_nCurrentStreak = nCurrentStreak;}
	void				SetCurrentScore(int nCurrentScore) {m_nCurrentScore = nCurrentScore;}
	void				SetTotalScore(int nTotalScore) {m_nTotalScore = nTotalScore;}
	void				SetPlayerDifficulty(BeatDifficulty eDifficulty) {m_eDifficulty = eDifficulty;}
	void				SetCurrentRotation(float fRotation) {m_fRotation = fRotation;}
	void				SetAimingDirection(BeatDirection eAimingDirection);


	
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

		// AI handling stuff
		vector<CBeat*> m_vAIBeats;

		// Player hit vector
		queue<TBeatHit> m_qKeyPresses;

		// Asset IDs
			// Images
			int m_nHitBoxImage;
			int m_nBeatConeID;
			// Sound
			int m_nBeatSuccessID;
			int m_nBeatMissID;
			bool m_IbwriteShit;



	/********** Private Accessors ************/
			int GetBeatConeID() {return m_nBeatConeID;}
			int GetBeatSuccessID() {return m_nBeatSuccessID;}
			int GetBeatMissID() {return m_nBeatMissID;}

	/********** Private Mutators ************/
			void SetBeatConeID(int nBeatConeID) {m_nBeatConeID = nBeatConeID;}
			void SetBeatSuccessID(int nBeatSuccessID) {m_nBeatSuccessID = nBeatSuccessID;}
			void SetBeatMissID(int nBeatMissID) {m_nBeatMissID = nBeatMissID;}

	/********** Private Utility Functions ************/
			void P1InputHandling();
			void P2InputHandling();
			void AIHandling();
};


#endif