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
#include <vector>
#include <string>
using std::vector;
using std::string;

#include "CBeat.h"

////////////////////////////////////////
//		   FORWARD DECLARATIONS
////////////////////////////////////////

////////////////////////////////////////
//				MISC
////////////////////////////////////////
#define MARGINOFERROR 0.1f

class CSong
{
public:	
	/********** Construct / Deconstruct / OP Overloads ************/
		CSong();
		~CSong();
		CSong(const CSong&);
		CSong& operator=(const CSong&);

	/********** Public Utility Functions ************/
		void RenderSong();
		void UpdateSong();
		void ResetSong();

	/********** Public Accessors ************/
		vector<CBeat>&	GetBeatList() {return m_vBeats;}
		vector<CBeat>&	GetActiveBeatList() {return m_vActiveBeats;}
		string			GetSongName() {return m_szName;}
		float			GetCurrentSongTime() {return m_fCurrentSongTime;}
		float			GetSongDuration() {return m_fSongDuration;}
		int				GetSongID()	{return m_nSoundID;}
		int				GetBackgroundID() {return m_nImageID;}
		int				GetCurrentBeatIndex() {return m_nCurrentBeat;}
		int				GetNextBeatIndex() {return m_nNextBeat;}

	/********** Public Mutators  ************/	
		void			SetSongName(string szName) {m_szName = szName;}
		void			SetCurrentSongTime(float fSongTime) {m_fCurrentSongTime = fSongTime;}
		void			SetSongDuration(float fDuration) {m_fSongDuration = fDuration;}
		void			SetSongID(int ID) {m_nSoundID = ID;}
		void			SetBackgroundID(int ID) {m_nImageID = ID;}
		void			SetCurrentBeatIndex(int nBeat) {m_nCurrentBeat = nBeat;}
		void			SetNextBeatIndex(int nBeat) {m_nNextBeat = nBeat;}

private:

	/********** Private Members ************/	
		vector<CBeat>	m_vBeats;
		vector<CBeat>	m_vActiveBeats;
		string			m_szName;
		float			m_fCurrentSongTime;
		float			m_fSongDuration;
		int				m_nCurrentBeat;
		int				m_nNextBeat;

		// Asset IDs
		int			 m_nSoundID;
		int			 m_nImageID;
		
	/********** Private Accessors ************/

	/********** Private Mutators ************/

	/********** Private Utility Functions ************/



};

#endif