//////////////////////////////////////////////////////////////////////////
//	File Name	:	"CFXManager.cpp"
//	
//	Purpose		:	To wrap up data for an effect
///////////////////////////////////////////////////////////////////////////

#include "CFXManager.h"
#include "../XML/tinystr.h"
#include "../XML/tinyxml.h"
#include <sstream>

CFXManager::CFXManager()
{

}

CFXManager::~CFXManager()
{
	UnloadAllFX();
}

const void CFXManager::Render()
{
	std::vector<CFX*>::iterator i;
	for(i = m_listActiveFX.begin();i != m_listActiveFX.end();++i)
		(*i)->Render();	
}

const void CFXManager::Update(float fElapsedTime)
{
	std::vector<CFX*>::iterator i;
	for(i = m_listActiveFX.begin();i != m_listActiveFX.end();++i)
	{
		(*i)->Update(fElapsedTime);

		if((*i)->IsDead())
		{
			i = m_listActiveFX.erase(i);

			if(i == m_listActiveFX.end())
				break;
		}
	}
}

const void CFXManager::AddFX(CFX* pEffect, string szKey)
{
	m_fxTable[szKey] = pEffect;
}

const void CFXManager::LoadFX(string szFileName, string szKey)
{
	TiXmlDocument Doc;

	if(!Doc.LoadFile(szFileName.c_str()))
		return;

	TiXmlElement* pRoot = Doc.RootElement();

	if(!pRoot)
		return;

	TiXmlElement* pEm = pRoot->FirstChildElement();
	CFX* Effect = new CFX();

	std::stringstream Repeat;
	Repeat << pRoot->Attribute("Repeat");
	if(Repeat.str() == "true")
		Effect->SetRepeat(true);
	else
		Effect->SetRepeat(false);

	while(pEm)
	{
		CEmitter* EM = new CEmitter();

		EM->SetName(pEm->Attribute("Name"));

		TiXmlElement* pEl = pEm->FirstChildElement();
		
		EM->SetLifeSpawn((float)atof(pEl->GetText()));
		pEl = pEl->NextSiblingElement();
		EM->SetParticleDurationMin((float)atof(pEl->GetText()));
		pEl = pEl->NextSiblingElement();
		EM->SetParticleDurationMax((float)atof(pEl->GetText()));
		pEl = pEl->NextSiblingElement();
		EM->SetSpawnRate((float)atof(pEl->GetText()));
		pEl = pEl->NextSiblingElement();
		EM->SetMaxParticles(atoi(pEl->GetText()));
		pEl = pEl->NextSiblingElement();
		EM->SetNumParticlesToSpit(atoi(pEl->GetText()));
		pEl = pEl->NextSiblingElement();
		double dGravPull; pEl->Attribute("Pull", &dGravPull);
		double dGravX; pEl->Attribute("X", &dGravX);
		double dGravY; pEl->Attribute("Y", &dGravY);
		EM->SetGravitationalPull((float)dGravPull);
		EM->SetGravityPosition(D3DXVECTOR2((float)dGravX, (float)dGravY));
		//EM->m_fGravitationalPull = (float)atof(pEl->GetText());
		pEl = pEl->NextSiblingElement();
		int nWidth;
		int nHeight;
		pEl->Attribute("Width", &nWidth);
		pEl->Attribute("Height", &nHeight);
		pEl = pEl->NextSiblingElement();
		int nSR; pEl->Attribute("Red", &nSR);
		int nSG; pEl->Attribute("Green", &nSG);
		int nSB; pEl->Attribute("Blue", &nSB);
		int nSA; pEl->Attribute("Alpha", &nSA);
		pEl = pEl->NextSiblingElement();
		int nER; pEl->Attribute("Red", &nER);
		int nEG; pEl->Attribute("Green", &nEG);
		int nEB; pEl->Attribute("Blue", &nEB);
		int nEA; pEl->Attribute("Alpha", &nEA);
		tColorF ColorVal;
		ColorVal.vValues = D3DXVECTOR4(((float)nSR / 255.0f), ((float)nSG / 255.0f), ((float)nSB / 255.0f), ((float)nSA / 255.0f));
		EM->SetStartColor(ColorVal);
		ColorVal.vValues = D3DXVECTOR4(((float)nER / 255.0f), ((float)nEG / 255.0f), ((float)nEB / 255.0f), ((float)nEA / 255.0f));
		EM->SetEndColor(ColorVal);
		pEl = pEl->NextSiblingElement();
		// TODO : BLENDS
		pEl = pEl->NextSiblingElement();
		pEl = pEl->NextSiblingElement();
		double StartX; pEl->Attribute("X", &StartX);
		double StartY; pEl->Attribute("Y", &StartY);
		std::stringstream RandValues;
		RandValues << pEl->Attribute("RandX");
		if(RandValues.str() == "true")
			EM->SetRandStartX(true);
		else
			EM->SetRandStartX(false);

		RandValues.str("");
		RandValues << pEl->Attribute("RandY");
		if(RandValues.str() == "true")
			EM->SetRandStartY(true);
		else
			EM->SetRandStartY(false);

		EM->SetStartVelocity(D3DXVECTOR2((float)StartX, (float)StartY));

		pEl = pEl->NextSiblingElement();
		double EndX; pEl->Attribute("X", &EndX);
		double EndY; pEl->Attribute("Y", &EndY);

		RandValues.str("");
		if(RandValues.str() == "true")
			EM->SetRandEndX(true);
		else
			EM->SetRandEndX(false);

		RandValues.str("");
		RandValues << pEl->Attribute("RandY");
		if(RandValues.str() == "true")
			EM->SetRandEndY(true);
		else
			EM->SetRandEndY(false);

		EM->SetEndVelocity(D3DXVECTOR2((float)EndX, (float)EndY));
		pEl = pEl->NextSiblingElement();
		double StartScale; pEl->Attribute("Start", &StartScale);
		double EndScale; pEl->Attribute("End", &EndScale);

		EM->SetStartScale((float)StartScale);
		EM->SetEndScale((float)EndScale);

		pEl = pEl->NextSiblingElement();
		double StartRotation; pEl->Attribute("Start", &StartRotation);
		double EndRotation; pEl->Attribute("End", &EndRotation);

		EM->SetStartRotation((float)StartRotation);
		EM->SetEndRotation((float)EndRotation);

		TiXmlElement* pTe = pEl->NextSiblingElement();
		while(pTe)
		{
			TiXmlElement* pTeEl = pTe->FirstChildElement();
			std::stringstream FileStream;
			FileStream << "Particle Images\\" << pTeEl->Attribute("File");
			EM->GetTextureList()->push_back(CSGD_TextureManager::GetInstance()->LoadTexture(FileStream.str().c_str()));
			pTe = pTe->NextSiblingElement();
		}
		Effect->AddEffect(EM);
		pEm = pEm->NextSiblingElement();
	}

	m_fxTable[szKey] = Effect;
}

const void CFXManager::UnloadFX(string szKey)
{
	if(m_fxTable[szKey] != nullptr)
	{
		delete m_fxTable[szKey];
		m_fxTable[szKey] = nullptr;
	}
}

const void CFXManager::UnloadAllFX(void)
{
	std::map<string, CFX*>::iterator i;

	for(i = m_fxTable.begin(); i != m_fxTable.end(); ++i)
	{
		delete i->second;
		i->second = nullptr;
	}

	m_fxTable.clear();
}

CFXManager* CFXManager::GetInstance()
{
	static CFXManager Instance;
	return &Instance;
}

const void CFXManager::QueueParticle(string szKey)
{
	if(m_fxTable[szKey] != nullptr)
		m_listActiveFX.push_back(m_fxTable[szKey]);
}

const void CFXManager::DequeueParticle(string szKey)
{
	CFX* pToRemove = m_fxTable[szKey];
	if(pToRemove != nullptr)
	{
		std::vector<CFX*>::iterator i;
		for(i = m_listActiveFX.begin(); i != m_listActiveFX.end(); ++i)
		{
			if((*i) == pToRemove)
			{
				m_listActiveFX.erase(i);
				break;
			}
		}
	}
}