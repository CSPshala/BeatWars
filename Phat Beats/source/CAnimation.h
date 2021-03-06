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
		void SetIsPlaying(bool lPlay);
		void SetName(std::string szName);
		void SetPlayAlready( bool bPlayed ) {m_PlayedAlready = bPlayed;}


		vector<CFrame*> m_vecFrames;

		void SetImageID( int nImage );
		int GetImageID();
		string GetName() {return m_szName;}
		bool GetPlayedAlready() {return m_PlayedAlready; }
		int GetCurrentFrame(){ return m_nCurrFrame; }

		
		void Play();
		void Stop();
		void Reset();
		void Update(float fElapsedTime);
		void Render(int posX, int posY, float fScale);
		
	
	private:
	//*********MEMBERS**********//
		bool m_bIsLooping;
		string m_szName;


		// Asset IDs
		int m_nFrameID;
		int m_nImageID;

		// Player Stuff
		
		bool m_bIsPlaying;
		bool m_PlayedAlready;
		int m_nCurrFrame;
		int m_nMaxFrame;
		float m_fTimeWaited;
};

#endif