//	CMainSession.cpp
//
//	CMainSession class
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

CPanelRect::CPanelRect (CPanel &AssociatedPanel, int iOriginX, int iOriginY, int iWidth, int iHeight) :
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

IInternalPanelling::IInternalPanelling(CPanel &ParentPanel) :
	m_ParentPanel(ParentPanel)
	{
	}

IInternalPanelling::~IInternalPanelling()
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		delete m_aPanels[i];
		}
	}

void IInternalPanelling::ShiftAllOrigins (int iShiftX, int iShiftY)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->PanelRect.ShiftOrigin(iShiftX, iShiftY);
		}
	}

// ===========================================================================

CPanelArray::CPanelArray (CPanel &ParentPanel) : IInternalPanelling(ParentPanel)
	{
	}

TArray <int> CPanelArray::SortByPanelRectEdgeLocation (DWORD dwEdge)
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

void CPanelArray::SmoothOut (DWORD dwSmoothType)
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

CPanel *CPanelArray::AddPanel (int iRelativeOriginX, int iRelativeOriginY, int iWidth, int iHeight, bool bHidden, CString sPanelConfiguration)
	{
	int iOriginX = m_ParentPanel.PanelRect.GetOriginX() + iRelativeOriginX;
	int iOriginY = m_ParentPanel.PanelRect.GetOriginY() + iRelativeOriginY;

	CPanel *NewPanel = new CPanel(iOriginX, iOriginY, iWidth, iHeight, sPanelConfiguration);

	NewPanel->SetHiddenFlag(bHidden);
	NewPanel->SetParentPanel(&m_ParentPanel);
	NewPanel->SetViewOffset(m_ParentPanel.GetViewOffsetX(), m_ParentPanel.GetViewOffsetY());

	m_aPanels.Insert(NewPanel);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);

	return NewPanel;
	}

void CPanelArray::DeletePanel (int iPanelIndex)
	{
	m_aPanels.Delete(iPanelIndex);
	}

TArray <IPanelContent *> CPanelArray::GetPanelContents (void)
	{
	TArray <IPanelContent *> InternalContents;

	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		InternalContents.Insert(m_aPanels[i]->GetAssociatedContent());
		}

	return InternalContents;
	}


TArray <IPanelContent *> CPanelArray::GetPanelContentsContainingPoint (int x, int y)
	{
	TArray <IPanelContent *> aRelevantContents;

	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		aRelevantContents.Insert(m_aPanels[i]->GetPanelContentsContainingPoint(x, y));
		}

	return aRelevantContents;
	}

int CPanelArray::GetPanelIndex (CPanel *pPanel)
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

void CPanelArray::HidePanel (CPanel *Panel)
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

void CPanelArray::HidePanel (int iPanelIndex)
	{
	//  hide the panel first so other panels can take its space during rearrangement
	m_aPanels[iPanelIndex]->SetHiddenFlag(true);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelArray::HideAll (void)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->SetHiddenFlag(true);
		}

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelArray::ShowPanel (CPanel *Panel)
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

void CPanelArray::ShowPanel (int iPanelIndex)
	{
	CPanel *FocusPanel = m_aPanels[iPanelIndex];

	//  unhide the panel so that space can be made for it
	FocusPanel->SetHiddenFlag(false);

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelArray::ShowAll (void)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->SetHiddenFlag(false);
		}

	SmoothOut(SMOOTH_LEFTRIGHT);
	SmoothOut(SMOOTH_UPDOWN);
	}

void CPanelArray::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		m_aPanels[i]->OnPaint(Screen, rcInvalid);
		}
	}

// ============================================================================

CPanelTree::CPanelTree(CPanel &ParentPanel) : IInternalPanelling(ParentPanel)
	{
	}

CPanelTree::~CPanelTree(void)
	{
	}

TArray <IPanelContent *> CPanelTree::GetPanelContents (void)
	{
	TArray <IPanelContent *> InternalContents;

	for (int i = 0; i < m_aLeafPanels.GetCount(); i++)
		{
		InternalContents.Insert(m_aLeafPanels[i]->GetAssociatedContent());
		}

	return InternalContents;
	}


