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
//#define MS CSGD_MessageSystem::GetInstance()
//#define ES CSGD_EventSystem::GetInstance()
#define GAME CGame::GetInstance()

#endif