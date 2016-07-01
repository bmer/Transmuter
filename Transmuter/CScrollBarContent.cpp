//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CScrollBarPanel::CScrollBarPanel(CString sID, CHumanInterface &HI, IPanel &AssociatedPanel, CTransmuterModel &model, IPanel &PanelToScroll) : CTransmuterPanel (sID, HI, AssociatedPanel, model),
	m_PanelToScroll(PanelToScroll)
	{
	}

void CScrollBarPanel::DetermineVisibility(void)
	{
	}

void CScrollBarPanel::CalculateBarRect(void)
	{
	}

void CScrollBarPanel::CalculateSlideRect(void)
	{
	}
