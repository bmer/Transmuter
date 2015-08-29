//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

double CalculateRelativeRigidity (double dRigidity1, double dRigidity2)
//  CalculateRelativeRigidity
//
//  Calculate the relative dRigidity of the first panel with respect to the second panel.
	{
	if ((dRigidity1 == dRigidity2) || (dRigidity1 == 0 && dRigidity2 == 0))
		{
		return 0.5;
		}
	else
		{
		return dRigidity1/(dRigidity1 + dRigidity2);
		}
	}

CPanel::CPanel(void) :
	m_dRigidity(0),
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	m_iNumInternalPanels(0)
	{
	m_rcPanel.bottom = -1;
	m_rcPanel.left = -1;
	m_rcPanel.right = -1;
	m_rcPanel.top = -1;
	}

CPanel::CPanel (RECT rcPanelRect) : m_rcPanel(rcPanelRect),
	m_dRigidity(0),
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	m_iNumInternalPanels(0)
	{
	}

CPanel::CPanel (RECT rcPanel, double dRigidity) : m_rcPanel(rcPanel),
	m_dRigidity(dRigidity),
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	m_iNumInternalPanels(0)
	{
	}

CPanel::CPanel (int iLeft, int iTop, int iRight, int iBottom) :
	m_dRigidity(0),
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	m_iNumInternalPanels(0)
	{
	m_rcPanel.bottom = iBottom;
	m_rcPanel.left = iLeft;
	m_rcPanel.right = iRight;
	m_rcPanel.top = iTop;
	}

CPanel::CPanel (int iLeft, int iTop, int iRight, int iBottom, double dRigidity) :
	m_dRigidity(dRigidity),
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	m_iNumInternalPanels(0)
	{
	m_rcPanel.bottom = iBottom;
	m_rcPanel.left = iLeft;
	m_rcPanel.right = iRight;
	m_rcPanel.top = iTop;
	}

CPanel::~CPanel ()
	{
	for (int i = 0; i < m_iNumInternalPanels; i++)
		{
		delete &m_aInternalPanels[i];
		}
	}

void CPanel::ShiftPanelEdgeLocation (DWORD dwEdge, int iShift)
	{
	int iLocation; 

	if (dwEdge == EDGE_BOTTOM)
		{
		iLocation = iShift + m_rcPanel.bottom;
		m_rcPanel.bottom = iLocation;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		iLocation = iShift + m_rcPanel.left;
		m_rcPanel.left = iLocation;
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		iLocation = iShift + m_rcPanel.right;
		m_rcPanel.right = iLocation;
		}
	else if (dwEdge == EDGE_TOP)
		{
		iLocation = iShift + m_rcPanel.top;
		m_rcPanel.top = iLocation;
		}

	for (int i = 0; i < m_aInternalPanels.GetCount(); i++)
		{
		m_aInternalPanels[i]->ShiftPanelEdgeLocation (dwEdge, iShift);
		}
	}

void CPanel::SetPanelEdgeLocation (DWORD dwEdge, int iLocation)
//  SetPanelEdgeLocation
// 
//  Set location of panel edge.
	{
	int iShift;

	if (dwEdge == EDGE_BOTTOM)
		{
		iShift = iLocation - m_rcPanel.bottom;
		m_rcPanel.bottom = iLocation;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		iShift = iLocation - m_rcPanel.left;
		m_rcPanel.left = iLocation;
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		iShift = iLocation - m_rcPanel.right;
		m_rcPanel.right = iLocation;
		}
	else if (dwEdge == EDGE_TOP)
		{
		iShift = iLocation - m_rcPanel.top;
		m_rcPanel.top = iLocation;
		}

	for (int i = 0; i < m_aInternalPanels.GetCount(); i++)
		{
		m_aInternalPanels[i]->ShiftPanelEdgeLocation (dwEdge, iShift);
		}
	}

