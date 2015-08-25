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
	if (IsPointInRect(x, y, m_AssociatedPanel.GetRectDefinition()))
		{
		m_IsLDown = true;
		}
	else
		{
		m_IsLDown = false;
		}
	}

void CButton::OnLButtonUp(int x, int y, DWORD dwFlags)
	{
	if (IsPointInRect(x, y, m_AssociatedPanel.GetRectDefinition()))
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
	}

void CButton::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	if (IsPointInRect(x, y, m_AssociatedPanel.GetRectDefinition()))
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
	if (IsPointInRect(x, y, m_AssociatedPanel.GetRectDefinition()))
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

