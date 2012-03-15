#include "CHighScoreState.h"
#include "../xml/tinyxml.h"
#include "CGameplay_State.h"
#include "../CGame.h"
#include "CMenu_State.h"
#include "../Managers/CBeatManager.h"
#include "../Globals.h"
#include "../../CLevelManager.h"
CHighScoreState* CHighScoreState::GetInstance( void )
{
	static CHighScoreState instance;
	return &instance;
}


void CHighScoreState::Enter( void )
{
	int selectedSpot = 0;
	//aPlayer->SetTotalScore(1000);

	//CLevelManager::GetInstance()->GetPlayer(PlayerOne)->SetTotalScore(9000);

	LoadXMLFile("resource/HighScores.xml");
	buffers = "";
	HighScores[selectedSpot].nScores = CLevelManager::GetInstance()->GetPlayer(PlayerOne)->GetTotalScore();
	
}

bool CHighScoreState::Input( void )
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
	{
		CGame::GetInstance()->ChangeState(CMenu_State::GetInstance());
		
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_BACKSPACE))
	{
		HighScores[selectedSpot].strInitials.pop_back();
		return true;
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN))
	{
		SaveXMLFile("resource/HighScores.xml");
		CGame::GetInstance()->ChangeState(CMenu_State::GetInstance());
	}


	if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx())
	{
		char currentKey = CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx();
		buffers += currentKey;
		HighScores[selectedSpot].strInitials.clear();
		if(HighScores[selectedSpot].strInitials.length() < 3 && currentKey != 0)
		{

			HighScores[selectedSpot].strInitials = buffers;
		}

	}
	
	


	return true;
}

void CHighScoreState::Update(void)
{

}

void CHighScoreState::Render(void)
{
	RECT rBody = {25, 0, CGame::GetInstance()->GetScreenWidth(), 20};
	RECT rMenu = {25, 50, CGame::GetInstance()->GetScreenWidth(), 635};
	CBitmapFont::GetInstance()->SetScale(1.0f);
	CBitmapFont::GetInstance()->PrintStrokedTextInRect("High Scores",&rBody,ALIGN_LEFT, D3DCOLOR_XRGB(0, 0, 0),D3DCOLOR_XRGB(255,255,255));

	for(int i = 0; i < 10; ++i)
	{
		char buffer[128];
		buffers = buffer;
		sprintf_s(buffer,"%i %s \t%i \t%s", i+1, HighScores[i].strInitials.c_str(), HighScores[i].nScores, HighScores[i].strDates.c_str());
		buffers = buffer;
		CBitmapFont::GetInstance()->PrintText(buffers,0,i,D3DCOLOR_XRGB(255,255,255));
	}
}

void CHighScoreState::Exit(void)
{
	
}

void CHighScoreState::LoadXMLFile( const char * fileToLoad )
{
	TiXmlDocument doc;

	if(doc.LoadFile(fileToLoad) == false)
		return;

	TiXmlElement * pRoot = doc.RootElement();
	
	if(pRoot == NULL)
		return;

	TiXmlElement * pHighScores = pRoot->FirstChildElement("HighScore");

	int nCount = 0;
	while(pHighScores != NULL)
	{
		if(pHighScores != NULL)
		{
			int score = 0;
			string date = pHighScores->Attribute("date");
			const char * szInitials = pHighScores->GetText();
			pHighScores->QueryIntAttribute("score", &score);
			HighScores[nCount].strInitials = szInitials;
			HighScores[nCount].strDates = date;
			HighScores[nCount].nScores = score;
		}
		pHighScores = pHighScores->NextSiblingElement("HighScore");
		++nCount;
	}
}

void CHighScoreState::SaveXMLFile( const char * fileToSave )
{
	TiXmlDocument doc;

	TiXmlDeclaration * pDec = new TiXmlDeclaration("1.0", "utf-8", "");

	doc.LinkEndChild(pDec);

	TiXmlElement * pRoot = new TiXmlElement("HighScores");

	doc.LinkEndChild(pRoot);

	for( int i = 0; i < 10; ++i)
	{
		TiXmlElement * pHighScore = new TiXmlElement("HighScore");

		pHighScore->SetAttribute("score", HighScores[i].nScores);
		pHighScore->SetAttribute("date", HighScores[i].strDates.c_str());
		TiXmlText * pText = new TiXmlText(HighScores[i].strInitials.c_str());
		pHighScore->LinkEndChild(pText);

		pRoot->LinkEndChild(pHighScore);
	}

	if(doc.SaveFile(fileToSave) == false)
		return;
}

int CHighScoreState::CheckTopTen( void )
{
	for(int i = 10; i > 0; --i)
	{
		if(CLevelManager::GetInstance()->GetPlayer(PlayerOne)->GetTotalScore() < HighScores[i].nScores)
			return i;
	}
	return 12;
}

void CHighScoreState::Placing( void )
{
	selectedSpot = CheckTopTen();
	if(selectedSpot != 12)
	{
		HighScore tempscores[10];

		for(int i = 0; i < selectedSpot; ++i)
			tempscores[i] = HighScores[i];

		for(int i = selectedSpot; i > 0; --i)
			HighScores[i-1] = tempscores[i];

		HighScores[selectedSpot].nScores = CLevelManager::GetInstance()->GetPlayer(PlayerOne)->GetTotalScore();
		HighScores[selectedSpot].strInitials = "";
		HighScores[selectedSpot].strDates = GetTodaysDate();
	}
}

char * CHighScoreState::GetTodaysDate( void )
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	char datebuffer[128] = {0};
	sprintf_s(datebuffer, _countof(datebuffer), "%02d/%02d/%02d",  time.wMonth, time.wDay, time.wYear);
	return datebuffer;
}
