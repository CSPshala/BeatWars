///////////////////////////////////////////////////////
// File Name	:	"CLevelSelect_State.cpp"
//
// Purpose		:	To Contain all menu related code
//////////////////////////////////////////////////////

#include "CLevelSelect_State.h"
#include "CBitmapFont.h"
#include "../CSong.h"
#include "CMenu_State.h"
#include "../CGame.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../States/CLU_State.h"
#include "../../CLevelManager.h"
#include "../Managers/CFXManager.h"

#include <fstream>
#include <sstream>

CLevelSelect_State::CLevelSelect_State() {

}

CLevelSelect_State::~CLevelSelect_State() {

}

void CLevelSelect_State::Enter(void) {	
	LoadLevels();
	Selected = 0;
	nBgID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MainMenuBG.jpg");
}

bool CLevelSelect_State::Input(void) {
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->MouseButtonPressed(0))
		CGame::GetInstance()->ChangeState(CMenu_State::GetInstance());

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP)) {
		if(Selected != 0)
			--Selected;
	}
	else if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN)) {
		if((unsigned)Selected < GetLevelData().size() - 1)
			++Selected;
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_P)) {
		if(GetPlaylist().size() < 6) {
			std::vector<int>::iterator i = GetPlaylist().begin();
			bool bSafe = true;

			for(; i != GetPlaylist().end(); ++i) {
				if((*i) == (int)Selected) {
					bSafe = false;
					break;
				}
			}

			if(bSafe)
				GetPlaylist().push_back((int)Selected);
		}
	}
	else if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_R)) {
		std::vector<int>::iterator i = GetPlaylist().begin();

		for(; i != GetPlaylist().end(); ++i) {
			if((*i) == (int)Selected) {
				GetPlaylist().erase(i);
				break;
			}
		}
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN)) {
		CLU_State::GetInstance()->SetNewState(CGameplay_State::GetInstance());
		CLevelManager::GetInstance()->EmptySongQueue();
		CGameplay_State::GetInstance()->SetPreviouslyPlaying(false);

		for(std::vector<int>::size_type i = 0; i < GetPlaylist().size(); ++i) {
			CLU_State::GetInstance()->QueueLoadCommand(GetLevelData()[GetPlaylist()[i]]->szFile, "", Song);
			CLevelManager::GetInstance()->QueueSong(GetLevelData()[GetPlaylist()[i]]->szSongName);
		}

		CFXManager::GetInstance()->UnloadAllFX();
		CLU_State::GetInstance()->QueueLoadCommand("GameBG.xml","P1ATTACK",Effect);
		CLU_State::GetInstance()->QueueLoadCommand("GuardBG.xml","P1GUARD",Effect);
		CLU_State::GetInstance()->QueueLoadCommand("GameBG.xml","P2ATTACK",Effect);
		CLU_State::GetInstance()->QueueLoadCommand("GuardBG.xml","P2GUARD",Effect);
		CLU_State::GetInstance()->QueueLoadCommand("Hit.xml","P1_HIT",Effect);
		CLU_State::GetInstance()->QueueLoadCommand("Hit.xml","P2_HIT",Effect);

		CBeatManager::GetInstance()->Stop();
		CBeatManager::GetInstance()->UnloadSongs();

		CGame::GetInstance()->ChangeState(CLU_State::GetInstance());
	}

	return true;
}

void CLevelSelect_State::Update(void) {

}

RECT layout = {0, 0, 800, 600};
RECT layoutTop = {32, 0, 800, 96};
RECT layoutMiddle = {0, 220, 800, 316};
RECT layoutBottom = {0, 490, 768, 600};
RECT layoutInfo = {0, 0, 800, 32};
RECT layoutPlaylist = {10, 250, 150, 580};

