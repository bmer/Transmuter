//	CSTransmuter.cpp
//
//	CSTransmuter class
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
	m_iNumInternalPanels(0),
	m_iViewSpaceTopOffset(0),
	m_iViewSpaceLeftOffset(0)
	{
	m_rcScreenSpace.bottom = -1;
	m_rcScreenSpace.left = -1;
	m_rcScreenSpace.right = -1;
	m_rcScreenSpace.top = -1;

	InitTrueSpace();
	}

CPanel::CPanel (RECT rcPanelRect) : m_rcScreenSpace(rcPanelRect),
	m_dRigidity(0),
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	m_iNumInternalPanels(0),
	m_iViewSpaceTopOffset(0),
	m_iViewSpaceLeftOffset(0)
	{
	InitTrueSpace();
	}

CPanel::CPanel (RECT rcScreenSpace, double dRigidity) : m_rcScreenSpace(rcScreenSpace),
	m_dRigidity(dRigidity),
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	m_iNumInternalPanels(0),
	m_iViewSpaceTopOffset(0),
	m_iViewSpaceLeftOffset(0)
	{
	InitTrueSpace();
	}

CPanel::CPanel (int iLeft, int iTop, int iRight, int iBottom) :
	m_dRigidity(0),
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedSession(NULL),
	m_bFocus(0),
	m_bHidden(false),
	m_iNumInternalPanels(0),
	m_iViewSpaceTopOffset(0),
	m_iViewSpaceLeftOffset(0)
	{
	m_rcScreenSpace.bottom = iBottom;
	m_rcScreenSpace.left = iLeft;
	m_rcScreenSpace.right = iRight;
	m_rcScreenSpace.top = iTop;

	InitTrueSpace();
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
	m_rcScreenSpace.bottom = iBottom;
	m_rcScreenSpace.left = iLeft;
	m_rcScreenSpace.right = iRight;
	m_rcScreenSpace.top = iTop;

	InitTrueSpace();
	}

CPanel::~CPanel ()
	{
	for (int i = 0; i < m_iNumInternalPanels; i++)
		{
		delete &m_aInternalPanels[i];
		}
	}

void CPanel::InitTrueSpace (void)
	{
	m_rcTrueSpace.bottom = m_rcScreenSpace.bottom;
	m_rcTrueSpace.left = m_rcScreenSpace.left;
	m_rcTrueSpace.right = m_rcScreenSpace.right;
	m_rcTrueSpace.top = m_rcScreenSpace.top;
	}

void CPanel::ShiftScreenSpaceLocation (DWORD dwEdge, int iShift)
	{
	int iLocation; 

	if (dwEdge == EDGE_BOTTOM)
		{
		iLocation = iShift + m_rcScreenSpace.bottom;
		m_rcScreenSpace.bottom = iLocation;
		InvalidatePanel();
		}
	else if (dwEdge == EDGE_LEFT)
		{
		iLocation = iShift + m_rcScreenSpace.left;
		m_rcScreenSpace.left = iLocation;
		InvalidatePanel();
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		iLocation = iShift + m_rcScreenSpace.right;
		m_rcScreenSpace.right = iLocation;
		InvalidatePanel();
		}
	else if (dwEdge == EDGE_TOP)
		{
		iLocation = iShift + m_rcScreenSpace.top;
		m_rcScreenSpace.top = iLocation;
		InvalidatePanel();
		}

	for (int i = 0; i < m_aInternalPanels.GetCount(); i++)
		{
		m_aInternalPanels[i]->ShiftScreenSpaceLocation (dwEdge, iShift);
		}
	}

void CPanel::SetScreenSpaceEdgeLocation (DWORD dwEdge, int iLocation)
//  SetScreenSpaceEdgeLocation
// 
//  Set location of panel edge.
	{
	int iShift;

	if (dwEdge == EDGE_BOTTOM)
		{
		iShift = iLocation - m_rcScreenSpace.bottom;
		m_rcScreenSpace.bottom = iLocation;
		InvalidatePanel();
		}
	else if (dwEdge == EDGE_LEFT)
		{
		iShift = iLocation - m_rcScreenSpace.left;
		m_rcScreenSpace.left = iLocation;
		InvalidatePanel();
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		iShift = iLocation - m_rcScreenSpace.right;
		m_rcScreenSpace.right = iLocation;
		InvalidatePanel();
		}
	else if (dwEdge == EDGE_TOP)
		{
		iShift = iLocation - m_rcScreenSpace.top;
		m_rcScreenSpace.top = iLocation;
		InvalidatePanel();
		}

	for (int i = 0; i < m_aInternalPanels.GetCount(); i++)
		{
		m_aInternalPanels[i]->ShiftScreenSpaceLocation (dwEdge, iShift);
		}
	}

