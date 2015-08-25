//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CPanel::CPanel(void) : m_xO(-1),
	m_yO(-1),
	m_Width(-1),
	m_Height(-1),
	m_Rigidity(0),
	m_ParentPanel(NULL),
	m_ErrorOccurred(FALSE),
	m_ErrorString(CONSTLIT("")),
	m_AssociatedSession(NULL),
	m_Focus(0)
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
	m_Focus(0)
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
	m_Focus(0)
	{
	}

CPanel::~CPanel()
	{
	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		delete &m_InternalPanels[i];
		}
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
		CurrentPanel->SetPanelSpace(CurrentPanel->GetWidth()*WidthDelta, CurrentPanel->GetHeight()*HeightDelta);
		}
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

CPanel *CPanel::AddInternalPanel(int xO, int yO, int width, int height, double rigidity)
	{
	CPanel *NewPanel = NULL;

	if (IsRect1InRect2(xO, yO, width, height, m_xO, m_yO, m_Width, m_Height))
		{
		NewPanel = new CPanel(xO, yO, width, height, rigidity);

		for (int i = 0; i < m_InternalPanels.GetCount(); i++)
			{
			CPanel *CurrentInternalPanel = m_InternalPanels[i];
			
			int cxO = CurrentInternalPanel->GetOriginX();
			int cyO = CurrentInternalPanel->GetOriginY();
			int cwidth = CurrentInternalPanel->GetWidth();
			int cheight = CurrentInternalPanel->GetHeight();
			double crigidity = CurrentInternalPanel->GetRigidity();

			TArray <bool> NewRectCornerStatus = DoesRect1ClashRect2(xO, yO, width, height, cxO, cyO, cwidth, cheight);

			double TotalRigidity = rigidity + crigidity;
			double RelativeRigidity = crigidity/TotalRigidity;

			if (NewRectCornerStatus[0])
				{
				//  calculating overlap between:
				//		bottom edge of existing rectangle
				//		top edge of new rectangle
				int DeltaH = yO - (cyO + cheight);

				//  calculating overlap between:
				//		right edge of existing rectangle
				//		left edge of new rectangle
				int DeltaW = xO - (cxO + cwidth);

				if (DeltaH > 0 && (DeltaH > DeltaW))
					{
					//  shifting bottom edge of existing panel UP
					CurrentInternalPanel->ShiftBottomEdge(-1*DeltaH*(1 - RelativeRigidity));

					//  shifting top edge of new rectangle DOWN
					NewPanel->ShiftTopEdge(DeltaH*RelativeRigidity);
					}
				else if (DeltaW > 0)
					{
					//  shifting right edge of existing panel LEFT
					CurrentInternalPanel->ShiftRightEdge(-1*DeltaW*(1 - RelativeRigidity));

					//  shifting left edge of new panel RIGHT
					NewPanel->ShiftLeftEdge(DeltaW*RelativeRigidity);
					}
				}
			else if (NewRectCornerStatus[1])
				{
				//  calculating overlap between:
				//		bottom edge of existing rectangle
				//		top edge of new rectangle
				int DeltaH = yO - (cyO + cheight);

				//  calculating overlap between:
				//		left edge of existing rectangle
				//		right edge of new rectangle
				int DeltaW = (xO + width) - cxO;

				if (DeltaH > 0 && (DeltaH > DeltaW))
					{
					//  shift bottom edge of existing rectangle UP
					CurrentInternalPanel->ShiftBottomEdge(-1*DeltaH*(1 - RelativeRigidity));

					//  shift top edge of new rectangle DOWN
					NewPanel->ShiftTopEdge(DeltaH*RelativeRigidity);
					}
				else if (DeltaW > 0)
					{
					//  shift left edge of existing rectangle RIGHT
					CurrentInternalPanel->ShiftLeftEdge(DeltaW*(1 - RelativeRigidity));

					//  shift right edge of new rectangle LEFT
					NewPanel->ShiftRightEdge(-1*DeltaW*RelativeRigidity);
					}
				}
			else if (NewRectCornerStatus[2])
				{
				//  calculating overlap between:
				//		top edge of existing rectangle
				//		bottom edge of new rectangle
				int DeltaH = (yO + height) - cyO;

				//  calculating overlap between:
				//		left edge of existing rectangle
				//		right edge of new rectangle
				int DeltaW = (xO + width) - cxO;

				if (DeltaH > 0 && (DeltaH > DeltaW))
					{
					//  shift top edge of existing rectangle DOWN
					CurrentInternalPanel->ShiftTopEdge(DeltaH*(1 - RelativeRigidity));

					//  shift bottom edge of new rectangle UP
					NewPanel->ShiftBottomEdge(-DeltaH*RelativeRigidity);
					}
				else if (DeltaW > 0)
					{
					//  shift left edge of existing rectangle RIGHT
					CurrentInternalPanel->ShiftLeftEdge(DeltaW*(1 - RelativeRigidity));

					//  shift right edge of new rectangle LEFT
					NewPanel->ShiftRightEdge(-1*DeltaW*RelativeRigidity);
					}
				}
			else if (NewRectCornerStatus[3])
				{
				//  calculating overlap between:
				//		top edge of existing rectangle
				//		bottom edge of new rectangle
				int DeltaH = (yO + height) - cyO;

				//  calculating overlap between:
				//		right edge of existing rectangle
				//		left edge of new rectangle
				int DeltaW = xO - (cxO + cwidth);

				if (DeltaH > 0 && (DeltaH > DeltaW))
					{
					//  shift top edge of existing rectangle DOWN
					CurrentInternalPanel->ShiftTopEdge(DeltaH*(1 - RelativeRigidity));

					//  shift bottom edge of new rectangle UP
					NewPanel->ShiftBottomEdge(-DeltaH*RelativeRigidity);
					}
				else if (DeltaW > 0)
					{
					//  shift right edge of existing rectangle LEFT
					CurrentInternalPanel->ShiftRightEdge(-1*DeltaW*(1 - RelativeRigidity));

					//  shift left edge of new rectangle RIGHT
					NewPanel->ShiftLeftEdge(DeltaW*RelativeRigidity);
					}
				}
			}

		m_InternalPanels.Insert(NewPanel);
		}

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanel(int xO, int yO, int width, int height)
	{
	CPanel *NewPanel = AddInternalPanel(xO, yO, width, height, m_Rigidity);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanelRelativeToOrigin(int DeltaX, int DeltaY, int width, int height, double rigidity)
	{
	int xO = m_xO + DeltaX;
	int yO = m_yO + DeltaY;

	CPanel *NewPanel = AddInternalPanel(xO, yO, width, height, rigidity);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanelRelativeToOrigin(int DeltaX, int DeltaY, int width, int height)
	{
	int xO = m_xO + DeltaX;
	int yO = m_yO + DeltaY;

	CPanel *NewPanel = AddInternalPanel(xO, yO, width, height, m_Rigidity);

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

void CPanel::ExpandInternalPanel(int PanelIndex)
	{
	CPanel *FocusPanel = m_InternalPanels[PanelIndex];

	int RelevantLeftEdgePanel = -1;
	int RelevantRightEdgePanel = -1;
	int RelevantTopEdgePanel = -1;
	int RelevantBottomEdgePanel = -1;

	int LeftEdgeSpace = 0;
	int RightEdgeSpace = 0;
	int TopEdgeSpace = 0;
	int BottomEdgeSpace = 0;

	int xO = FocusPanel->GetOriginX();
	int yO = FocusPanel->GetOriginY();
	int width = FocusPanel->GetWidth();
	int height = FocusPanel->GetHeight();
	double rigidity = FocusPanel->GetRigidity();

	int DeltaX;
	int DeltaY;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		if (i != PanelIndex)
			{
			CPanel *InternalPanel = m_InternalPanels[i];

			DeltaX = InternalPanel->GetXDisplacementToLeftEdge(xO + width);
			if (DeltaX > 0 && DeltaX > RightEdgeSpace)
				{
				RightEdgeSpace = DeltaX;
				RelevantRightEdgePanel = i;
				}
			
			DeltaX = InternalPanel->GetXDisplacementToRightEdge(xO);
			if (DeltaX > 0 && DeltaX > LeftEdgeSpace)
				{
				LeftEdgeSpace = DeltaX;
				RelevantLeftEdgePanel = i;
				}

			DeltaY = InternalPanel->GetYDisplacementToTopEdge(yO + height);
			if (DeltaY > 0 && DeltaY > BottomEdgeSpace)
				{
				BottomEdgeSpace = DeltaY;
				RelevantBottomEdgePanel = i;
				}

			DeltaY = InternalPanel->GetYDisplacementToBottomEdge(yO);
			if (DeltaY > 0 && DeltaY > TopEdgeSpace)
				{
				TopEdgeSpace = DeltaY;
				RelevantTopEdgePanel = i;
				}
			}
		}

	double TotalRigidity;
	double RelativeRigidity;
	double OtherPanelRigidity;
	CPanel *OtherPanel;

	if (RelevantLeftEdgePanel != -1)
		{
		OtherPanel = m_InternalPanels[RelevantLeftEdgePanel];
		OtherPanelRigidity = OtherPanel->GetRigidity();
		TotalRigidity = (OtherPanelRigidity + rigidity);
		RelativeRigidity = OtherPanelRigidity/TotalRigidity;

		//  shift left edge LEFT
		FocusPanel->ShiftLeftEdge(-1*LeftEdgeSpace*RelativeRigidity);
		//  shift other panel's right edge RIGHT
		OtherPanel->ShiftRightEdge(LeftEdgeSpace*(1-RelativeRigidity));
		}

	if (RelevantRightEdgePanel != -1)
		{
		OtherPanel = m_InternalPanels[RelevantRightEdgePanel];
		OtherPanelRigidity = OtherPanel->GetRigidity();
		TotalRigidity = (OtherPanelRigidity + rigidity);
		RelativeRigidity = OtherPanelRigidity/TotalRigidity;

		//  shift right edge RIGHT
		FocusPanel->ShiftRightEdge(RightEdgeSpace*RelativeRigidity);
		//  shift other panel's left edge LEFT
		OtherPanel->ShiftLeftEdge(-1*RightEdgeSpace*(1-RelativeRigidity));
		}

	if (RelevantBottomEdgePanel != -1)
		{
		OtherPanel = m_InternalPanels[RelevantBottomEdgePanel];
		OtherPanelRigidity = OtherPanel->GetRigidity();
		TotalRigidity = (OtherPanelRigidity + rigidity);
		RelativeRigidity = OtherPanelRigidity/TotalRigidity;

		//  shift bottom edge DOWN
		FocusPanel->ShiftBottomEdge(BottomEdgeSpace*RelativeRigidity);
		//  shift other panel's top edge UP
		OtherPanel->ShiftTopEdge(-1*BottomEdgeSpace*(1-RelativeRigidity));
		}

	if (RelevantTopEdgePanel != -1)
		{
		OtherPanel = m_InternalPanels[RelevantTopEdgePanel];
		OtherPanelRigidity = OtherPanel->GetRigidity();
		TotalRigidity = (OtherPanelRigidity + rigidity);
		RelativeRigidity = OtherPanelRigidity/TotalRigidity;

		//  shift top edge UP
		FocusPanel->ShiftTopEdge(-1*TopEdgeSpace*RelativeRigidity);
		//  shift other panel's bottom edge DOWN
		OtherPanel->ShiftBottomEdge(TopEdgeSpace*(1-RelativeRigidity));
		}
	}

void CPanel::ExpandAllInternalPanels(void)
	{
	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		ExpandInternalPanel(i);
		}
	}

TArray <int> CPanel::GetRectDefinition(void)
	{
	TArray <int> RectDefinition;

	RectDefinition.Insert(m_xO);
	RectDefinition.Insert(m_yO);
	RectDefinition.Insert(m_Width);
	RectDefinition.Insert(m_Height);

	return RectDefinition;
	}

void CPanel::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (!IsEmpty())
		{
		m_AssociatedSession->OnPaint(Screen, rcInvalid);
		}

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		m_InternalPanels[i]->OnPaint(Screen, rcInvalid);
		}
	}