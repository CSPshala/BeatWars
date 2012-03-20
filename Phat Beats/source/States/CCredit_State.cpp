#include "CCredit_State.h"
#include "CMenu_State.h"
#include "../SGD Wrappers/CSGD_FModManager.h"
#include <DxErr.h>
#include "CHighScoreState.h"

#pragma comment(lib, "dxerr.lib")

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

CCredit_State* CCredit_State::GetInstance()
{
	static CCredit_State Instance;
	return &Instance;
}


CCredit_State::CCredit_State()
{
	m_bFont = NULL;
	m_nSoundID = -1;
	m_nBackgroundID = -1;
	m_nLogo = -1;
}


CCredit_State::~CCredit_State()
{
	SAFE_RELEASE( quadbuff );
	SAFE_RELEASE( cubedecl );
}


void CCredit_State::Enter( void )
{
	if( !CSGD_FModManager::GetInstance()->IsSoundPlaying(m_nSoundID))
		CSGD_FModManager::GetInstance()->PlaySound(m_nSoundID);

	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MainMenuBG.jpg");
	m_nLogo = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/logo_beatWars_1024.png");
	m_nSoundID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sound/Star_Wars_-_Main_Title_Theme.mp3",FMOD_LOOP_NORMAL);

	//m_nSoundID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sound/jeditheme.mp3");

	HRESULT HR = CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->CreateTexture(512, 1024, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RtText, NULL);

	m_TexCredits = LoadTexture("resource/graphics/Credits.png");

	struct CUSTOMVERTEX t_vert[] =
	{
		{5, 0, -15, 0xffffffff, 1, 1,},
		{-5, 0, -15, 0xffffffff, 0, 1,},
		{5, 0, 0, 0xffffffff, 1, 0,},
		{-5, 0, 0, 0xffffffff, 0, 0,},
	};

     // create a vertex buffer interface called t_buffer
     CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->CreateVertexBuffer(4*sizeof(CUSTOMVERTEX),
                                0,
                                CUSTOMFVF,
                                D3DPOOL_MANAGED,
                                &quadbuff,
                                NULL);

	 // CREATE INDEX BUFFER FOR QUAD
	 VOID* pVoid;    // a void pointer

     // lock t_buffer and load the vertices into it
     quadbuff->Lock(0, 0, (void**)&pVoid, 0);
     memcpy(pVoid, t_vert, sizeof(t_vert));
     quadbuff->Unlock();
}


bool CCredit_State::Input( void )
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_BACKSPACE))
		CGame::GetInstance()->GoBack();

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN))
		CGame::GetInstance()->ChangeState(CHighScoreState::GetInstance());

	return true;
}


void CCredit_State::Update()
{
	//m_nSwitchState++;
	//if (m_nSwitchState == 160000)
	//{
	//	CGame::GetInstance()->ChangeState(CHighScoreState::GetInstance());
	//}
}


void CCredit_State::Render( void )
{
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetFVF(CUSTOMFVF);

	IDirect3DSurface9* BackBuffer;
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderTarget(0, &BackBuffer);

	CSGD_Direct3D::GetInstance()->GetSprite()->End();
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->EndScene();
	
	IDirect3DSurface9* RT;
	m_RtText->GetSurfaceLevel(0, &RT);

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderTarget(0, RT);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();

	CSGD_Direct3D::GetInstance()->Clear(0, 0, 0);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	// Draw Shit HERE
	RECT Source = {0, 0, 512, 1024};
	CSGD_Direct3D::GetInstance()->GetSprite()->Draw(m_TexCredits, &Source, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->DeviceEnd();

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderTarget(0, BackBuffer);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();
	CSGD_Direct3D::GetInstance()->Clear(0, 0, 0);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	D3DXMATRIX Identity;
	D3DXMATRIX Trans;
	D3DXMATRIX Rot;
	D3DXMATRIX Combined;

	 // set the view transform
     D3DXMATRIX matView;    // the view transform matrix
     D3DXMatrixLookAtLH(&matView,
     &D3DXVECTOR3 (0.0f, 8.0f, 5.0f),    // the camera position
     &D3DXVECTOR3 (0.0f, 3.0f, 15.0f),      // the look-at position
     &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction
     CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

    // set the projection transform
     D3DXMATRIX matProjection;    // the projection transform matrix
     D3DXMatrixPerspectiveFovLH(&matProjection,
                                D3DXToRadian(45),    // the horizontal field of view
                                (FLOAT)800 / (FLOAT)600, // aspect ratio
                                1.0f,   // the near view-plane
                                100.0f);    // the far view-plane

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTransform(D3DTS_PROJECTION, &matProjection);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, quadbuff, 0, sizeof(CUSTOMVERTEX));
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTexture(0, m_RtText);

    D3DXMATRIX matRotateY;
	D3DXMATRIX trans;
	static float change = 0.0f; change += CGame::GetInstance()->GetTimer().GetDeltaTime() * 1.0f;
	D3DXMatrixTranslation(&trans, 0.0f, 0.0f, change);
	D3DXMatrixRotationX(&matRotateY, D3DXToRadian(0.0f));
    CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTransform(D3DTS_WORLD, &(matRotateY * trans));    // set the world transform
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}


void CCredit_State::Exit( void )
{
	if(m_RtText)
	{
		m_RtText->Release();
		m_RtText = nullptr;
	}

	if(m_TexCredits)
	{
		m_TexCredits->Release();
		m_TexCredits = nullptr;
	}

	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nBackgroundID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nLogo);

	for (unsigned int i = 0; i < m_vNames.size(); ++i)
	{
		m_vNames[i].clear();
	}

	
	CSGD_FModManager::GetInstance()->UnloadSound(m_nSoundID);

}

LPDIRECT3DTEXTURE9 CCredit_State::LoadTexture(const char* szFilename, DWORD dwColorkey)
{
		//	Make sure the filename is valid.
	if (!szFilename)	return NULL;

	LPDIRECT3DTEXTURE9 loaded;

	// Load the texture from the given file.
	HRESULT hr = 0;
	if (FAILED(hr = D3DXCreateTextureFromFileEx(CSGD_Direct3D::GetInstance()->GetDirect3DDevice(), szFilename, 0, 0, D3DX_DEFAULT, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, dwColorkey, 0, 0, &loaded)))
	{
		// Failed.
		char szBuffer[256] = {0};
		sprintf_s(szBuffer,256, "Failed to Create Texture - %s", szFilename); 
		MessageBox(0, szBuffer, "TextureManager Error", MB_OK);
		return NULL;
	};

	return loaded;
}
