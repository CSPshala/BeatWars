/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File Name	:	"CXBOXManager.h"
//
//	Author		:	Aaron Kleker
//
//	Purpose		:	To contain all data and functionality of our XBox Controller input
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

// No MFC
#define WIN32_LEAN_AND_MEAN

// We need the Windows Header and the XInput Header
#include <windows.h>
#include <XInput.h>

// Now, the XInput Library
// NOTE: COMMENT THIS OUT IF YOU ARE NOT USING A COMPILER THAT SUPPORTS THIS METHOD OF LINKING LIBRARIES
#pragma comment(lib, "XInput.lib")

// XBOX Controller Class Definition
class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum;
public:
	CXBOXController(){};
	CXBOXController(int playerNumber);
	XINPUT_STATE GetState();
	bool IsConnected();
	void Vibrate(int leftVal = 0, int rightVal = 0);
	void SetPlayer(int _nPlayerNum){this->_controllerNum = _nPlayerNum;}
};

#endif