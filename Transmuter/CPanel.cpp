//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CPanelRect::CPanelRect (IPanel &AssociatedPanel) : 
	m_iOriginX(0),
	m_iOriginY(0),
	m_iHeight(0),
	m_iWidth(0),
	m_AssociatedPanel(AssociatedPanel)
	{
	}

CPanelRect::CPanelRect (IPanel &AssociatedPanel, int iOriginX, int iOriginY, int iWidth, int iHeight) :
	m_iOriginX(iOriginX),
	m_iOriginY(iOriginY),
	m_iHeight(iHeight),
	m_iWidth(iWidth),
	m_AssociatedPanel(AssociatedPanel)
	{
	}

CPanelRect::CPanelRect(IPanel & AssociatedPanel, int iWidth, int iHeight) :
	m_iOriginX(0),
	m_iOriginY(0),
	m_iHeight(iHeight),
	m_iWidth(iWidth),
	m_AssociatedPanel(AssociatedPanel)
	{
	}

CPanelRect::CPanelRect (IPanel &AssociatedPanel, RECT rc) :
	m_AssociatedPanel(AssociatedPanel)
	{
	SetWithRect(rc);
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

void CPanelRect::SetWithRect (RECT rc)
	{
	m_iOriginX = rc.left;
	m_iOriginY = rc.top;
	m_iHeight = rc.bottom - rc.top;
	m_iWidth = rc.right - rc.left;
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

// ===========================================================================

CPanelOrganizer::CPanelOrganizer(IPanel &ParentPanel) : m_iPanelConfigType(-1),
	m_ParentPanel(ParentPanel),
	m_iSeparatorPos(-1),
	m_iSeparatorThickness(-1)
	{
	}

CPanelOrganizer::~CPanelOrganizer()
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		delete m_aPanels[i];
		}
	}

void CPanelOrganizer::ShiftAllOrigins (int iShiftX, int iShiftY)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->PanelRect.ShiftOrigin(iShiftX, iShiftY);
		}
	}

TArray <int> CPanelOrganizer::SortByPanelRectEdgeLocation (DWORD dwEdge)
//  SortByPanelRectEdgeLocation
// 
//  Return an array of integers whose elements correspond with indices of panels
//  in the m_aInternalPanels array. Panel indices are sorted by edge location
//  (smallest location value first), where the specific edge is specified by dwEdge .
	{
	TArray <int> aPanelIndices;
	TArray <int> aEdgeLocations;
	IPanel *pPanel;

	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		aPanelIndices.Insert(i);
		pPanel = m_aPanels[i];
		if (pPanel->IsHidden())
			{
			aEdgeLocations.Insert(-1);
			}
		else
			{
			aEdgeLocations.Insert(pPanel->PanelRect.GetEdgePosition(dwEdge));
			}
		}

	TArray <int> aSortedPanelIndices = QuickSortIntegerArray(aEdgeLocations, aPanelIndices);

	return aSortedPanelIndices;
	}

void CPanelOrganizer::SmoothOut (DWORD dwSmoothType)
	{
	TArray <int> aSortedPanelIndices;
	int iShift;
	int iSharedRectBoundary;

	RECT rcOther;

	if (dwSmoothType == SMOOTH_LEFTRIGHT)
		{
		aSortedPanelIndices = SortByPanelRectEdgeLocation(EDGE_LEFT);
		}
	else if (dwSmoothType == SMOOTH_UPDOWN)
		{
		aSortedPanelIndices = SortByPanelRectEdgeLocation(EDGE_TOP);
		}

	int iFocusIndex;
	int iOtherIndex;

	int iFocusLocation;
	int iOtherEdgeLocation;

	int iProspectiveOtherIndex;

	IPanel *pFocusPanel;
	IPanel *pOtherPanel;

	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		iFocusIndex = aSortedPanelIndices[i];
		pFocusPanel = m_aPanels[iFocusIndex];

		if (pFocusPanel->IsHidden())
			continue;

		if (i != 0)
			{
			iOtherIndex = -1;
			
			for (int iLoopCount = 0; iLoopCount < i; iLoopCount++)
				{
				iProspectiveOtherIndex = aSortedPanelIndices[i - 1 - iLoopCount];
				pOtherPanel = m_aPanels[iProspectiveOtherIndex];

				if (!(pOtherPanel->IsHidden()))
					{
					iOtherIndex = iProspectiveOtherIndex;
					break;
					}
				}
			}
		else
			{
			iOtherIndex = -1;
			}
			
		if (iOtherIndex != -1)
			{
			pOtherPanel = m_aPanels[iOtherIndex];
			rcOther = pOtherPanel->PanelRect.GetAsRect();

			if (dwSmoothType == SMOOTH_LEFTRIGHT)
				{
				iOtherEdgeLocation = pOtherPanel->PanelRect.GetEdgePosition(EDGE_RIGHT);
				}
			else if (dwSmoothType == SMOOTH_UPDOWN)
				{
				iOtherEdgeLocation = pOtherPanel->PanelRect.GetEdgePosition(EDGE_BOTTOM);
				}
			}
		else
			{
			rcOther = m_ParentPanel.PanelRect.GetAsRect();

			if (dwSmoothType == SMOOTH_LEFTRIGHT)
				{
				iOtherEdgeLocation = rcOther.left;
				}
			else if (dwSmoothType == SMOOTH_UPDOWN)
				{
				iOtherEdgeLocation = rcOther.top;
				}
			}

		if (dwSmoothType == SMOOTH_LEFTRIGHT)
			{
			iFocusLocation = pFocusPanel->PanelRect.GetEdgePosition(EDGE_LEFT);
			}
		else if (dwSmoothType == SMOOTH_UPDOWN)
			{
			iFocusLocation = pFocusPanel->PanelRect.GetEdgePosition(EDGE_TOP);
			}
			
		iShift = (iFocusLocation - iOtherEdgeLocation);

		if (iShift == 0)
			{
			continue;
			}

		 if (dwSmoothType == SMOOTH_LEFTRIGHT)
			{
			pFocusPanel->PanelRect.ShiftOrigin(iShift, 0);

			iSharedRectBoundary = GetSharedLeftRightEdgeLength(&pFocusPanel->PanelRect.GetAsRect(), &rcOther);

			if (!(iSharedRectBoundary > 0))
				{
				pFocusPanel->PanelRect.ShiftOrigin(-1*iShift, 0);
				}
			}
		else if (dwSmoothType == SMOOTH_UPDOWN)
			{
			pFocusPanel->PanelRect.ShiftOrigin(0, iShift);

			iSharedRectBoundary = GetSharedTopBottomEdgeLength(&pFocusPanel->PanelRect.GetAsRect(), &rcOther);
			if (!(iSharedRectBoundary > 0))
				{
				pFocusPanel->PanelRect.ShiftOrigin(0, -1*iShift);
				}
			}
		}
	}

