///////////////////////////////////////////////////////
// File Name	:	"CBeatManager.h"
//
// Author		:	JC Ricks(@CSPshala)
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////
#ifndef C_BEATMANAGER_H
#define C_BEATMANAGER_H

////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include <string>
#include <vector>
using std::vector;
using std::string;

#include "../CSong.h"  
#include "IListener.h"
////////////////////////////////////////
//		   FORWARD DECLARATIONS
////////////////////////////////////////
class CPlayer;
////////////////////////////////////////
//				MISC
////////////////////////////////////////


class CBeatManager: public IListener
{
public:
		
	/********** Public Utility Functions ************/
		bool LoadSong(string szFileName);
		bool UnloadSongs();
		void Play(string szSongName = "");
		void Pause();
		void Stop();
		void Reset();
		void Update();
		void Render();
		void CheckPlayerInput(CPlayer* aPlayer);
		
		static CBeatManager* GetInstance();
		
	/********** Public Accessors ************/
		int					GetNumberNotesHit() {return m_nNumHit;}
		vector<CSong*>&		GetSongList() {return m_vSongs;}
		vector<int>&		GetSongBackground() {return m_nvImageID;}
		string				GetCurrentlyPlayingSongName();
		bool				IsPaused() {return m_bPause;}
		CSong* GetCurrentlyPlayingSong() {return m_vSongs[m_nCurrentlyPlayingSongIndex];}
		string GetFileName() {return m_szFileName;}
		int GetFXSound() {return m_nDamageSFX;}
		int GetSoundFX() {return m_nSFX;}
	/********** Public Mutators  ************/	
		void SetNumberNotesHit(int nNumber) {m_nNumHit = nNumber;}
		void SetCurrentlyPlayingSong(string szSongName);
		void HandleEvent(CEvent* pEvent);
		void SetFileName(string szFileName) {m_szFileName = szFileName;}
		void SetFXSound(int nDamageSFX){m_nDamageSFX = nDamageSFX;}
		void SetSoundFX(int nSFX){m_nSFX = nSFX;}
	/********** Singleton Pointer *************/
			
		

private:
	/********** Construct / Deconstruct / OP Overloads ************/
		// Proper singleton
		CBeatManager(const CBeatManager&);
		//		Assignment op
		CBeatManager& operator=(const CBeatManager&);
		//		Destructor
		~CBeatManager();
		//		Constructor
		CBeatManager();

	/********** Private Members ************/
		// Current combo counter for Combo purposes.
		int m_nPlayer1CurrentCombo;  // Counts how many in the current combo threshold that P1 Hit
		int m_nPlayer2CurrentCombo;  // Counts how many in the current combo threshold that P2 Hit
		int m_nComboThreshold; // The combo threshold. Checks player's current combo against this 
		int m_nNotesPassed; // Counts how many notes have passed in the current combo threshold

		vector<CSong*> m_vSongs;
		vector<int> m_nvImageID;
		int m_nNumHit;
		bool m_bPause;
		int m_nSFX;
		int m_nDamageSFX;
		// Used for setting song
		string szCurrentlyPlayingSong;
		// Index into vector of songs to play song
		int	 m_nCurrentlyPlayingSongIndex;
		bool fuckyou;
		string m_szFileName;

		

		/********** Private Accessors ************/		
		int GetCurrentlyPlayingSongIndex() {return m_nCurrentlyPlayingSongIndex;}
		int GetP1CurrentCombo() {return m_nPlayer1CurrentCombo;}
		int GetP2CurrentCombo() {return m_nPlayer2CurrentCombo;}
		int GetComboThreshold() {return m_nComboThreshold;}
		int GetNotesPassed() {return m_nNotesPassed;}

	/********** Private Mutators ************/
		void SetPause(bool bPause) {m_bPause = bPause;}
		void SetP1CurrentCombo(int nCurrent) {m_nPlayer1CurrentCombo = nCurrent;}
		void SetP2CurrentCombo(int nCurrent) {m_nPlayer2CurrentCombo = nCurrent;}
		void SetComboThreshold(int nThresh) {m_nComboThreshold = nThresh;}
		void SetNotesPassed(int nPassed) {m_nNotesPassed = nPassed;}

	/********** Private Utility Functions ************/
		void EvaluatePlayerCombos(); // Evaluates whether combo threshold has passed, and if so who gets damage (if any)
		void DealDamageToPlayer(CPlayer* playerToDmg, CPlayer* damageDealer); // Applies damage to a player



};

#endif