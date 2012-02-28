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
	/********** Public Mutators  ************/	
		void SetNumberNotesHit(int nNumber) {m_nNumHit = nNumber;}
		void SetCurrentlyPlayingSong(string szSongName);
		void HandleEvent(CEvent* pEvent);
		void SetFileName(string szFileName) {m_szFileName = szFileName;}
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
		vector<CSong*> m_vSongs;
		vector<int> m_nvImageID;
		int m_nNumHit;
		bool m_bPause;
		// Used for setting song
		string szCurrentlyPlayingSong;
		// Index into vector of songs to play song
		int	 m_nCurrentlyPlayingSongIndex;
		bool fuckyou;
		string m_szFileName;
		/********** Private Accessors ************/
		
		int GetCurrentlyPlayingSongIndex() {return m_nCurrentlyPlayingSongIndex;}
		

	/********** Private Mutators ************/
		void SetPause(bool bPause) {m_bPause = bPause;}

	/********** Private Utility Functions ************/
	



};

#endif