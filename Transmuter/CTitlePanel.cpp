//	CTitlePanel.cpp
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#include "PreComp.h"

CTitlePanel::CTitlePanel(CString sName, CHumanInterface &HI, int iWidth, int iHeight, CString sTitleText) : IPanel(sName, HI, iWidth, iHeight),
	m_sTitleText(sTitleText),
	m_pFont(&((g_pHI->GetVisuals()).GetFont(fontConsoleMediumHeavy))),
	m_rgbTextColor(CG32bitPixel(255, 255, 255)),
	m_rgbBackgroundColor(CG32bitPixel(110, 110, 110))
	{
	}

void CTitlePanel::OnContentPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	Screen.Fill(PanelRect.GetOriginX(), PanelRect.GetOriginY(), PanelRect.GetWidth(), PanelRect.GetHeight(), m_rgbBackgroundColor);

	CG32bitPixel TextColor = CG32bitPixel(255, 255, 255);

	const CVisualPalette &VI = g_pHI->GetVisuals();
	const CG16bitFont &font = VI.GetFont(fontConsoleMediumHeavy);

	// this->DrawPanelOutline(Screen);
	int iEdgeLeft = PanelRect.GetEdgePosition(EDGE_LEFT);
	int iEdgeTop = PanelRect.GetEdgePosition(EDGE_TOP);
	int iWidth = PanelRect.GetWidth();
	int iHeight = PanelRect.GetHeight();

	Screen.DrawText(iEdgeLeft + 10, iEdgeTop + 10, *m_pFont, m_rgbTextColor, m_sTitleText);
	}

