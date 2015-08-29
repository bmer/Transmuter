//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CButton::CButton(CHumanInterface &HI, CPanel &AssociatedPanel) : CSubSession(HI, AssociatedPanel)
	{
	}

CButton::CButton(CHumanInterface &HI, CPanel &AssociatedPanel, CG32bitPixel BGColor) : CSubSession(HI, AssociatedPanel),
	m_rgbBackColor(BGColor)
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
	int iPanelWidth = m_AssociatedPanel.GetPanelWidth();
	int iPanelHeight = m_AssociatedPanel.GetPanelHeight();

	//  paint "active" area of button (80% scaled inner rectangle)
	if (m_IsLDown)
		{
		Screen.Fill(int(m_AssociatedPanel.GetPanelEdgeLocation(EDGE_LEFT) + 0.1*iPanelWidth), int(m_AssociatedPanel.GetPanelEdgeLocation(EDGE_TOP) + 0.1*iPanelHeight), int(0.8*iPanelWidth), int(0.8*iPanelHeight), CG32bitPixel(0, 255, 0));
		}
	else
		{
		Screen.Fill(int(m_AssociatedPanel.GetPanelEdgeLocation(EDGE_LEFT) + 0.1*iPanelWidth), int(m_AssociatedPanel.GetPanelEdgeLocation(EDGE_TOP) + 0.1*iPanelHeight), int(0.8*iPanelWidth), int(0.8*iPanelHeight), CG32bitPixel(255, 0, 0));
		}
	}