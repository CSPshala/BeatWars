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
#include "source/CAnimation.h"
#include "source/States/COptionsState.h"
#include "source/States/CLevelSelect_State.h"
#include "source/StringHelper.h"
#include "source/Random.h"
#include <fstream>

#include <sstream>
#define MAX_TAKEDOWNS 2
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
	if (CLevelSelect_State::GetInstance()->GetVsMode() == true)
	{
		PlayerList().push_back(new CPlayer(OBJ_PLAYER2));
	}
	else
		PlayerList().push_back(new CPlayer(OBJ_AI));

	m_SongTransitionAlpha = 255;
	m_bStartTransition = true;
	

	//Player 1 Animations
	if (CGame::GetInstance()->GetCharacterSelection() == true)
	{

		GetPlayer(PlayerOne)->SetSingleAnimation(AnMan.LoadSingleAnimation("IdleLuke.xml","sprites_luke_001.png"));
		GetPlayer(PlayerOne)->SetSingleAnimation(AnMan.LoadSingleAnimation("HighBlockLuke.xml","sprites_luke_002.png"));
		GetPlayer(PlayerOne)->SetSingleAnimation(AnMan.LoadSingleAnimation("LowBlockLuke.xml","sprites_luke_003.png"));

	}

	if (CGame::GetInstance()->GetCharacterSelection() == false)
	{

		GetPlayer(PlayerOne)->SetSingleAnimation(AnMan.LoadSingleAnimation("IdleVader.xml","sprites_vader_001.png"));
		GetPlayer(PlayerOne)->SetSingleAnimation(AnMan.LoadSingleAnimation("HighBlockVader.xml","sprites_vader_002.png"));
		GetPlayer(PlayerOne)->SetSingleAnimation(AnMan.LoadSingleAnimation("LowBlockVader.xml","sprites_vader_003.png"));
		GetPlayer(PlayerOne)->SetSingleAnimation(AnMan.LoadSingleAnimation("HighHitVader.xml","sprites_vader_004.png"));
		GetPlayer(PlayerOne)->SetSingleAnimation(AnMan.LoadSingleAnimation("LowHitVader.xml","sprites_vader_005.png"));
	}

	if (CGame::GetInstance()->GetCharacterSelection2() == true)
	{
		GetPlayer(PlayerTwo)->SetSingleAnimation(AnMan.LoadSingleAnimation("IdleLuke.xml","sprites_luke_001.png"));
		GetPlayer(PlayerTwo)->SetSingleAnimation(AnMan.LoadSingleAnimation("HighBlockLuke.xml","sprites_luke_002.png"));
		GetPlayer(PlayerTwo)->SetSingleAnimation(AnMan.LoadSingleAnimation("LowBlockLuke.xml","sprites_luke_003.png"));
	}

	if (CGame::GetInstance()->GetCharacterSelection2() == false)
	{

		GetPlayer(PlayerTwo)->SetSingleAnimation(AnMan.LoadSingleAnimation("IdleVader.xml","sprites_vader_001.png"));
		GetPlayer(PlayerTwo)->SetSingleAnimation(AnMan.LoadSingleAnimation("HighBlockVader.xml","sprites_vader_002.png"));
		GetPlayer(PlayerTwo)->SetSingleAnimation(AnMan.LoadSingleAnimation("LowBlockVader.xml","sprites_vader_003.png"));
		GetPlayer(PlayerTwo)->SetSingleAnimation(AnMan.LoadSingleAnimation("HighHitVader.xml","sprites_vader_004.png"));
		GetPlayer(PlayerTwo)->SetSingleAnimation(AnMan.LoadSingleAnimation("LowHitVader.xml","sprites_vader_005.png"));
	}
	GetPlayer(PlayerOne)->SetSingleAnimation(AnMan.LoadSingleAnimation("HighHitLuke.xml","sprites_luke_004.png"));
	GetPlayer(PlayerOne)->SetSingleAnimation(AnMan.LoadSingleAnimation("LowHitLuke.xml","sprites_luke_005.png"));
	//Player 2 Animations


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
	
	RECT LeftPowerup	= {64,157,203,171};
	RECT RightPowerup	= {64,157,203,171}; 

	rectLeftPowerup		= LeftPowerup;
	rectRightPowerup	= RightPowerup;

	// Set up Offsets
	m_nRightOffset = 0;
	m_nRightPowerOffset = 0;
	p1PrevHP = 100;
	p2PrevHP = 100;

	m_bCheck = true;
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
	GetPlayer(PlayerOne)->SetCurrentHP(GetPlayer(PlayerOne)->GetMaxHP());
	ObjMan->AddObject(GetPlayer(PlayerTwo));
	GetPlayer(PlayerTwo)->SetCurrentHP(GetPlayer(PlayerTwo)->GetMaxHP());
	GetPlayer(PlayerOne)->SetCurrentPowerup(0);
	GetPlayer(PlayerTwo)->SetCurrentPowerup(0);
	
	p2PrevHP = 101;
	p1PrevHP = 101;
	RECT rLeftHandle = {20, 10, 67, 27};
	RECT rRightHandle = {445, 12, 492, 23};
	RECT rLeftSaber = {20, 349, 227, 381};
	RECT rRightSaber = {257, 348, 466, 382};
	RECT rLeftPowerBar = {22, 107, 214, 140};
	RECT rRightPowerBar = {303, 107, 495, 141};
	RECT LeftPowerup	= {64,157,203,171};
	RECT RightPowerup	= {64,157,203,171}; 
	rectLeftHandle		= rLeftHandle;
	rectRightHandle		= rRightHandle;
	rectLeftSaber		= rLeftSaber;
	rectRightSaber		= rRightSaber;
	rectLeftPowerBar	= rLeftPowerBar;
	rectRightPowerBar	= rRightPowerBar;
	rectLeftPowerup		= LeftPowerup;
	rectRightPowerup	= RightPowerup;
	m_nRightOffset = 0;
	m_nRightPowerOffset = 0;
	p2PrevPowerup = -1;
	p1PrevPowerup = -1;
	BeatMan->Play(m_vSongs.front());
	BeatMan->GetCurrentlyPlayingSong()->CreateAIHits(); // Resolving AI hits before level even starts
	/*
	CSGD_FModManager::GetInstance()->SetVolume(BeatMan->GetCurrentlyPlayingSong()->GetSongID()
	,COptionsState::GetInstance()->GetMusicVol());
	*/

	int x = Random::Next(0,2);

	if( x == 0 )
	{
		m_nBackgroundID = TexMan->LoadTexture("resource/graphics/bg1.png");
	}
	else if( x == 1 )
	{
		m_nBackgroundID = TexMan->LoadTexture("resource/graphics/bg2.png");
	}
	else
	{
		m_nBackgroundID = TexMan->LoadTexture("resource/graphics/bg3.png");
	}


	m_bCheck = true;
	SetState(Playing);
}
const void CLevelManager::LeaveLevel(void) {
	BeatMan->Pause();
	ObjMan->RemoveObject(GetPlayer(PlayerOne));
	ObjMan->RemoveObject(GetPlayer(PlayerTwo));
}
const void CLevelManager::SkipLevel(void)
{
	// Stopping currently playing song
	BeatMan->Stop();
	
	// Popping off the song queue
	m_vSongs.pop();

	// Playing (if something to play), else exit
	if(!m_vSongs.empty())
		SetState(Pausing);
	else
		SetState(Exiting);

	// Flushing player two AI hits (dosen't matter if he's not AI)
	GetPlayer(PlayerTwo)->GetAIBeats().clear();
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
	BeatMan->CheckPlayerInput(GetPlayer(PlayerTwo));
}
const void CLevelManager::HandlePausingInput(void) {
	if(InMan->KeyPressed(DIK_RETURN) || InMan->JoystickGetLStickDirPressed(0, 0)) 
	{
		// Add Song to unlock list
		if(BeatMan->GetCurrentlyPlayingSong()->GetSongName() != "jeditheme")
		{
			if(!StrHlp::FileSearch("resource/Levels.txt", BeatMan->GetCurrentlyPlayingSong()->GetSongName().c_str()))
			{
				std::fstream F("resource/Levels.txt", std::ios::app);
				std::stringstream S;
			S << '\n' << BeatMan->GetCurrentlyPlayingSong()->GetSongName();
				F.write(S.str().c_str(), S.str().length());
				F.close();
			}
		}

		m_vSongs.empty() ? SetState(Exiting) : SetState(Playing);

		BeatMan->Stop();

		if(!m_vSongs.empty())
		{
			BeatMan->Play(m_vSongs.front());
			BeatMan->GetCurrentlyPlayingSong()->CreateAIHits(); // Resolving AI hits before level even starts
		}

	
		GetPlayer(PlayerOne)->SetTotalScore(GetPlayer(PlayerOne)->GetTotalScore() + GetPlayer(PlayerOne)->GetCurrentScore());
		GetPlayer(PlayerTwo)->SetTotalScore(GetPlayer(PlayerTwo)->GetTotalScore() + GetPlayer(PlayerTwo)->GetCurrentScore());

		

		GetPlayer(PlayerOne)->SetCurrentScore(0);
		GetPlayer(PlayerTwo)->SetCurrentScore(0);

		TexMan->UnloadTexture(m_nBackgroundID);
		
		int x = Random::Next(0,2);

		if( x == 0 )
		{
			m_nBackgroundID = TexMan->LoadTexture("resource/graphics/bg1.png");
		}
		else if( x == 1 )
		{
			m_nBackgroundID = TexMan->LoadTexture("resource/graphics/bg2.png");
		}
		else
		{
			m_nBackgroundID = TexMan->LoadTexture("resource/graphics/bg3.png");
		}


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

	m_bCheck = true;

	if(!FmMan->IsSoundPlaying(BeatMan->GetCurrentlyPlayingSong()->GetSongID()) || 
		GetPlayer(PlayerOne)->GetCurrentTakeDown() == MAX_TAKEDOWNS || 
		GetPlayer(PlayerTwo)->GetCurrentTakeDown() == MAX_TAKEDOWNS) 
	{
		m_fGameTransitionAlpha += (175 * fElapsedTime);
		BeatMan->Stop();
		if (m_fGameTransitionAlpha >= 255)
		{
			m_vSongs.pop();
			SetState(Pausing);
			m_fGameTransitionAlpha = 1;
			
		}	
	}
	else if(GetPlayer(PlayerOne)->GetCurrentHP() <= 0 || GetPlayer(PlayerTwo)->GetCurrentHP() <= 0) 
	{
		
		if (GetPlayer(PlayerOne)->GetCurrentHP() <= 0)
		{
			GetPlayer(PlayerOne)->SetCurrentHP(100);
			GetPlayer(PlayerTwo)->SetTakeDown(GetPlayer(PlayerTwo)->GetCurrentTakeDown()+1);
			
		}
		else
		{
			GetPlayer(PlayerTwo)->SetCurrentHP(100);
			GetPlayer(PlayerTwo)->SetTakeDown(GetPlayer(PlayerTwo)->GetCurrentTakeDown()+1);
			
		}
		
	}
	
	if( GetPlayer(PlayerOne)->GetCurrentHP() != p1PrevHP )
	{
		if( GetPlayer(PlayerOne)->GetCurrentHP() <= 100 )
		{
			p1PrevHP = GetPlayer(PlayerOne)->GetCurrentHP();

			float firstcalc = 207 * (p1PrevHP / 100.0f);

			int m_nLeftOffset = int(207 - firstcalc);

			rectLeftSaber.right =  227 - m_nLeftOffset;
		}
	}

	if( GetPlayer(PlayerTwo)->GetCurrentHP() != p2PrevHP )
	{

		if( GetPlayer(PlayerTwo)->GetCurrentHP() <= 100 )
		{
			p2PrevHP = GetPlayer(PlayerTwo)->GetCurrentHP();

			float firstcalc = 209 * (p2PrevHP / 100.0f);

			m_nRightOffset = int(209 - firstcalc);

			rectRightSaber.left =  257 + m_nRightOffset;
		}
	}

	if( GetPlayer(PlayerOne)->GetCurrentPowerup() != p1PrevPowerup )
	{
		p1PrevPowerup = GetPlayer(PlayerOne)->GetCurrentPowerup();
		rectLeftPowerup.right = rectLeftPowerup.left + GetPlayer(PlayerOne)->GetCurrentPowerup();
	}
	
	if( GetPlayer(PlayerTwo)->GetCurrentPowerup() != p2PrevPowerup )
	{
		p2PrevPowerup = GetPlayer(PlayerTwo)->GetCurrentPowerup();
		
		m_nRightPowerOffset = int(140 - p2PrevPowerup);

		rectRightPowerup.left = 64 + m_nRightPowerOffset;
	}

		
	if( GetPlayer(PlayerOne)->GetCurrAnim()->GetPlayedAlready() )
	{
		GetPlayer(PlayerOne)->SetCurrAnimation("Idle");
	}

	if( GetPlayer(PlayerTwo)->GetCurrAnim()->GetPlayedAlready() )
	{
		GetPlayer(PlayerTwo)->SetCurrAnimation("Idle");
	}

	if( GetPlayer(PlayerOne)->GetCurrentPowerup() > GetPlayer(PlayerOne)->GetMaxPowerup())
	{
		GetPlayer(PlayerOne)->SetCurrentPowerup(GetPlayer(PlayerOne)->GetMaxPowerup()); 
	}

	if(GetPlayer(PlayerTwo)->GetCurrentPowerup() > GetPlayer(PlayerTwo)->GetMaxPowerup())
	{
		GetPlayer(PlayerTwo)->SetCurrentPowerup(GetPlayer(PlayerTwo)->GetMaxPowerup()); 
	}


}
const void CLevelManager::UpdatePausingState(const float fElapsedTime) {

	if (m_bStartTransition)
	{
		m_SongTransitionAlpha -= 0.25f;
		if (m_SongTransitionAlpha <= 1)
		{
			m_bStartTransition = false;
			m_SongTransitionAlpha = 255;
		}
	}


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

	TexMan->DrawF(m_nBackgroundID,0,0);
	// Draw HUD
	TexMan->DrawF(m_nHudID, 70.0f, 75.0f, 1.0f, 1.0f, &rectLeftPowerup);
	TexMan->DrawF(m_nHudID, 572.0f + m_nRightPowerOffset, 75.0f, 1.0f, 1.0f, &rectRightPowerup);

	TexMan->DrawF(m_nHudID, 59.0f, 30.0f, 1.0f, 1.0f, &rectLeftSaber);
	TexMan->DrawF(m_nHudID, 513.0f + m_nRightOffset, 30.0f, 1.0f, 1.0f, &rectRightSaber);

	TexMan->DrawF(m_nHudID, 20.0f, 37.0f, 1.0f, 1.0f, &rectLeftHandle);
	TexMan->DrawF(m_nHudID, 722.0f, 41.0f, 1.0f, 1.0f, &rectRightHandle);

	TexMan->DrawF(m_nHudID, 59.0f, 65.0f, 1.0f, 1.0f, &rectLeftPowerBar,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
	TexMan->DrawF(m_nHudID, 529.0f, 65.0f, 1.0f, 1.0f, &rectRightPowerBar,0,0,0,D3DCOLOR_ARGB(255,255,255,255));

	// Draw Particles
	FxMan->Render();

	// Draw Objects
	ObjMan->RenderObjects();

	// Draw Health
	static char szHpBuffer[8];
	CBitmapFont::GetInstance()->SetScale(1.0f);

	//// Player 1
	_itoa_s(GetPlayer(PlayerOne)->GetCurrentHP(), szHpBuffer, 10);
	CBitmapFont::GetInstance()->PrintStrokedText(szHpBuffer, 10, 256, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));
	//
	//// Player 2
	_itoa_s(GetPlayer(PlayerTwo)->GetCurrentHP(), szHpBuffer, 10);
	CBitmapFont::GetInstance()->PrintStrokedText(szHpBuffer, 10, 300, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));

	if(!FmMan->IsSoundPlaying(BeatMan->GetCurrentlyPlayingSong()->GetSongID()))
	{
		CGameplay_State::GetInstance()->DrawARGB("blackscreen.png", D3DCOLOR_ARGB((int)m_fGameTransitionAlpha, 0, 0, 0));
	}
	else if ( GetPlayer(PlayerOne)->GetCurrentHP() <= 0 || GetPlayer(PlayerTwo)->GetCurrentHP() <= 0)
	{
		CGameplay_State::GetInstance()->DrawARGB("blackscreen.png", D3DCOLOR_ARGB((int)m_fGameTransitionAlpha, 0, 0, 0));		
	}


}
const void CLevelManager::RenderPausingState(void) {
	static RECT rectLayout = {0, 0, 800, 600};
	static std::stringstream pauseText;

	pauseText.str("");
	pauseText << "round over\nnext song: ";

	m_vSongs.empty() ? pauseText << "none\n" : pauseText << m_vSongs.front() << '\n';
	pauseText << "press return to continue..." << "\n\n";
	if (GetPlayer(PlayerOne)->GetCurrentHP() > GetPlayer(PlayerTwo)->GetCurrentHP())
	{
		pauseText << "Player1 Wins" << '\n';
		pauseText << "Notes Hit: " << BeatMan->GetNumberNotesHit() << '\n';
		pauseText << "Current Combo: " << GetPlayer(PlayerOne)->GetCurrentStreak() << '\n';
		pauseText << "Current TakeDowns: " << GetPlayer(PlayerOne)->GetCurrentTakeDown() << '\n';
		pauseText << "Current Score:" << GetPlayer(PlayerOne)->GetCurrentScore()<<'\n';
		pauseText << "Total Score:" << GetPlayer(PlayerOne)->GetTotalScore();
	}
	
	if (GetPlayer(PlayerOne)->GetCurrentHP() < GetPlayer(PlayerTwo)->GetCurrentHP())
	{
		pauseText << "Player2 Wins" << '\n';
		pauseText << "Notes Hit: " << BeatMan->GetNumberNotesHit() << '\n';
		pauseText << "Current Combo: " << GetPlayer(PlayerTwo)->GetCurrentStreak() << '\n';
		pauseText << "Current TakeDowns: " << GetPlayer(PlayerTwo)->GetCurrentTakeDown() << '\n';
		pauseText << "Current Score:" << GetPlayer(PlayerTwo)->GetCurrentScore()<<'\n';
		pauseText << "Total Score:" << GetPlayer(PlayerOne)->GetTotalScore();
	}

	CBitmapFont::GetInstance()->SetScale(1.0f);
	CBitmapFont::GetInstance()->PrintInRect(pauseText.str(), &rectLayout, 2, D3DCOLOR_XRGB(230, 230, 55));

	if (m_bStartTransition)
	{
		CGameplay_State::GetInstance()->DrawARGB("blackscreen.png", D3DCOLOR_ARGB((int)m_SongTransitionAlpha, 0, 0, 0));

	}
}
const void CLevelManager::Exit(void) {

	if( m_nBackgroundID > -1 )
		TexMan->UnloadTexture(m_nBackgroundID);

	queue<string>::size_type i = 0;
	for(; i < m_vSongs.size(); ++i)
		m_vSongs.pop();
}