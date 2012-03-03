///////////////////////////////////////////////////////
// File Name	:	"CBeatManager.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include "CBeatManager.h"
#include "../JCMacros.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../States/CGameplay_State.h"
#include "../XML/tinystr.h"
#include "../XML/tinyxml.h"
#include "CEventSystem.h"
#include "CObjectManager.h"
#include "../CGame.h"
#include "../CPlayer.h"
#include "CFXManager.h"
#include "../../CLevelManager.h"

////////////////////////////////////////
//				MISC
////////////////////////////////////////
// 300ms margin for hitting beat
#define MARGINOFERROR 300
// Define for easy changing of Damage Threshold
#define COMBODAMAGETHRESHOLD 5

///////////////////////////////////////////////
//  CONSTRUCTOR / DECONSTRUCT / OP OVERLOADS
///////////////////////////////////////////////
CBeatManager::CBeatManager()
{
	SetNumberNotesHit(0);
	SetPause(true);
	CEventSystem::GetInstance()->RegisterClient("test.event",this);
	ES->RegisterClient("notepassed",this);
	fuckyou = false;
	SetCurrentlyPlayingSong("none");
	SetComboThreshold(COMBODAMAGETHRESHOLD);
}

CBeatManager::~CBeatManager()
{
	CEventSystem::GetInstance()->UnregisterClient("test.event",this);
	ES->UnregisterClient("notepassed",this);
}

