//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"


CExtensionNavigator::CExtensionNavigator(CHumanInterface &HI, CPanel &AssociatedPanel, TArray <CExtension *> Extensions) : CSubSession(HI, AssociatedPanel),
	m_Extensions(Extensions),
	m_MenuSlotHeight(40)
	{
	CreateExtensionNavigatorMenuItems();
	}

CExtensionNavigator::~CExtensionNavigator(void)
	{
	for (int i = 0; i < m_NavigatorMenuItems.GetCount(); i++)
		{
		delete m_NavigatorMenuItems[i];
		}
	}

void CExtensionNavigator::CreateExtensionNavigatorMenuItems(void)
	{
	CPanel *MenuPanel = m_AssociatedPanel.AddInternalPanelRelativeToOrigin(0, m_MenuSlotHeight, m_AssociatedPanel.GetWidth(), m_AssociatedPanel.GetHeight() - m_MenuSlotHeight, false, false);
	int MenuPanelWidth = MenuPanel->GetWidth();
	
	for (int i = 0; i < m_Extensions.GetCount(); i++)
		{
		CPanel *MenuSlot = MenuPanel->AddInternalPanelRelativeToOrigin(0, m_MenuSlotHeight*i, MenuPanelWidth, 40, false, false);
		CExtensionMenuItem *MenuItem = new CExtensionMenuItem(m_HI, *MenuSlot, m_Extensions[i]);
		m_NavigatorMenuItems.Insert(MenuItem);
		MenuSlot->AssociateSession(MenuItem);
		};
	}

void CExtensionNavigator::DrawTitleBar(CG32bitImage &Screen)
	{
	Screen.DrawText(m_AssociatedPanel.GetOriginX() + 10, m_AssociatedPanel.GetOriginY() + 10, m_HeadingFont, m_HeadingColor, CONSTLIT("Extension Navigator"));
	}

void CExtensionNavigator::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//  may remove panel outlining in future
	DrawPanelOutline(Screen);

	DrawTitleBar(Screen);

	for (int i = 0; i < m_NavigatorMenuItems.GetCount(); i++)
		{
		m_NavigatorMenuItems[i]->OnPaint(Screen, rcInvalid);
		}
	}

//  =======================================================================


CExtensionDetails::CExtensionDetails(CHumanInterface &HI, CPanel &AssociatedPanel) : CSubSession(HI, AssociatedPanel)
	{
	}


//  =======================================================================

CExtensionMenuItem::CExtensionMenuItem(CHumanInterface &HI, CPanel &AssociatedPanel, CExtension *Extension) : CSubSession(HI, AssociatedPanel),
	m_Extension(*Extension)
	{
	CPanel *ButtonPanel = m_AssociatedPanel.AddInternalPanelRelativeToOrigin(0, 0, 40, m_AssociatedPanel.GetHeight(), false, false);
	m_Button = new CButton(HI, *ButtonPanel);
	m_Button->SetBGColor(CG32bitPixel(100, 100, 100));

	ButtonPanel->AssociateSession(m_Button);
	}

CExtensionMenuItem::~CExtensionMenuItem(void)
	{
	delete m_Button;
	}

void CExtensionMenuItem::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (!m_AssociatedPanel.IsHidden())
		{
		if (m_Button->CheckIfLPressed())
			{
			m_AssociatedPanel.Hide();
			HIInvalidate();
			}
		else
			{
			m_Button->OnPaint(Screen, rcInvalid);
			CPanel &ButtonPanel = m_Button->GetAssociatedPanel();

			int TextX = ButtonPanel.GetOriginX() + ButtonPanel.GetWidth();
			int TextY = ButtonPanel.GetOriginY();
			Screen.DrawText(TextX + 10, TextY + 10, m_HeadingFont, m_HeadingColor, m_Extension.GetName());
			}
		}
	}
