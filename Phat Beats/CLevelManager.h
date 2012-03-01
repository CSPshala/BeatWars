#ifndef CLEVELMANAGER_H
#define CLEVELMANAGER_H

#include <queue>
using std::queue;

#include <string>
using std::string;

class CLevelManager
{
	// Constructor
	CLevelManager(void);

	// State Enum
	enum LevelState : char {Playing = 0, Pausing, Exiting} m_eCurrentState;

	// Data Members
	queue<string> m_vSongs;
	
	// Private Methods
	const LevelState GetState(void){return m_eCurrentState;}
	const void SetState(const LevelState& eNewState){m_eCurrentState = eNewState;}

public:
	
	// Singleton Accessor
	static const CLevelManager* GetInstance(void);

	// Methods
	const void QueueSong(const string szSong);

	// Logic Methods
	const void Update(const float fElapsedTime);
	const void Render(void);
};

#endif