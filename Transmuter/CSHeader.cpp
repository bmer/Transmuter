//	CSHeader.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CSHeader::CSHeader (CHumanInterface &HI, CPanel &AssociatedPanel, CString sPanelName) :
	CSChild(HI, AssociatedPanel),
	m_sText(sPanelName),
	m_dwStyle(HEADERBAR_STYLE_FULL)
	{
	}

void CSHeader::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	RECT rcPanelArea = m_AssociatedPanel.ScreenArea.GetViewOffsetRect();

	const CVisualPalette &VI = g_pHI->GetVisuals();
	const CG16bitFont &font = VI.GetFont(fontConsoleMediumHeavy);

	Screen.DrawText(rcPanelArea.left + 10, rcPanelArea.top + 10, font, CG32bitPixel(255, 255, 255), m_sText);
	}