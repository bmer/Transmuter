//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"


CContextSession::CContextSession(CHumanInterface &HI, CPanel &AssociatedPanel) : CTransmuterSession(CONSTLIT("context view"), HI, AssociatedPanel)
	{
	}

CContextSession::~CContextSession (void)
	{
	m_NavigatorMenuItems.DeleteAll();
	}

void CContextSession::CreateExtensionNavigatorMenuItems (void)
	{
	CPanel *pMenuPanel = m_AssociatedPanel.InternalPanels.AddPanel(0, m_iHeaderBarHeight, m_AssociatedPanel.PanelRect.GetWidth(), m_iMenuSlotHeight, false);
	CPanel *pMenuSlot;
	int iMenuPanelWidth = pMenuPanel->PanelRect.GetWidth();
	
	int iNumExtensions = m_Extensions.GetCount();
	for (int i = 0; i < iNumExtensions; i++)
		{
		pMenuSlot = pMenuPanel->InternalPanels.AddPanel(0, m_iMenuSlotHeight*i, iMenuPanelWidth, m_iMenuSlotHeight, false);
		CSExtensionMenuItem *MenuItem = new CSExtensionMenuItem(m_HI, *pMenuSlot, m_Extensions[i]);
		m_NavigatorMenuItems.Insert(MenuItem);
		pMenuSlot->AssociateSession(MenuItem);
		};
	}

void CContextSession::DrawTitleBar (CG32bitImage &Screen)
	{
	int iOffsetX = m_AssociatedPanel.PanelRect.GetEdgePosition(EDGE_LEFT) + 10;
	int iOffsetY = m_AssociatedPanel.PanelRect.GetEdgePosition(EDGE_TOP) + 10;

	Screen.DrawText(iOffsetX, iOffsetY, m_HeadingFont, m_HeadingColor, CONSTLIT("Extension Navigator"));
	}

void CContextSession::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//  may remove panel outlining in future
	DrawPanelOutline(Screen);

	DrawTitleBar(Screen);

	int iNumNavigatorMenuItems = m_NavigatorMenuItems.GetCount();
	for (int i = 0; i < iNumNavigatorMenuItems; i++)
		{
		m_NavigatorMenuItems[i]->OnPaint(Screen, rcInvalid);
		}
	}

//  =======================================================================


CSExtensionDetails::CSExtensionDetails (CHumanInterface &HI, CPanel &AssociatedPanel) : CTransmuterSession(HI, AssociatedPanel)
	{
	}


//  =======================================================================

CSExtensionMenuItem::CSExtensionMenuItem (CHumanInterface &HI, CPanel &AssociatedPanel, CExtension *Extension) : CTransmuterSession(HI, AssociatedPanel),
	m_Extension(*Extension)
	{
	//  button panels should be sticky
	CPanel *ButtonPanel = m_AssociatedPanel.InternalPanels.AddPanel(0, 0, 40, m_AssociatedPanel.PanelRect.GetHeight(), false);
	m_Button = new CButtonSession(HI, *ButtonPanel, 0.8, CG32bitPixel(100, 100, 100));
	ButtonPanel->AssociateSession(m_Button);

	CPanel *TextPanel = m_AssociatedPanel.InternalPanels.AddPanel(40, 0, m_AssociatedPanel.PanelRect.GetWidth() - 40, m_AssociatedPanel.PanelRect.GetHeight(), false);
	m_TextArea = new CTextAreaSession(HI, *TextPanel);
	m_TextArea->SetFontTable(&HI.GetVisuals());
	m_TextArea->SetFont(&HI.GetVisuals().GetFont(fontConsoleMediumHeavy));
	TextPanel->AssociateSession(m_TextArea);

	m_TextArea->SetPadding(10);
	m_TextArea->SetAsRichText(Extension->GetName());
	m_TextArea->Justify();

	}

CSExtensionMenuItem::~CSExtensionMenuItem(void)
	{
	delete m_Button;
	delete m_TextArea;
	}

void CSExtensionMenuItem::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (!m_AssociatedPanel.IsHidden())
		{
		if (m_Button->CheckIfLPressed())
			{
			m_AssociatedPanel.Hide();
			}
		else
			{
			DrawPanelOutline(Screen);
			m_Button->OnPaint(Screen, rcInvalid);
			m_TextArea->OnPaint(Screen, rcInvalid);
			}
		}
	}
