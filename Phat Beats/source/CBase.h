///////////////////////////////////////////////////////////////////////////
//	File Name	:	"CBase.h"
//	
//	Purpose		:	To contain all related data and
//					functionality for our game objects.
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "IBaseInterface.h"
#include "SGD_Math.h"
enum ObjType{OBJ_BASE,OBJ_PLAYER = 0,OBJ_NOTE,OBJ_MAX};
class CBase : public IBaseInterface
{
private:
	unsigned int m_uiRefCount;	//	how many objs have a ptr to me
	//	position
	float m_nPosX;
	float m_nPosY;
	//	velocity
	float m_nVelX;
	float m_nVelY;
	
	int m_nWidth;
	int m_nHeight;
	//	Image
	int m_nImageID;

	bool m_bIsActive;

	

protected:
	int m_nType;

public:
	CBase(void);
	virtual ~CBase(void);
	CBase(const CBase&);
	CBase& operator=(const CBase&);

	///////////////////////////////////////////////////////////////////////
	//	Function:	Accessors
	//	
	//	Purpose :	To get the specified type.
	///////////////////////////////////////////////////////////////////////
	float GetPosX(void)	{return m_nPosX;}
	float GetPosY(void)	{return m_nPosY;}
	float GetVelX(void)	{return m_nVelX;}
	float GetVelY(void)	{return m_nVelY;}
	int GetImageID(void)	{return m_nImageID;}
	bool GetIsActive(void)	{return m_bIsActive;}
	int GetWidth(void)	{return m_nWidth;}
	int GetHeight(void)	{return m_nHeight;}
	///////////////////////////////////////////////////////////////////////
	//	Function:	Modifiers
	//
	//	Purpose	:	To set the specified type
	///////////////////////////////////////////////////////////////////////
	void SetPosX(float nPosX)	{m_nPosX = nPosX;}
	void SetPosY(float nPosY)	{m_nPosY = nPosY;}
	void SetVelX(float nVelX)	{m_nVelX = nVelX;}
	void SetVelY(float nVelY)	{m_nVelY = nVelY;}
	void SetImageID(int nImageID)	{m_nImageID = nImageID;}
	void SetIsActive(bool bIsActive)	{m_bIsActive = bIsActive;}
	void SetWidth(int nWidth) {m_nWidth = nWidth;}
	void SetHeight(int nHeight) {m_nHeight = nHeight;}	
	//	Input	
	//	Update
	virtual void Update(float fElapsedTime);
	//	Draw
	virtual void Render(void);

	void AddRef(void)
	{
		m_uiRefCount++;
	}
	void Release(void)
	{
		m_uiRefCount--;
		if(m_uiRefCount == 0)
			delete this;
	}
	int GetType(void) {return m_nType;}

	virtual RECT GetCollisionRect();
	virtual bool CheckCollision(IBaseInterface* pBase);
};