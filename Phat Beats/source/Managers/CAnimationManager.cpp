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

vector<CAnimation*> CAnimationManager::LoadAnimation(string szFileName, string szImageName)
{
	vector<CAnimation*> m_vecAnimations;

	TiXmlDocument doc;
	// Adding Paths to filename for animations
	string szPath = "resource/animations/";
	szPath += szFileName;

	string szImagePath = "resource/graphics/";
	szImagePath += szImageName;

	do
	{
		if( doc.LoadFile(szPath.c_str() ) == false )
			break;

		TiXmlElement* pRoot = doc.RootElement();

		if( !pRoot )
			break;

		TiXmlElement* pAnim = pRoot->FirstChildElement("Animation");

		if( pAnim == NULL )
			break;


		while( pAnim )
		{
			
			CAnimation* Anim = new CAnimation();
			TiXmlElement* pNewAnim = pAnim;

			//***************Animation IsLooping**********************//
			if( pNewAnim->Attribute("IsLooping") == NULL )
				break;

			int dIsLooping = 0;

			pNewAnim->Attribute("IsLooping", &dIsLooping);

			if( dIsLooping != 0 )
				Anim->SetIsLooping(true);
			else
				Anim->SetIsLooping(false);
	

			Anim->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture(szImagePath.c_str() ) );

			//********************Animation Name*************************//

			if( pNewAnim->Attribute("Name") == NULL )
				break;

			const char* szName;

			szName = pNewAnim->Attribute("Name");
			Anim->SetName(szName);

			TiXmlElement* pFrame = pNewAnim->FirstChildElement("Frames");

			if( pFrame == NULL )
				break;

			// Animation Frames
			while( pFrame )
			{
				CFrame* Frame = new CFrame();
				TiXmlElement* pNewFrame = pFrame;
				////*****************Setting Height*********************************//

				if( pNewFrame->Attribute("Height") == NULL )
					break;

				double dHeight = 0.0;
				pNewFrame->Attribute("Height",&dHeight);
				Frame->SetHeight((int)dHeight);

				////*******************Setting Width*******************************//

				if( pNewFrame->Attribute("Width") == NULL )
					break;

				double dWidth = 0.0;
				pNewFrame->Attribute("Width",&dWidth);
				Frame->SetWidth((int)dWidth);

				//*****************Setting Anchor X*********************************//

				if( pNewFrame->Attribute("AnchorX") == NULL)
					break;

				double dAnchorX = 0.0;
				pNewFrame->Attribute("AnchorX",&dAnchorX);
				Frame->SetAnchorX((int)dAnchorX);

				////*******************Setting Anchor Y*******************************//

				if( pNewFrame->Attribute("AnchorY") == NULL )
					break;

				double dAnchorY = 0.0;
				pNewFrame->Attribute("AnchorY",&dAnchorY);
				Frame->SetanchorY((int)dAnchorY);

				//*****************Setting Draw X*********************************//

				if( pNewFrame->Attribute("FrameX") == NULL)
					break;

				double dDrawX = 0.0;
				pNewFrame->Attribute("FrameX",&dDrawX);
				Frame->SetDrawX((int)dDrawX);

				//*******************Setting Draw Y*******************************//

				if( pNewFrame->Attribute("FrameY") == NULL )
					break;

				double dDrawY = 0.0;
				pNewFrame->Attribute("FrameY",&dDrawY);
				Frame->SetDrawY((int)dDrawY);


				//*******************Setting Duration of Frame*********************//
				if ( pNewFrame->Attribute("Duration") == NULL )
					break;

				double dDuration = 0.0;

				pNewFrame->Attribute("Duration",&dDuration);
				Frame->SetDuration((float)dDuration);

				//*********************Setting Event**********************************//
				if( pNewFrame->Attribute("Event") == NULL)
				{

				}
				else
				{

					const char* szEvent = NULL;
					char buffer[128] = {0};

					szEvent = pNewFrame->Attribute("Event");

					strcpy_s(buffer,_countof(buffer),szEvent);
					Frame->SetEvent(buffer);
				}

				//*********************Add frame to vector and go to next*****************//

				Anim->m_vecFrames.push_back(Frame);

				pFrame = pFrame->NextSiblingElement("Frames");
			}

			//*******************Add animation to vector and continue*******************//

			m_vecAnimations.push_back(Anim);

			pAnim = pAnim->NextSiblingElement("Animation");

		}	
		break;
	}while(true);

	return m_vecAnimations;
}

