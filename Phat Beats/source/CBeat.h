///////////////////////////////////////////////////////
// File Name	:	"CBeat.h"
//
// Author		:	JC Ricks
//
// Purpose		:	To contain beat / note info
//////////////////////////////////////////////////////
#ifndef C_BEAT_H
#define C_BEAT_H

////////////////////////////////////////
//				INCLUDES
////////////////////////////////////////
#include "CBase.h"

////////////////////////////////////////
//		   FORWARD DECLARATIONS
////////////////////////////////////////

////////////////////////////////////////
//				MISC
////////////////////////////////////////
// Enum to define what direction a beat approaches from
enum BeatDirection {LEFT,UP,RIGHT,DOWN,LEFTUP,RIGHTUP,RIGHTDOWN,LEFTDOWN};
enum BeatDifficulty {EASY,NORMAL,HARD};

class CBeat : public CBase
{
public:
	/********** Construct / Deconstruct / OP Overloads ************/
		CBeat();
		~CBeat();
		CBeat(const CBeat&);
		CBeat& operator=(const CBeat&);
	/********** Public Utility Functions ************/
		void ResetBeat();

	/********** Public Accessors ************/
		float			GetTimeOfBeat() {return m_fTimeofBeat;}
		int				GetDifficulty() {return m_nDifficulty;}
		char			GetKeyToPress() {return m_cKeyToPress;}
		BeatDirection	GetDirection() {return m_eDirection;}
	/********** Public Mutators  ************/	
		void	SetTimeOfBeat(float fTime) {m_fTimeofBeat = fTime;}
		void	SetDifficulty(int	nDifficulty) {m_nDifficulty = nDifficulty;}
		void	SetKeyToPress(char	cKey) {m_cKeyToPress = cKey;}
		void	SetDirection(BeatDirection);

private:	

	/********** Private Members ************/
		float	m_fTimeofBeat;
		int		m_nDifficulty;
		char	m_cKeyToPress;
		BeatDirection m_eDirection;

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