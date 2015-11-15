//	CSTransmuter.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CPanelRect::CPanelRect (CPanel &AssociatedPanel) : 
	m_iOriginX(0),
	m_iOriginY(0),
	m_iHeight(0),
	m_iWidth(0),
	m_AssociatedPanel(AssociatedPanel)
	{
	}

CPanelRect::CPanelRect (CPanel &AssociatedPanel, int iOriginX, int iOriginY, int iHeight, int iWidth) :
	m_iOriginX(iOriginX),
	m_iOriginY(iOriginY),
	m_iHeight(iHeight),
	m_iWidth(iWidth),
	m_AssociatedPanel(AssociatedPanel)
	{
	}

CPanelRect::CPanelRect (CPanel &AssociatedPanel, RECT rc) :
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
		throw CMessageException("Unknown DWORD given for EDGE!");
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
		throw CMessageException("Unknown DWORD given for EDGE!");
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
		throw CMessageException("Unknown DWORD given for EDGE!");
		}
	}

// ===========================================================================

CInternalPanels::CInternalPanels(CPanel &ParentPanel) :
	m_ParentPanel(ParentPanel)
	{
	}

CInternalPanels::~CInternalPanels()
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		delete m_aPanels[i];
		}
	}

TArray <int> CInternalPanels::SortByPanelRectEdgeLocation (DWORD dwEdge)
//  SortByPanelRectEdgeLocation
// 
//  Return an array of integers whose elements correspond with indices of panels
//  in the m_aInternalPanels array. Panel indices are sorted by edge location
//  (smallest location value first), where the specific edge is specified by dwEdge .
	{
	TArray <int> aPanelIndices;
	TArray <int> aEdgeLocations;
	CPanel *pPanel;

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

void CInternalPanels::SmoothOut (DWORD dwSmoothType)
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

	int iStickyLeftOffset;
	int iStickyTopOffset;
	int iProspectiveOtherIndex;

	CPanel *pFocusPanel;
	CPanel *pOtherPanel;

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

CPanel *CInternalPanels::AddPanel (int iRelativeOriginX, int iRelativeOriginY, int iWidth, int iHeight, bool bHidden)
	{
	CPanel *NewPanel = NULL;
	NewPanel = new CPanel();

	NewPanel->PanelRect.SetOrigin(m_ParentPanel.PanelRect.GetOriginX() + iRelativeOriginX, m_ParentPanel.PanelRect.GetOriginY() + iRelativeOriginY);

	NewPanel->SetHiddenFlag(bHidden);
	NewPanel->SetParentPanel(&m_ParentPanel);
	NewPanel->SetViewOffset(m_ParentPanel.GetViewOffsetX(), m_ParentPanel.GetViewOffsetY());

	m_aPanels.Insert(NewPanel);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);

	return NewPanel;
	}

TArray <CSChild *> CInternalPanels::GetAssociatedSessions (void)
	{
	TArray <CSChild *> InternalSessions;

	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		InternalSessions.Insert(m_aPanels[i]->GetAssociatedSession());
		}

	return InternalSessions;
	}


TArray <CSChild *> CInternalPanels::GetSessionsContainingPoint (int x, int y)
	{
	TArray <CSChild *> aRelevantSessions;

	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		aRelevantSessions.Insert(m_aPanels[i]->GetSessionsContainingPoint(x, y));
		}

	return aRelevantSessions;
	}

int CInternalPanels::GetPanelIndex (CPanel *pPanel)
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

void CInternalPanels::HidePanel (CPanel *Panel)
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

void CInternalPanels::HidePanel (int iPanelIndex)
	{
	//  hide the panel first so other panels can take its space during rearrangement
	m_aPanels[iPanelIndex]->SetHiddenFlag(true);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CInternalPanels::HideAll (void)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->SetHiddenFlag(true);
		}

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CInternalPanels::ShowPanel (CPanel *Panel)
	{
	int iPanelIndex = GetPanelIndex(Panel);

	if (iPanelIndex == -1)
		{
		return;
		}

	CPanel *FocusPanel = m_aPanels[iPanelIndex];

	//  unhide the panel so that space can be made for it
	FocusPanel->SetHiddenFlag(false);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CInternalPanels::ShowPanel (int iPanelIndex)
	{
	CPanel *FocusPanel = m_aPanels[iPanelIndex];

	//  unhide the panel so that space can be made for it
	FocusPanel->SetHiddenFlag(false);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CInternalPanels::ShowAll (void)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->SetHiddenFlag(false);
		}

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CInternalPanels::InvalidateAll (void)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->Invalidate();
		}
	}

