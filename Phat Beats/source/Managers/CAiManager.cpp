#include "CAiManager.h"
#include <ctime>
#include <time.h>
#include "../Random.h"


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
	INT randChance = Random::Next(0, 100);
	bool bToReturn;

	switch (Level)
	{
	case AI_EASY:
		bToReturn = randChance < 11 ? true : false;
		break;
	case AI_NORMAL:
		bToReturn = randChance < 26 ? true : false;
		break;
	case AI_HARD:
		bToReturn = randChance < 67 ? true : false;
		break;
	case AI_INSANE:
		bToReturn = randChance < 90 ? true : false;
		break;
	default:
		return false;
	}

	return bToReturn;
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
		{
			addNote(hit);
			return false;
		}
	}
	return false;
}

void CAiManager::addNote(CBeat* hit)
{
	vector<CBeat*>::iterator iter;
	for (iter = notesHit.begin(); iter != notesHit.end(); ++iter)
	{
		notesHit.push_back(hit);

	}
}


