///////////////////////////////////////////////////////
// File Name	:	"CAnimationManager.cpp"
//
// Purpose		:	To manage all loaded animations
//////////////////////////////////////////////////////

#include "CAnimationManager.h"
#include "../CAnimation.h"
#include "../XML/tinystr.h"
#include "../XML/tinyxml.h"
#include "../CFrame.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"


CAnimationManager::CAnimationManager()
{

}

bool CAnimationManager::LoadAnimation(string szFileName)
{
	TiXmlDocument doc;


	// Adding Paths to filename for animations
	string szPath = "resource/animations/";
	szPath += szFileName;

	if( doc.LoadFile(szPath.c_str() ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();

	if( !pRoot )
		return false;

	TiXmlElement* pAnim = pRoot->FirstChildElement("Animation");

	if( pAnim == NULL )
		return false;

	
	while( pAnim )
	{
		CAnimation* Anim = new CAnimation();

		TiXmlElement* pNewAnim = pAnim;
		
		//***************Animation IsLooping**********************//
		if( pNewAnim->Attribute("IsLooping") == NULL )
			return false;

		double dIsLooping = 0;

		pNewAnim->Attribute("IsLooping",&dIsLooping);
		Anim->SetIsLooping((bool)dIsLooping);
		
		//***************Animation File****************************//
		if( pNewAnim->Attribute("File") == NULL )
			return false;

	
		const char* szFileName = NULL;
		szFileName = pNewAnim->Attribute("File");

		char buffer[128] ={0};
		strcpy_s(buffer,_countof(buffer), szFileName);
		string szPath = "resource/graphics/";

		szPath += buffer;

		Anim->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture(szPath.c_str() ) );

		//********************Animation Name*************************//

		if( pNewAnim->Attribute("Name") == NULL )
			return false;
		
		const char* szName;

		szName = pNewAnim->Attribute("Name");
		Anim->SetName(szName);

		TiXmlElement* pFrame = pNewAnim->FirstChildElement("Frames");
		
		if( pFrame == NULL )
			return false;
		
		// Animation Frames
		while( pFrame )
		{
			CFrame* Frame = new CFrame();
			TiXmlElement* pNewFrame = pFrame;
			////*****************Setting Height*********************************//
			
			if( pNewFrame->Attribute("Height") == NULL )
				return false;
			
			double dHeight = 0.0;
			pNewFrame->Attribute("Height",&dHeight);
			Frame->SetHeight((int)dHeight);

			////*******************Setting Width*******************************//
			
			if( pNewFrame->Attribute("Width") == NULL )
				return false;
		
			double dWidth = 0.0;
			pNewFrame->Attribute("Width",&dWidth);
			Frame->SetWidth((int)dWidth);

			//*****************Setting Anchor X*********************************//

			if( pNewFrame->Attribute("AnchorX") == NULL)
				return false;

			double dAnchorX = 0.0;
			pNewFrame->Attribute("AnchorX",&dAnchorX);
			Frame->SetAnchorX((int)dAnchorX);
			
			////*******************Setting Anchor Y*******************************//

			if( pNewFrame->Attribute("AnchorY") == NULL )
				return false;

			double dAnchorY = 0.0;
			pNewFrame->Attribute("AnchorY",&dAnchorY);
			Frame->SetanchorY((int)dAnchorY);

			//*****************Setting Draw X*********************************//

			if( pNewFrame->Attribute("FrameX") == NULL)
				return false;

			double dDrawX = 0.0;
			pNewFrame->Attribute("FrameX",&dDrawX);
			Frame->SetDrawX((int)dDrawX);

			//*******************Setting Draw Y*******************************//

			if( pNewFrame->Attribute("FrameY") == NULL )
				return false;

			double dDrawY = 0.0;
			pNewFrame->Attribute("FrameY",&dDrawY);
			Frame->SetDrawY((int)dDrawY);


			//*******************Setting Duration of Frame*********************//
			if ( pNewFrame->Attribute("Duration") == NULL )
				return false;

			double dDuration = 0.0;

			pNewFrame->Attribute("Duration",&dDuration);
			Frame->SetDuration((float)dDuration);

			//*********************Add frame to vector and go to next*****************//
			
			Anim->m_vecFrames.push_back(Frame);

			pFrame = pFrame->NextSiblingElement("Frames");
		}

		//*******************Add animation to vector and continue*******************//

		m_vecAnimations.push_back(Anim);

		pAnim = pAnim->NextSiblingElement("Animation");
		
	}	


	return true;
}

bool CAnimationManager::UnloadAnimations()
{
	for (int i = m_vecAnimations.size() - 1; i > -1; --i )
	{
		for (int j = m_vecAnimations[i]->m_vecFrames.size() - 1; j > -1; --j )
		{
			delete m_vecAnimations[i]->m_vecFrames[j];
		}

		delete m_vecAnimations[i];
	}
	m_vecAnimations.clear();
	return true;
}

void CAnimationManager::Update(float fElapsedTime)
{
	m_vecAnimations[0]->Update(fElapsedTime);
}
void CAnimationManager::Render()
{
	m_vecAnimations[0]->Render();
}
void CAnimationManager::Play()	
{
	m_vecAnimations[0]->Play();
}
void CAnimationManager::Stop()	
{
	m_vecAnimations[0]->Stop();

}
void CAnimationManager::Reset()	
{
	m_vecAnimations[0]->Reset();

}
