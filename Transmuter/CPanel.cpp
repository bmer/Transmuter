//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

double CalculateRelativeRigidity(double rigidity1, double rigidity2)
	{
	if ((rigidity1 == rigidity2) || (rigidity1 == 0 && rigidity2 == 0))
		{
		return 0.5;
		}
	else
		{
		return rigidity1/(rigidity1 + rigidity2);
		}
	}

CPanel::CPanel(void) : m_xO(-1),
	m_yO(-1),
	m_Width(-1),
	m_Height(-1),
	m_Rigidity(0),
	m_ParentPanel(NULL),
	m_ErrorOccurred(FALSE),
	m_ErrorString(CONSTLIT("")),
	m_AssociatedSession(NULL),
	m_Focus(0),
	m_Hidden(false),
	m_NumInternalPanels(0)
	{
	}

CPanel::CPanel(int xO, int yO, int SpaceWidth, int SpaceHeight) : m_xO(xO),
	m_yO(yO),
	m_Width(SpaceWidth),
	m_Height(SpaceHeight),
	m_Rigidity(0),
	m_ParentPanel(NULL),
	m_ErrorOccurred(FALSE),
	m_ErrorString(CONSTLIT("")),
	m_AssociatedSession(NULL),
	m_Focus(0),
	m_Hidden(false),
	m_NumInternalPanels(0)
	{
	}

CPanel::CPanel(int xO, int yO, int SpaceWidth, int SpaceHeight, double Rigidity) : m_xO(xO),
	m_yO(yO),
	m_Width(SpaceWidth),
	m_Height(SpaceHeight),
	m_Rigidity(Rigidity),
	m_ParentPanel(NULL),
	m_ErrorOccurred(FALSE),
	m_ErrorString(CONSTLIT("")),
	m_AssociatedSession(NULL),
	m_Focus(0),
	m_Hidden(false),
	m_NumInternalPanels(0)
	{
	}

CPanel::~CPanel()
	{
	for (int i = 0; i < m_NumInternalPanels; i++)
		{
		delete &m_InternalPanels[i];
		}
	}

void CPanel::SetPanelOrigin (int xO, int yO)
	{
	int DeltaX = xO - m_xO;
	int DeltaY = yO - m_yO;

	m_xO = xO;
	m_yO = yO;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		m_InternalPanels[i]->ShiftPanelOrigin(DeltaX, DeltaY);
		}
	}

void CPanel::ShiftPanelOrigin (int DeltaX, int DeltaY)
	{
	m_xO += DeltaX;
	m_yO += DeltaY;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		m_InternalPanels[i]->ShiftPanelOrigin(DeltaX, DeltaY);
		}
	}

void CPanel::SetOriginX (int xO)
	{
	int DeltaX = xO - m_xO;

	ShiftPanelOrigin(DeltaX, 0);
	}

void CPanel::SetOriginY (int yO)
	{
	int DeltaY = yO - m_yO;

	ShiftPanelOrigin(0, DeltaY);
	}

void CPanel::SetPanelSpace(int width, int height)
	{
	double WidthDelta = double(width)/m_Width;
	double HeightDelta = double(height)/m_Height;

	m_Width = width;
	m_Height = height;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		CPanel *CurrentPanel = m_InternalPanels[i];
		CurrentPanel->SetPanelSpace(int(CurrentPanel->GetWidth()*WidthDelta), int(CurrentPanel->GetHeight()*HeightDelta));
		}
	}

void CPanel::SetPanelWidth(int width)
	{
	SetPanelSpace(width, m_Height);
	}

void CPanel::SetPanelHeight(int height)
	{
	SetPanelSpace(m_Width, height);
	}

void CPanel::ShiftTopEdge(int Delta)
	{
	m_yO = m_yO + Delta;
	SetPanelSpace(m_Width, m_Height + Delta);
	}

void CPanel::ShiftRightEdge(int Delta)
	{
	SetPanelSpace(m_Width + Delta, m_Height);
	}

void CPanel::ShiftBottomEdge(int Delta)
	{
	SetPanelSpace(m_Width, m_Height + Delta);
	}

