#pragma once

#include <Windows.h>

class IBaseInterface
{
public:
	virtual ~IBaseInterface(void) = 0 {;}
	virtual void Update(float fElapsedTime) = 0;
	virtual void Render(void) = 0;
	
	virtual void AddRef(void) = 0;
	virtual void Release(void) = 0;

	virtual int GetType(void) = 0;

	virtual RECT GetCollisionRect() = 0;
	virtual bool CheckCollision(IBaseInterface* pBase) = 0;
};