int CPanel::GetPanelEdgeLocation (DWORD dwEdge)
	{
	if (dwEdge == EDGE_BOTTOM)
		{
		return m_rcPanel.bottom;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		return m_rcPanel.left;
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		return m_rcPanel.right;
		}
	else if (dwEdge == EDGE_TOP)
		{
		return m_rcPanel.top;
		}
	else
		{
		//  error
		return -1;
		}
	}

TArray <int> CPanel::SortInternalPanelsByEdgeLocation (DWORD dwEdge)
//  SortInternalPanelsByEdgeLocation
// 
//  Return an array of integers whose elements correspond with indices of panels
//  in the m_aInternalPanels array. Panel indices are sorted by edge location
//  (smallest location value first), where the specific edge is specified by dwEdge .
	{
	TArray <int> aPanelIndices;
	TArray <int> aEdgeLocations;
	CPanel *pInternalPanel;

	for (int i = 0; i < m_aInternalPanels.GetCount(); i++)
		{
		aPanelIndices.Insert(i);
		pInternalPanel = m_aInternalPanels[i];
		if (pInternalPanel->IsHidden())
			{
			aEdgeLocations.Insert(-1);
			}
		else
			{
			aEdgeLocations.Insert(pInternalPanel->GetPanelEdgeLocation(dwEdge));
			}
		}

	TArray <int> aSortedPanelIndices = QuickSortIntegerArray(aEdgeLocations, aPanelIndices);

	return aSortedPanelIndices;
	}

