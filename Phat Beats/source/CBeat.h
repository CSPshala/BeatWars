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


class CBeat : public CBase
{
public:
	/********** Construct / Deconstruct / OP Overloads ************/
		CBeat();
		~CBeat();
	/********** Public Utility Functions ************/

	/********** Public Accessors ************/
		float	GetTimeOfBeat() {return m_fTimeofBeat;}
		int		GetDifficulty() {return m_nDifficulty;}
	/********** Public Mutators  ************/	
		void	SetTimeOfBeat(float nTime) {m_fTimeofBeat = nTime;}
		void	SetDifficulty(int	nDifficulty) {m_nDifficulty = nDifficulty;}

private:	

	/********** Private Members ************/
		float	m_fTimeofBeat;
		int		m_nDifficulty;
	/********** Private Accessors ************/

	/********** Private Mutators ************/

	/********** Private Utility Functions ************/
	

};

#endif