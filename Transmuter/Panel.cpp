//	Panel.cpp
//
//	Copyright (c) 2016 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"
#include "Panel.h"

CPanelRect::CPanelRect (IPanel &AssociatedPanel, int iWidth, int iHeight) :
	m_iOriginX(0),
	m_iOriginY(0),
	m_iHeight(iHeight),
	m_iWidth(iWidth),
	m_AssociatedPanel(AssociatedPanel)
	{
	}

void CPanelRect::MakeTopLeftOrigin (void)
	{
	if (m_iHeight < 0)
		{
		m_iOriginY = m_iHeight + m_iOriginY;
		}
	if (m_iWidth < 0)
		{
		m_iOriginX = m_iHeight + m_iOriginX;
		}
	}

void CPanelRect::SetOrigin (int iOriginX, int iOriginY)
	{
	m_iOriginX = iOriginX;
	m_iOriginY = iOriginY;
	}

void CPanelRect::ShiftOrigin (int iDeltaX, int iDeltaY)
	{
	m_iOriginX += iDeltaX;
	m_iOriginY += iDeltaY;
	}

RECT CPanelRect::GetAsRect (void)
	{
	RECT rc;

	MakeTopLeftOrigin();

	rc.left = m_iOriginX;
	rc.right = m_iOriginX + m_iWidth;
	rc.top = m_iOriginY;
	rc.bottom = m_iHeight + m_iOriginY;

	return rc;
	}

int CPanelRect::GetEdgePosition (DWORD dwEdge)
	{
	MakeTopLeftOrigin();

	if (dwEdge == EDGE_TOP)
		{
		return m_iOriginY;
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		return m_iOriginX + m_iWidth;
		}
	else if (dwEdge == EDGE_BOTTOM)
		{
		return m_iOriginY + m_iHeight;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		return m_iOriginX;
		}
	else
		{
		return -1;
		//throw UndefinedEdgeError;
		}
	}

void CPanelRect::SetEdgePosition (DWORD dwEdge, int iPosition)
	{
	MakeTopLeftOrigin();

	if (dwEdge == EDGE_TOP)
		{
		m_iOriginY = iPosition;
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		m_iWidth = iPosition - m_iOriginX;
		}
	else if (dwEdge == EDGE_BOTTOM)
		{
		m_iHeight = iPosition- m_iOriginY;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		m_iOriginX = iPosition;
		}
	else
		{
		// throw CMessageException("Unknown DWORD given for EDGE!");
		m_iOriginX = 0;
		}
	}

void CPanelRect::ShiftEdgePosition (DWORD dwEdge, int iShift)
	{
	MakeTopLeftOrigin();

	if (dwEdge == EDGE_TOP)
		{
		m_iOriginY += iShift;
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		m_iWidth = (GetEdgePosition(dwEdge) + iShift) - m_iOriginX;
		}
	else if (dwEdge == EDGE_BOTTOM)
		{
		m_iHeight = (GetEdgePosition(dwEdge) + iShift)- m_iOriginY;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		m_iOriginX += iShift;
		}
	else
		{
		// throw CMessageException("Unknown DWORD given for EDGE!");
		m_iOriginX = 0;
		}
	}

// ============================================================================

IPanel::IPanel (CString sName, CHumanInterface &HI, int iWidth, int iHeight) : IHISession(HI),
	m_sName(sName),
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_bHidden(false),
	m_iViewOffsetX(0),
	m_iViewOffsetY(0),
	m_bLButtonDown(false),
	m_bLClicked(false),
	m_bLDblClicked(false),
	m_bRButtonDown(false),
	m_bRClicked(false),
	m_bFocus(false),
	m_rgbBackgroundColor(CG32bitPixel(0, 0, 0)),
	m_rgbOutlineColor(CG32bitPixel(255, 255, 255)),
	m_bCapture(false),
	m_bIsPlaced(false),
	PanelRect(*this, iWidth, iHeight)
	{
	}

IPanel::~IPanel ()
	{
	}

void IPanel::PaintBackground (CG32bitImage &Screen)
	{
	Screen.Fill(PanelRect.GetOriginX(), PanelRect.GetOriginY(), PanelRect.GetWidth(), PanelRect.GetHeight(), m_rgbBackgroundColor);
	}