////////////////////////////////////////
//		PUBLIC UTILITY FUNCTIONS
////////////////////////////////////////
bool CBeatManager::LoadSong(string szFileName)
{
	TiXmlDocument doc;

	// Song for new song creation
	CSong* theSong = new CSong();

	// Adding path to filename for beat lists
	string szPath = "resource/beatlist/";
	szPath += szFileName;
	SetFileName(szFileName);
	if(doc.LoadFile(szPath.c_str()) == false)
		return false;

	TiXmlElement* pRoot = doc.RootElement();

	if(!pRoot)
		return false;

//**************LOADING SONG SPECIFICS********************//
	// Getting song's filename to load
	if(pRoot->Attribute("name") == NULL)
		return false;


	//**************PATH AND LOADING SONG SOUND**********//
		// Char pointer for reading text
		const char* txtAttrb = NULL;
		txtAttrb = pRoot->Attribute("file");

		char buffer[128] = {0};
		strcpy_s(buffer,_countof(buffer),txtAttrb);

		string szSongPath = "resource/Sound/";
		szSongPath += buffer;

		// Loading song and setting SongID.
		theSong->SetSongID(CSGD_FModManager::GetInstance()->LoadSound(szSongPath.c_str()));
		
	//*************SETTING SONG NAME********************//
		const char* txtName = NULL;
		txtName = pRoot->Attribute("name");

		if(txtName == NULL)
			return false;

		strcpy_s(buffer,_countof(buffer),txtName);

		theSong->SetSongName(buffer);

	//*************SETTING DURATION********************//
		if(pRoot->Attribute("duration") == NULL)
			return false;

		int nDuration;

		pRoot->Attribute("duration",&nDuration);

		theSong->SetSongDuration(nDuration);

//***************END OF SONG SPECIFICS********************//

		//*********SETTING ICON PATHS*********************//
		TiXmlElement* pIcons = pRoot->FirstChildElement("icons");

		string szIconPath = "resource/graphics/";
		string szWKey;
		string szAKey;
		string szSKey;
		string szDKey;

		//******WKEY*********//
		const char* txtIcon = NULL;
		txtIcon = pIcons->Attribute("wkey");

		if(txtIcon == NULL)
			return false;

		strcpy_s(buffer,_countof(buffer),txtIcon);

		szWKey = szIconPath + buffer;

		//******AKEY*********//
		txtIcon = NULL;
		txtIcon = pIcons->Attribute("akey");

		if(txtIcon == NULL)
			return false;

		strcpy_s(buffer,_countof(buffer),txtIcon);

		szAKey = szIconPath + buffer;

		//******SKEY*********//
		txtIcon = NULL;
		txtIcon = pIcons->Attribute("skey");

		if(txtIcon == NULL)
			return false;

		strcpy_s(buffer,_countof(buffer),txtIcon);

		szSKey = szIconPath + buffer;
		
		//******DKEY*********//
		txtIcon = NULL;
		txtIcon = pIcons->Attribute("dkey");

		if(txtIcon == NULL)
			return false;

		strcpy_s(buffer,_countof(buffer),txtIcon);

		szDKey = szIconPath + buffer;

		//*********LOADING NOTE GRAPHICS*************//
		// Pre-loading textures for notes
		int nImpNote = CSGD_TextureManager::GetInstance()->LoadTexture(szAKey.c_str());
		int nRepNote = CSGD_TextureManager::GetInstance()->LoadTexture(szDKey.c_str());
		int nSkullNote = CSGD_TextureManager::GetInstance()->LoadTexture(szWKey.c_str());
		int nSunNote = CSGD_TextureManager::GetInstance()->LoadTexture(szSKey.c_str());

//***************BEGIN BEAT SETTING***********************//
		TiXmlElement* pBeat = pRoot->FirstChildElement("Beat");

		

		while(pBeat)
		{
			CBeat theBeat;

			//****************CHECKING FOR NOTE COMPLETENESS**********//
			if(pBeat->Attribute("beatis") == NULL)
				return false;

			int nBeatIs = 0;

			pBeat->Attribute("beatis",&nBeatIs);

			// Ignoring non-complete note sets
			if(nBeatIs != 0)
			{
				pBeat = pBeat->NextSiblingElement("Beat");
				continue;
			}
			

			//****************GETTING BEAT HIT TIME***************//
			if(pBeat->Attribute("timeofbeat") == NULL)
				return false;

			int nBeatTime = 0;

			pBeat->Attribute("timeofbeat",&nBeatTime);

			theBeat.SetTimeOfBeat(nBeatTime);

			//********HEIGHT AND WIDTH ARENT IN ORDER W/ XML FILE B/C I NEED THEM FOR POSITION*******//
			//******************SETTING NOTE WIDTH***************//
			if(pBeat->Attribute("width") == NULL)
				return false;

			int nWidth = 0;

			pBeat->Attribute("width",&nWidth);

			theBeat.SetWidth(nWidth);

			//******************SETTING NOTE HEIGHT**************//
			if(pBeat->Attribute("height") == NULL)
				return false;

			int nHeight = 0;

			pBeat->Attribute("height",&nHeight);

			theBeat.SetHeight(nHeight);

			//*****************GETTING DIRECTION OF NOTE**********//
			if(pBeat->Attribute("direction") == NULL)
				return false;

			const char* txtDirection = NULL;

			txtDirection = pBeat->Attribute("direction");
			strcpy_s(buffer,_countof(buffer),txtDirection);

			string szDirection = buffer;

			if(szDirection == "left")
				theBeat.SetDirection(LEFT);
			else if(szDirection == "up")
				theBeat.SetDirection(UP);
			else if(szDirection == "right")
				theBeat.SetDirection(RIGHT);
			else if(szDirection == "down")
				theBeat.SetDirection(DOWN);
			else if(szDirection == "leftup")
				theBeat.SetDirection(LEFTUP);
			else if(szDirection == "rightup")
				theBeat.SetDirection(RIGHTUP);
			else if(szDirection == "rightdown")
				theBeat.SetDirection(RIGHTDOWN);
			else if(szDirection == "leftdown")
				theBeat.SetDirection(LEFTDOWN);

			//******************GETTING KEYPRESS OF NOTE***********//
			if(pBeat->Attribute("key") == NULL)
				return false;

			const char* txtKey = NULL;

			txtKey = pBeat->Attribute("key");

			theBeat.SetKeyToPress(*txtKey);

			//******************SETTING IMAGE OF NOTE*************//
		
			if(*txtKey == 'w')
				theBeat.SetImageID(nSkullNote);
			else if(*txtKey == 'd')
				theBeat.SetImageID(nRepNote);
			else if(*txtKey == 'a')
				theBeat.SetImageID(nImpNote);
			else if(*txtKey == 's')
				theBeat.SetImageID(nSunNote);

			//*****************SETTING NOTE DIFFICULTY************//
			if(pBeat->Attribute("difficulty") == NULL)
				return false;

			const char* txtDifficulty = NULL;
			txtDifficulty = pBeat->Attribute("difficulty");
			strcpy_s(buffer,_countof(buffer),txtDifficulty);

			string szDiff = buffer;

			if(szDiff == "easy")
				theBeat.SetDifficulty(EASY);
			else if(szDiff == "normal")
				theBeat.SetDifficulty(NORMAL);
			else if(szDiff == "hard")
				theBeat.SetDifficulty(HARD);

			

			//******************GETTING NOTE EVENT***************//
			if(pBeat->Attribute("event") == NULL)
				return false;

			const char* txtEvent = NULL;
			txtEvent = pBeat->Attribute("event");
			strcpy_s(buffer,_countof(buffer),txtEvent);

			string szEvent = buffer;

			theBeat.SetEvent(szEvent);

			//******************MOVING ON TO NEXT NOTE***********//
			
			// Adding beat to Song List
			theSong->GetBeatList().push_back(theBeat);
			pBeat = pBeat->NextSiblingElement("Beat");
		}

		//***************ADDING SONG TO SONG LIST****************//
		// Releasing initial ref to song

		// Adding song to Object Manager
			CObjectManager::GetInstance()->AddObject(theSong);

			// Releasing initial ref to the song
			theSong->Release();

			// Adding song pointer to song list (just to keep track of what songs we have)
			GetSongList().push_back(theSong);			
		
	return true;
}