void CPanelOrganizer::PlacePanel (IPanel *pPanel, int iRelativeOriginX, int iRelativeOriginY)
	{
	int iOriginX = m_ParentPanel.PanelRect.GetOriginX() + iRelativeOriginX;
	int iOriginY = m_ParentPanel.PanelRect.GetOriginY() + iRelativeOriginY;

	pPanel->PanelRect.SetOrigin(iOriginX, iOriginY);
	m_aPanels.Insert(pPanel);
	pPanel->ConfirmPlacement();

	pPanel->SetParentPanel(&m_ParentPanel);
	pPanel->SetViewOffset(m_ParentPanel.GetViewOffsetX(), m_ParentPanel.GetViewOffsetY());

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelOrganizer::DeletePanel (int iPanelIndex)
	{
	m_aPanels.Delete(iPanelIndex);
	}

TArray <IPanel *> CPanelOrganizer::GetPanelsContainingPoint (int x, int y)
	{
	TArray <IPanel *> aRelevantPanels;

	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		aRelevantPanels.Insert(m_aPanels[i]->GetPanelsContainingPoint(x, y));
		}

	return aRelevantPanels;
	}

int CPanelOrganizer::GetPanelIndex (IPanel *pPanel)
	{
	int iDefaultIndex = -1;	//  not found in m_aInternalPanels

	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		if (m_aPanels[i] == pPanel)
			{
			return i;
			}
		}

	return iDefaultIndex;
	}

void CPanelOrganizer::HidePanel (IPanel *Panel)
	{
	int iPanelIndex = GetPanelIndex(Panel);

	if (iPanelIndex == -1)
		{
		return;
		}

	//  hide the panel first so other panels can take its space during rearrangement
	m_aPanels[iPanelIndex]->SetHiddenFlag(true);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelOrganizer::HidePanel (int iPanelIndex)
	{
	//  hide the panel first so other panels can take its space during rearrangement
	m_aPanels[iPanelIndex]->SetHiddenFlag(true);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelOrganizer::HideAll (void)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->SetHiddenFlag(true);
		}

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelOrganizer::ShowPanel (IPanel *Panel)
	{
	int iPanelIndex = GetPanelIndex(Panel);

	if (iPanelIndex == -1)
		{
		return;
		}

	IPanel *FocusPanel = m_aPanels[iPanelIndex];

	//  unhide the panel so that space can be made for it
	FocusPanel->SetHiddenFlag(false);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelOrganizer::ShowPanel (int iPanelIndex)
	{
	IPanel *FocusPanel = m_aPanels[iPanelIndex];

	//  unhide the panel so that space can be made for it
	FocusPanel->SetHiddenFlag(false);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelOrganizer::ShowAll (void)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->SetHiddenFlag(false);
		}

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelOrganizer::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->OnPaint(Screen, rcInvalid);
		}
	}

void CPanelOrganizer::Invalidate(void)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->Invalidate();
		}
	}

// ============================================================================

