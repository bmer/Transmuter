//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CScrollBarContent::CScrollBarContent(CString sID, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model, CPanel &PanelToScroll) : CTransmuterContent (sID, HI, AssociatedPanel, model),
	m_PanelToScroll(PanelToScroll)
	{
	}

void CScrollBarContent::DetermineVisibility(void)
	{
	}

void CScrollBarContent::CalculateBarRect(void)
	{
	}

void CScrollBarContent::CalculateSlideRect(void)
	{
	}