#include "CLevelManager.h"
#include "source/Managers/CBeatManager.h"
#include "source/SGD Wrappers/CSGD_FModManager.h"
#include "source/SGD Wrappers/CSGD_DirectInput.h"
#include "source/SGD Wrappers/CSGD_Direct3D.h"
#include "source/SGD Wrappers/CSGD_TextureManager.h"
#include "source/Managers/CObjectManager.h"
#include "source/Managers/CFXManager.h"
#include "source/CPlayer.h"
#include "source/States/CLoad_State.h"
#include "source/States/CBitmapFont.h"

#include <sstream>

// Constructor/Destructor
CLevelManager::CLevelManager(void) {
	// Set Up Easy Access
	BeatMan	= CBeatManager::GetInstance();
	FxMan	= CFXManager::GetInstance();
	ObjMan	= CObjectManager::GetInstance();
	D3DMan	= CSGD_Direct3D::GetInstance();
	InMan	= CSGD_DirectInput::GetInstance();
	FmMan	= CSGD_FModManager::GetInstance();
	TexMan	= CSGD_TextureManager::GetInstance();
	
	// Set Up Players
	PlayerList().push_back(new CPlayer(OBJ_PLAYER1));
	PlayerList().push_back(new CPlayer(OBJ_AI));
	

	// Set Up Assets
	m_nBgID		= TexMan->LoadTexture("resource/graphics/star_wars___battle_1182.jpg");
	m_nHudID	= TexMan->LoadTexture("resource/graphics/bag_HUD.png");

	// Set Up RECTS
	RECT rLeftHandle = {20, 10, 67, 27};
	RECT rRightHandle = {445, 12, 492, 23};
	RECT rLeftSaber = {20, 349, 227, 381};
	RECT rRightSaber = {257, 348, 466, 382};
	RECT rLeftPowerBar = {22, 107, 214, 140};
	RECT rRightPowerBar = {303, 107, 495, 141};

	rectLeftHandle		= rLeftHandle;
	rectRightHandle		= rRightHandle;
	rectLeftSaber		= rLeftSaber;
	rectRightSaber		= rRightSaber;
	rectLeftPowerBar	= rLeftPowerBar;
	rectRightPowerBar	= rRightPowerBar;
}
CLevelManager::~CLevelManager(void) {
	std::vector<CPlayer*>::iterator i;

	for(i = PlayerList().begin(); i != PlayerList().end(); ++i) {
		delete (*i);
		(*i) = nullptr;
	}

	PlayerList().clear();
}

// Singleton Accessor
CLevelManager* CLevelManager::GetInstance(void){
	static CLevelManager Instance;
	return &Instance;
}

// Methods
const void CLevelManager::QueueSong(const string szSong){
	m_vSongs.push(szSong);
}
CPlayer* CLevelManager::GetPlayer(const PlayerIndex eIndex) {
	switch(eIndex) {
	case PlayerOne:
		return (PlayerList()[0]);
		break;

	case PlayerTwo:
		return (PlayerList()[1]);
		break;

	default:
		return nullptr;
	}
}
const void CLevelManager::EnterLevel(void) {
	ObjMan->AddObject(GetPlayer(PlayerOne));
	ObjMan->AddObject(GetPlayer(PlayerTwo));
	BeatMan->Play(m_vSongs.front());
}
const void CLevelManager::LeaveLevel(void) {
	BeatMan->Pause();
	ObjMan->RemoveObject(GetPlayer(PlayerOne));
	ObjMan->RemoveObject(GetPlayer(PlayerTwo));
}

