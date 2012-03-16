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
#include <fstream>
using std::ifstream;
using std::ios_base;

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
	m_nSFX = CSGD_FModManager::GetInstance()->LoadSound("resource/light_saber.wav");
	m_nDamageSFX = CSGD_FModManager::GetInstance()->LoadSound("resource/SGD_hurt.wav");
	SetFXSound(m_nSFX);
	SetSoundFX(m_nDamageSFX);
}

CBeatManager::~CBeatManager()
{
	CEventSystem::GetInstance()->UnregisterClient("test.event",this);
	ES->UnregisterClient("notepassed",this);
}

////////////////////////////////////////
//		PUBLIC UTILITY FUNCTIONS
////////////////////////////////////////
bool CBeatManager::LoadSongXML(string szFileName)
{
	TiXmlDocument doc;

	// Song for new song creation
	CSong* theSong = new CSong();

	// Adding path to filename for beat lists
	string szPath = "resource/beatlist/";
	szPath += szFileName;
	theSong->SetCurrentPlayingSongFileName(szFileName);
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
	theSong->SetSongID(CSGD_FModManager::GetInstance()->LoadSound(szSongPath.c_str(),FMOD_CREATESTREAM));

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

bool CBeatManager::LoadSongBIN(string szFileName)
{
	// Adding path to filename for loading purposes
	string szPath = "resource/beatlist/";
	szPath += szFileName;
	

	ifstream ifs(szPath.c_str(),ios_base::in | ios_base::binary);
	if(ifs.is_open())
	{
		int nLength;


		// Song for new song creation
		CSong* theSong = new CSong();

		// Setting filename in Song
		theSong->SetCurrentPlayingSongFileName(szFileName);

		//**************FILENAME*********************//
			// Length of string		
			ifs.read((char*)&nLength,4);
			
			// Temp char array to hold filename
			char* nFileName = new char[nLength + 1];
			nFileName[nLength] = '\0';

			// Reading filename then copying to string vector
			ifs.read(nFileName,nLength);

			// Adding song path
			string szSongPath = "resource/Sound/";
			szSongPath += nFileName;

			// Cleanup
			 
			delete[] nFileName;

			// Loading song
			theSong->SetSongID(CSGD_FModManager::GetInstance()->LoadSound(szSongPath.c_str(),FMOD_CREATESTREAM));

		//***************SONG NAME*******************//
			// Length of string
			int nLen;
			ifs.read((char*)&nLen,4);
			
			// Temp char array to hold filename
			nFileName = new char[nLen + 1];
			nFileName[nLen] = '\0';

			// Reading filename then copying to string vector
			ifs.read(nFileName,nLen);

			string sName = nFileName;

			// Setting Song Name
			theSong->SetSongName(sName);
			
			// Cleanup
			//delete nLen;
			delete[] nFileName;

		//************DURATION***********************//
			int* duration = new int;

			ifs.read((char*)duration,4);

			theSong->SetSongDuration(*duration);

			delete duration;

		//************ICON FILE NAMES****************//

			// Path and icon filename strings
			string szIconPath = "resource/graphics/";
			string szWKey;
			string szAKey;
			string szSKey;
			string szDKey;

			//**********WKEY********//
			// Length of string
			 
			ifs.read((char*)&nLength,4);
			
			// Temp char array to hold filename
			nFileName = new char[nLength + 1];
			nFileName[nLength] = '\0';

			// Reading filename then copying to string vector
			ifs.read(nFileName,nLength);
					
			// W Key path
			szWKey = szIconPath + nFileName;

			// Cleanup
			 
			delete[] nFileName;

			//**********AKEY********//
			// Length of string
			 
			ifs.read((char*)&nLength,4);
			
			// Temp char array to hold filename
			nFileName = new char[nLength + 1];
			nFileName[nLength] = '\0';

			// Reading filename then copying to string vector
			ifs.read(nFileName,nLength);
					
			// A Key path
			szAKey = szIconPath + nFileName;

			// Cleanup
			 
			delete[] nFileName;

			//**********SKEY********//
			// Length of string
			 
			ifs.read((char*)&nLength,4);
			
			// Temp char array to hold filename
			nFileName = new char[nLength + 1];
			nFileName[nLength] = '\0';

			// Reading filename then copying to string vector
			ifs.read(nFileName,nLength);
					
			// S Key path
			szSKey = szIconPath + nFileName;
			
			// Cleanup
			 
			delete[] nFileName;


			//**********DKEY********//
			// Length of string
			 
			ifs.read((char*)&nLength,4);
			
			// Temp char array to hold filename
			nFileName = new char[nLength + 1];
			nFileName[nLength] = '\0';

			// Reading filename then copying to string vector
			ifs.read(nFileName,nLength);
					
			// D Key path
			szDKey = szIconPath + nFileName;		
			
			// Cleanup
			 
			delete[] nFileName;


		//*********LOADING NOTE GRAPHICS*************//
		// Pre-loading textures for notes
		int nImpNote = CSGD_TextureManager::GetInstance()->LoadTexture(szAKey.c_str());
		int nRepNote = CSGD_TextureManager::GetInstance()->LoadTexture(szDKey.c_str());
		int nSkullNote = CSGD_TextureManager::GetInstance()->LoadTexture(szWKey.c_str());
		int nSunNote = CSGD_TextureManager::GetInstance()->LoadTexture(szSKey.c_str());


		//*********BEAT LOADING**********************//

			// Getting number of beats
			int nCount;

			ifs.read((char*)&nCount,4);

			for(int i = 0; i < nCount; ++i)
			{
				CBeat theBeat;

				// Beat time
				int nTime;
				ifs.read((char*)&nTime,4);
				theBeat.SetTimeOfBeat(nTime);
				
				//*************DIRECTION*************//
					// Length of string
					 
					ifs.read((char*)&nLength,4);
			
					// Temp char array to hold filename
					nFileName = new char[nLength + 1];
					nFileName[nLength] = '\0';

					// Reading filename then copying to string vector
					ifs.read(nFileName,nLength);

					// Putting direction into a non C string
					string szDirection = nFileName;

					// Setting direction in beat using ENUM
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
					
					// Cleanup
					 
					delete[] nFileName;

				//***********KEY***************//
					char nKey;
					ifs.read(&nKey,1);

					theBeat.SetKeyToPress(nKey);					

					//******************SETTING IMAGE OF NOTE*************//

					if(nKey == 'w')
						theBeat.SetImageID(nSkullNote);
					else if(nKey == 'd')
						theBeat.SetImageID(nRepNote);
					else if(nKey == 'a')
						theBeat.SetImageID(nImpNote);
					else if(nKey == 's')
						theBeat.SetImageID(nSunNote);

			    //***********DIFFICULTY********************//
					// Length of string
					 
					ifs.read((char*)&nLength,4);
			
					// Temp char array to hold filename
					nFileName = new char[nLength + 1];
					nFileName[nLength] = '\0';

					// Reading filename then copying to string vector
					ifs.read(nFileName,nLength);

					string szDiff = nFileName;

					// Setting beat difficulty
					if(szDiff == "easy")
						theBeat.SetDifficulty(EASY);
					else if(szDiff == "normal")
						theBeat.SetDifficulty(NORMAL);
					else if(szDiff == "hard")
						theBeat.SetDifficulty(HARD);
								
					// Cleanup
					 
					delete[] nFileName;

				//************WIDTH***********************//
					int nWidth;
					ifs.read((char*)&nWidth,4);
					theBeat.SetWidth(nWidth);					

				//************Height***********************//
					int nHeight;
					ifs.read((char*)&nHeight,4);
					theBeat.SetHeight(nHeight);					

				//************BEAT COMPLETION**************//
					// Tossing notes
					int nComplete;
					ifs.read((char*)&nComplete,4);			
					

				//***************NOTE EVENT*******************//
					// Length of string
					 
					ifs.read((char*)&nLength,4);
			
					// Temp char array to hold filename
					nFileName = new char[nLength + 1];
					nFileName[nLength] = '\0';

					// Reading filename then copying to string vector
					ifs.read(nFileName,nLength);
					
					string sEvent = nFileName;

					theBeat.SetEvent(sEvent);
			
					// Cleanup					 
					delete[] nFileName;

					// Bouncing out if note isn't complete
					if(nComplete != 0)					
						continue;	

				//******************MOVING ON TO NEXT NOTE***********//
					// Adding beat to Song List
					theSong->GetBeatList().push_back(theBeat);
			}			

			// Adding to OM
			CObjectManager::GetInstance()->AddObject(theSong);

			theSong->Release();

			// Adding song to song list
			GetSongList().push_back(theSong);

			return true;

	}

	return false;
}

void CBeatManager::DelayLoadSongBIN(string szFileName)
{
	m_qDelayLoad.push(szFileName);	
}

void CBeatManager::LoadDelayedSongNow()
{
	if(!m_qDelayLoad.empty())
	{

	}
}

bool CBeatManager::UnloadSongs()
{
	CObjectManager::GetInstance()->RemoveAllObjects();

	//for(unsigned int i = 0; i < GetSongList().size(); ++i)
	//	m_vSongs[i]->Release();

	GetSongList().clear();
	GetSongBackground().clear();
	SetNumberNotesHit(0);

	for(unsigned int i = 0; i < m_qDelayLoad.size(); ++i)
		m_qDelayLoad.pop();

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
	if(aPlayer->GetMostRecentKeyPress() != 'g' && GetCurrentlyPlayingSong()->GetHittableBeatList().size() > 0)
	{
			if(aPlayer->GetMostRecentKeyPress() == 'q')
			{
				if( aPlayer->GetCurrentHP() != aPlayer->GetMaxHP() )
				{
					aPlayer->SetCurrentHP( aPlayer->GetCurrentHP() + 10);

					if( aPlayer->GetCurrentHP() > aPlayer->GetMaxHP() )
						aPlayer->SetCurrentHP(aPlayer->GetMaxHP());

					aPlayer->SetCurrentPowerup(0);
				}
			}
			else
			{
				// Hittable list iterator
				list<CBeat*>::iterator iter = GetCurrentlyPlayingSong()->GetHittableBeatList().begin();

				//for(; iter != GetCurrentlyPlayingSong()->GetHittableBeatList().end(); ++iter)
				//{					

					// Only checking the first note that hasn't been hit by that player
					if((aPlayer->GetType() == OBJ_PLAYER1 && !(*iter)->GetPlayer1Hit()) ||
						(aPlayer->GetType() == OBJ_PLAYER2 && !(*iter)->GetPlayer2Hit()))
					{

						// Here we're looking at the current hittable beat, checking if the player is aiming at it,
						// and if they hit the correct key or not
						if(aPlayer->GetAimingDirection() == (*iter)->GetDirection()
							&& aPlayer->GetMostRecentKeyPress() == (*iter)->GetKeyToPress())
						{			
							switch(aPlayer->GetType())
							{
							case OBJ_PLAYER1:
								{					
									if(!(*iter)->GetPlayer1Hit())
									{
										(*iter)->SetPlayer1Hit(true);
										// Player hit the note, handling all relevant info.
										aPlayer->SetCurrentStreak(aPlayer->GetCurrentStreak() + 1);
										aPlayer->SetCurrentScore(aPlayer->GetCurrentScore() + 1);
										CFXManager::GetInstance()->QueueParticle("P1_HIT");

										if (aPlayer->GetCurrentStreak() >= 25)
										{
											aPlayer->SetCurrentScore(aPlayer->GetCurrentScore() + (aPlayer->GetCurrentStreak()/5)); 
										}
										else if (aPlayer->GetCurrentStreak() >= 100)
										{
											aPlayer->SetCurrentScore(aPlayer->GetCurrentScore() + (aPlayer->GetCurrentStreak()/5));
										}
										else if (aPlayer->GetCurrentStreak() >= 200)
										{
											aPlayer->SetCurrentScore(aPlayer->GetCurrentScore() + (aPlayer->GetCurrentStreak()/5));
										}
										else
											aPlayer->SetCurrentScore(aPlayer->GetCurrentScore() + 1);

										// Upping Player1's Current combo for damage
										SetP1CurrentCombo(GetP1CurrentCombo() + 1);
										SetNumberNotesHit(GetNumberNotesHit() + 1);
									}						
									aPlayer->SetMostRecentKeyPress('g');
								}
								break;

							case OBJ_PLAYER2:
								{
									if(!(*iter)->GetPlayer2Hit())
									{
										(*iter)->SetPlayer2Hit(true);
										// Player hit the note, handling all relevant info.
										aPlayer->SetCurrentStreak(aPlayer->GetCurrentStreak() + 1);
										aPlayer->SetCurrentScore(aPlayer->GetCurrentScore() + 1);	
										CFXManager::GetInstance()->QueueParticle("P2_HIT");

										// Upping Player2's Current combo for damage
										SetP2CurrentCombo(GetP2CurrentCombo() + 1);
										SetNumberNotesHit(GetNumberNotesHit() + 1);

										aPlayer->SetMostRecentKeyPress('g');
									}

								}
								break;					
							}							
						}
						else
						{
							if(aPlayer->GetType() == OBJ_PLAYER1)
								SetP1CurrentCombo(0);
							else if(aPlayer->GetType() == OBJ_PLAYER2 )
								SetP2CurrentCombo(0);

							aPlayer->SetCurrentStreak(0);
						}

						//break; // leaving the loop
					}

				//}
			}
				
	}



	if(aPlayer->GetType() == OBJ_AI)
	{
		// Hittable list iterator
		list<CBeat*>::iterator iter = GetCurrentlyPlayingSong()->GetHittableBeatList().begin();

		for(; iter != GetCurrentlyPlayingSong()->GetHittableBeatList().end(); ++iter)
		{  
			bool found = false;

			// Checking AI beat list to make sure we haven't hit it before
			for(unsigned int x = 0; x < aPlayer->GetAIBeats().size(); ++x)
			{
				if((aPlayer->GetAIBeats())[x] == (*iter))
					found = true;
			}

			if(!found)
				if((*iter)->GetPlayer2Hit())
				{				 
					// AI hit the note, handling all relevant info.
					aPlayer->SetCurrentStreak(aPlayer->GetCurrentStreak() + 1);
					aPlayer->SetCurrentScore(aPlayer->GetCurrentScore() + 1);	
					CFXManager::GetInstance()->QueueParticle("P2_HIT");

					// Upping Player2's Current combo for damage
					SetP2CurrentCombo(GetP2CurrentCombo() + 1);

					(aPlayer->GetAIBeats()).push_back(*iter);
				}			
				else
					aPlayer->SetCurrentStreak(0);
		}
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
	static bool P1Power = false, P2Power = false;

	if(GetNotesPassed() >= GetComboThreshold())
	{
		if(GetP1CurrentCombo() > GetP2CurrentCombo())
		{
			CLevelManager::GetInstance()->GetPlayer(PlayerOne)->SetCurrentPowerup( CLevelManager::GetInstance()->GetPlayer(PlayerOne)->GetCurrentPowerup() + 20);
			DealDamageToPlayer(CLevelManager::GetInstance()->GetPlayer(PlayerTwo), CLevelManager::GetInstance()->GetPlayer(PlayerOne));
		}
		else if(GetP2CurrentCombo() > GetP1CurrentCombo())
		{
			CLevelManager::GetInstance()->GetPlayer(PlayerTwo)->SetCurrentPowerup( CLevelManager::GetInstance()->GetPlayer(PlayerTwo)->GetCurrentPowerup() + 20);
			DealDamageToPlayer(CLevelManager::GetInstance()->GetPlayer(PlayerOne), CLevelManager::GetInstance()->GetPlayer(PlayerTwo));
		}

		SetP1CurrentCombo(0);
		SetP2CurrentCombo(0);
		SetNotesPassed(0);
	}

	int x = CLevelManager::GetInstance()->GetPlayer(PlayerOne)->GetCurrentPowerup();

	if(x >= 140 && P1Power == false)
	{
		CFXManager::GetInstance()->QueueParticle("P1_PBAR");
		P1Power = true;
	}
	else if(CLevelManager::GetInstance()->GetPlayer(PlayerOne)->GetCurrentPowerup() < 140 && P1Power == true)
	{
		CFXManager::GetInstance()->DequeueParticle("P1_BAR");
		P1Power = false;
	}

	if(CLevelManager::GetInstance()->GetPlayer(PlayerTwo)->GetCurrentPowerup() >= 140 && P2Power == false)
	{
		CFXManager::GetInstance()->QueueParticle("P2_PBAR");
		P2Power = true;
	}
	else if(CLevelManager::GetInstance()->GetPlayer(PlayerTwo)->GetCurrentPowerup() < 140 && P2Power == true)
	{
		CFXManager::GetInstance()->DequeueParticle("P2_BAR");
		P2Power = false;
	}
}

void CBeatManager::DealDamageToPlayer(CPlayer* playerToDmg, CPlayer* damageDealer)
{
	// Player is in attack mode
	if(playerToDmg->GetAttackMode())
	{
		if(damageDealer->GetAttackMode())
		{
			playerToDmg->SetCurrentHP(playerToDmg->GetCurrentHP() - 8); // Attacking player is in attack and so is defender = full damage
			playerToDmg->SetCurrAnimation("High Block");
			damageDealer->SetCurrAnimation("High Hit");

			CSGD_FModManager::GetInstance()->PlaySound(m_nDamageSFX);
			//damageDealer->SetCurrAnimation("High Hit");
		}
		else
		{
			playerToDmg->SetCurrentHP(playerToDmg->GetCurrentHP() - 4); // Attacking player is in defense mode so defender = half damage
			playerToDmg->SetCurrAnimation("Low Block");
			damageDealer->SetCurrAnimation("Low Hit");
			CSGD_FModManager::GetInstance()->PlaySound(m_nDamageSFX);
		}
	}
	else
	{
		if(damageDealer->GetAttackMode())
		{
			playerToDmg->SetCurrentHP(playerToDmg->GetCurrentHP() - 4); // Atking player is in attack and defender is in defence = half damage
			playerToDmg->SetCurrAnimation("High Block");
			damageDealer->SetCurrAnimation("High Hit");
			CSGD_FModManager::GetInstance()->PlaySoundA(m_nDamageSFX);
			//damageDealer->SetCurrAnimation("Low Hit");
		}
		else
		{
			playerToDmg->SetCurrentHP(playerToDmg->GetCurrentHP() - 2); // Atking player is in defensive mode and so is defender = quarter damage
			playerToDmg->SetCurrAnimation("Low Block");
			damageDealer->SetCurrAnimation("Low Hit");
			CSGD_FModManager::GetInstance()->PlaySound(m_nSFX);
		}
	}
}
////////////////////////////////////////
//	    PUBLIC ACCESSORS / MUTATORS
////////////////////////////////////////

////////////////////////////////////////
//	    PRIVATE ACCESSORS / MUTATORS
////////////////////////////////////////



