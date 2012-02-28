#ifndef C_AIMANAGER_H
#define C_AIMANAGER_H
#include <Windows.h>
#include <iostream>
#include <vector>
#include "../CBase.h"
#include "../CBeat.h"
using namespace std;

static enum{AI_EASY, AI_NORMAL, AI_HARD, AI_INSANE, NUM_DIFFICULTY};
class CAiManager
{
private:
	CAiManager(void);
	~CAiManager(void);
	CAiManager(const CAiManager&);
	CAiManager& operator=(const CAiManager&);
	CBeat* hitBeat;

public:

	static CAiManager* GetInsatance();
	//void Update(float fElaspedTime);

	bool RandomDifficult(int Level);
	//accessors
	CBeat* GetBeat() { return hitBeat;}

	//mutator
	void SetBeatHit(CBeat* BeatHit) {hitBeat = BeatHit;}
};
#endif