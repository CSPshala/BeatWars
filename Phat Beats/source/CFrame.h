///////////////////////////////////////////////////////
// File Name	:	"CFrame.h"
//
// Purpose		:	To Contain a single animation frame's data
//////////////////////////////////////////////////////

#ifndef C_FRAME_H
#define C_FRAME_H

class CFrame
{
public:
	CFrame();
	~CFrame();

	int GetAnchorX();
	int GetAnchorY();
	float GetDuration();
	int GetDrawX();
	int GetDrawY();
	int GetWidth();
	int GetHeight();

	void SetAnchorX(int X);
	void SetanchorY(int Y);
	void SetDuration(float Dur);
	void SetDrawX(int X);
	void SetDrawY(int Y);
	void SetWidth(int nWidth);
	void SetHeight(int nHeight);

private:

	//**********MEMBERS**********//
	int m_nAnchorX;
	int m_nAnchorY;
	float m_fDuration;
	int m_nDrawX;
	int m_nDrawY;
	int m_nHeight;
	int m_nWidth;
};

#endif