void CLevelSelect_State::Render(void) {
	CSGD_TextureManager::GetInstance()->Draw(nBgID, 0, 0, 0.78125f, 0.5859375f);
	if(!GetLevelData().size()) {
		CBitmapFont::GetInstance()->SetScale(1.5f);
		CBitmapFont::GetInstance()->PrintStrokedTextInRect("no songs!", &layout, 2, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));
	}
	else {
		CBitmapFont::GetInstance()->SetScale(1.5f);
		CSGD_TextureManager::GetInstance()->Draw(GetLevelData()[Selected]->szImage, 400 - (CSGD_TextureManager::GetInstance()->GetTextureWidth(GetLevelData()[Selected]->szImage) >> 1),
			300 - (CSGD_TextureManager::GetInstance()->GetTextureHeight(GetLevelData()[Selected]->szImage) >> 1));

		// Top
		if(Selected - 1 >= 0)
			CBitmapFont::GetInstance()->PrintStrokedTextInRect(GetLevelData()[Selected-1]->szMenuName, &layoutTop, 1, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));

		// Middle
		CBitmapFont::GetInstance()->PrintStrokedTextInRect(GetLevelData()[Selected]->szMenuName, &layoutMiddle, 2, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));

		// Bottom
		if((unsigned)Selected + 1 < GetLevelData().size())
			CBitmapFont::GetInstance()->PrintStrokedTextInRect(GetLevelData()[Selected+1]->szMenuName, &layoutBottom, 4, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));

		CBitmapFont::GetInstance()->SetScale(0.75f);
		CBitmapFont::GetInstance()->PrintInRect("press p to add song to playlist and r to remove\npress escape to go back to the main menu or return to play", &layoutInfo, 2, D3DCOLOR_XRGB(230, 230, 55));

		static std::stringstream ss;

		ss.str("");
		for(std::vector<int>::size_type i = 0; i < GetPlaylist().size(); ++i)
			ss << GetLevelData()[GetPlaylist()[i]]->szMenuName << '\n';

		CBitmapFont::GetInstance()->PrintText(ss.str(), layoutPlaylist.left, layoutPlaylist.top, D3DCOLOR_XRGB(255, 255, 255));
		//CBitmapFont::GetInstance()->PrintInRect(ss.str(), &layoutPlaylist, 1, D3DCOLOR_XRGB(255, 255, 255));

		CBitmapFont::GetInstance()->SetScale(1.0f);
		CBitmapFont::GetInstance()->PrintText("playlist", layoutPlaylist.left, layoutPlaylist.top - 32, D3DCOLOR_XRGB(230, 230, 55));
	}
}

void CLevelSelect_State::Exit(void) {
	std::vector<LevelData*>::size_type i = 0;
	for(; i < GetLevelData().size(); ++i) {
		CSGD_TextureManager::GetInstance()->UnloadTexture(GetLevelData()[i]->szImage);
		delete GetLevelData()[i];
		GetLevelData()[i] = nullptr;
	}

	GetLevelData().clear();
	CSGD_TextureManager::GetInstance()->UnloadTexture(nBgID);
}

CLevelSelect_State* CLevelSelect_State::GetInstance() {
	// Lazy instantiation
	static CLevelSelect_State instance; // Static allows passing back of address
	return &instance;	
}

const void CLevelSelect_State::LoadLevels(void) {
	std::ifstream in;
	std::stringstream stringHelper;

	in.open("resource/beatlist/Levels.txt");

	if(!in)
		return;

	char buffer[128];

	while(!in.eof())
	{
		LevelData* pNewData = new LevelData();

		// Get the Menu Name for Displaying
		in.getline(buffer, 128, '\t');
		pNewData->szMenuName = buffer;

		// Get the File
 		in.ignore(INT_MAX, '[');
		in.getline(buffer, 128, ',');
		pNewData->szFile = buffer;

		// Get the song name
		in.ignore(INT_MAX, ' ');
		in.getline(buffer, 128, ',');
		pNewData->szSongName = buffer;

		// Get The Image File
		in.ignore(INT_MAX, ' ');
		in.getline(buffer, 128, ']');

		in.ignore(INT_MAX, '\n');

		stringHelper.str("");
		stringHelper << "resource/beatlist/preview images/" << buffer << '\0';
		pNewData->szImage = CSGD_TextureManager::GetInstance()->LoadTexture(stringHelper.str().c_str());

		GetLevelData().push_back(pNewData);
	}

	in.close();
}