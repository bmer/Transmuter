//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"
#include "Panel.h"

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
	m_cSplitDirn('i'),
	m_fSeparatorPosFactor(0.5),
	m_iSeparatorPos(-1),
	m_iSeparatorThickness(5),
	m_pLeafPanel0(NULL),
	m_pLeafPanel1(NULL)
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

	if (m_iPanelConfigType == ORG_LIST)
		{
		for (int i = 0; i < m_aPanels.GetCount(); i++)
			{
			m_aPanels[i]->PanelRect.ShiftOrigin(iShiftX, iShiftY);
			}
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		m_pLeafPanel0->PanelRect.ShiftOrigin(iShiftX, iShiftY);
		m_pLeafPanel1->PanelRect.ShiftOrigin(iShiftX, iShiftY);
		}
	}

TArray <int> CPanelOrganizer::SortByPanelRectEdgeLocation (DWORD dwEdge)
//  SortByPanelRectEdgeLocation
// 
//  Return an array of integers whose elements correspond with indices of panels
//  in the m_aInternalPanels array. Panel indices are sorted by edge location
//  (smallest location value first), where the specific edge is specified by dwEdge .
	{
	TArray<int> aSortedPanelIndices;

	if (m_iPanelConfigType == ORG_LIST)
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

		aSortedPanelIndices = QuickSortIntegerArray(aEdgeLocations, aPanelIndices);
		}

	return aSortedPanelIndices;
	}

void CPanelOrganizer::SmoothOut (DWORD dwSmoothType)
	{
	if (m_iPanelConfigType == ORG_LIST && dwSmoothType != SMOOTH_NONE)
		{
		SmoothOutList(dwSmoothType);
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		SmoothOutTree();
		}
	}

void CPanelOrganizer::SmoothOutList (DWORD dwSmoothType)
	{
	if (m_iPanelConfigType == ORG_LIST)
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

		int iCurrentIndex;
		int iOtherIndex;

		int iFocusLocation;
		int iOtherEdgeLocation;

		int iProspectiveOtherIndex;

		IPanel *pCurrentPanel;
		IPanel *pOtherPanel;

		for (int i = 0; i < m_aPanels.GetCount(); i++)
			{
			iCurrentIndex = aSortedPanelIndices[i];
			pCurrentPanel = m_aPanels[iCurrentIndex];

			if (pCurrentPanel->IsHidden())
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
				iFocusLocation = pCurrentPanel->PanelRect.GetEdgePosition(EDGE_LEFT);
				}
			else if (dwSmoothType == SMOOTH_UPDOWN)
				{
				iFocusLocation = pCurrentPanel->PanelRect.GetEdgePosition(EDGE_TOP);
				}

			iShift = (iFocusLocation - iOtherEdgeLocation);

			if (iShift == 0)
				{
				continue;
				}

			if (dwSmoothType == SMOOTH_LEFTRIGHT)
				{
				pCurrentPanel->PanelRect.ShiftOrigin(iShift, 0);

				iSharedRectBoundary = GetSharedLeftRightEdgeLength(&pCurrentPanel->PanelRect.GetAsRect(), &rcOther);

				if (!(iSharedRectBoundary > 0))
					{
					pCurrentPanel->PanelRect.ShiftOrigin(-1 * iShift, 0);
					}
				}
			else if (dwSmoothType == SMOOTH_UPDOWN)
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
	}