void CInternalPanels::PaintAll (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->OnPaint(Screen, rcInvalid);
		}
	}


void CInternalPanels::ShiftAllOrigins (int iShiftX, int iShiftY)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->PanelRect.ShiftOrigin(iShiftX, iShiftY);
		}
	}

// ============================================================================

CPanel::CPanel(void) :
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	PanelRect(*this),
	InternalPanels(*this)
	{
	}

void CPanel::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (!IsEmpty() && !IsHidden())
		{
		m_pAssociatedSession->OnPaint(Screen, rcInvalid);
		}

	InternalPanels.PaintAll(Screen, rcInvalid);
	}

TArray <CSChild *> CPanel::GetSessionsContainingPoint (int x, int y)
	{
	TArray <CSChild *> aRelevantSessions;

	if (!IsEmpty() && !IsHidden())
		{
		if (IsPointInRect(x, y, PanelRect.GetAsRect()))
			{
			aRelevantSessions.Insert(m_pAssociatedSession);
			}
		}

	aRelevantSessions.Insert(InternalPanels.GetSessionsContainingPoint(x, y));

	return aRelevantSessions;
	}

void CPanel::Hide(void)
	{
	m_bHidden = true;
	InternalPanels.HideAll();

	if (m_pParentPanel != NULL)
		{
		m_pParentPanel->InternalPanels.SmoothOut(SMOOTH_UPDOWN);
		m_pParentPanel->InternalPanels.SmoothOut(SMOOTH_LEFTRIGHT);
		}
	}

void CPanel::Show(void)
	{
	m_bHidden = false;
	InternalPanels.ShowAll();

	if (m_pParentPanel != NULL)
		{
		m_pParentPanel->InternalPanels.SmoothOut(SMOOTH_UPDOWN);
		m_pParentPanel->InternalPanels.SmoothOut(SMOOTH_LEFTRIGHT);
		}
	}

void CPanel::Invalidate()
	{
	if (!IsEmpty())
		{
		m_pAssociatedSession->HIInvalidate();
		}

	InternalPanels.InvalidateAll();
	}

void CPanel::FitChildrenExactly (void)
	{
	if (m_pParentPanel == NULL)
		{
		return;
		}

	TArray <int> aPanelIndices = InternalPanels.SortByPanelRectEdgeLocation(EDGE_BOTTOM);
	TArray <CPanel *> aPanels = InternalPanels.GetPanels();

	int iNumPanels = aPanelIndices.GetCount();
	int iLastPanelIndex = aPanelIndices[iNumPanels-1];
	int iLastPanelBottom = aPanels[iLastPanelIndex]->PanelRect.GetEdgePosition(EDGE_BOTTOM);
	
	m_pParentPanel->PanelRect.SetEdgePosition(EDGE_BOTTOM, iLastPanelBottom);

	aPanelIndices = InternalPanels.SortByPanelRectEdgeLocation(EDGE_RIGHT);
	iNumPanels = aPanelIndices.GetCount();
	iLastPanelIndex = aPanelIndices[iNumPanels-1];
	int iLastPanelRight = aPanels[iLastPanelIndex]->PanelRect.GetEdgePosition(EDGE_RIGHT);
	
	m_pParentPanel->PanelRect.SetEdgePosition(EDGE_RIGHT, iLastPanelRight);
	}

void CPanel::SetViewOffset (int iOffsetX, int iOffsetY)
	{
	int iShiftX = iOffsetX - m_iViewOffsetX;
	m_iViewOffsetX = iOffsetX;

	int iShiftY = iOffsetY - m_iViewOffsetY;
	m_iViewOffsetY = iOffsetY;

	PanelRect.ShiftOrigin(iShiftX, iShiftY);
	InternalPanels.ShiftAllOrigins(iShiftX, iShiftY);
	}