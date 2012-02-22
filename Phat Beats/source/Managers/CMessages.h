#pragma once

typedef int MSGID;

//enum eActions { ACT_RUN, ACT_FIRE, JUMP, DUCK };
//enum eElements { ELM_EARTH, WIND, ELM_FIRE, WATER, HEART};

enum eMsgTypes { MSG_NULL = 0,MSG_TEST,MSG_COLLIDE, MSG_MAX };


class CBaseMessage
{
private:
	MSGID	m_msgID;

public:
	CBaseMessage(MSGID msgID)
	{
		m_msgID = msgID;
	}

	virtual ~CBaseMessage(void) {}

	MSGID GetMsgID(void)	{ return m_msgID; }
};
class CCreateTestMsg : public CBaseMessage
{
private:
	
	
public:
	CCreateTestMsg() : CBaseMessage(MSG_TEST)
	{
			
	}

	//	Accessors:
	
};


