//	CSTransmuter.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

// ===========================================================================

CPanelArea::CPanelArea (CPanel &AssociatedPanel) :
	m_AssociatedPanel(AssociatedPanel),
	m_rcArea(MakeRect(0, 0, 0, 0))
	{
	}

CPanelArea::CPanelArea (CPanel &AssociatedPanel, RECT rcArea) :
	m_AssociatedPanel(AssociatedPanel),
	m_rcArea(rcArea)
	{
	}

RECT CPanelArea::GetAbsoluteRect (void)
	{
	return m_rcArea;
	}

RECT CPanelArea::GetViewOffsetRect (void)
	{
	RECT rc;

	rc.bottom = GetViewOffsetEdgeLocation(EDGE_BOTTOM);
	rc.left = GetViewOffsetEdgeLocation(EDGE_LEFT);
	rc.right = GetViewOffsetEdgeLocation(EDGE_RIGHT);
	rc.top = GetViewOffsetEdgeLocation(EDGE_TOP);

	return rc;
	}

int CPanelArea::GetAbsoluteEdgeLocation (DWORD dwEdge)
	{
	if (dwEdge == EDGE_BOTTOM)
		{
		return m_rcArea.bottom;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		return m_rcArea.left;
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		return m_rcArea.right;
		}
	else if (dwEdge == EDGE_TOP)
		{
		return m_rcArea.top;
		}
	else
		{
		//  error
		m_AssociatedPanel.SetError (CONSTLIT("Invalid edge flag provided."));
		return 0;
		}
	}

int CPanelArea::GetViewOffsetEdgeLocation (DWORD dwEdge)
	{
	int iViewOffsetY = m_AssociatedPanel.GetViewOffsetY();
	int iViewOffsetX = m_AssociatedPanel.GetViewOffsetX();

	if (dwEdge == EDGE_BOTTOM)
		{
		return m_rcArea.bottom + m_AssociatedPanel.GetViewOffsetY();
		}
	else if (dwEdge == EDGE_LEFT)
		{
		return m_rcArea.left + m_AssociatedPanel.GetViewOffsetX();
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		return m_rcArea.right + m_AssociatedPanel.GetViewOffsetX();
		}
	else if (dwEdge == EDGE_TOP)
		{
		return m_rcArea.top + m_AssociatedPanel.GetViewOffsetY();
		}
	else
		{
		//  error
		m_AssociatedPanel.SetError (CONSTLIT("Invalid edge flag provided."));
		return 0;
		}
	}

void CPanelArea::SetAbsoluteEdgeAt (DWORD dwEdge, int iLocation)
//  SetAbsoluteEdgeAt
// 
//  Set location of area edge.
	{
	int iShift;

	if (dwEdge == EDGE_BOTTOM)
		{
		iShift = iLocation - GetAbsoluteEdgeLocation(EDGE_BOTTOM);
		m_rcArea.bottom = iLocation;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		iShift = iLocation - GetAbsoluteEdgeLocation(EDGE_LEFT);
		m_rcArea.left = iLocation;

		m_AssociatedPanel.InternalPanels.ShiftEdges(dwEdge, iShift);
		m_AssociatedPanel.Invalidate();
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		iShift = iLocation - GetAbsoluteEdgeLocation(EDGE_RIGHT);
		m_rcArea.right = iLocation;
		}
	else if (dwEdge == EDGE_TOP)
		{
		iShift = iLocation - GetAbsoluteEdgeLocation(EDGE_TOP);
		m_rcArea.top = iLocation;
		m_AssociatedPanel.InternalPanels.ShiftEdges(dwEdge, iShift);
		m_AssociatedPanel.Invalidate();
		}
	}

void CPanelArea::ShiftEdge (DWORD dwEdge, int iShift)
	{
	int iLocation; 

	if (dwEdge == EDGE_BOTTOM)
		{
		iLocation = iShift + m_rcArea.bottom;
		m_rcArea.bottom = iLocation;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		iLocation = iShift + m_rcArea.left;
		m_rcArea.left = iLocation;

		m_AssociatedPanel.InternalPanels.ShiftEdges(dwEdge, iShift);
		m_AssociatedPanel.Invalidate();
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		iLocation = iShift + m_rcArea.right;
		m_rcArea.right = iLocation;
		}
	else if (dwEdge == EDGE_TOP)
		{
		iLocation = iShift + m_rcArea.top;
		m_rcArea.top = iLocation;

		m_AssociatedPanel.InternalPanels.ShiftEdges(dwEdge, iShift);
		m_AssociatedPanel.Invalidate();
		}
	}

