#pragma once

typedef int MSGID;

//enum eActions { ACT_RUN, ACT_FIRE, JUMP, DUCK };
//enum eElements { ELM_EARTH, WIND, ELM_FIRE, WATER, HEART};

enum eMsgTypes { MSG_NULL = 0,MSG_CREATE_ENEMY, MSG_CREATE_BULLET,MSG_CREATE_EBULLET, MSG_DESTROY_BULLET,MSG_DESTROY_ENEMY,MSG_DESTROY_EBULLET, MSG_MAX };

class CBullet;
class CPlayer;
class CEnemy;
class CEBullet;
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
class CCreateEnemyMsg : public CBaseMessage
{
private:
	float	m_fPosX;
	float	m_fPosY;
	float	m_fVelX;
	float	m_fVelY;
	int		m_nImageID;
	int		m_nWidth;
	int		m_nHeight;
	
public:
	CCreateEnemyMsg(float	fPosX, float fPosY, float fVelX, float fVelY,
		int nImageID, int nWidth, int nHeight) : CBaseMessage(MSG_CREATE_ENEMY)
	{
		m_fPosX		= fPosX;
		m_fPosY		= fPosY;
		m_fVelX		= fVelX;
		m_fVelY		= fVelY;
		m_nImageID	= nImageID;
		m_nWidth	= nWidth;
		m_nHeight	= nHeight;
		
	}

	//	Accessors:
	float	GetPosX(void)	{ return m_fPosX; }
	float	GetPosY(void)	{ return m_fPosY; }
	float	GetVelX(void)	{ return m_fVelX; }
	float	GetVelY(void)	{ return m_fVelY; }
	int		GetImageID(void){ return m_nImageID; }
	int		GetWidth(void)	{ return m_nWidth; }
	int		GetHeight(void)	{ return m_nHeight; }
	
};

class CCreateBulletMessage : public CBaseMessage
{
private:
	CPlayer*	m_pTank;
public:
	CCreateBulletMessage(CPlayer* pTank);
	~CCreateBulletMessage();

	CPlayer* GetTank(void); 
};
//	Forward declaration

class CDestroyBulletMessage : public CBaseMessage
{
private:
	CBullet*	m_pBullet;
public:
	CDestroyBulletMessage(CBullet* pBullet);
	~CDestroyBulletMessage();

	CBullet* GetBullet(void);
};

class CDestroyEnemyMessage : public CBaseMessage
{
private:
	CEnemy* m_pEnemy;
public:
	CDestroyEnemyMessage(CEnemy* pEnemy);
	~CDestroyEnemyMessage();
	CEnemy* GetEnemy(void){return m_pEnemy;}
};

class CCreateEBulletMessage : public CBaseMessage
{
private:
	CEnemy*		m_pEnemyBullet;

public:
	CCreateEBulletMessage(CEnemy*  pEnemyBullet);
	~CCreateEBulletMessage();
	
	CEnemy* GetEnemy(void);

};

class CDestroyEBulletMessage : public CBaseMessage
{
private:
	CEBullet*	m_pEBullet;
public:
	CDestroyEBulletMessage(CEBullet* pEBullet);
	~CDestroyEBulletMessage();

	CEBullet* GetEBullet(void);
};