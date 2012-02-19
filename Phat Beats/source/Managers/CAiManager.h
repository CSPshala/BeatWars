#ifndef C_AIMANAGER_H
#define C_AIMANAGER_H
#include <Windows.h>
#include <iostream>
#include <vector>
#include "CBase.h"
using namespace std;

class CBeat;
class CAiManager
{
private:
	CAiManager(void);
	~CAiManager(void);
	CAiManager(const CAiManager&);
	CAiManager& operator=(const CAiManager&);

	vector<CBeat*> m_vBeatNotes;

public:

	void Update(float fElaspedTime);

	//accessors


	//mutator
};
#endif