void CPanel::ShiftLeftEdge(int Delta)
	{
	m_xO = m_xO + Delta;
	SetPanelSpace(m_Width + Delta, m_Height);
	}

TArray <int> CPanel::LeftEdgeLocationInternalPanelSort(void)
	{
	TArray <int> PanelIndices;
	TArray <int> LeftEdgeLocations;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		PanelIndices.Insert(i);
		CPanel *InternalPanel = m_InternalPanels[i];
		if (InternalPanel->IsHidden())
			{
			LeftEdgeLocations.Insert(-1);
			
			}
		else
			{
			LeftEdgeLocations.Insert(InternalPanel->GetOriginX());
			}
		}

	TArray <int> SortedPanelIndices = QuickSortIntegerArray(LeftEdgeLocations, PanelIndices);

	return SortedPanelIndices;
	}

TArray <int> CPanel::RightEdgeLocationInternalPanelSort(void)
	{
	TArray <int> PanelIndices;
	TArray <int> RightEdgeLocations;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		PanelIndices.Insert(i);
		CPanel *InternalPanel = m_InternalPanels[i];
		if (InternalPanel->IsHidden())
			{
			RightEdgeLocations.Insert(-1);
			}
		else
			{
			RightEdgeLocations.Insert(InternalPanel->GetOriginX() + InternalPanel->GetWidth());
			}
		}

	TArray <int> SortedPanelIndices = QuickSortIntegerArray(RightEdgeLocations, PanelIndices);

	return SortedPanelIndices;
	}

TArray <int> CPanel::TopEdgeLocationInternalPanelSort(void)
	{
	TArray <int> PanelIndices;
	TArray <int> TopEdgeLocations;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		PanelIndices.Insert(i);
		CPanel *InternalPanel = m_InternalPanels[i];

		if (InternalPanel->IsHidden())
			{
			TopEdgeLocations.Insert(-1);
			}
		else
			{
			TopEdgeLocations.Insert(InternalPanel->GetOriginY());
			}
		}

	TArray <int> SortedPanelIndices = QuickSortIntegerArray(TopEdgeLocations, PanelIndices);

	return SortedPanelIndices;
	}

TArray <int> CPanel::BottomEdgeLocationInternalPanelSort(void)
	{
	TArray <int> PanelIndices;
	TArray <int> BottomEdgeLocations;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		PanelIndices.Insert(i);
		CPanel *InternalPanel = m_InternalPanels[i];

		if (InternalPanel->IsHidden())
			{
			BottomEdgeLocations.Insert(-1);
			}
		else
			{
			BottomEdgeLocations.Insert(InternalPanel->GetOriginY() + InternalPanel->GetHeight());
			}
		}

	TArray <int> SortedPanelIndices = QuickSortIntegerArray(BottomEdgeLocations, PanelIndices);

	return SortedPanelIndices;
	}

