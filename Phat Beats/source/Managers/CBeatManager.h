///////////////////////////////////////////////////////
// File Name	:	"CBeatManager.h"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////
#ifndef C_BEATMANAGER_H
#define C_BEATMANAGER_H


#include <string>
#include <list>
using std::list;
using std::string;

#include "../CSong.h"

class CBeatManager
{
public:
	// Construct / Decontruct
	CBeatManager();
	~CBeatManager();

	// Public Utility Functions
	bool LoadSong(string szFileName);
	bool UnloadSongs();
	void Play();
	void Pause();
	void Stop();
	void Reset();

private:

	//******MEMBERS*********//
	list<CSong> m_listSongs;
	list<int> m_nListImageID;
	int m_nNumHit;



};

#endif