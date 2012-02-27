#include "CFX.h"

CFX::CFX()
{

}

CFX::~CFX()
{
	std::vector<CEmitter*>::size_type i;
	for(i = 0; i < m_listAliveEmitters.size(); ++i)
	{
		delete m_listAliveEmitters[i];
		m_listAliveEmitters[i] = nullptr;
	}
	m_listAliveEmitters.clear();

	for(i = 0; i < m_listDeadEmitters.size(); ++i)
	{
		delete m_listDeadEmitters[i];
		m_listDeadEmitters[i] = nullptr;
	}
	m_listDeadEmitters.clear();
}

void CFX::Render()
{
	std::vector<CEmitter*>::iterator i;
	for(i = m_listAliveEmitters.begin();i != m_listAliveEmitters.end();++i)
		(*i)->Render();
}
void CFX::Update(float fElapsedTime)
{
	std::vector<CEmitter*>::iterator i;
	for(i = m_listAliveEmitters.begin();i != m_listAliveEmitters.end();++i)
	{
		(*i)->Update(fElapsedTime);

		if(!GetRepeat())
		{
			if((*i)->GetCurrentLife() > (*i)->GetLifeSpan())
			{
				m_listDeadEmitters.push_back((*i));
				i = m_listAliveEmitters.erase(i);

				if(i == m_listAliveEmitters.end())
					break;
			}
		}
	}
}

void CFX::Refresh(void)
{
	std::vector<CEmitter*>::iterator i;

	for(i = m_listDeadEmitters.begin(); i != m_listDeadEmitters.end(); ++i)
		m_listAliveEmitters.push_back((*i));

	m_listDeadEmitters.clear();

	for(i = m_listAliveEmitters.begin(); i != m_listAliveEmitters.end(); ++i)
	{
		(*i)->SetCurrentLife(0.0f);
		(*i)->SetTick(0.0f);
	}
}

void CFX::AddEffect(CEmitter* pEffect)
{
	m_listAliveEmitters.push_back(pEffect);
}

const bool CFX::IsDead(void)
{
	if(GetRepeat())
		return false;

	if(!m_listAliveEmitters.size() && !m_listDeadEmitters.size())
		return true;
	else if(!m_listAliveEmitters.size() && m_listDeadEmitters.size())
		return true;

	return false;

}