void IPanel::PaintOutline (CG32bitImage &Screen)
	{
	RECT rcPanel = PanelRect.GetAsRect();

	//  corners are numbered starting from top left hand corner being zero, and then going counter clockwise

	int c0x = rcPanel.left;
	int c0y = rcPanel.top;

	int c1x = rcPanel.right;
	int c1y = c0y;

	int c2x = c1x;
	int c2y = rcPanel.bottom;

	int c3x = c0x;
	int c3y = c2y;

	Screen.DrawLine(c0x, c0y, c1x, c1y, 1, m_rgbOutlineColor);
	Screen.DrawLine(c1x, c1y, c2x, c2y, 1, m_rgbOutlineColor);
	Screen.DrawLine(c2x, c2y, c3x, c3y, 1, m_rgbOutlineColor);
	Screen.DrawLine(c3x, c3y, c0x, c0y, 1, m_rgbOutlineColor);
	}

void IPanel::UpdateOutlineColorGivenFocusStatus(CG32bitPixel rgbNoFocus, CG32bitPixel rgbFocus)
	{
	if (GetFocusStatus() == true)
		{
		UpdateOutlineColor(rgbNoFocus);
		}
	else
		{
		UpdateOutlineColor(rgbFocus);
		}
	}

void IPanel::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (IsPlaced())
		{
		if (GetFocusStatus() == true)
			{
			UpdateOutlineColor(CG32bitPixel(255, 0, 0));
			}
		else
			{
			UpdateOutlineColor(CG32bitPixel(255, 255, 255));
			}

		PaintBackground(Screen);
		PaintOutline(Screen);

		OnContentPaint(Screen, rcInvalid);
		}
	}

CString IPanel::CreateSubPanelName(CString sSubPanelNamePart)
	{
	CString sSubPanelName = strCat(m_sName, CONSTLIT("."));
	sSubPanelName = strCat(sSubPanelName, sSubPanelNamePart);
	
	return sSubPanelName;
	}
//
//TArray <IPanel *> IPanel::GetPanelsContainingPoint (int x, int y)
//	{
//	TArray <IPanel *> aRelevantPanels;
//
//	if (!IsHidden())
//		{
//		if (IsPointInRect(x, y, PanelRect.GetAsRect()))
//			{
//			aRelevantPanels.Insert(this);
//
//			// if this panel contains this point, then some of its children might too
//			aRelevantPanels.Insert(GetPanelsContainingPoint(x, y));
//			}
//		}
//
//	return aRelevantPanels;
//	}


void IPanel::Hide(void)
	{
	m_bHidden = true;
	HideAll();
	}

void IPanel::Show(void)
	{
	m_bHidden = false;
	ShowAll();
	}

void IPanel::Invalidate()
	{
	HIInvalidate();
	}

void IPanel::SetViewOffset (int iOffsetX, int iOffsetY)
	{
	int iShiftX = iOffsetX - m_iViewOffsetX;
	m_iViewOffsetX = iOffsetX;

	int iShiftY = iOffsetY - m_iViewOffsetY;
	m_iViewOffsetY = iOffsetY;
	}

void IPanel::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{
	m_bLButtonDown = true;
	OnContentLButtonDown(x, y, dwFlags, retbCapture);
	}

void IPanel::OnLButtonUp(int x, int y, DWORD dwFlags)
	{
	bool bPointInRect = IsPointInRect(x, y, PanelRect.GetAsRect());

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

void IPanel::OnLButtonDblClick(int x, int y, DWORD dwFlags)
	{
	m_bLDblClicked = true;
	m_bLButtonDown = false;
	m_bLClicked = false;

	OnContentLButtonDblClick(x, y, dwFlags);
	}

void IPanel::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	m_bRButtonDown = true;

	OnContentRButtonDown(x, y, dwFlags);
	}

void IPanel::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	if (IsRButtonDown())
		{
		m_bRButtonDown = false;
		m_bRClicked = true;
		}
	OnContentRButtonUp(x, y, dwFlags);
	}

void IPanel::OnKeyDown(int iVirtKey, DWORD dwKeyData)
	{
	OnContentKeyDown(iVirtKey, dwKeyData);
	}

void IPanel::OnKeyUp(int iVirtKey, DWORD dwKeyData)
	{
	OnContentKeyUp(iVirtKey, dwKeyData);
	}

void IPanel::OnChar(char chChar, DWORD dwKeyData)
	{
	OnContentChar(chChar, dwKeyData);
	}
/*
void IPanel::HidePanel (IPanel *Panel)
	{
	int iPanelIndex = GetPanelIndex(Panel);

	if (iPanelIndex == -1)
		{
		return;
		}
	else
		{
		HidePanel(iPanelIndex);
		}
	}*/

//void IPanel::ShowPanel (IPanel *Panel)
//	{
//	int iPanelIndex = GetPanelIndex(Panel);
//
//	if (iPanelIndex == -1)
//		{
//		return;
//		}
//	else
//		{
//		ShowPanel(iPanelIndex);
//		}
//	}