bool CBeatManager::UnloadSongs()
{
	CObjectManager::GetInstance()->RemoveAllObjects();

	//for(unsigned int i = 0; i < GetSongList().size(); ++i)
	//	m_vSongs[i]->Release();
	
	GetSongList().clear();
	GetSongBackground().clear();
	SetNumberNotesHit(0);

	SetCurrentlyPlayingSong("none");

	if(GetSongList().size() == 0 && GetSongBackground().size() == 0)
		return true;
	else
		return false;
}

void CBeatManager::Play(string szSongName)
{
	SetPause(false);

	SetCurrentlyPlayingSong(szSongName);

	// If song was already playing when play was hit, unpause
	if(GetCurrentlyPlayingSongIndex() != -1)
		if(FMODMAN->IsSoundPlaying(GetCurrentlyPlayingSong()->GetSongID()))
		{
			FMOD::Channel* derp = FMODMAN->GetLatestChannel(GetCurrentlyPlayingSong()->GetSongID());
			derp->setPaused(IsPaused());
		}
		else
			FMODMAN->PlaySound(GetCurrentlyPlayingSong()->GetSongID());
}

void CBeatManager::Pause()
{
	// Toggleing pause
	SetPause(!IsPaused());

	// Setting channel to paused
	if(FMODMAN->IsSoundPlaying(GetCurrentlyPlayingSong()->GetSongID()))
	{
		FMOD::Channel* derp = FMODMAN->GetLatestChannel(GetCurrentlyPlayingSong()->GetSongID());
				derp->setPaused(IsPaused());
	}
}

void CBeatManager::Stop()
{
	SetPause(true);

	
	// Stopping currently playing song
	if(m_nCurrentlyPlayingSongIndex != -1)
	{
		if(FMODMAN->IsSoundPlaying(GetCurrentlyPlayingSong()->GetSongID()))
		{
			FMODMAN->StopSound(GetCurrentlyPlayingSong()->GetSongID());
		}
	
		m_vSongs[m_nCurrentlyPlayingSongIndex]->ResetSong();
	}
}

void CBeatManager::Reset()
{
	Stop();
	Play();
}

void CBeatManager::Update()
{
	// Checking combo
	EvaluatePlayerCombos();
}

void CBeatManager::Render()
{	
	
}

void CBeatManager::HandleEvent( CEvent* pEvent )
{
	if(pEvent->GetEventID() == "test.event")
	{
		fuckyou = true;
		return;
	}

	if(pEvent->GetEventID() == "notepassed")
	{
		// Upping the note count of notes that have passed by
		SetNotesPassed(GetNotesPassed() + 1);
		return;
	}

	
}

string CBeatManager::GetCurrentlyPlayingSongName()
{
	return szCurrentlyPlayingSong;
}

void CBeatManager::SetCurrentlyPlayingSong(string szSongName)
{
	if(szSongName == "none")
	{
		m_nCurrentlyPlayingSongIndex = -1;
		return;
	}
	else if(szSongName == "")
	{
		return;
	}
	else
	{
		for(unsigned int i = 0; i < m_vSongs.size(); ++i)
			if(m_vSongs[i]->GetSongName() == szSongName)
			{
				m_nCurrentlyPlayingSongIndex = i;
				szCurrentlyPlayingSong = szSongName;
				m_vSongs[i]->SetCurrentlyPlayingSong(true);
				return;
			}
	}
	return;
}