void CPanel::SmoothOutInternalPanels(bool bExpandInPlace, bool bUpDownSmooth)
	{
	TArray <int> aSortedPanelIndices;
	int iProposedChange;
	int iSharedRectBoundary;
	int iDelta;

	RECT rcChangedFocus;
	RECT rcOther;

	if (bUpDownSmooth)
		{
		aSortedPanelIndices = TopEdgeLocationInternalPanelSort();
		}
	else
		{
		aSortedPanelIndices = LeftEdgeLocationInternalPanelSort();
		}

	int iFocusIndex;
	int iOtherIndex;

	int iFocusLocation;
	int iOtherLocation;

	int iProspectiveOtherIndex;

	CPanel *pFocusPanel;
	CPanel *pOtherPanel;

	for (int i = 0; i < m_NumInternalPanels; i++)
		{
		iFocusIndex = aSortedPanelIndices[i];
		pFocusPanel = m_InternalPanels[iFocusIndex];

		if (pFocusPanel->IsHidden())
			continue;
		
		rcChangedFocus = pFocusPanel->GetPanelRect();

		if (i != 0)
			{
			iOtherIndex = -1;
			
			for (int iLoopCount = 0; iLoopCount < i; iLoopCount++)
				{
				iProspectiveOtherIndex = aSortedPanelIndices[i - 1 - iLoopCount];
				pOtherPanel = m_InternalPanels[iProspectiveOtherIndex];

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
			pOtherPanel = m_InternalPanels[iOtherIndex];
			rcOther = pOtherPanel->GetPanelRect();

			if (bUpDownSmooth)
				{
				iOtherLocation = pOtherPanel->GetOriginY() + pOtherPanel->GetHeight();
				}
			else
				{
				iOtherLocation = pOtherPanel->GetOriginX() + pOtherPanel->GetWidth();
				}
			}
		else
			{
			rcOther = this->GetPanelRect();
			if (bUpDownSmooth)
				{
				iOtherLocation = m_yO;
				if (pFocusPanel->IsFixed())
					{
					iOtherLocation += pFocusPanel->GetFixedDeltaY();
					}
				}
			else
				{
				iOtherLocation = m_xO;
				if (pFocusPanel->IsFixed())
					{
					iOtherLocation += pFocusPanel->GetFixedDeltaX();
					}
				}
			}

		if (bUpDownSmooth)
			{
			iFocusLocation = pFocusPanel->GetOriginY();
			}
		else
			{
			iFocusLocation = pFocusPanel->GetOriginX();
			}
			
		iProposedChange = (iFocusLocation - iOtherLocation);

		if (iProposedChange == 0)
			{
			continue;
			}

		if (bUpDownSmooth)
			{
			iDelta = rcChangedFocus.bottom - rcChangedFocus.top;
			rcChangedFocus.top = iOtherLocation;
			rcChangedFocus.bottom = iOtherLocation + iDelta; 
			}
		else
			{
			iDelta = rcChangedFocus.right - rcChangedFocus.left;
			rcChangedFocus.left = iOtherLocation;
			rcChangedFocus.right = iOtherLocation + iDelta;
			}

		if (bUpDownSmooth)
			{
			iSharedRectBoundary = GetSharedTopBottomEdgeLength(&rcChangedFocus, &rcOther);
			if (iSharedRectBoundary > 0)
				{
				pFocusPanel->SetOriginY(iOtherLocation);
				}
			}
		else
			{
			iSharedRectBoundary = GetSharedLeftRightEdgeLength(&rcChangedFocus, &rcOther);
			if (iSharedRectBoundary > 0)
				{
				pFocusPanel->SetOriginX(iOtherLocation);
				}
			}
		}
	}

CPanel *CPanel::AddInternalPanel(int xO, int yO, int width, int height, double rigidity, bool hidden, bool ExpandInPlace, bool FixedRelativeToParent)
	{
	CPanel *NewPanel = NULL;
	NewPanel = new CPanel(xO, yO, width, height, rigidity);
	RECT rcNewPanel = NewPanel->GetPanelRect();

	if (IsRect1InRect2(rcNewPanel, this->GetPanelRect()))
		{
		NewPanel->SetHiddenFlag(hidden);
		NewPanel->SetParentPanel(this);

		if (FixedRelativeToParent)
			{
			NewPanel->SetAsFixed(xO - m_xO, yO - m_yO);
			}
		m_InternalPanels.Insert(NewPanel);
		UpdateNumInternalPanels();

		SmoothOutInternalPanels(ExpandInPlace, true);
		SmoothOutInternalPanels(ExpandInPlace, false);

		return NewPanel;
		}
	else
		{
		delete NewPanel;
		return NULL;
		}
	}

CPanel *CPanel::AddInternalPanel(int xO, int yO, int width, int height, bool hidden, bool ExpandInPlace, bool FixedRelativeToParent)
	{
	CPanel *NewPanel = AddInternalPanel(xO, yO, width, height, m_Rigidity, hidden, ExpandInPlace, FixedRelativeToParent);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanelRelativeToOrigin(int DeltaX, int DeltaY, int width, int height, double rigidity, bool hidden, bool ExpandInPlace, bool FixedRelativeToParent)
	{
	int xO = m_xO + DeltaX;
	int yO = m_yO + DeltaY;

	CPanel *NewPanel = AddInternalPanel(xO, yO, width, height, rigidity, hidden, ExpandInPlace, FixedRelativeToParent);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanelRelativeToOrigin(int DeltaX, int DeltaY, int width, int height, bool hidden, bool ExpandInPlace, bool FixedRelativeToParent)
	{
	int xO = m_xO + DeltaX;
	int yO = m_yO + DeltaY;

	CPanel *NewPanel = AddInternalPanel(xO, yO, width, height, m_Rigidity, hidden, ExpandInPlace, FixedRelativeToParent);

	return NewPanel;
	}

TArray <CSubSession *> CPanel::GetInternalPanelSessions(void)
	{
	TArray <CSubSession *> InternalSessions;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		InternalSessions.Insert(m_InternalPanels[i]->GetAssociatedSession());
		}

	return InternalSessions;
	}

RECT CPanel::GetPanelRect(void)
	{
	RECT rc;

	rc.left = m_xO;
	rc.top = m_yO;
	rc.right = m_xO + m_Width;
	rc.bottom = m_yO + m_Height;

	return rc;
	}

RECT CPanel::GetScaledInnerRect(double scale)
	{
	RECT rc;

	int iLeft = int(m_xO + m_Width*(1 - scale)*0.5);
	int iTop = int(m_yO + m_Height*(1 - scale)*0.5);

	rc.left = iLeft;
	rc.top = iTop; 
	rc.right = iLeft + int(m_Width*scale);
	rc.bottom = iTop + int(m_Height*scale);

	return rc;
	}

void CPanel::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (!IsEmpty() && !IsHidden())
		{
		m_AssociatedSession->OnPaint(Screen, rcInvalid);
		}

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		m_InternalPanels[i]->OnPaint(Screen, rcInvalid);
		}
	}

