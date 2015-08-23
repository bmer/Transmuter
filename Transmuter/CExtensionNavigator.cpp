//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"


CExtensionNavigator::CExtensionNavigator(CHumanInterface &HI, CElasticPanel &AssociatedPanel, TArray <CExtension *> Extensions) : CSubSession(HI, AssociatedPanel),
	m_Extensions(Extensions),
	m_InternalPanelling(CElasticPanel(AssociatedPanel.GetOriginX(), AssociatedPanel.GetOriginY() + 40, AssociatedPanel.GetSpaceWidth(), AssociatedPanel.GetSpaceHeight() - 20))
	{
	CreateExtensionNavigatorMenuItems();
	}

void CExtensionNavigator::CreateExtensionNavigatorMenuItems(void)
	{
	for (int i = 0; i < m_Extensions.GetCount(); i++)
		{
		CElasticPanel *EmptyPanel = m_InternalPanelling.RequestEmptyPanel(-1, 40, 0);
		int xO = EmptyPanel->GetOriginX();
		int yO = EmptyPanel->GetOriginY();
		int SpaceWidth = EmptyPanel->GetSpaceWidth();
		int SpaceHeight = EmptyPanel->GetSpaceHeight();
		m_NavigatorMenuItems.Insert(CExtensionNavigatorMenuItem(m_HI, *EmptyPanel, m_Extensions[i]));
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
		m_NavigatorMenuItems[i].OnPaint(Screen, rcInvalid);
		}
	}

//  =======================================================================


CExtensionDetails::CExtensionDetails(CHumanInterface &HI, CElasticPanel &AssociatedPanel) : CSubSession(HI, AssociatedPanel)
	{
	}


//  =======================================================================

CExtensionNavigatorMenuItem::CExtensionNavigatorMenuItem(CHumanInterface &HI, CElasticPanel &AssociatedPanel, CExtension *Extension) : CSubSession(HI, AssociatedPanel),
	m_Extension(*Extension)
	{
	}

void CExtensionNavigatorMenuItem::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//  may remove panel outlining in future
	DrawPanelOutline(Screen);

	int xO = m_AssociatedPanel.GetOriginX();
	int yO = m_AssociatedPanel.GetOriginY();
	int SpaceWidth = m_AssociatedPanel.GetSpaceWidth();
	int SpaceHeight = m_AssociatedPanel.GetSpaceHeight();

	Screen.DrawText(m_AssociatedPanel.GetOriginX() + 10, m_AssociatedPanel.GetOriginY() + 10, m_HeadingFont, m_HeadingColor, m_Extension.GetName());
	}