void CPanelOrganizer::SmoothOutTree (void)
	{
	if (m_pLeafPanel0 == NULL || m_pLeafPanel0->IsHidden())
		{
		if (m_pLeafPanel1 == NULL || m_pLeafPanel1->IsHidden())
			{
			return;
			}
		else
			{
			m_pLeafPanel1->PanelRect.SetOrigin(m_ParentPanel.PanelRect.GetOriginX(), m_ParentPanel.PanelRect.GetOriginY());
			m_pLeafPanel1->PanelRect.SetHeight(m_ParentPanel.PanelRect.GetHeight());
			m_pLeafPanel1->PanelRect.SetWidth(m_ParentPanel.PanelRect.GetWidth());
			m_iSeparatorPos = -1;
			}
		}
	else
		{
		if (m_pLeafPanel1 == NULL || m_pLeafPanel1->IsHidden())
			{
			m_pLeafPanel0->PanelRect.SetOrigin(m_ParentPanel.PanelRect.GetOriginX(), m_ParentPanel.PanelRect.GetOriginY());
			m_pLeafPanel0->PanelRect.SetHeight(m_ParentPanel.PanelRect.GetHeight());
			m_pLeafPanel0->PanelRect.SetWidth(m_ParentPanel.PanelRect.GetWidth());
			m_iSeparatorPos = -1;
			}
		else
			{
			if (m_cSplitDirn == 'h')
				{
				m_iSeparatorPos = int(m_fSeparatorPosFactor*m_ParentPanel.PanelRect.GetWidth());

				m_pLeafPanel0->PanelRect.SetOrigin(m_ParentPanel.PanelRect.GetOriginX(), m_ParentPanel.PanelRect.GetOriginY());
				m_pLeafPanel0->PanelRect.SetWidth(m_ParentPanel.PanelRect.GetWidth() - m_iSeparatorPos);
				m_pLeafPanel0->PanelRect.SetHeight(m_ParentPanel.PanelRect.GetHeight());

				m_pLeafPanel1->PanelRect.SetOrigin(m_iSeparatorPos + m_iSeparatorThickness, m_ParentPanel.PanelRect.GetOriginY());
				m_pLeafPanel1->PanelRect.SetWidth(m_ParentPanel.PanelRect.GetWidth() - m_pLeafPanel0->PanelRect.GetWidth() - m_iSeparatorThickness);
				m_pLeafPanel1->PanelRect.SetHeight(m_ParentPanel.PanelRect.GetHeight());
				}
			else if (m_cSplitDirn == 'v')
				{
				m_iSeparatorPos = int(m_fSeparatorPosFactor*m_ParentPanel.PanelRect.GetHeight());

				m_pLeafPanel0->PanelRect.SetOrigin(m_ParentPanel.PanelRect.GetOriginX(), m_ParentPanel.PanelRect.GetOriginY());
				m_pLeafPanel0->PanelRect.SetWidth(m_ParentPanel.PanelRect.GetWidth());
				m_pLeafPanel0->PanelRect.SetHeight(m_ParentPanel.PanelRect.GetHeight() - m_iSeparatorPos);

				m_pLeafPanel1->PanelRect.SetOrigin(m_ParentPanel.PanelRect.GetOriginX(), m_iSeparatorPos + m_iSeparatorThickness);
				m_pLeafPanel1->PanelRect.SetWidth(m_ParentPanel.PanelRect.GetWidth());
				m_pLeafPanel1->PanelRect.SetHeight(m_ParentPanel.PanelRect.GetHeight() - m_pLeafPanel0->PanelRect.GetHeight() - m_iSeparatorThickness);
				}
			}
		}
	}

bool CPanelOrganizer::PlacePanel (IPanel *pPanel, int iRelativeOriginX, int iRelativeOriginY)
	{
	if (m_iPanelConfigType == ORG_TREE)
		{
		return false;
		}

	m_iPanelConfigType = ORG_LIST;

	int iOriginX = m_ParentPanel.PanelRect.GetOriginX() + iRelativeOriginX;
	int iOriginY = m_ParentPanel.PanelRect.GetOriginY() + iRelativeOriginY;

	pPanel->PanelRect.SetOrigin(iOriginX, iOriginY);
	m_aPanels.Insert(pPanel);
	pPanel->ConfirmPlacement();

	pPanel->SetParentPanel(&m_ParentPanel);
	pPanel->SetViewOffset(m_ParentPanel.GetViewOffsetX(), m_ParentPanel.GetViewOffsetY());

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);

	return true;
	}

bool CPanelOrganizer::PlacePanel (IPanel *pPanel, char cSplitDirn, int iInsertIndex)
	{
	if (m_iPanelConfigType != -1)
		{
		if (m_iPanelConfigType == ORG_LIST || cSplitDirn != m_cSplitDirn)
			{
			return false;
			}
		}
	else
		{
		m_iPanelConfigType = ORG_TREE;
		}

	if (cSplitDirn != 'h' || cSplitDirn != 'c')
		{
		return false;
		}
							 
	if (m_pLeafPanel0 != NULL && m_pLeafPanel1 != NULL)
		{
		return false;
		}

	if (iInsertIndex == 0)
		{
		if (m_pLeafPanel0 != NULL)
			{
			m_pLeafPanel1 = m_pLeafPanel0;
			}
		m_pLeafPanel0 = pPanel;
		}
	else
		{
		if (m_pLeafPanel1 != NULL)
			{
			m_pLeafPanel0 = m_pLeafPanel1;
			}
		m_pLeafPanel1 = pPanel;
		}

	SmoothOutTree();

	pPanel->ConfirmPlacement();

	return true;
	}

