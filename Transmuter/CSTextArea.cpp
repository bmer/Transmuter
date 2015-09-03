//	CSTransmuter.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CSTextArea::CSTextArea (CHumanInterface &HI, CPanel &AssociatedPanel) : CSChild(HI, AssociatedPanel)
	{
	}

int CSTextArea::JustifyInPanel (void)
	{
	return CGTextArea::Justify(m_AssociatedPanel.GetPanelRect());
	}

void CSTextArea::PaintInPanel (CG32bitImage &Dest)
	{
	CGTextArea::Paint(Dest, m_AssociatedPanel.GetPanelRect());
	}

void CSTextArea::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (m_AssociatedPanel.IsPanelInvalid())
		{
		JustifyInPanel();
		}
	
	PaintInPanel(Screen);
	}