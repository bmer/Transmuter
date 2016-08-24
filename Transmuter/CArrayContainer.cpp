//	CArrayContainer.cpp
//
//	Copyright (c) 2016 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"
#include "CArrayContainer.h"

CArrayContainer::CArrayContainer(CString sName, CHumanInterface &HI, int iWidth, int iHeight) : CContainer(sName, HI, iWidth, iHeight)
	{

	}

TArray <int> CArrayContainer::SortByPanelRectEdgeLocation (DWORD dwEdge)
//  SortByPanelRectEdgeLocation
// 
//  Return an array of integers whose elements correspond with indices of panels
//  in CArrayContainer's array of panels. Panel indices are sorted by edge location
//  (smallest location value first), where the specific edge is specified by dwEdge .
	{
	TArray<int> aSortedPanelIndices;

	TArray <int> aPanelIndices;
	TArray <int> aEdgeLocations;
	IPanel *pPanel;

	for (int i = 0; i < m_Panels.GetCount(); i++)
		{
		aPanelIndices.Insert(i);
		pPanel = m_Panels[i];
		if (pPanel->IsHidden())
			{
			aEdgeLocations.Insert(-1);
			}
		else
			{
			aEdgeLocations.Insert(pPanel->PanelRect.GetEdgePosition(dwEdge));
			}
		}

	aSortedPanelIndices = QuickSortIntegerArray(aEdgeLocations, aPanelIndices);

	return aSortedPanelIndices;
	}

void CArrayContainer::SmoothOut (DWORD dwSmoothType)
/*
Makes panels fit "smoothly" on the screen: that is, without any gaps or overlaps.

There are two ways to do such a smoothing: horizontally, or vertically. 

In a horizontal smoothing, all the panels will be made to line up right edge to left.

In a vertical smoothing, all the panels will be made to line up bottom edge to top.
*/
	{
	TArray <int> aSortedPanelIndices;
	int iShift;
	int iSharedRectBoundary;

	RECT rcOther;

	if (dwSmoothType == SMOOTH_HORIZONTAL)
		{
		aSortedPanelIndices = SortByPanelRectEdgeLocation(EDGE_LEFT);
		}
	else if (dwSmoothType == SMOOTH_VERTICAL)
		{
		aSortedPanelIndices = SortByPanelRectEdgeLocation(EDGE_TOP);
		}

	int iCurrentIndex;
	int iOtherIndex;

	int iFocusLocation;
	int iOtherEdgeLocation;

	int iProspectiveOtherIndex;

	IPanel *pCurrentPanel;
	IPanel *pOtherPanel;

	for (int i = 0; i < m_Panels.GetCount(); i++)
		{
		iCurrentIndex = aSortedPanelIndices[i];
		pCurrentPanel = m_Panels[iCurrentIndex];

		if (pCurrentPanel->IsHidden())
			continue;

		// we assume that there is no "other" panel to necessarily snug up to, 
		// apart from the (top or left, for a vertical or horizontal smooth respectively)
		// boundaries of the container itself
		iOtherIndex = -1;

		if (i != 0)
			{
			for (int j = 0; j < i; j++)
				{
				iProspectiveOtherIndex = aSortedPanelIndices[i - 1 - j];
				pOtherPanel = m_Panels[iProspectiveOtherIndex];

				if (!(pOtherPanel->IsHidden()))
					{
					iOtherIndex = iProspectiveOtherIndex;
					break;
					}
				}
			}

		if (iOtherIndex > -1)
			{
			pOtherPanel = m_Panels[iOtherIndex];
			rcOther = pOtherPanel->PanelRect.GetAsRect();

			if (dwSmoothType == SMOOTH_HORIZONTAL)
				{
				iOtherEdgeLocation = pOtherPanel->PanelRect.GetEdgePosition(EDGE_RIGHT);
				}
			else if (dwSmoothType == SMOOTH_VERTICAL)
				{
				iOtherEdgeLocation = pOtherPanel->PanelRect.GetEdgePosition(EDGE_BOTTOM);
				}
			}
		else
			{
			rcOther = this->PanelRect.GetAsRect();

			if (dwSmoothType == SMOOTH_HORIZONTAL)
				{
				iOtherEdgeLocation = rcOther.left;
				}
			else if (dwSmoothType == SMOOTH_VERTICAL)
				{
				iOtherEdgeLocation = rcOther.top;
				}
			}


		if (dwSmoothType == SMOOTH_HORIZONTAL)
			{
			iFocusLocation = pCurrentPanel->PanelRect.GetEdgePosition(EDGE_LEFT);
			}
		else if (dwSmoothType == SMOOTH_VERTICAL)
			{
			iFocusLocation = pCurrentPanel->PanelRect.GetEdgePosition(EDGE_TOP);
			}

		iShift = (iFocusLocation - iOtherEdgeLocation);

		if (iShift == 0)
			{
			continue;
			}

		if (dwSmoothType == SMOOTH_HORIZONTAL)
			{
			pCurrentPanel->PanelRect.ShiftOrigin(iShift, 0);

			iSharedRectBoundary = GetSharedLeftRightEdgeLength(&pCurrentPanel->PanelRect.GetAsRect(), &rcOther);

			if (!(iSharedRectBoundary > 0))
				{
				pCurrentPanel->PanelRect.ShiftOrigin(-1 * iShift, 0);
				}
			}
		else if (dwSmoothType == SMOOTH_VERTICAL)
			{
			pCurrentPanel->PanelRect.ShiftOrigin(0, iShift);

			iSharedRectBoundary = GetSharedTopBottomEdgeLength(&pCurrentPanel->PanelRect.GetAsRect(), &rcOther);
			if (!(iSharedRectBoundary > 0))
				{
				pCurrentPanel->PanelRect.ShiftOrigin(0, -1 * iShift);
				}
			}
		}
	}

