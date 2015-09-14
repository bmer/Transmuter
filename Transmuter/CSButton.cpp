//	CSTransmuter.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CSButton::CSButton(CHumanInterface &HI, CPanel &AssociatedPanel) : CSChild(HI, AssociatedPanel)
	{
	}

CSButton::CSButton(CHumanInterface &HI, CPanel &AssociatedPanel, double dActiveAreaScale) : CSChild(HI, AssociatedPanel),
	m_dActiveAreaScale(dActiveAreaScale)
	{
	}

CSButton::CSButton(CHumanInterface &HI, CPanel &AssociatedPanel, double dActiveAreaScale, CG32bitPixel BGColor) : CSChild(HI, AssociatedPanel),
	m_rgbBackgroundColor(BGColor),
	m_dActiveAreaScale(dActiveAreaScale)
	{
	}

void CSButton::UpdateActiveAreaScale (double dNewActiveAreaScale)
	{
	m_dActiveAreaScale = dNewActiveAreaScale;
	}

RECT CSButton::GetActiveAreaRect (void)
	{
	return ScaleRect(m_dActiveAreaScale, m_AssociatedPanel.ScreenArea.GetViewOffsetRect());
	}

void CSButton::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{
	//  check if click was in "active area" of button session (80% scale inner rectangle)
	RECT rcActiveArea = GetActiveAreaRect();
	if (IsPointInRect(x, y, rcActiveArea))
		{
		m_IsLDown = true;
		}
	else
		{
		m_IsLDown = false;
		}

	HIInvalidate();
	}

void CSButton::OnLButtonUp(int x, int y, DWORD dwFlags)
	{
	// check if click was in "active area" of button session (80% scale inner rectangle)
	RECT rcActiveArea = GetActiveAreaRect();
	if (IsPointInRect(x, y, rcActiveArea))
		{
		if (m_IsLDown)
			{
			m_IsLDown = false;
			m_IsLPressed = true;
			}
		else
			{
			m_IsLPressed = false;
			}
		}
	else
		{
		m_IsLDown = false;
		m_IsLPressed = false;
		}

	HIInvalidate();
	}

void CSButton::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	RECT rcActiveArea = GetActiveAreaRect();
	if (IsPointInRect(x, y, rcActiveArea))
		{
		m_IsRDown = true;
		}
	else
		{
		m_IsRDown = false;
		}
	}

void CSButton::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	RECT rcActiveArea = GetActiveAreaRect();
	if (IsPointInRect(x, y, rcActiveArea))
		{
		if (m_IsRDown)
			{
			m_IsRDown = false;
			m_IsRPressed = true;
			}
		else
			{
			m_IsRPressed = false;
			}
		}
	else
		{
		m_IsRDown = false;
		m_IsRPressed = false;
		}
	}

void CSButton::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	RECT rcActiveArea = GetActiveAreaRect();
	if (m_IsLDown)
		{
		Screen.Fill(rcActiveArea.left, rcActiveArea.top, RectWidth(rcActiveArea), RectHeight(rcActiveArea), CG32bitPixel(0, 255, 0));
		}
	else
		{
		Screen.Fill(rcActiveArea.left, rcActiveArea.top, RectWidth(rcActiveArea), RectHeight(rcActiveArea), CG32bitPixel(255, 0, 0));
		}
	}