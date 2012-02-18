///////////////////////////////////////////////////////
// File Name	:	"CFrame.cpp"
//
// Purpose		:	To Contain a single animation frame's data
//////////////////////////////////////////////////////

#include "CFrame.h"

CFrame::CFrame()
{
	// Members
	m_nAnchorX = 0;
	m_nAnchorY = 0;
	m_fDuration = 0.0f;
	m_nDrawX = 0;
	m_nDrawY = 0;
}

CFrame::~CFrame()
{

}

int CFrame::GetAnchorX()
{
	return m_nAnchorX;
}
int CFrame::GetAnchorY()
{
	return m_nAnchorY;
}
float CFrame::GetDuration()
{
	return m_fDuration;
}
int CFrame::GetDrawX()
{
	return m_nDrawX;
}
int CFrame::GetDrawY()
{
	return m_nDrawY;
}

void CFrame::SetAnchorX(int X)
{
	m_nAnchorX = X;
}
void CFrame::SetanchorY(int Y)
{
	m_nAnchorY = Y;
}
void CFrame::SetDuration(float Dur)
{
	m_fDuration = Dur;
}
void CFrame::SetDrawX(int X)
{
	m_nDrawX = X;
}
void CFrame::SetDrawY(int Y)
{
	m_nDrawY = Y;
}

void CFrame::SetWidth(int nWidth)
{
	m_nWidth = nWidth;
}
void CFrame::SetHeight(int nHeight)
{
	m_nHeight = nHeight;
}

int CFrame::GetWidth()
{

	return m_nWidth;
}

int CFrame::GetHeight()
{
	return m_nHeight;

}