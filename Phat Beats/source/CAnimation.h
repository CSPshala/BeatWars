///////////////////////////////////////////////////////
// File Name	:	"CAnimation.h"
//
// Purpose		:	To Contain all animation related data
//////////////////////////////////////////////////////

#ifndef C_ANIMATION_H
#define C_ANIMATION_H

#include <vector>
using std::vector;

#include <string>
using std::string;

#include "SGD Wrappers\CSGD_Direct3D.h"

// Forward declarations
class CFrame;

class CAnimation
{
	public:
		CAnimation();
		~CAnimation();

		void SetIsLooping(bool bLoop);
		void SetName(std::string szName);


		vector<CFrame*> m_vecFrames;

		void SetImageID( int nImage );
		int GetImageID();

		
		void Play();
		void Stop();
		void Reset();
		void Update(float fElapsedTime);
		void Render();
		
	
	private:
	//*********MEMBERS**********//
		bool m_bIsLooping;
		string m_szName;


		// Asset IDs
		int m_nFrameID;
		int m_nImageID;

		// Player Stuff
		
		bool m_bIsPlaying;
		int m_nCurrFrame;
		int m_nMaxFrame;
		float m_fTimeWaited;
};

#endif