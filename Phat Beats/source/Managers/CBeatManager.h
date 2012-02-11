///////////////////////////////////////////////////////
// File Name	:	"CBeatManager.h"
//
// Author		:	JC Ricks
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

////////////////////////////////////////
//		   FORWARD DECLARATIONS
////////////////////////////////////////

////////////////////////////////////////
//				MISC
////////////////////////////////////////


class CBeatManager
{
public:

	/********** Construct / Deconstruct / OP Overloads ************/
		CBeatManager();
		~CBeatManager();
	/********** Public Utility Functions ************/
		bool LoadSong(string szFileName);
		bool UnloadSongs();
		void Play();
		void Pause();
		void Stop();
		void Reset();
		void Update();
		void Render();
	/********** Public Accessors ************/
		int				GetNumberNotesHit() {return m_nNumHit;}
		vector<CSong>&	GetSongList() {return m_vSongs;}
		vector<int>&		GetSongBackground() {return m_nvImageID;}
	/********** Public Mutators  ************/	
		void SetNumberNotesHit(int nNumber) {m_nNumHit = nNumber;}

private:

	/********** Private Members ************/
		vector<CSong> m_vSongs;
		vector<int> m_nvImageID;
		int m_nNumHit;
		bool m_bPause;
	/********** Private Accessors ************/
		bool GetPause() {return m_bPause;}

	/********** Private Mutators ************/
		void SetPause(bool bPause) {m_bPause = bPause;}

	/********** Private Utility Functions ************/
	



};

#endif