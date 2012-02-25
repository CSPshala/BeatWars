///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CEmitter.cpp"
//	
//	Purpose		:	To hold data for an emitter
///////////////////////////////////////////////////////////////////////////

#include "CEmitter.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "Random.h"

CEmitter::CEmitter()
{
	m_szName = "";
	m_rectRange.left = 384;
	m_rectRange.top = 384;
	m_rectRange.right = 384 + 32;
	m_rectRange.bottom = 384 + 32;
	m_tGravityPos = D3DXVECTOR2(0.0f, 0.0f);
	m_fGravitationalPull = 0.0f;
	m_fParticleDurationMin = 0.5f;
	m_fPatricleDurationMax = 0.5f;
	m_fLifeSpan = 5.0f;
	m_nNumParticlesToSpit = 1;
	m_nMaxParticles = 30;
	m_tStartColor.vValues = D3DXVECTOR4(1.0f, 0.8f, 0.0f, 1.0f);
	m_tEndColor.vValues = D3DXVECTOR4(0.65f, 0.1f, 0.1f, 1.0f);
	m_vStartVelocity = D3DXVECTOR2(0.0f, -200.0f);
	m_vEndVelocity = D3DXVECTOR2(0.0f, 0.0f);
	m_fStartScale = 1.0f;
	m_fEndScale = 0.0f;
	m_fStartRotation = 0.0f;
	m_fEndRotation = 0.0f;
	m_fCurLife = 0.0f;
	m_d3dSource = D3DBLEND_SRCALPHA;
	m_d3dDestination = D3DBLEND_ONE;
	m_fUpdate = 0.0f;

	for(short i = 0; i < m_nMaxParticles; ++i)
		m_ListDeadParticles.push_back(new CParticle());
}

CEmitter::~CEmitter()
{
	std::vector<CParticle*>::size_type i;

	for(i = 0; i < m_ListAliveParticles.size(); ++i)
	{
		delete m_ListAliveParticles[i];
		m_ListAliveParticles[i] = nullptr;
	}

	m_ListAliveParticles.clear();

	for(i = 0; i < m_ListDeadParticles.size(); ++i)
	{
		delete m_ListDeadParticles[i];
		m_ListDeadParticles[i] = nullptr;
	}

	m_ListDeadParticles.clear();
}

void CEmitter::RecycleParticle()
{
	if(m_ListDeadParticles.size() > 0)
	{
		int index = (int)m_ListAliveParticles.size();
		m_ListAliveParticles.push_back(m_ListDeadParticles[0]);
		m_ListDeadParticles.erase(m_ListDeadParticles.begin());
		
		m_ListAliveParticles[index]->SetPosition(D3DXVECTOR2((float)Random::Next(GetRange().left, GetRange().right), (float)Random::Next(GetRange().top, GetRange().bottom)));
		int imgindex = rand() % m_vTextureList.size();
		m_ListAliveParticles[index]->SetImageID(m_vTextureList[imgindex]);
		m_ListAliveParticles[index]->SetVelocity(m_vStartVelocity);
		m_ListAliveParticles[index]->SetColor(m_tStartColor);
		m_ListAliveParticles[index]->SetRotation(m_fStartRotation);
		m_ListAliveParticles[index]->SetScale(m_fStartScale);
		m_ListAliveParticles[index]->SetDuration((float)Random::NextDouble(GetParticleDurationMin(), GetParticleDurationMax()));
		m_ListAliveParticles[index]->SetCurrentLife(0.0f);

		// Set up the deltas
		m_ListAliveParticles[index]->SetColorDelta(R, m_tEndColor.GR() - m_tStartColor.GR());
		m_ListAliveParticles[index]->SetColorDelta(G, m_tEndColor.GG() - m_tStartColor.GG());
		m_ListAliveParticles[index]->SetColorDelta(B, m_tEndColor.GB() - m_tStartColor.GB());
		m_ListAliveParticles[index]->SetColorDelta(A, m_tEndColor.GA() - m_tStartColor.GA());

		m_ListAliveParticles[index]->SetRotationDelta(m_fEndRotation - m_fStartRotation);
		m_ListAliveParticles[index]->SetScaleDelta(m_fEndScale - m_fStartScale);


		D3DXVECTOR2 RandStartVel, RandEndVel;

		if(m_bRandStartX)
			RandStartVel.x = (float)Random::Next(-300, 300);
		else
			RandStartVel.x = m_vStartVelocity.x;

		if(m_bRandStartY)
			RandStartVel.y = (float)Random::Next(-300, 300);
		else
			RandStartVel.y = m_vStartVelocity.y;

		if(m_bRandEndX)
			RandEndVel.x = (float)Random::Next(-300, 300);
		else
			RandEndVel.x = m_vEndVelocity.x;

		if(m_bRandEndY)
			RandEndVel.y = (float)Random::Next(-300, 300);
		else
			RandEndVel.y = m_vEndVelocity.y;

		m_ListAliveParticles[index]->SetVelocityDelta(D3DXVECTOR2(RandEndVel.x - RandStartVel.x, RandEndVel.y - RandStartVel.y));
	}
}
void CEmitter::Update(float fElapsedTime)
{
	m_fCurLife += fElapsedTime;
	m_fUpdate += fElapsedTime;

	if(m_vTextureList.size())
	{
		std::vector<CParticle*>::iterator i;

		if(m_ListAliveParticles.size() > 0)
		{
			for(i = m_ListAliveParticles.begin();i != m_ListAliveParticles.end(); ++i)
			{
				(*i)->Update(fElapsedTime, m_tGravityPos, m_fGravitationalPull);

				if((*i)->GetCurrentLife() > (*i)->GetDuration())
				{
					m_ListDeadParticles.push_back((*i));
					i = m_ListAliveParticles.erase(i);

					if(i == m_ListAliveParticles.end())
						break;
				}
			}

		}

		if(m_fUpdate > m_fSpawnRate)
		{
			for(int i = 0; i < m_nNumParticlesToSpit; ++i)
			{
				if((short)m_ListAliveParticles.size() < m_nMaxParticles)
					RecycleParticle();
				else
					break;
			}

			m_fUpdate -= m_fSpawnRate;
		}
	}
}

void CEmitter::Render()
{
	std::vector<CParticle*>::iterator i;

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, m_d3dSource);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, m_d3dDestination);
	for(i = m_ListAliveParticles.begin(); i != m_ListAliveParticles.end(); ++i)
	{
		(*i)->Render(m_vTextureList);
	}
}

const void CEmitter::SetMaxParticles(const short nNewMaxParticles)
{
	m_nMaxParticles = nNewMaxParticles;

	std::vector<CParticle*>::size_type i;

	for(i = 0; i < m_ListAliveParticles.size(); ++i)
	{
		delete m_ListAliveParticles[i];
		m_ListAliveParticles[i] = nullptr;
	}

	m_ListAliveParticles.clear();

	for(i = 0; i < m_ListDeadParticles.size(); ++i)
	{
		delete m_ListDeadParticles[i];
		m_ListDeadParticles[i] = nullptr;
	}

	m_ListDeadParticles.clear();

	for(short n = 0; n < GetMaxParticles(); ++n)
		m_ListDeadParticles.push_back(new CParticle());
}