void CPanelOrganizer::ChangePanelIndex(int iOldPanelIndex, int iNewPanelIndex)
	{
	if (m_iPanelConfigType == ORG_LIST)
		{
		if (0 <= iOldPanelIndex &&
			iOldPanelIndex < m_aPanels.GetCount() &&
			0 <= iNewPanelIndex && 
			iNewPanelIndex < m_aPanels.GetCount() && 
			iOldPanelIndex != iNewPanelIndex)
			{
			IPanel *pTempA = m_aPanels[iOldPanelIndex];
			IPanel *pTempB = m_aPanels[iNewPanelIndex];

			m_aPanels[iOldPanelIndex] = pTempB;
			m_aPanels[iNewPanelIndex] = pTempA;
			}
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		if ((iOldPanelIndex == 0 || iOldPanelIndex == 1) &&
			(iNewPanelIndex == 0 || iNewPanelIndex == 1) &&
			iOldPanelIndex != iNewPanelIndex)
			{
			IPanel *pTemp = m_pLeafPanel0;

			m_pLeafPanel0 = m_pLeafPanel1;
			m_pLeafPanel1 = pTemp;
			}
		}
	}

void CPanelOrganizer::DeletePanel (int iPanelIndex)
	{
	if (m_iPanelConfigType == ORG_LIST)
		{
		m_aPanels.Delete(iPanelIndex);
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		if (iPanelIndex == 0)
			{
			if (m_pLeafPanel0 != NULL)
				{
				delete m_pLeafPanel0;
				m_pLeafPanel0 = NULL;
				}
			}
		else
			{
			if (m_pLeafPanel1 != NULL)
				{
				delete m_pLeafPanel1;
				m_pLeafPanel1 = NULL;
				}
			}
		}
	}

TArray <IPanel *> CPanelOrganizer::GetPanelsContainingPoint (int x, int y)
	{
	TArray <IPanel *> aRelevantPanels;

	if (m_iPanelConfigType == ORG_LIST)
		{
		for (int i = 0; i < m_aPanels.GetCount(); i++)
			{
			aRelevantPanels.Insert(m_aPanels[i]->GetPanelsContainingPoint(x, y));
			}
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		if (m_pLeafPanel0 != NULL)
			{
			aRelevantPanels.Insert(m_pLeafPanel0->GetPanelsContainingPoint(x, y));
			}
		if (m_pLeafPanel1 != NULL)
			{
			aRelevantPanels.Insert(m_pLeafPanel1->GetPanelsContainingPoint(x, y));
			}
		}
	

	return aRelevantPanels;
	}

int CPanelOrganizer::GetPanelIndex (IPanel *pPanel)
	{
	if (m_iPanelConfigType == ORG_LIST)
		{
		for (int i = 0; i < m_aPanels.GetCount(); i++)
			{
			if (m_aPanels[i] == pPanel)
				{
				return i;
				}
			}
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		if (m_pLeafPanel0 != NULL)
			{
			if (m_pLeafPanel0 == pPanel)
				{
				return 0;
				}
			}
		if (m_pLeafPanel1 != NULL)
			{
			if (m_pLeafPanel1 == pPanel)
				{
				return 1;
				}
			}
		}

	return -1;
	}

void CPanelOrganizer::HidePanel (IPanel *Panel)
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
	}

void CPanelOrganizer::HidePanel (int iPanelIndex)
	{
	if (m_iPanelConfigType == ORG_LIST)
		{
		//  hide the panel first so other panels can take its space during rearrangement
		m_aPanels[iPanelIndex]->SetHiddenFlag(true);

		SmoothOut(SMOOTH_LEFTRIGHT);
		SmoothOut(SMOOTH_UPDOWN);
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		if (iPanelIndex == 0)
			{
			if (m_pLeafPanel0 != NULL)
				{
				m_pLeafPanel0->SetHiddenFlag(true);
				SmoothOutTree();
				}
			}
		else if (iPanelIndex == 1)
			{
			if (m_pLeafPanel1 != NULL)
				{
				m_pLeafPanel1->SetHiddenFlag(true);
				SmoothOutTree();
				}
			}
		}
	}

