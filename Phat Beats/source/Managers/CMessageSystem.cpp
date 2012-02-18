//	CSGD_MessageSystem.cpp

#include "CMessageSystem.h"

CMessageSystem* CMessageSystem::GetInstance(void)
{
	static CMessageSystem instance;
	return &instance;
}

void CMessageSystem::InitMessageSystem(MESSAGEPROC pfnMsgProc)
{
	//	Error check to make sure the message proc is valid.
	if (!pfnMsgProc)	return;

	//	Get the Msg Proc
	m_pfnMsgProc = pfnMsgProc;
}

void CMessageSystem::SendMsg(CBaseMessage* pMsg)
{
	//	Make sure the message exists.
	if (!pMsg)	return;

	//	Send the message to the queue for processing later on.
	m_MsgQueue.push(pMsg);
}

void CMessageSystem::ProcessMessages(void)
{
	//	Error check to make sure we get a message proc.
	if(!m_pfnMsgProc)	return;

	//	go through the entire queue and process the messsages that are waiting.
	while(!m_MsgQueue.empty())
	{
		m_pfnMsgProc(m_MsgQueue.front());		//	Process the first message.
		delete m_MsgQueue.front();				//	Delete the message memory.
		m_MsgQueue.pop();						//	Go to the next message.
	}
}

void CMessageSystem::ClearMessages(void)
{
	//	Clear out any messages waititng.
	while(!m_MsgQueue.empty())
	{
		delete m_MsgQueue.front();
		m_MsgQueue.pop();
	}
}

void CMessageSystem::ShutdownMessageSystem(void)
{
	//	Clear out any messages waiting.
	ClearMessages();

	//	Clear the function pointer
	m_pfnMsgProc = NULL;
}