TArray <CSubSession *> CPanel::ReturnSessionsContainingPoint(int x, int y)
	{
	TArray <CSubSession *> aRelevantSessions;

	if (!IsEmpty() && !IsHidden())
		{
		if (IsPointInRect(x, y, GetPanelRect()))
			{
			aRelevantSessions.Insert(m_AssociatedSession);
			}
		}

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		aRelevantSessions.Insert(m_InternalPanels[i]->ReturnSessionsContainingPoint(x, y));
		}

	return aRelevantSessions;
	}

int CPanel::GetInternalPanelIndex(CPanel *Panel)
	{
	int iDefaultIndex = -1;	//  not found in m_InternalPanels

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		if (m_InternalPanels[i] == Panel)
			{
			return i;
			}
		}

	return iDefaultIndex;
	}

void CPanel::HideInternalPanel(int PanelIndex)
	{
	//  hide the panel first so other panels can take its space during rearrangement
	m_InternalPanels[PanelIndex]->SetHiddenFlag(true);

	SmoothOutInternalPanels(false, true);
	SmoothOutInternalPanels(false, false);
	}

void CPanel::ShowInternalPanel(int PanelIndex)
	{
	CPanel *FocusPanel = m_InternalPanels[PanelIndex];

	//  unhide the panel so that space can be made for it
	FocusPanel->SetHiddenFlag(false);

	SmoothOutInternalPanels(false, true);
	SmoothOutInternalPanels(false, false);
	}

void CPanel::Hide(void)
	{
	if (m_ParentPanel != NULL)
		{
		int iRelevantPanelIndex = m_ParentPanel->GetInternalPanelIndex(this);
		m_ParentPanel->HideInternalPanel(iRelevantPanelIndex);
		}
	//  else, cannot hide since this is a "main panel" and must be shown at all times
	}                                                                   

void CPanel::Show(void)
	{
	if (m_ParentPanel != NULL)
		{
		int RelevantPanelIndex = m_ParentPanel->GetInternalPanelIndex(this);
		m_ParentPanel->ShowInternalPanel(RelevantPanelIndex);
		}
	//  else, does not need to be shown since this is a "main panel" and cannot be hidden
	}
