#ifndef COLORF_H
#define COLORF_H

#include "SGD Wrappers/CSGD_TextureManager.h"

struct tColorF
{
	D3DXVECTOR4 vValues;

	tColorF()
	{
		vValues = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	D3DCOLOR TOD3DCOLOR()
	{
		return D3DCOLOR_COLORVALUE(vValues.x, vValues.y, vValues.z, vValues.w);
	}

	tColorF& operator=(const tColorF& ToEqual)
	{
		vValues = ToEqual.vValues;
		return *this;
	}

	float GR()
	{
		return vValues.x;
	}
	float GG()
	{
		return vValues.y;
	}
	float GB()
	{
		return vValues.z;
	}
	float GA()
	{
		return vValues.w;
	}

	void SR(float fValue)
	{
		vValues.x = fValue;
	}
	void SG(float fValue)
	{
		vValues.y = fValue;
	}
	void SB(float fValue)
	{
		vValues.z = fValue;
	}
	void SA(float fValue)
	{
		vValues.w = fValue;
	}
};

#endif