///////////////////////////////////////////////////////
// File Name	:	"CSong.h"
//
// Author		:	JC Ricks(@CSPshala)
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
#include <list>
using std::vector;
using std::string;
using std::list;
using std::iterator;

#include "CBeat.h"
#include "IBaseInterface.h"
#include "Managers\IListener.h"

////////////////////////////////////////
//		   FORWARD DECLARATIONS
////////////////////////////////////////

////////////////////////////////////////
//				MISC
////////////////////////////////////////


class CSong : public IBaseInterface, public IListener
{
public:	
	/********** Construct / Deconstruct / OP Overloads ************/
		CSong();
		~CSong();
		CSong(const CSong&);
		CSong& operator=(const CSong&);

	/********** Public Utility Functions ************/
		void Render();
		void Update(float fElapsedTime);
		void ResetSong();
		void HandleEvent(CEvent* pEvent);
		void CreateAIHits();

	/********** Public Accessors ************/
		vector<CBeat>&	GetBeatList() {return m_vBeats;}
		list<CBeat*>&	GetActiveBeatList() {return m_vActiveBeats;}
		//list<CBeat*>& GetHittableBeatList() {return m_vHittableBeats;}
		string			GetSongName() {return m_szName;}
		string			GetCurrentFileName() {return m_szFileName;}
		unsigned int	GetCurrentSongTime() {return m_nCurrentSongTime;}
		int				GetSongDuration() {return m_nSongDuration;}
		int				GetSongID()	{return m_nSoundID;}
		int				GetBackgroundID() {return m_nImageID;}
		int				GetCurrentBeatIndex() {return m_nCurrentBeat;}
		CBeat*			GetCurrentBeat() {return m_vActiveBeats.front();}
		int				GetNextBeatIndex() {return m_nNextBeat;}
		int				GetType() {return m_nType;}
		RECT			GetCollisionRect();
		bool			CheckCollision(IBaseInterface* pBase);
		bool			IsCurrentlyPlayingSong() {return m_bAmIPlaying;}
		CBeat*			GetPlayer1HittableBeat() {return m_pPlayer1HittableBeat;}
		CBeat*			GetPlayer2HittableBeat() {return m_pPlayer2HittableBeat;}
		int				GetPlayer1NextBeatIndex()	{return m_nPlayer1NextBeat;}
		int				GetPlayer2NextBeatIndex()	{return m_nPlayer2NextBeat;}

		// Ref stuff
		void			AddRef() {++m_uiRefCount;}
		void			Release();


	/********** Public Mutators  ************/	
		void			SetSongName(string szName) {m_szName = szName;}
		void			SetCurrentSongTime(unsigned int fSongTime) {m_nCurrentSongTime = fSongTime;}
		void			SetSongDuration(int fDuration) {m_nSongDuration = fDuration;}
		void			SetSongID(int ID) {m_nSoundID = ID;}
		void			SetBackgroundID(int ID) {m_nImageID = ID;}
		void			SetCurrentBeatIndex(int nBeat) {m_nCurrentBeat = nBeat;}
		void			SetNextBeatIndex(int nBeat) {m_nNextBeat = nBeat;}
		void			SetCurrentlyPlayingSong(bool bPlay) {m_bAmIPlaying = bPlay;}
		void			SetCurrentPlayingSongFileName(string szFileName) {m_szFileName = szFileName;}
		void			NextPlayer1HittableBeat();
		void			NextPlayer2HittableBeat();	
		void			SetPlayer1NextBeatIndex(int nDex) {m_nPlayer1NextBeat = nDex;}
		void			SetPlayer2NextBeatIndex(int nDex) {m_nPlayer2NextBeat = nDex;}

protected:
		int m_nType;

private:

	/********** Private Members ************/	
		vector<CBeat>	m_vBeats;
		list<CBeat*>	m_vActiveBeats; // Active Beats manages which beats are currently visible on screen
		//list<CBeat*>	m_vHittableBeats; // Hittable beats holds which beats are able to currently be hit
		// Player beat hit pointers
		CBeat*			m_pPlayer1HittableBeat;
		int				m_nPlayer1NextBeat;
		CBeat*			m_pPlayer2HittableBeat;
		int				m_nPlayer2NextBeat;

		string			m_szName;
		unsigned int	m_nCurrentSongTime;
		int				m_nSongDuration;
		int				m_nCurrentBeat;
		int				m_nNextBeat;
		string			m_szFileName;
		// Is this the current song that is playing?
		bool			m_bAmIPlaying;

		// References
		unsigned int m_uiRefCount;

		// Asset IDs
		int			 m_nSoundID;
		int			 m_nImageID;
		
	/********** Private Accessors ************/

	/********** Private Mutators ************/

	/********** Private Utility Functions ************/



};

#endif