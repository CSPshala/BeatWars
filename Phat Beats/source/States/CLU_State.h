#ifndef CLU_STATE_H
#define CLU_STATE_H

#include "IGameState.h"
#include <string>
#include <queue>

#include "../States/IGameState.h"

enum AssetType : char {Animation, Effect, Song};

class CLU_State : public IGameState
{
	enum LoadState : char {Loading, Unloading, Animating};
	struct LoadCommand
	{
		std::string AssetID;
		std::string AssetFile;
		AssetType Type;
	};

private:
	// Proper singleton
	CLU_State(const CLU_State&);
	//		Assignment op
	CLU_State& operator=(const CLU_State&);
	//		Destructor
	~CLU_State();
	//		Constructor
	CLU_State();

	std::queue<LoadCommand*> Commands;
	LoadState CurrentState;

	// Data Members
	bool m_bAnimComplete;
	IGameState* m_pNewState;
	float m_fAlive;
	float m_fDotTick;
	char m_cNumDots;

	// Asset ID's
	int m_nBgImage;

public:
	void Enter(void); // Enters the Game State
	bool Input(void); // Input
	void Update(void); // Update
	void Render(void); // Draw
	void Exit(void); // Leaves the Game State

	void QueueLoadCommand(std::string szAssetFile, std::string szAssetID, AssetType eType);
	void SetNewState(IGameState* pNewState) {m_pNewState = pNewState;}

	static CLU_State* GetInstance();	
};

#endif