int CPanel::GetScreenSpaceEdgeLocation (DWORD dwEdge)
	{
	if (dwEdge == EDGE_BOTTOM)
		{
		return m_rcScreenSpace.bottom;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		return m_rcScreenSpace.left;
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		return m_rcScreenSpace.right;
		}
	else if (dwEdge == EDGE_TOP)
		{
		return m_rcScreenSpace.top;
		}
	else
		{
		//  error
		return -1;
		}
	}

TArray <int> CPanel::SortInternalPanelsByScreenSapceEdgeLocation (DWORD dwEdge)
//  SortInternalPanelsByScreenSapceEdgeLocation
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
			aEdgeLocations.Insert(pInternalPanel->GetScreenSpaceEdgeLocation(dwEdge));
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
		aSortedPanelIndices = SortInternalPanelsByScreenSapceEdgeLocation(EDGE_LEFT);
		}
	else if (dwSmoothType == SMOOTH_UPDOWN)
		{
		aSortedPanelIndices = SortInternalPanelsByScreenSapceEdgeLocation(EDGE_TOP);
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
				iOtherEdgeLocation = pOtherPanel->GetScreenSpaceEdgeLocation(EDGE_RIGHT);
				}
			else if (dwSmoothType == SMOOTH_UPDOWN)
				{
				iOtherEdgeLocation = pOtherPanel->GetScreenSpaceEdgeLocation(EDGE_BOTTOM);
				}
			}
		else
			{
			rcOther = this->GetPanelRect();
			if (dwSmoothType == SMOOTH_LEFTRIGHT)
				{
				iOtherEdgeLocation = m_rcScreenSpace.left;
				if (pFocusPanel->IsFixed())
					{
					iOtherEdgeLocation += pFocusPanel->GetFixedDeltaLeft();
					}
				}
			else if (dwSmoothType == SMOOTH_UPDOWN)
				{
				iOtherEdgeLocation = m_rcScreenSpace.top;
				if (pFocusPanel->IsFixed())
					{
					iOtherEdgeLocation += pFocusPanel->GetFixedDeltaTop();
					}
				}
			}

		if (dwSmoothType == SMOOTH_LEFTRIGHT)
			{
			iFocusLocation = pFocusPanel->GetScreenSpaceEdgeLocation(EDGE_LEFT);
			}
		else if (dwSmoothType == SMOOTH_UPDOWN)
			{
			iFocusLocation = pFocusPanel->GetScreenSpaceEdgeLocation(EDGE_TOP);
			}
			
		iProposedChange = (iFocusLocation - iOtherEdgeLocation);

		if (iProposedChange == 0)
			{
			continue;
			}

		 if (dwSmoothType == SMOOTH_LEFTRIGHT)
			{
			rcChangedFocus.left = iOtherEdgeLocation;
			rcChangedFocus.right = iOtherEdgeLocation + pFocusPanel->GetScreenSpaceWidth();

			iSharedRectBoundary = GetSharedLeftRightEdgeLength(&rcChangedFocus, &rcOther);
			if (iSharedRectBoundary > 0)
				{
				pFocusPanel->SetScreenSpaceEdgeLocation(EDGE_LEFT, rcChangedFocus.left);
				pFocusPanel->SetScreenSpaceEdgeLocation(EDGE_RIGHT, rcChangedFocus.right);
				}
			}
		else if (dwSmoothType == SMOOTH_UPDOWN)
			{
			rcChangedFocus.top = iOtherEdgeLocation;
			rcChangedFocus.bottom = iOtherEdgeLocation + pFocusPanel->GetScreenSpaceHeight(); 

			iSharedRectBoundary = GetSharedTopBottomEdgeLength(&rcChangedFocus, &rcOther);
			if (iSharedRectBoundary > 0)
				{
				pFocusPanel->SetScreenSpaceEdgeLocation(EDGE_BOTTOM, rcChangedFocus.bottom);
				pFocusPanel->SetScreenSpaceEdgeLocation(EDGE_TOP, rcChangedFocus.top);
				}
			}
		}
	}

