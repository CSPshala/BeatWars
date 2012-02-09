///////////////////////////////////////////////////////
// File Name	:	"CSong.cpp"
//
// Purpose		:	To contain all notes in a song
//////////////////////////////////////////////////////

#include "CSong.h"

CSong::CSong()
{
	m_fCurrentSongTime = 0.0f;
	
	// Asset IDs
	m_nSoundID = -1;
	m_nImageID = -1;
}

CSong::~CSong()
{

}