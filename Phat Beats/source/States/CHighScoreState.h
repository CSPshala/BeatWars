#pragma once
#include "IGameState.h"
#include "../CGame.h"
#include "CBitmapFont.h"
#include "../CPlayer.h"

#include <vector>
#include <string>
using namespace std;
class CHighScoreState : public IGameState
{
private:
	struct HighScore
	{
		string strDates;
		string strInitials;
		int nScores;
	};
	CHighScoreState(){}
	CHighScoreState(const CHighScoreState&);
	CHighScoreState& operator=(const CHighScoreState&);
	~CHighScoreState(){}
	CBitmapFont * bMpfHighScores;
	HighScore HighScores[10];
	int selectedSpot;
	int m_nImageFontID;
	string buffers;
	int m_nStingIndex;
	int m_nBackgroundID;
	
public:

	static CHighScoreState* GetInstance(void);

	void Enter(void);
	bool Input(void);
	void Update(void);
	void Render(void);
	void Exit(void);
	void LoadXMLFile(const char * fileToLoad);
	void SaveXMLFile(const char * fileToSave);
	int CheckTopTen(void);
	void Placing(void);
	/*char * GetTodaysDate(void);*/
};