IPanel::IPanel (CString sName, CHumanInterface &HI) : IHISession(HI),
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
	m_pHeader(NULL),
	m_pScrollBar(NULL),
	PanelRect(*this),
	PanelOrganizer(*this)
	{
	}

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
	m_pHeader(NULL),
	m_pScrollBar(NULL),
	PanelRect(*this, iWidth, iHeight),
	PanelOrganizer(*this)
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

void IPanel::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	PaintBackground(Screen);
	PaintOutline(Screen);

	OnPanelPaint(Screen, rcInvalid);

	PanelOrganizer.OnPaint(Screen, rcInvalid);
	}

TArray <IPanel *> IPanel::GetPanelsContainingPoint (int x, int y)
	{
	TArray <IPanel *> aRelevantPanels;

	if (!IsHidden())
		{
		if (IsPointInRect(x, y, PanelRect.GetAsRect()))
			{
			aRelevantPanels.Insert(this);

			// if this panel contains this point, then some of its children might too
			aRelevantPanels.Insert(PanelOrganizer.GetPanelsContainingPoint(x, y));
			}
		}

	return aRelevantPanels;
	}


void IPanel::Hide(void)
	{
	m_bHidden = true;
	PanelOrganizer.HideAll();

	if (m_pParentPanel != NULL)
		{
		m_pParentPanel->PanelOrganizer.SmoothOut(SMOOTH_UPDOWN);
		m_pParentPanel->PanelOrganizer.SmoothOut(SMOOTH_LEFTRIGHT);
		}
	}

void IPanel::Show(void)
	{
	m_bHidden = false;
	PanelOrganizer.ShowAll();

	if (m_pParentPanel != NULL)
		{
		m_pParentPanel->PanelOrganizer.SmoothOut(SMOOTH_UPDOWN);
		m_pParentPanel->PanelOrganizer.SmoothOut(SMOOTH_LEFTRIGHT);
		}
	}

void IPanel::Invalidate()
	{
	HIInvalidate();
	PanelOrganizer.Invalidate();
	}

void IPanel::FitChildrenExactly (void)
	{
	if (m_pParentPanel == NULL)
		{
		return;
		}

	TArray <int> aPanelIndices = PanelOrganizer.SortByPanelRectEdgeLocation(EDGE_BOTTOM);
	TArray <IPanel *> aPanels = PanelOrganizer.GetPanels();

	int iNumPanels = aPanelIndices.GetCount();
	int iLastPanelIndex = aPanelIndices[iNumPanels-1];
	int iLastPanelBottom = aPanels[iLastPanelIndex]->PanelRect.GetEdgePosition(EDGE_BOTTOM);
	
	m_pParentPanel->PanelRect.SetEdgePosition(EDGE_BOTTOM, iLastPanelBottom);

	aPanelIndices = PanelOrganizer.SortByPanelRectEdgeLocation(EDGE_RIGHT);
	iNumPanels = aPanelIndices.GetCount();
	iLastPanelIndex = aPanelIndices[iNumPanels-1];
	int iLastPanelRight = aPanels[iLastPanelIndex]->PanelRect.GetEdgePosition(EDGE_RIGHT);
	
	m_pParentPanel->PanelRect.SetEdgePosition(EDGE_RIGHT, iLastPanelRight);
	}

void IPanel::SetViewOffset (int iOffsetX, int iOffsetY)
	{
	int iShiftX = iOffsetX - m_iViewOffsetX;
	m_iViewOffsetX = iOffsetX;

	int iShiftY = iOffsetY - m_iViewOffsetY;
	m_iViewOffsetY = iOffsetY;

	PanelRect.ShiftOrigin(iShiftX, iShiftY);
	PanelOrganizer.ShiftAllOrigins(iShiftX, iShiftY);
	}

void IPanel::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{
	m_bLButtonDown = true;
	OnPanelLButtonDown(x, y, dwFlags, retbCapture);
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

	OnPanelLButtonUp(x, y, dwFlags);
	}

void IPanel::OnLButtonDblClick(int x, int y, DWORD dwFlags, bool * retbCapture)
	{
	m_bLDblClicked = true;
	m_bLButtonDown = false;
	m_bLClicked = false;

	OnPanelLButtonDblClick(x, y, dwFlags, retbCapture);
	}

void IPanel::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	m_bRButtonDown = true;

	OnPanelRButtonDown(x, y, dwFlags);
	}

void IPanel::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	if (IsRButtonDown())
		{
		m_bRButtonDown = false;
		m_bRClicked = true;
		}
	OnPanelRButtonUp(x, y, dwFlags);
	}

void IPanel::OnKeyDown(int iVirtKey, DWORD dwKeyData)
	{
	OnPanelKeyDown(iVirtKey, dwKeyData);
	}

void IPanel::OnKeyUp(int iVirtKey, DWORD dwKeyData)
	{
	OnPanelKeyUp(iVirtKey, dwKeyData);
	}

void IPanel::OnChar(char chChar, DWORD dwKeyData)
	{
	OnPanelChar(chChar, dwKeyData);
	}