void CPanel::SmoothOutInternalPanels (bool bExpandInPlace, DWORD dwSmoothType)
	{
	TArray <int> aSortedPanelIndices;
	int iProposedChange;
	int iSharedRectBoundary;

	RECT rcChangedFocus;
	RECT rcOther;

	if (dwSmoothType == SMOOTH_LEFTRIGHT)
		{
		aSortedPanelIndices = SortInternalPanelsByEdgeLocation(EDGE_LEFT);
		}
	else if (dwSmoothType == SMOOTH_UPDOWN)
		{
		aSortedPanelIndices = SortInternalPanelsByEdgeLocation(EDGE_TOP);
		}

	int iFocusIndex;
	int iOtherIndex;

	int iFocusLocation;
	int iOtherEdgeLocation;

	int iProspectiveOtherIndex;

	CPanel *pFocusPanel;
	CPanel *pOtherPanel;

	for (int i = 0; i < m_iNumInternalPanels; i++)
		{
		iFocusIndex = aSortedPanelIndices[i];
		pFocusPanel = m_aInternalPanels[iFocusIndex];

		if (pFocusPanel->IsHidden())
			continue;
		
		rcChangedFocus = pFocusPanel->GetPanelRect();

		if (i != 0)
			{
			iOtherIndex = -1;
			
			for (int iLoopCount = 0; iLoopCount < i; iLoopCount++)
				{
				iProspectiveOtherIndex = aSortedPanelIndices[i - 1 - iLoopCount];
				pOtherPanel = m_aInternalPanels[iProspectiveOtherIndex];

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
			pOtherPanel = m_aInternalPanels[iOtherIndex];
			rcOther = pOtherPanel->GetPanelRect();

			if (dwSmoothType == SMOOTH_LEFTRIGHT)
				{
				iOtherEdgeLocation = pOtherPanel->GetPanelEdgeLocation(EDGE_RIGHT);
				}
			else if (dwSmoothType == SMOOTH_UPDOWN)
				{
				iOtherEdgeLocation = pOtherPanel->GetPanelEdgeLocation(EDGE_BOTTOM);
				}
			}
		else
			{
			rcOther = this->GetPanelRect();
			if (dwSmoothType == SMOOTH_LEFTRIGHT)
				{
				iOtherEdgeLocation = m_rcPanel.left;
				if (pFocusPanel->IsFixed())
					{
					iOtherEdgeLocation += pFocusPanel->GetFixedDeltaLeft();
					}
				}
			else if (dwSmoothType == SMOOTH_UPDOWN)
				{
				iOtherEdgeLocation = m_rcPanel.top;
				if (pFocusPanel->IsFixed())
					{
					iOtherEdgeLocation += pFocusPanel->GetFixedDeltaTop();
					}
				}
			}

		if (dwSmoothType == SMOOTH_LEFTRIGHT)
			{
			iFocusLocation = pFocusPanel->GetPanelEdgeLocation(EDGE_LEFT);
			}
		else if (dwSmoothType == SMOOTH_UPDOWN)
			{
			iFocusLocation = pFocusPanel->GetPanelEdgeLocation(EDGE_TOP);
			}
			
		iProposedChange = (iFocusLocation - iOtherEdgeLocation);

		if (iProposedChange == 0)
			{
			continue;
			}

		 if (dwSmoothType == SMOOTH_LEFTRIGHT)
			{
			rcChangedFocus.left = iOtherEdgeLocation;
			rcChangedFocus.right = iOtherEdgeLocation + pFocusPanel->GetPanelWidth();

			iSharedRectBoundary = GetSharedLeftRightEdgeLength(&rcChangedFocus, &rcOther);
			if (iSharedRectBoundary > 0)
				{
				pFocusPanel->SetPanelEdgeLocation(EDGE_LEFT, rcChangedFocus.left);
				pFocusPanel->SetPanelEdgeLocation(EDGE_RIGHT, rcChangedFocus.right);
				}
			}
		else if (dwSmoothType == SMOOTH_UPDOWN)
			{
			rcChangedFocus.top = iOtherEdgeLocation;
			rcChangedFocus.bottom = iOtherEdgeLocation + pFocusPanel->GetPanelHeight(); 

			iSharedRectBoundary = GetSharedTopBottomEdgeLength(&rcChangedFocus, &rcOther);
			if (iSharedRectBoundary > 0)
				{
				pFocusPanel->SetPanelEdgeLocation(EDGE_BOTTOM, rcChangedFocus.bottom);
				pFocusPanel->SetPanelEdgeLocation(EDGE_TOP, rcChangedFocus.top);
				}
			}
		}
	}

CPanel *CPanel::AddInternalPanel(int iLeft, int iTop, int iRight, int iBottom, double dRigidity, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent)
	{
	CPanel *NewPanel = NULL;
	NewPanel = new CPanel(iLeft, iTop, iRight, iBottom, dRigidity);
	RECT rcNewPanel = NewPanel->GetPanelRect();

	if (IsRect1InRect2(rcNewPanel, this->GetPanelRect()))
		{
		NewPanel->SetHiddenFlag(bHidden);
		NewPanel->SetParentPanel(this);

		if (bFixedRelativeToParent)
			{
			NewPanel->SetAsFixed(iLeft - rcNewPanel.left, iTop - rcNewPanel.top);
			}
		m_aInternalPanels.Insert(NewPanel);
		UpdateNumInternalPanels();

		SmoothOutInternalPanels(bExpandInPlace, SMOOTH_LEFTRIGHT);
		SmoothOutInternalPanels(bExpandInPlace, SMOOTH_UPDOWN);

		return NewPanel;
		}
	else
		{
		delete NewPanel;
		return NULL;
		}
	}

CPanel *CPanel::AddInternalPanel(int iLeft, int iTop, int iRight, int iBottom, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent)
	{
	CPanel *NewPanel = AddInternalPanel(iLeft, iTop, iRight, iBottom, m_dRigidity, bHidden, bExpandInPlace, bFixedRelativeToParent);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanelRelativeToOrigin (int iDeltaX, int iDeltaY, int iWidth, int iHeight, double dRigidity, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent)
	{
	int iLeft = m_rcPanel.left + iDeltaX;
	int iTop = m_rcPanel.top + iDeltaY;

	CPanel *NewPanel = AddInternalPanel(iLeft, iTop, iLeft + iWidth, iTop + iHeight, dRigidity, bHidden, bExpandInPlace, bFixedRelativeToParent);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanelRelativeToOrigin (int iDeltaX, int iDeltaY, int iWidth, int iHeight, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent)
	{
	int iLeft = m_rcPanel.left + iDeltaX;
	int iTop = m_rcPanel.top + iDeltaY;

	CPanel *NewPanel = AddInternalPanel(iLeft, iTop, iLeft + iWidth, iTop + iHeight, m_dRigidity, bHidden, bExpandInPlace, bFixedRelativeToParent);

	return NewPanel;
	}

TArray <CSubSession *> CPanel::GetInternalPanelSessions (void)
	{
	TArray <CSubSession *> InternalSessions;

	for (int i = 0; i < m_aInternalPanels.GetCount(); i++)
		{
		InternalSessions.Insert(m_aInternalPanels[i]->GetAssociatedSession());
		}

	return InternalSessions;
	}

RECT CPanel::GetScaledInnerRect (double scale)
	{
	RECT rc;

	int iLeft = int(m_rcPanel.left + m_rcPanel.right*(1 - scale)*0.5);
	int iTop = int(m_rcPanel.top + m_rcPanel.bottom*(1 - scale)*0.5);

	rc.left = iLeft;
	rc.top = iTop; 
	rc.right = iLeft + int(m_rcPanel.right*scale);
	rc.bottom = iTop + int(m_rcPanel.bottom*scale);

	return rc;
	}

void CPanel::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (!IsEmpty() && !IsHidden())
		{
		m_pAssociatedSession->OnPaint(Screen, rcInvalid);
		}

	for (int i = 0; i < m_aInternalPanels.GetCount(); i++)
		{
		m_aInternalPanels[i]->OnPaint(Screen, rcInvalid);
		}
	}

TArray <CSubSession *> CPanel::ReturnSessionsContainingPoint (int x, int y)
	{
	TArray <CSubSession *> aRelevantSessions;

	if (!IsEmpty() && !IsHidden())
		{
		if (IsPointInRect(x, y, GetPanelRect()))
			{
			aRelevantSessions.Insert(m_pAssociatedSession);
			}
		}

	for (int i = 0; i < m_aInternalPanels.GetCount(); i++)
		{
		aRelevantSessions.Insert(m_aInternalPanels[i]->ReturnSessionsContainingPoint(x, y));
		}

	return aRelevantSessions;
	}

int CPanel::GetInternalPanelIndex (CPanel *pPanel)
	{
	int iDefaultIndex = -1;	//  not found in m_aInternalPanels

	for (int i = 0; i < m_aInternalPanels.GetCount(); i++)
		{
		if (m_aInternalPanels[i] == pPanel)
			{
			return i;
			}
		}

	return iDefaultIndex;
	}

void CPanel::HideInternalPanel (int iPanelIndex)
	{
	//  hide the panel first so other panels can take its space during rearrangement
	m_aInternalPanels[iPanelIndex]->SetHiddenFlag(true);

	SmoothOutInternalPanels(false, SMOOTH_LEFTRIGHT);
	SmoothOutInternalPanels(false, SMOOTH_UPDOWN);
	}

void CPanel::ShowInternalPanel (int iPanelIndex)
	{
	CPanel *FocusPanel = m_aInternalPanels[iPanelIndex];

	//  unhide the panel so that space can be made for it
	FocusPanel->SetHiddenFlag(false);

	SmoothOutInternalPanels(false, SMOOTH_LEFTRIGHT);
	SmoothOutInternalPanels(false, SMOOTH_UPDOWN);
	}

void CPanel::Hide (void)
	{
	if (m_pParentPanel != NULL)
		{
		int iRelevantPanelIndex = m_pParentPanel->GetInternalPanelIndex(this);
		m_pParentPanel->HideInternalPanel(iRelevantPanelIndex);
		}
	//  else, cannot hide since this is a "main panel" and must be shown at all times
	}                                                                   

void CPanel::Show (void)
	{
	if (m_pParentPanel != NULL)
		{
		int RelevantPanelIndex = m_pParentPanel->GetInternalPanelIndex(this);
		m_pParentPanel->ShowInternalPanel(RelevantPanelIndex);
		}
	//  else, does not need to be shown since this is a "main panel" and cannot be bHidden
	}
