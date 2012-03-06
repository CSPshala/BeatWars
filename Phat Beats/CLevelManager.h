#ifndef CLEVELMANAGER_H
#define CLEVELMANAGER_H

#include <Windows.h>

#include <queue>
using std::queue;

#include <string>
using std::string;

#include "source/Managers/CAnimationManager.h"

// Forward Declaration
class CBeatManager;
class CFXManager;
class CObjectManager;
class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_FModManager;
class CSGD_TextureManager;
class CPlayer;
enum PlayerIndex : char {PlayerOne, PlayerTwo};

class CLevelManager
{
	// Constructor/Destructor
	CLevelManager(void);
	~CLevelManager(void);

	// State Enum
	enum LevelState : char {Playing = 0, Pausing, Exiting} m_eCurrentState;

	// Data Members
	std::vector<CPlayer*>	m_vPlayers;
	queue<string>			m_vSongs;

	// Easy Access
	CBeatManager*			BeatMan;
	CFXManager*				FxMan;
	CObjectManager*			ObjMan;
	CSGD_Direct3D*			D3DMan;
	CSGD_DirectInput*		InMan;
	CSGD_FModManager*		FmMan;
	CSGD_TextureManager*	TexMan;
	CAnimationManager		AnMan;

	// Asset IDs
	int m_nBgID;
	int m_nBgSfxID;
	int m_nFontID;
	int m_nTitleID;
	int m_nHudID;

	// Offset for Hud
	int m_nRightOffset;
	int m_nLeftOffset;
	int p2PrevHP;
	int p1PrevHP;

	// Asset Data
	RECT rectLeftHandle;
	RECT rectRightHandle;
	RECT rectLeftSaber;
	RECT rectRightSaber;
	RECT rectLeftPowerBar;
	RECT rectRightPowerBar;
	
	// Private Methods
	const LevelState GetState(void){return m_eCurrentState;}
	const void SetState(const LevelState& eNewState){m_eCurrentState = eNewState;}
	const void UpdatePlayingState(const float fElapsedTime);
	const void UpdatePausingState(const float fElapsedTime);
	const void RenderPlayingState(void);
	const void RenderPausingState(void);
	const void HandlePlayingInput(void);
	const void HandlePausingInput(void);
	std::vector<CPlayer*>& PlayerList(void) {return m_vPlayers;};

public:
	
	// Singleton Accessor
	static CLevelManager* GetInstance(void);

	// Methods
	const void QueueSong(const string szSong);
	CPlayer* GetPlayer(const PlayerIndex eIndex);
	const void EnterLevel(void);
	const void LeaveLevel(void);
	const void EmptySongQueue(void) {
		for(queue<string>::size_type i = 0; i < m_vSongs.size(); ++i)
			m_vSongs.pop();
	}

	// Logic Methods
	const void HandleLevelInput(void);
	const void Update(const float fElapsedTime);
	const void Render(void);
	const void Exit(void);

	// Accessors
	queue<string>* GetQueueString() {return &m_vSongs;}
};

#endif