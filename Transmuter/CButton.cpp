//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CButton::CButton(CHumanInterface &HI, CPanel &AssociatedPanel) : CSubSession(HI, AssociatedPanel)
	{
	}

void CButton::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{
	//  check if click was in "active area" of button session (80% scale inner rectangle)
	if (IsPointInRect(x, y, m_AssociatedPanel.GetScaledInnerRect(0.8)))
		{
		m_IsLDown = true;
		}
	else
		{
		m_IsLDown = false;
		}

	HIInvalidate();
	}

void CButton::OnLButtonUp(int x, int y, DWORD dwFlags)
	{
	// check if click was in "active area" of button session (80% scale inner rectangle)
	if (IsPointInRect(x, y, m_AssociatedPanel.GetScaledInnerRect(0.8)))
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

void CButton::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	if (IsPointInRect(x, y, m_AssociatedPanel.GetScaledInnerRect(0.8)))
		{
		m_IsRDown = true;
		}
	else
		{
		m_IsRDown = false;
		}
	}

void CButton::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	if (IsPointInRect(x, y, m_AssociatedPanel.GetScaledInnerRect(0.8)))
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

void CButton::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	int width = m_AssociatedPanel.GetWidth();
	int height = m_AssociatedPanel.GetHeight();

	//  paint "active" area of button (80% scaled inner rectangle)
	if (m_IsLDown)
		{
		Screen.Fill(m_AssociatedPanel.GetOriginX() + 0.1*width, m_AssociatedPanel.GetOriginY() + 0.1*height, 0.8*width, 0.8*height, CG32bitPixel(0, 255, 0));
		}
	else
		{
		Screen.Fill(m_AssociatedPanel.GetOriginX() + 0.1*width, m_AssociatedPanel.GetOriginY() + 0.1*height, 0.8*width, 0.8*height, CG32bitPixel(255, 0, 0));
		}
	}