bool CAnimationManager::UnloadAnimations(vector<CAnimation*> vecAnim)
{
	for (int i = vecAnim.size() - 1; i > -1; --i )
	{
		for (int j = vecAnim[i]->m_vecFrames.size() - 1; j > -1; --j )
		{
			delete vecAnim[i]->m_vecFrames[j];
		}
		CSGD_TextureManager::GetInstance()->UnloadTexture(vecAnim[i]->GetImageID());
		delete vecAnim[i];
	}
	vecAnim.clear();
	return true;
}

CAnimation* CAnimationManager::LoadSingleAnimation(string szFileName, string szImageName)
{
	
	CAnimation* Anim = new CAnimation();

	TiXmlDocument doc;
	// Adding Paths to filename for animations
	string szPath = "resource/animations/";
	szPath += szFileName;

	string szImagePath = "resource/graphics/";
	szImagePath += szImageName;

	do
	{
		if( doc.LoadFile(szPath.c_str() ) == false )
			break;

		TiXmlElement* pRoot = doc.RootElement();

		if( !pRoot )
			break;

		TiXmlElement* pAnim = pRoot->FirstChildElement("Animation");

		if( pAnim == NULL )
			break;


		while( pAnim )
		{

			TiXmlElement* pNewAnim = pAnim;

			//***************Animation IsLooping**********************//
			if( pNewAnim->Attribute("IsLooping") == NULL )
				break;

			int dIsLooping = 0;

			pNewAnim->Attribute("IsLooping", &dIsLooping);

			if( dIsLooping != 0 )
				Anim->SetIsLooping(true);
			else
				Anim->SetIsLooping(false);
	

			Anim->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture(szImagePath.c_str() ) );

			//********************Animation Name*************************//

			if( pNewAnim->Attribute("Name") == NULL )
				break;

			const char* szName;

			szName = pNewAnim->Attribute("Name");
			Anim->SetName(szName);

			TiXmlElement* pFrame = pNewAnim->FirstChildElement("Frames");

			if( pFrame == NULL )
				break;

			// Animation Frames
			while( pFrame )
			{
				CFrame* Frame = new CFrame();
				TiXmlElement* pNewFrame = pFrame;
				////*****************Setting Height*********************************//

				if( pNewFrame->Attribute("Height") == NULL )
					break;

				double dHeight = 0.0;
				pNewFrame->Attribute("Height",&dHeight);
				Frame->SetHeight((int)dHeight);

				////*******************Setting Width*******************************//

				if( pNewFrame->Attribute("Width") == NULL )
					break;

				double dWidth = 0.0;
				pNewFrame->Attribute("Width",&dWidth);
				Frame->SetWidth((int)dWidth);

				//*****************Setting Anchor X*********************************//

				if( pNewFrame->Attribute("AnchorX") == NULL)
					break;

				double dAnchorX = 0.0;
				pNewFrame->Attribute("AnchorX",&dAnchorX);
				Frame->SetAnchorX((int)dAnchorX);

				////*******************Setting Anchor Y*******************************//

				if( pNewFrame->Attribute("AnchorY") == NULL )
					break;

				double dAnchorY = 0.0;
				pNewFrame->Attribute("AnchorY",&dAnchorY);
				Frame->SetanchorY((int)dAnchorY);

				//*****************Setting Draw X*********************************//

				if( pNewFrame->Attribute("FrameX") == NULL)
					break;

				double dDrawX = 0.0;
				pNewFrame->Attribute("FrameX",&dDrawX);
				Frame->SetDrawX((int)dDrawX);

				//*******************Setting Draw Y*******************************//

				if( pNewFrame->Attribute("FrameY") == NULL )
					break;

				double dDrawY = 0.0;
				pNewFrame->Attribute("FrameY",&dDrawY);
				Frame->SetDrawY((int)dDrawY);


				//*******************Setting Duration of Frame*********************//
				if ( pNewFrame->Attribute("Duration") == NULL )
					break;

				double dDuration = 0.0;

				pNewFrame->Attribute("Duration",&dDuration);
				Frame->SetDuration((float)dDuration);

				//*********************Setting Event**********************************//
				if( pNewFrame->Attribute("Event") == NULL)
				{

				}
				else
				{

					const char* szEvent = NULL;
					char buffer[128] = {0};

					szEvent = pNewFrame->Attribute("Event");

					strcpy_s(buffer,_countof(buffer),szEvent);
					Frame->SetEvent(buffer);
				}

				//*********************Add frame to vector and go to next*****************//

				Anim->m_vecFrames.push_back(Frame);

				pFrame = pFrame->NextSiblingElement("Frames");
			}

			//*******************Add animation to vector and continue*******************//
			
			pAnim = pAnim->NextSiblingElement("Animation");

		}	
		break;

	}while(true);

	return Anim;
}
