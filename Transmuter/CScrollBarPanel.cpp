//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CScrollBarPanel::CScrollBarPanel(CString sID, CHumanInterface &HI, CTransmuterModel &model, IPanel &refPanelToScroll, int iThickness, char cScrollDirection) : CTransmuterPanel (sID, HI, model, 0, 0),
	m_PanelToScroll(refPanelToScroll),
	m_iThickness(iThickness),
	m_cScrollDirection(cScrollDirection)
	{
	if (m_cScrollDirection == 'h')
		{
		PanelRect.SetWidth(m_PanelToScroll.PanelRect.GetWidth());
		PanelRect.SetHeight(iThickness);
		}
	if (m_cScrollDirection == 'v')
		{
		PanelRect.SetWidth(iThickness);
		PanelRect.SetHeight(m_PanelToScroll.PanelRect.GetHeight());
		}
	}

void CScrollBarPanel::CalculateBarRect(void)
	{
	}

void CScrollBarPanel::CalculateSlideRect(void)
	{
	}
