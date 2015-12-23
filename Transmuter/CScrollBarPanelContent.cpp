//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CSScrollBar::CSScrollBar(CHumanInterface &HI, CPanel &AssociatedPanel, CPanel &PanelToScroll) : CButtonSession (HI, AssociatedPanel),
	m_PanelToScroll(PanelToScroll)
	{
	}