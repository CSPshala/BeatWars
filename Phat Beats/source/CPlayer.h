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
#include <list>
#include <queue>
using std::vector;
using std::queue;

#include "CBase.h"
#include "CBeat.h"
#include "IBaseInterface.h"
#include "Managers/IListener.h"
#include "CAnimation.h"
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
	void PlayAnimation();
	void StopAnimation();
	void ResetAnimation();
	/********** Public Accessors ************/
	int					GetCurrentHP() {return m_nHP;}
	int					GetMaxHP() {return m_nMaxHP;}
	int					GetCurrentPower() {return m_nPower;}
	int					GetCurrentPowerup(void) { return m_nCurrentPowerup; }
	int					GetMaxPower() {return m_nMaxPower;}
	int					GetTotalBeatsHit() {return m_nBeatsHitTotal;}
	int					GetCurrentStreak() {return m_nCurrentStreak;}
	int					GetCurrentScore() {return m_nCurrentScore;}
	int					GetTotalScore() {return m_nTotalScore;}
	int					GetCurrentTakeDown() {return m_nTakeDown;}
	BeatDifficulty		GetPlayerDifficulty() {return m_eDifficulty;}
	float				GetCurrentRotation() {return m_fRotation;}
	BeatDirection		GetAimingDirection() {return m_eAimingDirection;}
	vector<CBeat*>&		GetAIBeats() {return m_vAIBeats;}
	queue<TBeatHit>&	GetPlayerHitQueue() {return m_qKeyPresses;}
	char			GetMostRecentKeyPress();
	int					GetAILevel() {return m_nAILevel;}
	bool				GetAttackMode() {return m_bAttackMode;}
	float				GetAttackModeTimer() {return m_fAttackModeTimer;}
	vector<CAnimation*> GetAnimations(){ return m_vecAnimations; }
	CAnimation*			GetCurrAnim(){ return m_vecAnimations[m_nCurrAnim]; }
	bool				GetAnimationIsEmpty() { return m_bAnimationsEmpty; }
	int					NumberofAnimations()
						{ 
							if( !m_bAnimationsEmpty )
								return m_vecAnimations.size(); 

							return 0;
						}
	int					GetMaxPowerup() { return m_nMaxPowerup; }
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
	void				SetAnimations( vector<CAnimation*> vecAnim ){  m_vecAnimations = vecAnim;}
	void				SetCurrAnimation(string szAnimName );
	void				SetAILevel(int nAILevel) {m_nAILevel = nAILevel;}
	void				SetAttackMode(bool nMode) {m_bAttackMode = nMode;}
	void				SetAttackModeTimer(float nTime) {m_fAttackModeTimer = nTime;}
	void				SetAnimationsIsEmpty(bool bEmpty) { m_bAnimationsEmpty = bEmpty; }
	void				SetSingleAnimation( CAnimation* pAnim );
	void				SetCurrentPowerup(int nCurrentPowerup) { m_nCurrentPowerup = nCurrentPowerup; }
	void				SetMaxPowerup(int nMaxPowerup) { m_nMaxPowerup = nMaxPowerup; }
	void				SetTakeDown(int nTakeDown) {m_nTakeDown = nTakeDown;}

	
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
		int m_nAILevel; // setting the ai level from option state
		// Aim Specific stuff
		float m_fRotation; // Rotation of cone (in degrees)
		
		// Attack mode bool.  If false = Defense mode
		bool m_bAttackMode;
			// Attack mode timer / so player can't spam attack/defense switching
		float m_fAttackModeTimer;

		// Beat Specifics
		BeatDirection m_eAimingDirection; // Current direction player is aiming (enum from CBeat.h)

		// AI check for already hit beats
		vector<CBeat*> m_vAIBeats;
		
		// Player hit vector
		queue<TBeatHit> m_qKeyPresses;

		// Player hit key
		char cHitKey;

		// Asset IDs
			// Images
			int m_nHitBoxImage;
			int m_nBeatConeID;
			// Sound
			int m_nBeatSuccessID;
			int m_nBeatMissID;
			bool m_IbwriteShit;

		// Player Animations
			vector<CAnimation*> m_vecAnimations;
			int m_nCurrAnim;
			bool m_bAnimationsEmpty;
		//Power Up stats
			int m_nMaxPowerup;
			int m_nCurrentPowerup;

		// Player take down count
			int m_nTakeDown;
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