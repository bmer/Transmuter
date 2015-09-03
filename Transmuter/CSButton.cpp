//	CSTransmuter.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CSButton::CSButton(CHumanInterface &HI, CPanel &AssociatedPanel) : CSChild(HI, AssociatedPanel)
	{
	}

CSButton::CSButton(CHumanInterface &HI, CPanel &AssociatedPanel, RECT rcRelativeButton) : CSChild(HI, AssociatedPanel),
	m_rcActive(rcRelativeButton)
	{
	}

CSButton::CSButton(CHumanInterface &HI, CPanel &AssociatedPanel, RECT rcRelativeButton, CG32bitPixel BGColor) : CSChild(HI, AssociatedPanel),
	m_rgbBackColor(BGColor),
	m_rcActive(rcRelativeButton)
	{
	}

void CSButton::UpdateRelativeButtonRect(RECT rc)
	{
	m_rcActive = rc;
	}

void CSButton::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{
	//  check if click was in "active area" of button session (80% scale inner rectangle)
	if (IsPointInRect(x, y, m_rcActive))
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
	if (IsPointInRect(x, y, m_rcActive))
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
	if (IsPointInRect(x, y, m_rcActive))
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
	if (IsPointInRect(x, y, m_rcActive))
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
	int iPanelWidth = m_AssociatedPanel.GetPanelWidth();
	int iPanelHeight = m_AssociatedPanel.GetPanelHeight();

	//  paint "active" area of button
	if (m_IsLDown)
		{
		Screen.Fill(m_rcActive.left, m_rcActive.top, RectWidth(m_rcActive), RectHeight(m_rcActive), CG32bitPixel(0, 255, 0));
		}
	else
		{
		Screen.Fill(m_rcActive.left, m_rcActive.top, RectWidth(m_rcActive), RectHeight(m_rcActive), CG32bitPixel(255, 0, 0));
		}
	}