void CPanelArea::FitChildrenExactly (void)
	{
	CPanel *pParentPanel = m_AssociatedPanel.GetParentPanel();

	if (pParentPanel == NULL)
		{
		return;
		}

	TArray <int> aPanelIndices = m_AssociatedPanel.InternalPanels.SortByScreenAreaEdgeLocation(EDGE_BOTTOM);
	TArray <CPanel *> aPanels = m_AssociatedPanel.InternalPanels.GetPanels();

	int iNumPanels = aPanelIndices.GetCount();
	int iLastPanelIndex = aPanelIndices[iNumPanels-1];
	int iLastPanelBottom = aPanels[iLastPanelIndex]->ScreenArea.GetAbsoluteEdgeLocation(EDGE_BOTTOM);
	
	pParentPanel->ScreenArea.SetAbsoluteEdgeAt(EDGE_BOTTOM, iLastPanelBottom);

	aPanelIndices = m_AssociatedPanel.InternalPanels.SortByScreenAreaEdgeLocation(EDGE_RIGHT);
	iNumPanels = aPanelIndices.GetCount();
	iLastPanelIndex = aPanelIndices[iNumPanels-1];
	int iLastPanelRight = aPanels[iLastPanelIndex]->ScreenArea.GetAbsoluteEdgeLocation(EDGE_RIGHT);
	
	pParentPanel->ScreenArea.SetAbsoluteEdgeAt(EDGE_RIGHT, iLastPanelRight);
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

TArray <int> CInternalPanels::SortByScreenAreaEdgeLocation (DWORD dwEdge)
//  SortInternalPanelsByScreenSapceEdgeLocation
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
			aEdgeLocations.Insert(pPanel->ScreenArea.GetAbsoluteEdgeLocation(dwEdge));
			}
		}

	TArray <int> aSortedPanelIndices = QuickSortIntegerArray(aEdgeLocations, aPanelIndices);

	return aSortedPanelIndices;
	}

void CInternalPanels::SmoothOut (DWORD dwSmoothType)
	{
	TArray <int> aSortedPanelIndices;
	int iProposedChange;
	int iSharedRectBoundary;

	RECT rcChangedFocus;
	RECT rcOther;

	if (dwSmoothType == SMOOTH_LEFTRIGHT)
		{
		aSortedPanelIndices = SortByScreenAreaEdgeLocation(EDGE_LEFT);
		}
	else if (dwSmoothType == SMOOTH_UPDOWN)
		{
		aSortedPanelIndices = SortByScreenAreaEdgeLocation(EDGE_TOP);
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

		iStickyLeftOffset = 0;
		iStickyTopOffset = 0;

		rcChangedFocus = pFocusPanel->ScreenArea.GetViewOffsetRect();

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
			rcOther = pOtherPanel->ScreenArea.GetViewOffsetRect();

			if (dwSmoothType == SMOOTH_LEFTRIGHT)
				{
				iOtherEdgeLocation = pOtherPanel->ScreenArea.GetAbsoluteEdgeLocation(EDGE_RIGHT);
				}
			else if (dwSmoothType == SMOOTH_UPDOWN)
				{
				iOtherEdgeLocation = pOtherPanel->ScreenArea.GetAbsoluteEdgeLocation(EDGE_BOTTOM);
				}
			}
		else
			{
			rcOther = m_ParentPanel.ScreenArea.GetAbsoluteRect();

			if (pFocusPanel->IsSticky())
				{
				iStickyLeftOffset = pFocusPanel->GetStickyLeftOffset();
				iStickyTopOffset = pFocusPanel->GetStickyTopOffset();
				}

			if (dwSmoothType == SMOOTH_LEFTRIGHT)
				{
				iOtherEdgeLocation = rcOther.left + iStickyLeftOffset;
				}
			else if (dwSmoothType == SMOOTH_UPDOWN)
				{
				iOtherEdgeLocation = rcOther.top + iStickyTopOffset;
				}
			}

		if (dwSmoothType == SMOOTH_LEFTRIGHT)
			{
			iFocusLocation = pFocusPanel->ScreenArea.GetAbsoluteEdgeLocation(EDGE_LEFT);
			}
		else if (dwSmoothType == SMOOTH_UPDOWN)
			{
			iFocusLocation = pFocusPanel->ScreenArea.GetAbsoluteEdgeLocation(EDGE_TOP);
			}
			
		iProposedChange = (iFocusLocation - iOtherEdgeLocation);

		if (iProposedChange == 0)
			{
			continue;
			}

		 if (dwSmoothType == SMOOTH_LEFTRIGHT)
			{
			rcChangedFocus.left = iOtherEdgeLocation;
			rcChangedFocus.right = iOtherEdgeLocation + pFocusPanel->ScreenArea.GetWidth();

			iSharedRectBoundary = GetSharedLeftRightEdgeLength(&rcChangedFocus, &rcOther);
			if (iSharedRectBoundary > 0)
				{
				pFocusPanel->ScreenArea.SetAbsoluteEdgeAt(EDGE_LEFT, rcChangedFocus.left);
				pFocusPanel->ScreenArea.SetAbsoluteEdgeAt(EDGE_RIGHT, rcChangedFocus.right);
				}
			}
		else if (dwSmoothType == SMOOTH_UPDOWN)
			{
			rcChangedFocus.top = iOtherEdgeLocation;
			rcChangedFocus.bottom = iOtherEdgeLocation + pFocusPanel->ScreenArea.GetHeight(); 

			iSharedRectBoundary = GetSharedTopBottomEdgeLength(&rcChangedFocus, &rcOther);
			if (iSharedRectBoundary > 0)
				{
				pFocusPanel->ScreenArea.SetAbsoluteEdgeAt(EDGE_BOTTOM, rcChangedFocus.bottom);
				pFocusPanel->ScreenArea.SetAbsoluteEdgeAt(EDGE_TOP, rcChangedFocus.top);
				}
			}
		}
	}

