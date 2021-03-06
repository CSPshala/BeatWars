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
class CPlayer;
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
		int				GetTimeOfBeat() {return m_nTimeofBeat;}
		BeatDifficulty	GetDifficulty() {return m_nDifficulty;}
		char			GetKeyToPress() {return m_cKeyToPress;}
		string			GetEvent() {return m_szEvent;}
		BeatDirection	GetDirection() {return m_eDirection;}
		bool			GetHasCollided() {return m_bCollision;}
		bool			GetPlayer1Hit() {return m_bPlayer1Hit;}
		bool			GetPlayer2Hit() {return m_bPlayer2Hit;}
		bool			GetPlayer1Miss() {return m_bPlayer1Miss;}
		bool			GetPlayer2Miss() {return m_bPlayer2Miss;}
	/********** Public Mutators  ************/	
		void	SetTimeOfBeat(int fTime) {m_nTimeofBeat = fTime;}
		void	SetDifficulty(BeatDifficulty nDifficulty) {m_nDifficulty = nDifficulty;}
		void	SetKeyToPress(char	cKey) {m_cKeyToPress = cKey;}
		void    SetEvent(string szEvent) {m_szEvent = szEvent;}
		void	SetDirection(BeatDirection);
		void	SetHasCollided(bool collide) {m_bCollision = collide;}
		void	SetPlayer1Hit(bool hit) {m_bPlayer1Hit = hit;}
		void    SetPlayer2Hit(bool hit) {m_bPlayer2Hit = hit;}
		void	SetPlayer1Miss(bool miss) {m_bPlayer1Miss = miss;}
		void	SetPlayer2Miss(bool miss) {m_bPlayer2Miss = miss;}

private:	

	/********** Private Members ************/
		int		m_nTimeofBeat;
		BeatDifficulty	m_nDifficulty;
		char	m_cKeyToPress;
		string  m_szEvent;
		BeatDirection m_eDirection;

		// For note collision event
		bool	m_bCollision;
		bool    m_bPlayer1Hit;
		bool    m_bPlayer2Hit;
		bool	m_bPlayer1Miss;
		bool	m_bPlayer2Miss;
		

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