//	CSTransmuter.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CSScrollBar::CSScrollBar(CHumanInterface &HI, CPanel &AssociatedPanel, CPanel &PanelToScroll) : CSButton (HI, AssociatedPanel),
	m_PanelToScroll(PanelToScroll)
	{
	}