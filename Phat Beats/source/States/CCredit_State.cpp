#include "CCredit_State.h"
#include "CMenu_State.h"


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

}


void CCredit_State::Enter( void )
{
	m_nBackgroundID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MainMenuBG.jpg");
	m_nLogo = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/logo_beatWars_1024.png");
	m_nSoundID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sound/jeditheme.mp3");

	m_vNames.push_back("programmers");
	m_vNames.push_back("arron kleker");
	m_vNames.push_back("jeremy desha");
	m_vNames.push_back("jeffery sventora");
	m_vNames.push_back("joe ricks");
	m_vNames.push_back("ralph concepcion");
	m_vNames.push_back("aps");
	m_vNames.push_back("sean hathaway");
	m_vNames.push_back("shawn paris");
	m_vNames.push_back("logo model and texture");
	m_vNames.push_back("jacob browder");
	m_vNames.push_back("logo modeling");
	m_vNames.push_back("natasha puckett");
	m_vNames.push_back("logo  Caricature");
	m_vNames.push_back("matthew ullrey");
	m_vNames.push_back("animation and game artist");
	m_vNames.push_back("chris jahosky");
	m_vNames.push_back("wapper technology");
	m_vNames.push_back("dave brown");
	m_vNames.push_back("particle effects");
	m_vNames.push_back("jeffery sventora");
	m_vNames.push_back("animation effects");
	m_vNames.push_back("ralph concepcion");
	m_vNames.push_back("beat editor and tech");
	m_vNames.push_back("joe ricks");

}


bool CCredit_State::Input( void )
{
	return true;
}


void CCredit_State::Update()
{

}


void CCredit_State::Render( void )
{
	
}


void CCredit_State::Exit( void )
{

}

