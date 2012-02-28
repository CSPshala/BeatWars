///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CFX.h"
//	
//	Purpose		:	To wrap up data for an effect
///////////////////////////////////////////////////////////////////////////

#ifndef C_FX_H
#define C_FX_H

#include "CEmitter.h"

class CFX
{
public:
	CFX();
	~CFX();

	void Render();
	void Update(float fElapsedTime);
	void AddEffect(CEmitter* pEffect);
	void Refresh(void);
	const bool IsDead(void);
	const bool GetRepeat(void) {return m_bRepeat;}
	const void SetRepeat(const bool bNewRepeat) {m_bRepeat = bNewRepeat;}
	const void MoveEffect(const D3DXVECTOR2 tNewPos);

private:

	//*****MEMBERS******//
	std::vector<CEmitter*> m_listAliveEmitters;
	std::vector<CEmitter*> m_listDeadEmitters;
	bool m_bRepeat;
};

#endif