void CBeatManager::CheckPlayerInput(CPlayer* aPlayer)
{
	if(aPlayer->GetPlayerHitQueue().size() > 0 && GetCurrentlyPlayingSong()->GetHittableBeatList().size() > 0)
	{
		for(unsigned int i = 0; i < GetCurrentlyPlayingSong()->GetHittableBeatList().size(); ++i)
		{			
			// Here we're looking at the current hittable beat, checking if the player is aiming at it,
			// and if they hit the correct key or not
			if(aPlayer->GetAimingDirection() == (GetCurrentlyPlayingSong()->GetHittableBeatList())[i]->GetDirection()
				&& aPlayer->GetMostRecentKeyPress().cHitNote == (GetCurrentlyPlayingSong()->GetHittableBeatList())[i]->GetKeyToPress())
			{			
				switch(aPlayer->GetType())
				{
				case OBJ_PLAYER1:
					{					
						if(! (GetCurrentlyPlayingSong()->GetHittableBeatList())[i]->GetPlayer1Hit())
						{
							(GetCurrentlyPlayingSong()->GetHittableBeatList())[i]->SetPlayer1Hit(true);
							// Player hit the note, handling all relevant info.
							aPlayer->SetCurrentStreak(aPlayer->GetCurrentStreak() + 1);
							aPlayer->SetCurrentScore(aPlayer->GetCurrentScore() + 1);
							CFXManager::GetInstance()->QueueParticle("P1_HIT");

							// Upping Player1's Current combo for damage
							SetP1CurrentCombo(GetP1CurrentCombo() + 1);							
						}						

					}
					break;
				
				case OBJ_PLAYER2:
					{
						if(! (GetCurrentlyPlayingSong()->GetHittableBeatList())[i]->GetPlayer2Hit())
						{
							 (GetCurrentlyPlayingSong()->GetHittableBeatList())[i]->SetPlayer2Hit(true);
							// Player hit the note, handling all relevant info.
							aPlayer->SetCurrentStreak(aPlayer->GetCurrentStreak() + 1);
							aPlayer->SetCurrentScore(aPlayer->GetCurrentScore() + 1);	
							CFXManager::GetInstance()->QueueParticle("P2_HIT");

							// Upping Player2's Current combo for damage
							SetP2CurrentCombo(GetP2CurrentCombo() + 1);
						}
						
					}
					break;

				case OBJ_AI:
					{
						if(! (GetCurrentlyPlayingSong()->GetHittableBeatList())[i]->GetPlayer2Hit())
						{
							 (GetCurrentlyPlayingSong()->GetHittableBeatList())[i]->SetPlayer2Hit(true);
							// Player hit the note, handling all relevant info.
							aPlayer->SetCurrentStreak(aPlayer->GetCurrentStreak() + 1);
							aPlayer->SetCurrentScore(aPlayer->GetCurrentScore() + 1);	
							CFXManager::GetInstance()->QueueParticle("P2_HIT");
							
							// Upping Player2's Current combo for damage
							SetP2CurrentCombo(GetP2CurrentCombo() + 1);
						}
						
					}
					break;
				}
			}
			else
			{
				if(aPlayer->GetType() == OBJ_PLAYER1)
					SetP1CurrentCombo(0);
				else if(aPlayer->GetType() == OBJ_PLAYER2 || aPlayer->GetType() == OBJ_AI)
					SetP2CurrentCombo(0);

				aPlayer->SetCurrentStreak(0);			
			}
			
		}

		// Clearing last keypress
			aPlayer->GetPlayerHitQueue().pop();
	}

}

CBeatManager* CBeatManager::GetInstance()
{
	// Lazy instantiation
	static CBeatManager instance; // Static allows passing back of address
	return &instance;	
}

////////////////////////////////////////
//		PRIVATE UTILITY FUNCTIONS
////////////////////////////////////////
void CBeatManager::EvaluatePlayerCombos()
{
	if(GetNotesPassed() >= GetComboThreshold())
	{
		if(GetP1CurrentCombo() > GetP2CurrentCombo())
		{
			DealDamageToPlayer(CLevelManager::GetInstance()->GetPlayer(PlayerTwo), CLevelManager::GetInstance()->GetPlayer(PlayerOne));
		}
		else if(GetP2CurrentCombo() > GetP1CurrentCombo())
		{
			DealDamageToPlayer(CLevelManager::GetInstance()->GetPlayer(PlayerOne), CLevelManager::GetInstance()->GetPlayer(PlayerTwo));
		}

		SetNotesPassed(0);
	}
}

void CBeatManager::DealDamageToPlayer(CPlayer* playerToDmg, CPlayer* damageDealer)
{
	// Player is in attack mode
	if(playerToDmg->GetAttackMode())
	{
		if(damageDealer->GetAttackMode())
			playerToDmg->SetCurrentHP(playerToDmg->GetCurrentHP() - 8); // Attacking player is in attack and so is defender = full damage
		else
			playerToDmg->SetCurrentHP(playerToDmg->GetCurrentHP() - 4); // Attacking player is in defense mode so defender = half damage
	}
	else
	{
		if(damageDealer->GetAttackMode())
			playerToDmg->SetCurrentHP(playerToDmg->GetCurrentHP() - 4); // Atking player is in attack and defender is in defence = half damage
		else
			playerToDmg->SetCurrentHP(playerToDmg->GetCurrentHP() - 2); // Atking player is in defensive mode and so is defender = quarter damage
	}
}
////////////////////////////////////////
//	    PUBLIC ACCESSORS / MUTATORS
////////////////////////////////////////

////////////////////////////////////////
//	    PRIVATE ACCESSORS / MUTATORS
////////////////////////////////////////



