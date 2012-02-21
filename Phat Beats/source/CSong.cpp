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
#include "CPlayer.h"
#include "SGD Wrappers\CSGD_FModManager.h"
#include "Managers\CEvent.h"
#include "Managers\CEventSystem.h"

////////////////////////////////////////
//				MISC
////////////////////////////////////////

///////////////////////////////////////////////
//  CONSTRUCTOR / DECONSTRUCT / OP OVERLOADS
///////////////////////////////////////////////
CSong::CSong()
{
	SetCurrentSongTime(0);
	SetSongName("DEFAULT SONG NAME");
	SetCurrentBeatIndex(0);
	SetNextBeatIndex(1);	

	// Start with ref to self
	m_uiRefCount = 1;

	m_nType = OBJ_SONG;

	// Asset IDs
	SetSongID(-1);
	SetBackgroundID(-1);

	CEventSystem::GetInstance()->RegisterClient("notecollision",this);
}

CSong::~CSong()
{
	CEventSystem::GetInstance()->UnregisterClient("notecollision",this);
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
	m_nCurrentSongTime = theSong.m_nCurrentSongTime;
	m_nSongDuration = theSong.m_nSongDuration;
	m_nCurrentBeat = theSong.m_nCurrentBeat;
	m_nNextBeat = theSong.m_nNextBeat;

	m_nSoundID = theSong.m_nSoundID;
	m_nImageID = theSong.m_nImageID;
	

	return *this;
}

////////////////////////////////////////
//		PUBLIC UTILITY FUNCTIONS
////////////////////////////////////////
void CSong::Update(float fElapsedTime)
{
	// Checking upcomming beat and adding it to active vector when it's within tolerance
	if(!((unsigned int)GetCurrentBeatIndex() >= m_vBeats.size()))
		if(m_vBeats[GetCurrentBeatIndex()].GetTimeOfBeat() < (GetCurrentSongTime() - 1000))
		{
			m_vActiveBeats.push_back(m_vBeats[GetCurrentBeatIndex()]);
			
			m_vActiveBeats.back().SetIsActive(true);

			// Setting the next beat to call
			SetCurrentBeatIndex(GetCurrentBeatIndex() + 1);
		}	


	list<CBeat>::iterator i = m_vActiveBeats.begin();
	// Updating active beats
	if(m_vActiveBeats.size() > 0)		
		do
		{	
			if(i->GetIsActive())
				i->Update(GAME->GetTimer().GetDeltaTime());
			else
				i = m_vActiveBeats.erase(i);
			
			// Jesus christ...
			if(i == m_vActiveBeats.end())
				break;
			else 
				++i;

			if(i == m_vActiveBeats.end())
				break;
			
		}while(true);
		

	// Updating song time	
	if(FMODMAN->IsSoundPlaying(GetSongID()))
	{
		FMOD::Channel* derp = FMODMAN->GetLatestChannel(GetSongID());

		unsigned int daTyme = 0;

		derp->getPosition(&daTyme,FMOD_TIMEUNIT_MS);

		// Sound info struct for Debugging
		//tSoundInfo test = FMODMAN->GetSound(GetSongID());

		SetCurrentSongTime((int)daTyme);
	}
}

void CSong::Render()
{
	// Rendering Notes
	list<CBeat>::iterator i = m_vActiveBeats.begin();

	if(m_vActiveBeats.size() > 0)
		for(; i != m_vActiveBeats.end(); ++i)
			i->Render();

}

void CSong::ResetSong()
{
	
	// Setting everything back to normal
	SetCurrentSongTime(0);
	SetCurrentBeatIndex(0);
	SetNextBeatIndex(1);


	
	m_vActiveBeats.clear();

	// Resetting beats
	for(unsigned int i = 0; i < m_vBeats.size(); ++i)
		m_vBeats[i].ResetBeat();
}

RECT CSong::GetCollisionRect()
{
	// Garbage ass rect because I don't need it
	RECT derp;
	derp.bottom = 0;
	derp.left = 0;
	derp.right = 0;
	derp.top = 0;

	return derp;
}

bool CSong::CheckCollision(IBaseInterface* pBase)
{
	if(IsCurrentlyPlayingSong())
	{
		CPlayer* pPlayer = (CPlayer*)pBase;
		// Only checking with beats that are currently active
		list<CBeat>::iterator i = m_vActiveBeats.begin();

		for(; i != m_vActiveBeats.end(); ++i)	
		{
			/// GROOOOOOOOOOOOOOOOOOOOOOOOSSSSSS
				if(pPlayer->CheckCollision(&(*i)))
				{
					CEventSystem::GetInstance()->SendEvent(i->GetEvent(),&(*i));
				}
				// Note has collided before and is not colliding now
				// so this means it's past the point where player can hit
				else if(i->GetHasCollided() == true)
				{
					i->SetIsActive(false);					
				}
		}
		return true;
	}
	else
		return false;
}

void CSong::Release()
{
	m_uiRefCount--;

	if(m_uiRefCount == 0)
		delete this;
}

void CSong::HandleEvent(CEvent* pEvent)
{

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


