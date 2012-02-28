///////////////////////////////////////////////////////
// File Name	:	"CAnimationManager.h"
//
// Purpose		:	To manage all loaded animations
//////////////////////////////////////////////////////

#ifndef C_ANIMATIONMANAGER_H
#define C_ANIMATIONMANAGER_H

#include <string>
#include <vector>
#include "IListener.h"
#include "CEventSystem.h"
using std::string;
using std::vector;


//Forward Declarations
class CAnimation;

class CAnimationManager : public IListener
{
	public:
	
		bool LoadAnimation(string szFileName, string szImageName);
		bool UnloadAnimations();
		
		vector<CAnimation*> GetAnimations()
		{
			return m_vecAnimations;
		}
	
		void Update(float fElapsedTime,CAnimation* pAnim);
		void Render(CAnimation* pAnim);
		void Play(CAnimation* pAnim);
		void Stop(CAnimation* pAnim);
		void Reset(CAnimation* pAnim);

		void  HandleEvent(CEvent* pEvent);
	
		CAnimationManager();
		~CAnimationManager() { CEventSystem::GetInstance()->UnregisterClient("comboend",this); }
	 
	private:
		vector<CAnimation*> m_vecAnimations;


};

#endif