// Logic Methods
const void CLevelManager::HandleLevelInput(void) {
	switch(GetState()) {
	case Playing:
		HandlePlayingInput();
		break;

	case Pausing:
		HandlePausingInput();
		break;
	}

	if(InMan->KeyPressed(DIK_R)) {
		BeatMan->Reset();
		SetState(Playing);
	}
}
const void CLevelManager::HandlePlayingInput(void) {
	BeatMan->CheckPlayerInput(GetPlayer(PlayerOne));
}
const void CLevelManager::HandlePausingInput(void) {
	if(InMan->KeyPressed(DIK_RETURN)) {
		m_vSongs.empty() ? SetState(Exiting) : SetState(Playing);

		GetPlayer(PlayerOne)->SetCurrentHP(100);
		GetPlayer(PlayerTwo)->SetCurrentHP(100);

		BeatMan->Stop();

		if(!m_vSongs.empty())
			BeatMan->Play(m_vSongs.front());
	}
}
const void CLevelManager::Update(const float fElapsedTime){
	switch(GetState()) {
	case Playing:
		UpdatePlayingState(fElapsedTime);
		break;

	case Pausing:
		UpdatePausingState(fElapsedTime);
		break;

	case Exiting:
		Exit();
		LeaveLevel();
		CGame::GetInstance()->ChangeState(CMenu_State::GetInstance());
		break;
	}
}
const void CLevelManager::UpdatePlayingState(const float fElapsedTime) {
	FxMan->Update(fElapsedTime);
	ObjMan->UpdateObjects(fElapsedTime);
	ObjMan->CheckCollisions(GetPlayer(PlayerOne));
	ObjMan->CheckCollisions(GetPlayer(PlayerTwo));
	BeatMan->Update();

	if(!FmMan->IsSoundPlaying(BeatMan->GetCurrentlyPlayingSong()->GetSongID())) {
		m_vSongs.pop();
		SetState(Pausing);
	}
	else if(GetPlayer(PlayerOne)->GetCurrentHP() <= 0 || GetPlayer(PlayerTwo)->GetCurrentHP() <= 0) {
		m_vSongs.pop();
		SetState(Pausing);
	}
}
const void CLevelManager::UpdatePausingState(const float fElapsedTime) {

}
const void CLevelManager::Render(void){
	switch(GetState()) {
	case Playing:
		RenderPlayingState();
		break;

	case Pausing:
		RenderPausingState();
		break;
	}
}
const void CLevelManager::RenderPlayingState(void) {
	// Draw HUD
	TexMan->DrawF(m_nHudID, 59.0f, 10.0f, 1.0f, 1.0f, &rectLeftSaber);
	TexMan->DrawF(m_nHudID, 513.0f, 10.0f, 1.0f, 1.0f, &rectRightSaber);
	TexMan->DrawF(m_nHudID, 20.0f, 17.0f, 1.0f, 1.0f, &rectLeftHandle);
	TexMan->DrawF(m_nHudID, 722.0f, 21.0f, 1.0f, 1.0f, &rectRightHandle);
	TexMan->DrawF(m_nHudID, 59.0f, 45.0f, 1.0f, 1.0f, &rectLeftPowerBar);
	TexMan->DrawF(m_nHudID, 529.0f, 45.0f, 1.0f, 1.0f, &rectRightPowerBar);

	// Draw Particles
	FxMan->Render();

	// Draw Objects
	ObjMan->RenderObjects();

	// Draw Health
	static char szHpBuffer[8];
	CBitmapFont::GetInstance()->SetScale(1.0f);

	// Player 1
	_itoa_s(GetPlayer(PlayerOne)->GetCurrentHP(), szHpBuffer, 10);
	CBitmapFont::GetInstance()->PrintStrokedText(szHpBuffer, 10, 256, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));

	// Player 2
	_itoa_s(GetPlayer(PlayerTwo)->GetCurrentHP(), szHpBuffer, 10);
	CBitmapFont::GetInstance()->PrintStrokedText(szHpBuffer, 10, 300, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));
}
const void CLevelManager::RenderPausingState(void) {
	static RECT rectLayout = {0, 0, 800, 600};
	static std::stringstream pauseText;

	pauseText.str("");
	pauseText << "round over\nnext song: ";

	m_vSongs.empty() ? pauseText << "none\n" : pauseText << m_vSongs.front() << '\n';
	pauseText << "press return to continue...";

	CBitmapFont::GetInstance()->SetScale(1.0f);
	CBitmapFont::GetInstance()->PrintInRect(pauseText.str(), &rectLayout, 2, D3DCOLOR_XRGB(230, 230, 55));
}
const void CLevelManager::Exit(void) {
	queue<string>::size_type i = 0;
	for(; i < m_vSongs.size(); ++i)
		m_vSongs.pop();
}