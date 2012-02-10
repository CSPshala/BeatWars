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
#include <list>
using std::list;
using std::string;

#include "../CSong.h"  

////////////////////////////////////////
//		   FORWARD DECLARATIONS
////////////////////////////////////////

////////////////////////////////////////
//				MISC
////////////////////////////////////////
#define MARGINOFERROR 0.1f

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
	/********** Public Accessors ************/
		int				GetNumberNotesHit() {return m_nNumHit;}
		list<CSong>&	GetSongList() {return m_listSongs;}
		list<int>&		GetSongBackground() {return m_nListImageID;}
	/********** Public Mutators  ************/	
		void SetNumberNotesHit(int nNumber) {m_nNumHit = nNumber;}

private:

	/********** Private Members ************/
		list<CSong> m_listSongs;
		list<int> m_nListImageID;
		int m_nNumHit;
	/********** Private Accessors ************/

	/********** Private Mutators ************/

	/********** Private Utility Functions ************/
	



};

#endif