bool CArrayContainer::PlacePanel (IPanel *pPanel, int iRelativeOriginX, int iRelativeOriginY)
	{
	int iOriginX = this->PanelRect.GetOriginX() + iRelativeOriginX;
	int iOriginY = this->PanelRect.GetOriginY() + iRelativeOriginY;

	pPanel->PanelRect.SetOrigin(iOriginX, iOriginY);
	m_Panels.Insert(pPanel);
	pPanel->ConfirmPlacement();

	pPanel->SetParentPanel(this);
	pPanel->SetViewOffset(this->GetViewOffsetX(), this->GetViewOffsetY());

	SmoothOut(SMOOTH_HORIZONTAL);
	SmoothOut(SMOOTH_VERTICAL);

	return true;
	}

void CArrayContainer::ChangePanelIndex (int iOldPanelIndex, int iNewPanelIndex)
	{
	if (0 <= iOldPanelIndex &&
		iOldPanelIndex < m_Panels.GetCount() &&
		0 <= iNewPanelIndex &&
		iNewPanelIndex < m_Panels.GetCount() &&
		iOldPanelIndex != iNewPanelIndex)
		{
		IPanel *pTempA = m_Panels[iOldPanelIndex];
		IPanel *pTempB = m_Panels[iNewPanelIndex];

		m_Panels[iOldPanelIndex] = pTempB;
		m_Panels[iNewPanelIndex] = pTempA;
		}
	}

void CArrayContainer::DeletePanel (int iPanelIndex)
	{
	m_Panels.Delete(iPanelIndex);
	}

TArray <IPanel *> CArrayContainer::GetPanelsContainingPoint (int x, int y)
	{
	TArray <IPanel *> aRelevantPanels;

	for (int i = 0; i < m_Panels.GetCount(); i++)
		{
		aRelevantPanels.Insert(m_Panels[i]->GetPanelsContainingPoint(x, y));
		}

	return aRelevantPanels;
	}

int CArrayContainer::GetPanelIndex (IPanel *pPanel)
	{
	for (int i = 0; i < m_Panels.GetCount(); i++)
		{
		if (m_Panels[i] == pPanel)
			{
			return i;
			}
		}

	return -1;
	}

void CArrayContainer::HidePanel (int iPanelIndex)
	{
	//  hide the panel first so other panels can take its space during rearrangement
	m_Panels[iPanelIndex]->SetHiddenFlag(true);

	SmoothOut(SMOOTH_HORIZONTAL);
	SmoothOut(SMOOTH_VERTICAL);
	}

void CArrayContainer::HideAll (void)
	{
	for (int i = 0; i < m_Panels.GetCount(); i++)
		{
		m_Panels[i]->SetHiddenFlag(true);
		}

	SmoothOut(SMOOTH_HORIZONTAL);
	SmoothOut(SMOOTH_VERTICAL);
	}


void CArrayContainer::ShowPanel (int iPanelIndex)
	{
	IPanel *FocusPanel = m_Panels[iPanelIndex];

	//  unhide the panel so that space can be made for it
	FocusPanel->SetHiddenFlag(false);

	SmoothOut(SMOOTH_HORIZONTAL);
	SmoothOut(SMOOTH_VERTICAL);
	}

void CArrayContainer::ShowAll (void)
	{
	for (int i = 0; i < m_Panels.GetCount(); i++)
		{
		m_Panels[i]->SetHiddenFlag(false);
		}

	SmoothOut(SMOOTH_HORIZONTAL);
	SmoothOut(SMOOTH_VERTICAL);
	}


