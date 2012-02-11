///////////////////////////////////////////////////////
// File Name	:	"CBeatManager.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include "CBeatManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
//#include "CObjectFactory.h"
#include "../XML/tinystr.h"
#include "../XML/tinyxml.h"

////////////////////////////////////////
//				MISC
////////////////////////////////////////

///////////////////////////////////////////////
//  CONSTRUCTOR / DECONSTRUCT / OP OVERLOADS
///////////////////////////////////////////////
CBeatManager::CBeatManager()
{
	SetNumberNotesHit(0);
}

CBeatManager::~CBeatManager()
{

}

////////////////////////////////////////
//		PUBLIC UTILITY FUNCTIONS
////////////////////////////////////////
bool CBeatManager::LoadSong(string szFileName)
{
	TiXmlDocument doc;

	// Song for new song creation
	CSong theSong;

	// Adding path to filename for beatlists
	string szPath = "resource/beatlist/";
	szPath += szFileName;

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
		theSong.SetSongID(CSGD_XAudio2::GetInstance()->MusicLoadSong(szSongPath.c_str()));
		
	//*************SETTING SONG NAME********************//
		const char* txtName = NULL;
		txtName = pRoot->Attribute("name");

		if(txtName == NULL)
			return false;

		strcpy_s(buffer,_countof(buffer),txtName);

		theSong.SetSongName(buffer);

	//*************SETTING DURATION********************//
		if(pRoot->Attribute("duration") == NULL)
			return false;

		double dDuration = 0.0;

		pRoot->Attribute("duration",&dDuration);

		theSong.SetSongDuration((float)dDuration);

//***************END OF SONG SPECIFICS********************//
		

//***************BEGIN BEAT SETTING***********************//
		TiXmlElement* pBeat = pRoot->FirstChildElement("Beat");

		// Pre-loading textures for notes
		int nImpNote = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/ImpNote.png");
		int nRepNote = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/RepNote.png");
		int nSkullNote = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/SkullNote.png");
		int nSunNote = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/SunNote.png");

		while(pBeat)
		{
			CBeat theBeat;

			//****************GETTING BEAT HIT TIME***************//
			if(pBeat->Attribute("timeofbeat") == NULL)
				return false;

			double dBeatTime = 0.0;

			pBeat->Attribute("timeofbeat",&dBeatTime);

			theBeat.SetTimeOfBeat((float)dBeatTime);

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
			if(pBeat->Attribute("image") == NULL)
				return false;

			const char* txtImage = NULL;
			txtImage = pBeat->Attribute("image");
			strcpy_s(buffer,_countof(buffer),txtImage);

			string szImage = buffer;

			if(szImage == "ImpNote.png")
				theBeat.SetImageID(nImpNote);
			else if(szImage == "RepNote.png")
				theBeat.SetImageID(nRepNote);
			else if(szImage == "SkullNote.png")
				theBeat.SetImageID(nSkullNote);
			else if(szImage == "SunNote.png")
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

			//******************MOVING ON TO NEXT NOTE***********//
				// Adding beat to song
			theSong.GetBeatList().push_back(theBeat);

			pBeat = pBeat->NextSiblingElement("Beat");
		}

		//***************ADDING SONG TO SONG LIST****************//
		GetSongList().push_back(theSong);

	return true;
}

bool CBeatManager::UnloadSongs()
{
	GetSongList().clear();
	GetSongBackground().clear();
	SetNumberNotesHit(0);

	if(GetSongList().size() == 0 && GetSongBackground().size() == 0)
		return true;
	else
		return false;
}

void CBeatManager::Play()
{

}

void CBeatManager::Pause()
{

}

void CBeatManager::Stop()
{

}

void CBeatManager::Reset()
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



