///////////////////////////////////////////////////////
// File Name	:	"CSong.h"
//
// Author		:	JC Ricks
//
// Purpose		:	To contain all notes in a song
//////////////////////////////////////////////////////

#ifndef C_SONG_H
#define C_SONG_H

////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include <list>
#include <string>
using std::list;
using std::string;

#include "CBeat.h"

////////////////////////////////////////
//		   FORWARD DECLARATIONS
////////////////////////////////////////

////////////////////////////////////////
//				MISC
////////////////////////////////////////

class CSong
{
public:	
	/********** Construct / Deconstruct / OP Overloads ************/
		CSong();
		~CSong();

	/********** Public Utility Functions ************/

	/********** Public Accessors ************/
		list<CBeat>&	GetBeatList() {return m_listBeats;}
		string			GetSongName() {return m_szName;}
		float			GetCurrentSongTime() {return m_fCurrentSongTime;}
		int				GetSongID()	{return m_nSoundID;}
		int				GetBackgroundID() {return m_nImageID;}
	/********** Public Mutators  ************/	
		void			SetSongName(string szName) {m_szName = szName;}
		void			SetCurrentSongTime(float fSongTime) {m_fCurrentSongTime = fSongTime;}
		void			SetSongID(int ID) {m_nSoundID = ID;}
		void			SetBackgroundID(int ID) {m_nImageID = ID;}

private:

	/********** Private Members ************/	
		list<CBeat>	 m_listBeats;
		string		 m_szName;
		float		 m_fCurrentSongTime;

		// Asset IDs
		int			 m_nSoundID;
		int			 m_nImageID;
		
	/********** Private Accessors ************/

	/********** Private Mutators ************/

	/********** Private Utility Functions ************/



};

#endif