TArray <IPanelContent *> CPanelTree::GetPanelContentsContainingPoint (int x, int y)
	{
	TArray <IPanelContent *> aRelevantContents;

	for (int i = 0; i < m_aLeafPanels.GetCount(); i++)
		{
		aRelevantContents.Insert(m_aLeafPanels[i]->GetPanelContentsContainingPoint(x, y));
		}

	return aRelevantContents;
	}

int CPanelTree::GetPanelIndex (CPanel *pPanel)
	{
	int iDefaultIndex = -1;	//  not found in m_aInternalPanels

	for (int i = 0; i < m_aLeafPanels.GetCount(); i++)
		{
		if (m_aLeafPanels[i] == pPanel)
			{
			return i;
			}
		}

	return iDefaultIndex;
	}

TArray <CPanel *> CPanelTree::Split (CString sSplitType, int iSeparatorPos)
	{
	if (sSplitType == CONSTLIT("vertical"))
		{
		int iMinX = m_ParentPanel.PanelRect.GetOriginX;
		int iMaxX = iMinX + m_ParentPanel.PanelRect.GetWidth();
		
		if (iMinX <= iSeparatorPos && iMaxX >= iSeparatorPos)
			{

			}
		}
	}

// ============================================================================

CPanel::CPanel (void) :
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedContent(NULL),
	m_bHidden(false),
	PanelRect(*this),
	InternalPanels(*this),
	m_iViewOffsetX(0),
	m_iViewOffsetY(0),
	m_sPanelConfiguration(CONSTLIT("array"));
	{
	}

CPanel::CPanel (int iOriginX, int iOriginY, int iWidth, int iHeight, CString sPanelConfiguration) :
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedContent(NULL),
	m_bHidden(false),
	PanelRect(*this, iOriginX, iOriginY, iWidth, iHeight),
	InternalPanels(*this),
	m_iViewOffsetX(0),
	m_iViewOffsetY(0),
	m_sPanelConfiguration(sPanelConfiguration)
	{
	}

CPanel::CPanel (IPanelContent *pAssociatedContent, int iOriginX, int iOriginY, int iWidth, int iHeight, CString sPanelConfiguration) :
	m_pParentPanel(NULL),
	m_bErrorOccurred(false),
	m_sErrorString(CONSTLIT("")),
	m_pAssociatedContent(pAssociatedContent),
	m_bHidden(false),
	PanelRect(*this, iOriginX, iOriginY, iWidth, iHeight),
	InternalPanels(*this),
	m_iViewOffsetX(0),
	m_iViewOffsetY(0),
	m_sPanelConfiguration(sPanelConfiguration)
	{
	}

CPanel::~CPanel ()
	{
	}

void CPanel::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (!IsEmpty() && !IsHidden())
		{
		m_pAssociatedContent->OnPaint(Screen, rcInvalid);
		}

	InternalPanels.PaintAllContent(Screen, rcInvalid);
	}

TArray <IPanelContent *> CPanel::GetPanelContentsContainingPoint (int x, int y)
	{
	TArray <IPanelContent *> aRelevantContents;

	bool bIsEmpty = this->IsEmpty();
	bool bIsHidden = this->IsHidden();

	if (!this->IsEmpty() && !this->IsHidden())
		{
		if (IsPointInRect(x, y, PanelRect.GetAsRect()))
			{
			aRelevantContents.Insert(m_pAssociatedContent);
			}
		}

	aRelevantContents.Insert(InternalPanels.GetPanelContentsContainingPoint(x, y));

	return aRelevantContents;
	}

IPanelContent * CPanel::GetContentContainingPoint(int x, int y)
	{
	if (!IsPointInRect(x, y, PanelRect.GetAsRect()))
		{
		return NULL;
		}
	
	IPanelContent *pRelevantContent;
	for (int i = 0; i < InternalPanels.GetCount(); i++)
		{
		pRelevantContent = InternalPanels.GetPanel(i)->GetContentContainingPoint(x, y);

		if (pRelevantContent != NULL)
			{
			return pRelevantContent;
			}
		}

	return m_pAssociatedContent;
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
		m_pAssociatedContent->HIInvalidate();
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
