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
#include "States\CGameplay_State.h"
#include "States\COptionsState.h"
#include "Managers\CAiManager.h"
#include "..\CLevelManager.h"

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

	m_bAmIPlaying = false;
	m_nSongDuration = -1;

	m_nCurrentBeat = 0;
	m_nNextBeat = 0;

	// Asset IDs
	SetSongID(-1);
	SetBackgroundID(-1);

	CEventSystem::GetInstance()->RegisterClient("notecollision",this);
	CEventSystem::GetInstance()->RegisterClient("combostart",this);
	CEventSystem::GetInstance()->RegisterClient("comboend",this);
}

CSong::~CSong()
{
	CEventSystem::GetInstance()->UnregisterClient("notecollision",this);
	CEventSystem::GetInstance()->UnregisterClient("combostart",this);
	CEventSystem::GetInstance()->UnregisterClient("comboend",this);
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
	m_bAmIPlaying = theSong.m_bAmIPlaying;

	return *this;
}

////////////////////////////////////////
//		PUBLIC UTILITY FUNCTIONS
////////////////////////////////////////
void CSong::Update(float fElapsedTime)
{
	if(IsCurrentlyPlayingSong())
	{
	
			// Checking upcomming beat and adding it to active vector when it's within tolerance
		if((unsigned int)GetCurrentBeatIndex() < m_vBeats.size())
			if(m_vBeats[GetCurrentBeatIndex()].GetTimeOfBeat() < ((int)GetCurrentSongTime() + 1000))
			{
				// Checking for difficulty, not adding it if it's below or equal to what we're set to
				if(COptionsState::GetInstance()->GetDifficulty() >= m_vBeats[GetCurrentBeatIndex()].GetDifficulty())
				{
					bool found = false;

					list<CBeat*>::iterator x = m_vActiveBeats.begin();

					for(; x != m_vActiveBeats.end(); ++x)
						if((*x)->GetTimeOfBeat() == m_vBeats[GetCurrentBeatIndex()].GetTimeOfBeat())
							found = true;

					if(!found)
					{
						m_vActiveBeats.push_back(&m_vBeats[GetCurrentBeatIndex()]);	
						m_vActiveBeats.back()->SetIsActive(true);	
					}
				}

				// Setting the next beat to call
				SetCurrentBeatIndex(GetCurrentBeatIndex() + 1);
			}	


		list<CBeat*>::iterator i = m_vActiveBeats.begin();
		// Updating active beats
		if(m_vActiveBeats.size() > 0)
			do
			{	
				if((*i)->GetIsActive())
					(*i)->Update(GAME->GetTimer().GetDeltaTime());
				else
					i = m_vActiveBeats.erase(i);
			
				// Jesus christ...
				if(i == m_vActiveBeats.end())
				{
					break;
				}
				else 
					++i;

				if(i == m_vActiveBeats.end())
				{
					break;
				}
			
			}while(true);
		

		// Updating song time	
		if(FMODMAN->IsSoundPlaying(GetSongID()))
		{
			FMOD::Channel* derp = FMODMAN->GetLatestChannel(GetSongID());

			unsigned int daTyme = 0;

			derp->getPosition(&daTyme,FMOD_TIMEUNIT_MS);

			// Sound info struct for Debugging
			//tSoundInfo test = FMODMAN->GetSound(GetSongID());

			SetCurrentSongTime(daTyme);
		}
	}
}

void CSong::Render()
{
	if(IsCurrentlyPlayingSong())
	{
		// Rendering Notes
		list<CBeat*>::iterator i = m_vActiveBeats.begin();

		if(m_vActiveBeats.size() > 0)
			for(; i != m_vActiveBeats.end(); ++i)
			{
				// Rendering Player 1 and Player 2 notes
				(*i)->Render();			
			}
	}
}

void CSong::ResetSong()
{
	
	// Setting everything back to normal
	SetCurrentSongTime(0);
	SetCurrentBeatIndex(0);
	SetNextBeatIndex(1);

	m_vHittableBeats.clear();
	
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
		list<CBeat*>::iterator i = m_vActiveBeats.begin();

		static int numHit = 0;

		if(pPlayer->GetType() == OBJ_AI)
			int derp = 1;

		for(; i != m_vActiveBeats.end(); ++i)	
		{
			/// GROOOOOOOOOOOOOOOOOOOOOOOOSSSSSS
				if(pPlayer->CheckCollision(*i))
				{
					bool found = false;

					list<CBeat*>::iterator y = m_vHittableBeats.begin();
					for(; y != m_vHittableBeats.end(); ++y)
						if((*y) == (*i))
							found = true;

					if(!found)
					{
						m_vHittableBeats.push_back(*i);						
						// Sending note event (only once)
						CEventSystem::GetInstance()->SendEvent((*i)->GetEvent(),&(*i));
					}					
										

					if( numHit > 5 )					
					{
						CEventSystem::GetInstance()->SendEvent("comboend");
						numHit = 0;
					}
				}
				// Note has collided before and is not colliding now
				// so this means it's past the point where player can hit
				else if((*i)->GetHasCollided() == true)
				{
					list<CBeat*>::iterator y = m_vHittableBeats.begin();
					for(; y != m_vHittableBeats.end(); ++y)
					{
						if((*y) == (*i))
						{
							// Player 1 never hit the note. Kill his streak
							if(!(*y)->GetPlayer1Hit())
								CLevelManager::GetInstance()->GetPlayer(PlayerOne)->SetCurrentStreak(0);

							// Player 2 never hit the note. Kill his streak
							if(!(*y)->GetPlayer2Hit())
								CLevelManager::GetInstance()->GetPlayer(PlayerTwo)->SetCurrentStreak(0);

							y = m_vHittableBeats.erase(y);
						}

						if(y == m_vHittableBeats.end())
							break;
					}
					
					ES->SendEvent("notepassed",NULL);

					(*i)->SetIsActive(false);	

					// Also removing from active beats (CAUSE ITS NOT ACTIVE HUR HUR)
					i = m_vActiveBeats.erase(i);

					if(i == m_vActiveBeats.end())
						break;
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

void CSong::CreateAIHits()
{
	for (unsigned int i = 0; i < m_vBeats.size(); ++i)
	{
		if (CAiManager::GetInsatance()->RandomDifficult(COptionsState::GetInstance()->GetAILevel()) == true)
		{
			m_vBeats[i].SetPlayer2Hit(true);			
		}
		else
		{
			m_vBeats[i].SetPlayer2Hit(false);				
		}
	}
	
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


