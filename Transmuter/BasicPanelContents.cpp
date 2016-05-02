//	BasicPanelContents.cpp
//
//	Definitions of the basic sessions declared in BasicPanelContents.h
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

#pragma once

IPanelContent::IPanelContent(CString sContentName, CHumanInterface &HI, IPanel &AssociatedPanel) : IHISession(HI),
m_AssociatedPanel(AssociatedPanel),
m_rgbBackgroundColor(CG32bitPixel(0, 0, 0)),
m_rgbPanelOutlineColor(CG32bitPixel(255, 255, 255)),
m_bFocus(false),
m_bLButtonDown(false),
m_bLClicked(false),
m_bLDblClicked(false),
m_bRButtonDown(false),
m_bRClicked(false),
m_pController(NULL),
m_bCapture(false),
m_pLeftSeparator(NULL),
m_pRightSeparator(NULL),
m_pTopSeparator(NULL),
m_pBottomSeparator(NULL)
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
	bool bPointInRect = IsPointInRect(x, y, m_AssociatedPanel.PanelRect.GetAsRect());

	if (IsLButtonDown())
		{
		m_bLButtonDown = false;
		if (bPointInRect)
			{
			m_bLClicked = true;
			}
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

void IPanelContent::UpdateEdgePositionsFromSeparators(void)
	{
	if (m_pLeftSeparator != NULL)
		{
		GetAssociatedPanel().PanelRect.SetEdgePosition(EDGE_LEFT, m_pLeftSeparator->GetRightEdgePosition());
		}
	if (m_pRightSeparator != NULL)
		{
		GetAssociatedPanel().PanelRect.SetEdgePosition(EDGE_RIGHT, m_pRightSeparator->GetLeftEdgePosition());
		}
	if (m_pTopSeparator != NULL)
		{
		GetAssociatedPanel().PanelRect.SetEdgePosition(EDGE_TOP, m_pTopSeparator->GetBottomEdgePosition());
		}
	if (m_pBottomSeparator != NULL)
		{
		GetAssociatedPanel().PanelRect.SetEdgePosition(EDGE_BOTTOM, m_pBottomSeparator->GetTopEdgePosition());
		}
	}

//  =======================================================================

CTransmuterContent::CTransmuterContent (CString sContentName, CHumanInterface &HI, IPanel &AssociatedPanel, CTransmuterModel &model) : IPanelContent(sContentName, HI, AssociatedPanel),
	m_pHeaderContent(NULL),
	m_sID(m_sID),
	m_model(model)
	{
	};

CTransmuterContent::~CTransmuterContent(void)
	{
	if (m_pHeaderContent != NULL)
		{
		delete m_pHeaderContent;
		}
	}

void CTransmuterContent::SetHeaderContent(CString sID, CString sHeaderText, int iWidth, int iHeight, const CG16bitFont * pFont, CG32bitPixel rgbTextColor, CG32bitPixel rgbBackgroundColor)
	{
	IPanel *pHeaderPanel = GetAssociatedPanel().InternalPanels.AddPanel(0, 0, iWidth, iHeight, false);
	m_pHeaderContent = new CHeaderContent(sID, sHeaderText, *g_pHI, *pHeaderPanel, *this);
	}

void CTransmuterContent::UpdateHeaderText(CString sHeaderText)
	{
	if (m_pHeaderContent != NULL)
		{
		m_pHeaderContent->SetHeaderText(sHeaderText);
		}
	}

void CTransmuterContent::UpdateHeaderFont(const CG16bitFont *pFont)
	{
	if (m_pHeaderContent != NULL)
		{
		m_pHeaderContent->SetHeaderTextFont(pFont);
		}
	}

void CTransmuterContent::UpdateHeaderTextColor(CG32bitPixel rgbColor)
	{
	if (m_pHeaderContent != NULL)
		{
		m_pHeaderContent->SetHeaderTextColor(rgbColor);
		}
	}

void CTransmuterContent::UpdateHeaderBackgroundColor(CG32bitPixel rgbColor)
	{
	if (m_pHeaderContent != NULL)
		{
		m_pHeaderContent->SetBackgroundColor(rgbColor);
		}
	}

//  =======================================================================

CHeaderContent::CHeaderContent(CString sParentSessionName, CString sHeaderText, CHumanInterface & HI, IPanel &AssociatedPanel, CTransmuterContent &AssociatedContent) : IPanelContent(strCat(sParentSessionName, CONSTLIT(".Header")), HI, AssociatedPanel),
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
	IPanel &refAssociatedPanel = this->GetAssociatedPanel();
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

//  =======================================================================

CSeparatorContent::CSeparatorContent(CString sID, CHumanInterface & HI, IPanel &AssociatedPanel, bool bHorizontal) : IPanelContent(sID, HI, AssociatedPanel),
	m_rgbBackgroundColor(CG32bitPixel(90, 90, 90)),
	m_bHorizontal(bHorizontal)
	{
	SetCaptureStatus(true);
	}

CSeparatorContent::~CSeparatorContent()
	{
	}

void CSeparatorContent::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	IPanel &refAssociatedPanel = this->GetAssociatedPanel();
	Screen.Fill(refAssociatedPanel.PanelRect.GetOriginX(), refAssociatedPanel.PanelRect.GetOriginY(), refAssociatedPanel.PanelRect.GetWidth(), refAssociatedPanel.PanelRect.GetHeight(), m_rgbBackgroundColor);
	}
