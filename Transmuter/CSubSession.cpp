//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CSubSession::CSubSession(CHumanInterface &HI, CElasticPanel &AssociatedPanel) : IHISession(HI),
	m_AssociatedPanel(AssociatedPanel),
	m_HeadingFont((g_pHI->GetVisuals()).GetFont(fontConsoleMediumHeavy)),
	m_HeadingColor(CG32bitPixel(160, 160, 160)),
	m_BorderColor(CG32bitPixel(64, 64, 64)),
	m_PanelOutlineWidth(1),
	m_PanelOutlineColor(CG32bitPixel(255, 255, 255))
	{
	m_AssociatedPanel.AssociateSession(this);
	}

void CSubSession::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	}

void CSubSession::DrawPanelOutline(CG32bitImage &Screen)
	{
	TArray <int> PanelOutline = m_AssociatedPanel.GetRectCoords();

	Screen.DrawLine(PanelOutline[0], PanelOutline[1], PanelOutline[0], PanelOutline[3], m_PanelOutlineWidth, m_PanelOutlineColor);
	Screen.DrawLine(PanelOutline[0], PanelOutline[1], PanelOutline[2], PanelOutline[1], m_PanelOutlineWidth, m_PanelOutlineColor);
	Screen.DrawLine(PanelOutline[2], PanelOutline[3], PanelOutline[0], PanelOutline[3], m_PanelOutlineWidth, m_PanelOutlineColor);
	Screen.DrawLine(PanelOutline[2], PanelOutline[3], PanelOutline[2], PanelOutline[1], m_PanelOutlineWidth, m_PanelOutlineColor);
	}

//  =======================================================================

CErrorSession::CErrorSession(CHumanInterface &HI, CElasticPanel &AssociatedPanel, CString ErrorString) : CSubSession(HI, AssociatedPanel),
	m_ErrorString(ErrorString)
	{
	}

void CErrorSession::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	DrawPanelOutline(Screen);

	Screen.DrawText(m_AssociatedPanel.GetOriginX() + 10, m_AssociatedPanel.GetOriginY() + 10, m_HeadingFont, m_HeadingColor, m_ErrorString);
	}

