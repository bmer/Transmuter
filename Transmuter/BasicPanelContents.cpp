//	BasicPanelContents.cpp
//
//	Definitions of the basic sessions declared in BasicPanelContents.h
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

#pragma once

IPanelContent::IPanelContent(CString sContentName, CHumanInterface &HI, CPanel &AssociatedPanel) : IHISession(HI),
m_AssociatedPanel(AssociatedPanel),
m_rgbBackgroundColor(CG32bitPixel(0, 0, 0)),
m_rgbPanelOutlineColor(CG32bitPixel(255, 255, 255)),
m_bFocus(false),
m_bLButtonDown(false),
m_bLClicked(false),
m_bLDblClicked(false),
m_bRButtonDown(false),
m_bRClicked(false)
	{
	m_AssociatedPanel.AssociateSession(this);
	}

void IPanelContent::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{
	m_bLButtonDown = true;
	OnContentLButtonDown(x, y, dwFlags, retbCapture);
	}

void IPanelContent::OnLButtonUp(int x, int y, DWORD dwFlags)
	{
	if (IsLButtonDown())
		{
		m_bLButtonDown = false;
		m_bLClicked = true;
		}
	OnContentLButtonUp(x, y, dwFlags);
	}

void IPanelContent::OnLButtonDblClick(int x, int y, DWORD dwFlags, bool * retbCapture)
	{
	m_bLDblClicked = true;
	m_bLButtonDown = false;
	m_bLClicked = false;
	OnContentLButtonDblClick(x, y, dwFlags, retbCapture);
	}

void IPanelContent::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	m_bRButtonDown = true;
	OnContentRButtonDown(x, y, dwFlags);
	}

void IPanelContent::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	if (IsRButtonDown())
		{
		m_bRButtonDown = false;
		m_bRClicked = true;
		}
	OnContentRButtonUp(x, y, dwFlags);
	}

void IPanelContent::OnKeyDown(int iVirtKey, DWORD dwKeyData)
	{
	OnContentKeyDown(iVirtKey, dwKeyData);
	}

void IPanelContent::OnKeyUp(int iVirtKey, DWORD dwKeyData)
	{
	OnContentKeyUp(iVirtKey, dwKeyData);
	}

void IPanelContent::OnChar(char chChar, DWORD dwKeyData)
	{
	OnContentChar(chChar, dwKeyData);
	}

void IPanelContent::DrawPanelOutline(CG32bitImage &Screen)
	{
	RECT PanelRect = this->GetAssociatedPanel().PanelRect.GetAsRect();

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

CTransmuterContent::CTransmuterContent (CString sContentName, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model) : IPanelContent(sContentName, HI, AssociatedPanel),
	m_HeaderContent(NULL),
	m_sID(m_sID),
	m_model(model)
	{
	};

CTransmuterContent::~CTransmuterContent(void)
	{
	if (m_HeaderContent != NULL)
		{
		delete m_HeaderContent;
		}
	}

void CTransmuterContent::SetHeaderContent(CString sID, CString sHeaderText, int iWidth, int iHeight, const CG16bitFont * pFont, CG32bitPixel rgbTextColor, CG32bitPixel rgbBackgroundColor)
	{
	CPanel *pHeaderPanel = GetAssociatedPanel().InternalPanels.AddPanel(0, 0, iWidth, iHeight, false);
	m_HeaderContent = new CHeaderContent(sID, sHeaderText, *g_pHI, *pHeaderPanel, *this);
	}

void CTransmuterContent::UpdateHeaderText(CString sHeaderText)
	{
	if (m_HeaderContent != NULL)
		{
		m_HeaderContent->SetHeaderText(sHeaderText);
		}
	}

void CTransmuterContent::UpdateHeaderFont(const CG16bitFont *pFont)
	{
	if (m_HeaderContent != NULL)
		{
		m_HeaderContent->SetHeaderTextFont(pFont);
		}
	}

void CTransmuterContent::UpdateHeaderTextColor(CG32bitPixel rgbColor)
	{
	if (m_HeaderContent != NULL)
		{
		m_HeaderContent->SetHeaderTextColor(rgbColor);
		}
	}

void CTransmuterContent::UpdateHeaderBackgroundColor(CG32bitPixel rgbColor)
	{
	if (m_HeaderContent != NULL)
		{
		m_HeaderContent->SetBackgroundColor(rgbColor);
		}
	}



//  =======================================================================

CHeaderContent::CHeaderContent(CString sParentSessionName, CString sHeaderText, CHumanInterface & HI, CPanel &AssociatedPanel, CTransmuterContent &AssociatedSession) : IPanelContent(strCat(sParentSessionName, CONSTLIT(".Header")), HI, AssociatedPanel),
	m_sHeaderText(sHeaderText),
	m_pFont(&((g_pHI->GetVisuals()).GetFont(fontConsoleMediumHeavy))),
	m_rgbTextColor(CG32bitPixel(255,255,255)),
	m_rgbBackgroundColor(CG32bitPixel(110,110,110))
	{
	};

CHeaderContent::~CHeaderContent(void)
	{
	}

void CHeaderContent::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	CPanel &refAssociatedPanel = this->GetAssociatedPanel();
	Screen.Fill(refAssociatedPanel.PanelRect.GetOriginX(), refAssociatedPanel.PanelRect.GetOriginY(), refAssociatedPanel.PanelRect.GetWidth(), refAssociatedPanel.PanelRect.GetHeight(), m_rgbBackgroundColor);

	CG32bitPixel TextColor = CG32bitPixel(255, 255, 255);

	const CVisualPalette &VI = g_pHI->GetVisuals();
	const CG16bitFont &font = VI.GetFont(fontConsoleMediumHeavy);

	// this->DrawPanelOutline(Screen);
	int iEdgeLeft = refAssociatedPanel.PanelRect.GetEdgePosition(EDGE_LEFT);
	int iEdgeTop = refAssociatedPanel.PanelRect.GetEdgePosition(EDGE_TOP);
	int iWidth = refAssociatedPanel.PanelRect.GetWidth();
	int iHeight = refAssociatedPanel.PanelRect.GetHeight();

	Screen.DrawText(iEdgeLeft + 10, iEdgeTop + 10, *m_pFont, m_rgbTextColor, m_sHeaderText);
	}