void CPanelOrganizer::HideAll (void)
	{
	if (m_iPanelConfigType == ORG_LIST)
		{
		for (int i = 0; i < m_aPanels.GetCount(); i++)
			{
			m_aPanels[i]->SetHiddenFlag(true);
			}

		SmoothOut(SMOOTH_LEFTRIGHT);
		SmoothOut(SMOOTH_UPDOWN);
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		if (m_pLeafPanel0 != NULL)
			{
			m_pLeafPanel0->SetHiddenFlag(true);
			}
		if (m_pLeafPanel1 != NULL)
			{
			m_pLeafPanel1->SetHiddenFlag(true);
			}
		SmoothOutTree();
		}
	}

void CPanelOrganizer::ShowPanel (IPanel *Panel)
	{
	int iPanelIndex = GetPanelIndex(Panel);

	if (iPanelIndex == -1)
		{
		return;
		}
	else
		{
		ShowPanel(iPanelIndex);
		}
	}

void CPanelOrganizer::ShowPanel (int iPanelIndex)
	{
	if (m_iPanelConfigType == ORG_LIST)
		{
		IPanel *FocusPanel = m_aPanels[iPanelIndex];

		//  unhide the panel so that space can be made for it
		FocusPanel->SetHiddenFlag(false);

		SmoothOut(SMOOTH_LEFTRIGHT);
		SmoothOut(SMOOTH_UPDOWN);
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		if (iPanelIndex == 0)
			{
			m_pLeafPanel0->SetHiddenFlag(false);
			}
		else if (iPanelIndex == 1)
			{
			m_pLeafPanel1->SetHiddenFlag(false);
			}

		SmoothOutTree();
		}
	}

void CPanelOrganizer::ShowAll (void)
	{
	if (m_iPanelConfigType == ORG_LIST)
		{
		for (int i = 0; i < m_aPanels.GetCount(); i++)
			{
			m_aPanels[i]->SetHiddenFlag(false);
			}

		SmoothOut(SMOOTH_LEFTRIGHT);
		SmoothOut(SMOOTH_UPDOWN);
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		if (m_pLeafPanel0 != NULL)
			{
			m_pLeafPanel0->SetHiddenFlag(false);
			}
		if (m_pLeafPanel1 != NULL)
			{
			m_pLeafPanel1->SetHiddenFlag(false);
			}

		SmoothOutTree();
		}
	}

void CPanelOrganizer::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (m_iPanelConfigType == ORG_LIST)
		{
		for (int i = 0; i < m_aPanels.GetCount(); i++)
			{
			m_aPanels[i]->OnPaint(Screen, rcInvalid);
			}
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		if (m_pLeafPanel0 != NULL)
			{
			m_pLeafPanel0->OnPaint(Screen, rcInvalid);
			}
		if (m_pLeafPanel1 != NULL)
			{
			m_pLeafPanel1->OnPaint(Screen, rcInvalid);
			}
		}
	}

void CPanelOrganizer::Invalidate(void)
	{
	if (m_iPanelConfigType == ORG_LIST)
		{
		for (int i = 0; i < m_aPanels.GetCount(); i++)
			{
			m_aPanels[i]->Invalidate();
			}
		}
	else if (m_iPanelConfigType == ORG_TREE)
		{
		if (m_pLeafPanel0 != NULL)
			{
			m_pLeafPanel0->Invalidate();
			}
		if (m_pLeafPanel1 != NULL)
			{
			m_pLeafPanel1->Invalidate();
			}
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
	PanelRect(*this, iWidth, iHeight),
	PanelOrganizer(*this)
	{
	}

IPanel::IPanel (CString sName, CHumanInterface &HI, int iOriginX, int iOriginY, int iWidth, int iHeight) : IHISession(HI),
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
PanelRect(*this, iOriginX, iOriginY, iWidth, iHeight),
PanelOrganizer(*this)
	{
	ConfirmPlacement();
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
	if (IsPlaced())
		{
#if DEBUG
		bool bFocusStatus = GetFocusStatus();
#endif
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

		OnPanelPaint(Screen, rcInvalid);

		PanelOrganizer.OnPaint(Screen, rcInvalid);
		}
	}

CString IPanel::CreateSubPanelName(CString sSubPanelNamePart)
	{
	CString sSubPanelName = strCat(m_sName, CONSTLIT("."));
	sSubPanelName = strCat(sSubPanelName, sSubPanelNamePart);
	
	return sSubPanelName;
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
