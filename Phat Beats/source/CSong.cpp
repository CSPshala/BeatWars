///////////////////////////////////////////////////////
// File Name	:	"CSong.cpp"
//
// Purpose		:	To contain all notes in a song
//////////////////////////////////////////////////////


////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include "JCMacros.h"
#include "CSong.h"
#include "CGame.h"
#include "SGD Wrappers\CSGD_XAudio2.h"

////////////////////////////////////////
//				MISC
////////////////////////////////////////

///////////////////////////////////////////////
//  CONSTRUCTOR / DECONSTRUCT / OP OVERLOADS
///////////////////////////////////////////////
CSong::CSong()
{
	SetCurrentSongTime(0.0f);
	SetSongName("DEFAULT SONG NAME");
	SetCurrentBeatIndex(0);
	SetNextBeatIndex(1);
	
	// Asset IDs
	SetSongID(-1);
	SetBackgroundID(-1);
}

CSong::~CSong()
{

}

CSong::CSong(const CSong& theSong)
{
	*this = theSong;
}

CSong& CSong::operator=(const CSong& theSong)
{
	m_vBeats = theSong.m_vBeats;
	m_vActiveBeats = theSong.m_vActiveBeats;
	m_szName = theSong.m_szName;
	m_fCurrentSongTime = theSong.m_fCurrentSongTime;
	m_fSongDuration = theSong.m_fSongDuration;
	m_nCurrentBeat = theSong.m_nCurrentBeat;
	m_nNextBeat = theSong.m_nNextBeat;

	m_nSoundID = theSong.m_nSoundID;
	m_nImageID = theSong.m_nImageID;
	

	return *this;
}

////////////////////////////////////////
//		PUBLIC UTILITY FUNCTIONS
////////////////////////////////////////
void CSong::UpdateSong()
{
	// Checking upcomming beat and adding it to active vector when it's within tolerance
	if(!((unsigned int)GetCurrentBeatIndex() >= m_vBeats.size()))
		if(m_vBeats[GetCurrentBeatIndex()].GetTimeOfBeat() < (GetCurrentSongTime() - 1.0))
		{
			m_vActiveBeats.push_back(m_vBeats[GetCurrentBeatIndex()]);
			m_vActiveBeats.back().SetIsActive(true);
			SetCurrentBeatIndex(GetCurrentBeatIndex() + 1);
		}	

	// Updating active beats
	if(m_vActiveBeats.size() > 0)
		for(unsigned int i = 0; i < m_vActiveBeats.size(); ++i)
			m_vActiveBeats[i].Update(GAME->GetTimer().GetDeltaTime());

	// Updating song time
	SetCurrentSongTime(GetCurrentSongTime() + GAME->GetTimer().GetDeltaTime());
}

void CSong::RenderSong()
{
	// Rendering Notes
	if(m_vActiveBeats.size() > 0)
		for(unsigned int i = 0; i < m_vActiveBeats.size(); ++i)
			m_vActiveBeats[i].Render();

}

void CSong::ResetSong()
{
	
	// Setting everything back to normal
	SetCurrentSongTime(0.0f);
	SetCurrentBeatIndex(0);
	SetNextBeatIndex(1);

	// Clearing active beats vector
	m_vActiveBeats.clear();

	// Resetting beats
	for(unsigned int i = 0; i < m_vBeats.size(); ++i)
		m_vBeats[i].ResetBeat();
}

////////////////////////////////////////
//		PRIVATE UTILITY FUNCTIONS
////////////////////////////////////////

////////////////////////////////////////
//	    PUBLIC ACCESSORS / MUTATORS
////////////////////////////////////////

////////////////////////////////////////
//	    PRIVATE ACCESSORS / MUTATORS
////////////////////////////////////////


