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
	srand(unsigned int(time(0)));
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
return true;
}

/*
void CAiManager::Update( float fElaspedTime )
{

}
*/

