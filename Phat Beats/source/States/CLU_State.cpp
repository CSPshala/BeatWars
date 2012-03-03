#include "CLU_State.h"

#include "../Managers/CFXManager.h"
#include "../Managers/CAnimationManager.h"
#include "../Managers/CBeatManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../CGame.h"
#include "CBitmapFont.h"
#include "CGameplay_State.h"
#include <sstream>

// Proper singleton
CLU_State::CLU_State(const CLU_State&)
{

}
CLU_State& CLU_State::operator=(const CLU_State&)
{
	return *this;
}
CLU_State::~CLU_State()
{

}
CLU_State::CLU_State()
{

}

void CLU_State::Enter(void)
{
	m_nBgImage = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Hoth.jpg");
	m_bAnimComplete = false;
	m_fAlive = 0.0f;
	m_fDotTick = 0.0f;
	m_cNumDots = 0;
}
bool CLU_State::Input(void)
{
	return true;
}
void CLU_State::Update(void)
{
	m_fDotTick += CGame::GetInstance()->GetTimer().GetDeltaTime();

	if(m_fDotTick > 1.0f)
	{
		if(m_cNumDots == 3)
			m_cNumDots = 0;
		else
			++m_cNumDots;

		m_fDotTick = 0.0f;
	}

	switch(CurrentState)
	{
	case Loading:
		{
			if(!Commands.empty())
			{
				// TODO : Load First then pop it
				///////////////////////////////////

				switch(Commands.front()->Type)
				{
				case Effect:
					{
						CFXManager::GetInstance()->LoadFX(Commands.front()->AssetFile, Commands.front()->AssetID);
					}
					break;

				case Animation:
					{
						
					}
					break;

				case Song:
					{
						CBeatManager::GetInstance()->LoadSong(Commands.front()->AssetFile);
					}
					break;
				};

				///////////////////////////////////
				delete Commands.front();
				Commands.front() = nullptr;
				Commands.pop();
				///////////////////////////////////
			}
			else
				CurrentState = Animating;

			// TODO : Update Animation
		}
		break;

	case Unloading:
		{

		}
		break;

	case Animating:
		{
			m_fAlive += CGame::GetInstance()->GetTimer().GetDeltaTime();

			if(m_fAlive > 5.0f)
				m_bAnimComplete = true;

			// TODO : Complete Animation
			if(m_bAnimComplete)
			{
				m_fGameTransitionAlpha += (175 * CGame::GetInstance()->GetTimer().GetDeltaTime());
				if (m_fGameTransitionAlpha >= 255)
				{
					CGame::GetInstance()->ChangeState(m_pNewState);
					m_fGameTransitionAlpha = 1;
				}				
			}
		}
		break;
	};

}
void CLU_State::Render(void)
{
	CSGD_TextureManager::GetInstance()->DrawF(m_nBgImage, 0.0f, 0.0f, 0.78125f, 0.5859375f);
	CBitmapFont::GetInstance()->SetScale(1.5f);

	std::stringstream Dots;

	Dots << "Loading";

	for(char i = 0; i < m_cNumDots; ++i)
		Dots << '.';

	CBitmapFont::GetInstance()->PrintStrokedText(Dots.str(), 10, 10, D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));
	if (m_bAnimComplete)
	{
		CGameplay_State::GetInstance()->DrawARGB("blackscreen.png", D3DCOLOR_ARGB((int)m_fGameTransitionAlpha, 0, 0, 0));
	}
}
void CLU_State::Exit(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nBgImage);
}

CLU_State* CLU_State::GetInstance()
{
	static CLU_State Instance;
	return &Instance;
}

void CLU_State::QueueLoadCommand(std::string szAssetFile, std::string szAssetID, AssetType eType)
{
	LoadCommand* NewCommand = new LoadCommand();
	NewCommand->AssetFile = szAssetFile;
	NewCommand->AssetID = szAssetID;
	NewCommand->Type = eType;
	Commands.push(NewCommand);
}