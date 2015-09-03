//	CSTransmuter.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CSChild::CSChild(CHumanInterface &HI, CPanel &AssociatedPanel) : IHISession(HI),
	m_AssociatedPanel(AssociatedPanel),
	m_HeadingFont((g_pHI->GetVisuals()).GetFont(fontConsoleMediumHeavy)),
	m_HeadingColor(CG32bitPixel(160, 160, 160)),
	m_BorderColor(CG32bitPixel(64, 64, 64)),
	m_PanelOutlineWidth(1),
	m_PanelOutlineColor(CG32bitPixel(255, 255, 255))
	{
	m_AssociatedPanel.AssociateSession(this);
	}

void CSChild::DrawPanelOutline(CG32bitImage &Screen)
	{
	RECT PanelRect = m_AssociatedPanel.GetPanelRect();

	//  corners are numbered starting from top left hand corner being zero, and then going counter clockwise

	int c0x = PanelRect.left;
	int c0y = PanelRect.top;
	
	int c1x = PanelRect.right;
	int c1y = c0y;

	int c2x = c1x;
	int c2y = PanelRect.bottom;

	int c3x = c0x;
	int c3y = c2y;

	Screen.DrawLine(c0x, c0y, c1x, c1y, m_PanelOutlineWidth, m_PanelOutlineColor);
	Screen.DrawLine(c1x, c1y, c2x, c2y, m_PanelOutlineWidth, m_PanelOutlineColor);
	Screen.DrawLine(c2x, c2y, c3x, c3y, m_PanelOutlineWidth, m_PanelOutlineColor);
	Screen.DrawLine(c3x, c3y, c0x, c0y, m_PanelOutlineWidth, m_PanelOutlineColor);
	}

//  =======================================================================

CSError::CSError(CHumanInterface &HI, CPanel &AssociatedPanel, CString ErrorString) : CSChild(HI, AssociatedPanel),
	m_sErrorString(ErrorString)
	{
	}

void CSError::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	}

