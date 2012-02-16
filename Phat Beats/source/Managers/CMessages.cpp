#include "Messages.h"
#include "../../CBullet.h"
#include "../../CPlayer.h"
#include "../../CEnemy.h"
#include "../../CEBullet.h"
CDestroyBulletMessage::CDestroyBulletMessage(CBullet* pBullet) : CBaseMessage( MSG_DESTROY_BULLET )
{
	m_pBullet = pBullet;
	m_pBullet->AddRef();

}

CBullet* CDestroyBulletMessage::GetBullet(void)
{
	return m_pBullet;
}


CDestroyBulletMessage::~CDestroyBulletMessage()
{
	m_pBullet->Release();
}


CCreateBulletMessage::CCreateBulletMessage(CPlayer* pTank) : CBaseMessage(MSG_CREATE_BULLET)
{
	m_pTank = pTank;
	m_pTank->AddRef();
}

CCreateBulletMessage::~CCreateBulletMessage()
{

	m_pTank->Release();
}

CPlayer* CCreateBulletMessage::GetTank(void)
{
	return m_pTank;
}


CCreateEBulletMessage::CCreateEBulletMessage(CEnemy* pEnemyBullet) : CBaseMessage(MSG_CREATE_EBULLET)
{
	m_pEnemyBullet = pEnemyBullet;
	m_pEnemyBullet->AddRef();
}

CCreateEBulletMessage::~CCreateEBulletMessage()
{
	m_pEnemyBullet->Release();
}

CEnemy* CCreateEBulletMessage::GetEnemy( void )
{
	return m_pEnemyBullet;
}

CDestroyEnemyMessage::CDestroyEnemyMessage( CEnemy* pEnemy ):CBaseMessage(MSG_DESTROY_ENEMY)
{
	m_pEnemy = pEnemy;
	pEnemy->AddRef();
}

CDestroyEnemyMessage::~CDestroyEnemyMessage()
{
	m_pEnemy->Release();
}


CDestroyEBulletMessage::CDestroyEBulletMessage(CEBullet* pEBullet) : CBaseMessage( MSG_DESTROY_EBULLET )
{
	m_pEBullet = pEBullet;
	m_pEBullet->AddRef();

}

CEBullet* CDestroyEBulletMessage::GetEBullet(void)
{
	return m_pEBullet;
}


CDestroyEBulletMessage::~CDestroyEBulletMessage()
{
	m_pEBullet->Release();
}