CPanel *CPanel::AddInternalPanel(int iLeft, int iTop, int iRight, int iBottom, double dRigidity, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent)
	{
	CPanel *NewPanel = NULL;
	NewPanel = new CPanel(iLeft, iTop, iRight, iBottom, dRigidity);
	RECT rcNewPanel = NewPanel->GetPanelRect();

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

	TArray <int> aPanelIndices = SortInternalPanelsByScreenSapceEdgeLocation(EDGE_BOTTOM);
	int iNumPanels = aPanelIndices.GetCount();
	int iLastPanelIndex = aPanelIndices[iNumPanels-1];
	int iLastPanelBottom = m_aInternalPanels[iLastPanelIndex]->GetScreenSpaceEdgeLocation(EDGE_BOTTOM);
	
	this->SetTrueSpaceEdgeLocation(EDGE_BOTTOM, iLastPanelBottom);

	TArray <int> aPanelIndices = SortInternalPanelsByScreenSapceEdgeLocation(EDGE_RIGHT);
	int iNumPanels = aPanelIndices.GetCount();
	int iLastPanelIndex = aPanelIndices[iNumPanels-1];
	int iLastPanelRight = m_aInternalPanels[iLastPanelIndex]->GetScreenSpaceEdgeLocation(EDGE_RIGHT);
	
	this->SetTrueSpaceEdgeLocation(EDGE_RIGHT, iLastPanelBottom);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanel(int iLeft, int iTop, int iRight, int iBottom, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent)
	{
	CPanel *NewPanel = AddInternalPanel(iLeft, iTop, iRight, iBottom, m_dRigidity, bHidden, bExpandInPlace, bFixedRelativeToParent);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanelRelativeToOrigin (int iDeltaX, int iDeltaY, int iWidth, int iHeight, double dRigidity, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent)
	{
	int iLeft = m_rcScreenSpace.left + iDeltaX;
	int iTop = m_rcScreenSpace.top + iDeltaY;

	CPanel *NewPanel = AddInternalPanel(iLeft, iTop, iLeft + iWidth, iTop + iHeight, dRigidity, bHidden, bExpandInPlace, bFixedRelativeToParent);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanelRelativeToOrigin (int iDeltaX, int iDeltaY, int iWidth, int iHeight, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent)
	{
	int iLeft = m_rcScreenSpace.left + iDeltaX;
	int iTop = m_rcScreenSpace.top + iDeltaY;

	CPanel *NewPanel = AddInternalPanel(iLeft, iTop, iLeft + iWidth, iTop + iHeight, m_dRigidity, bHidden, bExpandInPlace, bFixedRelativeToParent);

	return NewPanel;
	}

TArray <CSChild *> CPanel::GetInternalPanelSessions (void)
	{
	TArray <CSChild *> InternalSessions;

	for (int i = 0; i < m_aInternalPanels.GetCount(); i++)
		{
		InternalSessions.Insert(m_aInternalPanels[i]->GetAssociatedSession());
		}

	return InternalSessions;
	}

RECT CPanel::GetScaledInnerRect (double scale)
	{
	RECT rc;

	int iLeft = int(m_rcScreenSpace.left + m_rcScreenSpace.right*(1 - scale)*0.5);
	int iTop = int(m_rcScreenSpace.top + m_rcScreenSpace.bottom*(1 - scale)*0.5);

	rc.left = iLeft;
	rc.top = iTop; 
	rc.right = iLeft + int(m_rcScreenSpace.right*scale);
	rc.bottom = iTop + int(m_rcScreenSpace.bottom*scale);

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

TArray <CSChild *> CPanel::ReturnSessionsContainingPoint (int x, int y)
	{
	TArray <CSChild *> aRelevantSessions;

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
