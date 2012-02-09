///////////////////////////////////////////////////////
// File Name	:	"CSong.h"
//
// Purpose		:	To contain all notes in a song
//////////////////////////////////////////////////////

#ifndef C_SONG_H
#define C_SONG_H

#include <list>
#include <string>
using std::list;
using std::string;

#include "CBeat.h"

class CSong
{
public:
	CSong();
	~CSong();

private:

	//******MEMBERS*********//
	list<CBeat> m_listBeats;
	string m_szName;
	float m_fCurrentSongTime;

	// Asset IDs
	int m_nSoundID;
	int m_nImageID;

};

#endif