///////////////////////////////////////////////////////
// File Name	:	"CBeat.h"
//
// Author		:	JC Ricks(@CSPshala)
//
// Purpose		:	To contain beat / note info
//////////////////////////////////////////////////////
#ifndef C_BEAT_H
#define C_BEAT_H

////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include "CBase.h"
#include "Managers/IListener.h"
#include "Managers/CEventSystem.h"
#include "Managers/CMessageSystem.h"

////////////////////////////////////////
//		   FORWARD DECLARATIONS
////////////////////////////////////////

////////////////////////////////////////
//				MISC
////////////////////////////////////////
// Enum to define what direction a beat approaches from
enum BeatDirection {LEFT,UP,RIGHT,DOWN,LEFTUP,RIGHTUP,RIGHTDOWN,LEFTDOWN};
enum BeatDifficulty {EASY,NORMAL,HARD};

class CBeat : public CBase, IListener
{
public:
	/********** Construct / Deconstruct / OP Overloads ************/
		CBeat();
		~CBeat();
		CBeat(const CBeat&);
		CBeat& operator=(const CBeat&);
	/********** Public Utility Functions ************/
		void ResetBeat();
		void Update(float fElapsedTime);
		void Render();
		RECT GetCollisionRect();
		void	HandleEvent(CEvent* pEvent);

	/********** Public Accessors ************/
		int			GetTimeOfBeat() {return m_nTimeofBeat;}
		int				GetDifficulty() {return m_nDifficulty;}
		char			GetKeyToPress() {return m_cKeyToPress;}
		string			GetEvent() {return m_szEvent;}
		BeatDirection	GetDirection() {return m_eDirection;}
		bool			GetHasCollided() {return m_bCollision;}
	/********** Public Mutators  ************/	
		void	SetTimeOfBeat(int fTime) {m_nTimeofBeat = fTime;}
		void	SetDifficulty(int	nDifficulty) {m_nDifficulty = nDifficulty;}
		void	SetKeyToPress(char	cKey) {m_cKeyToPress = cKey;}
		void    SetEvent(string szEvent) {m_szEvent = szEvent;}
		void	SetDirection(BeatDirection);
		
private:	

	/********** Private Members ************/
		int		m_nTimeofBeat;
		int		m_nDifficulty;
		char	m_cKeyToPress;
		string  m_szEvent;
		BeatDirection m_eDirection;

		// For note collision event
		bool	m_bCollision;
		

		// For saving original position for reset call
		float m_fOriginalXPos;
		float m_fOriginalYPos;
	/********** Private Accessors ************/
		float GetOriginalXPos() {return m_fOriginalXPos;}
		float GetOriginalYPos() {return m_fOriginalYPos;}
	/********** Private Mutators ************/
		void SetOriginalXPos(float fOGX) {m_fOriginalXPos = fOGX;}
		void SetOriginalYPos(float fOGY) {m_fOriginalYPos = fOGY;}
	/********** Private Utility Functions ************/
	

};

#endif