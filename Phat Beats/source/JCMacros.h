#ifndef _MACROS_H_
#define _MACROS_H_
////////////////////////////////////////////////////////////////////////
// File Name	:	"Macros.h"
//
// Author Name	:	JC Ricks(@CSPshala)
//
// Purpose		:	Macros for overused stuff, mainly singleton access
////////////////////////////////////////////////////////////////////////

#define D3D CSGD_Direct3D::GetInstance()
#define TEXTUREMAN CSGD_TextureManager::GetInstance()
#define FMODMAN CSGD_FModManager::GetInstance()
#define DI CSGD_DirectInput::GetInstance()
#define OM CObjectManager::GetInstance()
#define MS CMessageSystem::GetInstance()
#define ES CEventSystem::GetInstance()
#define GAME CGame::GetInstance()
#define GAMEPLAY CGameplay_State::GetInstance()
#define CLU	CLU_State::GetInstance()
#define BEATMAN CBeatManager::GetInstance()

#endif