CPanel *CInternalPanels::Add (int iRelativeLeft, int iRelativeTop, int iWidth, int iHeight, bool bHidden, bool bSticky)
	{
	CPanel *NewPanel = NULL;
	RECT rcParent = m_ParentPanel.ScreenArea.GetAbsoluteRect();
	RECT rcScreenArea = MakeRect(rcParent, iRelativeLeft, iRelativeTop, iWidth, iHeight);
	NewPanel = new CPanel(rcScreenArea);

	NewPanel->SetHiddenFlag(bHidden);
	NewPanel->SetParentPanel(&m_ParentPanel);
	NewPanel->SetViewOffsetX(m_ParentPanel.GetViewOffsetX());
	NewPanel->SetViewOffsetY(m_ParentPanel.GetViewOffsetY());

	if (bSticky)
		{
		NewPanel->MakeSticky(iRelativeLeft, iRelativeTop);
		}

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

void CInternalPanels::Invalidate (void)
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

void CInternalPanels::SetViewOffsetX (int iOffset)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->SetViewOffsetX(iOffset);
		}
	}

void CInternalPanels::SetViewOffsetY (int iOffset)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->SetViewOffsetY(iOffset);
		}
	}

void CInternalPanels::ShiftEdges (DWORD dwEdge, int iShift)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->ScreenArea.ShiftEdge(dwEdge, iShift);
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
	m_bSticky(false),
	m_iStickyLeftOffset(0),
	m_iStickyTopOffset(0),
	ScreenArea(*this),
	InternalPanels(*this)
	{
	}

CPanel::CPanel (RECT rcScreenArea) : 
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	m_bSticky(false),
	m_iStickyLeftOffset(0),
	m_iStickyTopOffset(0),
	ScreenArea(*this, rcScreenArea),
	InternalPanels(*this)
	{
	}

CPanel::CPanel (int iLeft, int iTop, int iWidth, int iHeight) :
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	m_bSticky(false),
	m_iStickyLeftOffset(0),
	m_iStickyTopOffset(0),
	ScreenArea(*this, MakeRect(iLeft, iTop, iWidth, iHeight)),
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
		if (IsPointInRect(x, y, ScreenArea.GetAbsoluteRect()))
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

	InternalPanels.Invalidate();
	}