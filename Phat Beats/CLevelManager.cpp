#include "CLevelManager.h"
#include "source/Managers/CBeatManager.h"
#include "source/SGD Wrappers/CSGD_FModManager.h"

// Constructor
CLevelManager::CLevelManager(void){}

// Singleton Accessor
const CLevelManager* CLevelManager::GetInstance(void){
	static CLevelManager Instance;
	return &Instance;
}

// Methods
const void CLevelManager::QueueSong(const string szSong){
	m_vSongs.push(szSong);
}

// Logic Methods
const void CLevelManager::Update(const float fElapsedTime){
	switch(GetState())
	{
	case Playing:
		{
			if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(CBeatManager::GetInstance()->GetCurrentlyPlayingSong()->GetSongID()))
			{

			}
		}
		break;

	case Pausing:
		{

		}
		break;

	case Exiting:
		{

		}
		break;
	}
}
const void CLevelManager::Render(void){

}