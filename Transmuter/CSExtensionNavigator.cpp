//	CSTransmuter.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"


CSExtensionNavigator::CSExtensionNavigator(CHumanInterface &HI, CPanel &AssociatedPanel, TArray <CExtension *> Extensions) : CSChild(HI, AssociatedPanel),
	m_Extensions(Extensions),
	m_iMenuSlotHeight(40),
	m_iHeaderBarHeight(40)
	{
	CreateExtensionNavigatorMenuItems();
	}

CSExtensionNavigator::~CSExtensionNavigator(void)
	{
	for (int i = 0; i < m_NavigatorMenuItems.GetCount(); i++)
		{
		delete m_NavigatorMenuItems[i];
		}
	}

void CSExtensionNavigator::CreateExtensionNavigatorMenuItems(void)
	{
	CPanel *pMenuPanel = m_AssociatedPanel.InternalPanels.AddPanel(0, m_iHeaderBarHeight, m_AssociatedPanel.PanelRect.GetWidth(), m_iMenuSlotHeight, false);
	CPanel *pMenuSlot;
	int iMenuPanelWidth = pMenuPanel->PanelRect.GetWidth();
	
	int iNumExtensions = m_Extensions.GetCount();
	for (int i = 0; i < iNumExtensions; i++)
		{
		pMenuSlot = pMenuPanel->InternalPanels.AddPanel(0, m_iMenuSlotHeight*i, iMenuPanelWidth, m_iMenuSlotHeight, false);
		CSMExtension *MenuItem = new CSMExtension(m_HI, *pMenuSlot, m_Extensions[i]);
		m_NavigatorMenuItems.Insert(MenuItem);
		pMenuSlot->AssociateSession(MenuItem);
		};
	}

void CSExtensionNavigator::DrawTitleBar(CG32bitImage &Screen)
	{
	Screen.DrawText(m_AssociatedPanel.PanelRect.GetEdgePosition(EDGE_LEFT) + 10, m_AssociatedPanel.PanelRect.GetEdgePosition(EDGE_TOP) + 10, m_HeadingFont, m_HeadingColor, CONSTLIT("Extension Navigator"));
	}

void CSExtensionNavigator::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
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


CSExtensionDetails::CSExtensionDetails(CHumanInterface &HI, CPanel &AssociatedPanel) : CSChild(HI, AssociatedPanel)
	{
	}


//  =======================================================================

CSMExtension::CSMExtension(CHumanInterface &HI, CPanel &AssociatedPanel, CExtension *Extension) : CSChild(HI, AssociatedPanel),
	m_Extension(*Extension)
	{
	//  button panels should be sticky
	CPanel *ButtonPanel = m_AssociatedPanel.InternalPanels.AddPanel(0, 0, 40, m_AssociatedPanel.PanelRect.GetHeight(), false);
	m_Button = new CSButton(HI, *ButtonPanel, 0.8, CG32bitPixel(100, 100, 100));
	ButtonPanel->AssociateSession(m_Button);

	CPanel *TextPanel = m_AssociatedPanel.InternalPanels.AddPanel(40, 0, m_AssociatedPanel.PanelRect.GetWidth() - 40, m_AssociatedPanel.PanelRect.GetHeight(), false);
	m_TextArea = new CSTextArea(HI, *TextPanel);
	// do set font table 
	m_TextArea->SetFontTable(&HI.GetVisuals());
	TextPanel->AssociateSession(m_TextArea);

	m_TextArea->SetPadding(10);
	m_TextArea->SetText(m_Extension.GetName());
	m_TextArea->Justify();

	}

CSMExtension::~CSMExtension(void)
	{
	delete m_Button;
	delete m_TextArea;
	}

void CSMExtension::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
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
