#include "CAiManager.h"
#include <ctime>
#include <time.h>


CAiManager::CAiManager( void )
{
	
}

CAiManager::~CAiManager( void )
{

}

CAiManager* CAiManager::GetInsatance()
{
	static CAiManager instance;
	return &instance;
}

bool CAiManager::RandomDifficult( int Level )
{
	switch (Level)
	{
	case AI_EASY:
		return rand()% 2;
	case AI_NORMAL:
		return rand()%25;
	case AI_HARD:
		return rand()%67;
	case AI_INSANE:
		return rand()%100;
	}
}

bool CAiManager::CheckNotesHit( CBeat* hit )
{
	vector<CBeat*>::iterator iter;
	for (iter = notesHit.begin(); iter != notesHit.end(); ++iter)
	{
		if (hit == (*iter))
		{
			return 0;
		}
		else
			addNote(hit);
			return false;
	}
	
}

void CAiManager::addNote(CBeat* hit)
{
	vector<CBeat*>::iterator iter;
	for (iter = notesHit.begin(); iter != notesHit.end(); ++iter)
	{
		notesHit.push_back(hit);

	}
}


