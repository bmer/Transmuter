//	BasicSessions.cpp
//
//	Definitions of the basic sessions declared in BasicSessions.h
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

#pragma once

//  =======================================================================

CLoadingSession::CLoadingSession (CHumanInterface &HI) : IHISession(HI)
//	CMainSession constructor
	{
	}

void CLoadingSession::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(64, 64, 64));

	CG32bitPixel TextColor = CG32bitPixel(255, 255, 255);

	const CVisualPalette &VI = g_pHI->GetVisuals();
	const CG16bitFont &font = VI.GetFont(fontConsoleMediumHeavy);

	Screen.DrawText(Screen.GetWidth() / 2, Screen.GetHeight() / 2, font, TextColor, CONSTLIT("Loading..."));
	}

//  =======================================================================

CTransmuterSession::CTransmuterSession (CString sSessionName, CHumanInterface &HI, CPanel &AssociatedPanel) : IHISession(HI),
	m_AssociatedPanel(AssociatedPanel),
	m_HeaderSession(NULL),
	m_sSessionName(sSessionName),
	m_rgbPanelOutlineColor(CG32bitPixel(255,255,255)),
	m_rgbBackgroundColor(CG32bitPixel(0,0,0))
	{
	};

CTransmuterSession::~CTransmuterSession(void)
	{
	if (m_HeaderSession != NULL)
		{
		delete m_HeaderSession;
		}
	}

void CTransmuterSession::DrawPanelOutline(CG32bitImage &Screen)
	{
	RECT PanelRect = m_AssociatedPanel.PanelRect.GetAsRect();

	//  corners are numbered starting from top left hand corner being zero, and then going counter clockwise

	int c0x = PanelRect.left;
	int c0y = PanelRect.top;

	int c1x = PanelRect.right;
	int c1y = c0y;

	int c2x = c1x;
	int c2y = PanelRect.bottom;

	int c3x = c0x;
	int c3y = c2y;

	Screen.DrawLine(c0x, c0y, c1x, c1y, 1, m_rgbPanelOutlineColor);
	Screen.DrawLine(c1x, c1y, c2x, c2y, 1, m_rgbPanelOutlineColor);
	Screen.DrawLine(c2x, c2y, c3x, c3y, 1, m_rgbPanelOutlineColor);
	Screen.DrawLine(c3x, c3y, c0x, c0y, 1, m_rgbPanelOutlineColor);
	}

//  =======================================================================

CHeaderSession::CHeaderSession(CString sParentSessionName, CHumanInterface & HI, CPanel &AssociatedPanel, CTransmuterSession & AssociatedSession) : CTransmuterSession(strCat(sParentSessionName, CONSTLIT(".Header")), HI, AssociatedPanel),
	m_pFont(&((g_pHI->GetVisuals()).GetFont(fontConsoleMediumHeavy))),
	m_rgbTextColor(CG32bitPixel(255,255,255)),
	m_rgbBackgroundColor(CG32bitPixel(110,110,110))
	{
	};

CHeaderSession::~CHeaderSession(void)
	{
	}
