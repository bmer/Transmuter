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
	m_Hidden(false)
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
	m_Hidden(false)
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
	m_Hidden(false)
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
		CurrentPanel->SetPanelSpace(int(CurrentPanel->GetWidth()*WidthDelta), int(CurrentPanel->GetHeight()*HeightDelta));
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

void CPanel::MakeSpaceForPanel(CPanel *FocusPanel, bool ExpandInPlace)
	{
	int xO = FocusPanel->GetOriginX();
	int yO = FocusPanel->GetOriginY();
	int width = FocusPanel->GetWidth();
	int height = FocusPanel->GetHeight();
	double rigidity = FocusPanel->GetRigidity();

	CPanel *CurrentInternalPanel;

	int CIP_xO;
	int CIP_yO;
	int CIP_width;
	int CIP_height;
	double CIP_rigidity;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		CurrentInternalPanel = m_InternalPanels[i];
		if (CurrentInternalPanel->IsHidden())
			{
			continue;
			}
		
		xO = FocusPanel->GetOriginX();
		yO = FocusPanel->GetOriginY();
		width = FocusPanel->GetWidth();
		height = FocusPanel->GetHeight();
		rigidity = FocusPanel->GetRigidity();

		CIP_xO = CurrentInternalPanel->GetOriginX();
		CIP_yO = CurrentInternalPanel->GetOriginY();
		CIP_width = CurrentInternalPanel->GetWidth();
		CIP_height = CurrentInternalPanel->GetHeight();
		CIP_rigidity = CurrentInternalPanel->GetRigidity();

		TArray <bool> FocusRectCornerStatus = DoesRect1ClashRect2(FocusPanel->GetPanelRect(), CurrentInternalPanel->GetPanelRect());

		double RelativeRigidity = CalculateRelativeRigidity(CIP_rigidity, rigidity);

		if (FocusRectCornerStatus[0])
			{
			//  calculating overlap between:
			//		bottom edge of existing rectangle
			//		top edge of new rectangle
			int DeltaH = yO - (CIP_yO + CIP_height);

			//  calculating overlap between:
			//		right edge of existing rectangle
			//		left edge of new rectangle
			int DeltaW = xO - (CIP_xO + CIP_width);

			if (ExpandInPlace && DeltaH > 0 && (DeltaH > DeltaW))
				{
				//  shifting bottom edge of existing panel UP
				CurrentInternalPanel->ShiftBottomEdge(int(-1*DeltaH*(1 - RelativeRigidity)));

				//  shifting top edge of new rectangle DOWN
				FocusPanel->ShiftTopEdge(int(DeltaH*RelativeRigidity));
				}
			else if (ExpandInPlace && DeltaW > 0)
				{
				//  shifting right edge of existing panel LEFT
				CurrentInternalPanel->ShiftRightEdge(int(-1*DeltaW*(1 - RelativeRigidity)));

				//  shifting left edge of new panel RIGHT
				FocusPanel->ShiftLeftEdge(int(DeltaW*RelativeRigidity));
				}
			else
				{
				if (DeltaH > 0 && (DeltaH > DeltaW))
					{
					//  shift new rectangle DOWN
					FocusPanel->SetOriginY(yO + DeltaH);
					}
				else if (DeltaW > 0)
					{
					//  shift new rectangle RIGHT
					FocusPanel->SetOriginX(xO + DeltaW);
					}
				}                            
			}

		else if (FocusRectCornerStatus[1])
			{
			//  calculating overlap between:
			//		bottom edge of existing rectangle
			//		top edge of new rectangle
			int DeltaH = yO - (CIP_yO + CIP_height);

			//  calculating overlap between:
			//		left edge of existing rectangle
			//		right edge of new rectangle
			int DeltaW = (xO + width) - CIP_xO;

			if (ExpandInPlace && DeltaH > 0 && (DeltaH > DeltaW))
				{
				//  shift bottom edge of existing rectangle UP
				CurrentInternalPanel->ShiftBottomEdge(int(-1*DeltaH*(1 - RelativeRigidity)));

				//  shift top edge of new rectangle DOWN
				FocusPanel->ShiftTopEdge(int(DeltaH*RelativeRigidity));
				}
			else if (ExpandInPlace && DeltaW > 0)
				{
				//  shift left edge of existing rectangle RIGHT
				CurrentInternalPanel->ShiftLeftEdge(int(DeltaW*(1 - RelativeRigidity)));

				//  shift right edge of new rectangle LEFT
				FocusPanel->ShiftRightEdge(int(-1*DeltaW*RelativeRigidity));
				}
			else
				{
				if (DeltaH > 0 && (DeltaH > DeltaW))
					{
					//  shift new rectangle DOWN
					FocusPanel->SetOriginY(yO + DeltaH);
					}
				else if (DeltaW > 0)
					{
					//  shift new rectangle LEFT
					FocusPanel->SetOriginX(CIP_xO - DeltaW);
					}
				}
			}
		else if (FocusRectCornerStatus[2])
			{
			//  calculating overlap between:
			//		top edge of existing rectangle
			//		bottom edge of new rectangle
			int DeltaH = (yO + height) - CIP_yO;

			//  calculating overlap between:
			//		left edge of existing rectangle
			//		right edge of new rectangle
			int DeltaW = (xO + width) - CIP_xO;

			if (ExpandInPlace && DeltaH > 0 && (DeltaH > DeltaW))
				{
				//  shift top edge of existing rectangle DOWN
				CurrentInternalPanel->ShiftTopEdge(int(DeltaH*(1 - RelativeRigidity)));

				//  shift bottom edge of new rectangle UP
				FocusPanel->ShiftBottomEdge(int(-DeltaH*RelativeRigidity));
				}
			else if (ExpandInPlace && DeltaW > 0)
				{
				//  shift left edge of existing rectangle RIGHT
				CurrentInternalPanel->ShiftLeftEdge(int(DeltaW*(1 - RelativeRigidity)));

				//  shift right edge of new rectangle LEFT
				FocusPanel->ShiftRightEdge(int(-1*DeltaW*RelativeRigidity));
				}
			else
				{
				if (DeltaH > 0 && (DeltaH > DeltaW))
					{
					//  shift existing rectangle DOWN
					CurrentInternalPanel->SetOriginY(CIP_yO + DeltaH);
					MakeSpaceForPanel(CurrentInternalPanel, ExpandInPlace);
					}
				else if (DeltaW > 0)
					{
					//  shift existing rectangle RIGHT
					CurrentInternalPanel->SetOriginX(CIP_xO + DeltaW);
					MakeSpaceForPanel(CurrentInternalPanel, ExpandInPlace);
					}
				}
			}
		else if (FocusRectCornerStatus[3])
			{
			//  calculating overlap between:
			//		top edge of existing rectangle
			//		bottom edge of new rectangle
			int DeltaH = (yO + height) - CIP_yO;

			//  calculating overlap between:
			//		right edge of existing rectangle
			//		left edge of new rectangle
			int DeltaW = xO - (CIP_xO + CIP_width);

			if (ExpandInPlace && DeltaH > 0 && (DeltaH > DeltaW))
				{
				//  shift top edge of existing rectangle DOWN
				CurrentInternalPanel->ShiftTopEdge(int(DeltaH*(1 - RelativeRigidity)));

				//  shift bottom edge of new rectangle UP
				FocusPanel->ShiftBottomEdge(int(-DeltaH*RelativeRigidity));
				}
			else if (ExpandInPlace && DeltaW > 0)
				{
				//  shift right edge of existing rectangle LEFT
				CurrentInternalPanel->ShiftRightEdge(int(-1*DeltaW*(1 - RelativeRigidity)));

				//  shift left edge of new rectangle RIGHT
				FocusPanel->ShiftLeftEdge(int(DeltaW*RelativeRigidity));
				}
			else
				{
				if (DeltaH > 0 && (DeltaH > DeltaW))
					{
					//  shift existing rectangle DOWN
					CurrentInternalPanel->SetOriginY(CIP_yO + DeltaH);
					MakeSpaceForPanel(CurrentInternalPanel, ExpandInPlace);
					}
				else if (DeltaW > 0)
					{
					//  shift existing rectangle LEFT
					CurrentInternalPanel->SetOriginX(CIP_xO - DeltaW);
					MakeSpaceForPanel(CurrentInternalPanel, ExpandInPlace);
					}
				}
			}
		}
	}

CPanel *CPanel::AddInternalPanel(int xO, int yO, int width, int height, double rigidity, bool hidden, bool ExpandInPlace)
	{
	CPanel *NewPanel = NULL;

	if (IsRect1InRect2(xO, yO, width, height, m_xO, m_yO, m_Width, m_Height))
		{
		NewPanel = new CPanel(xO, yO, width, height, rigidity);
		NewPanel->SetParentPanel(this);

		if (!hidden)
			{
			MakeSpaceForPanel(NewPanel, ExpandInPlace);
			}

		m_InternalPanels.Insert(NewPanel);
		}

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanel(int xO, int yO, int width, int height, bool hidden, bool ExpandInPlace)
	{
	CPanel *NewPanel = AddInternalPanel(xO, yO, width, height, m_Rigidity, hidden, ExpandInPlace);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanelRelativeToOrigin(int DeltaX, int DeltaY, int width, int height, double rigidity, bool hidden, bool ExpandInPlace)
	{
	int xO = m_xO + DeltaX;
	int yO = m_yO + DeltaY;

	CPanel *NewPanel = AddInternalPanel(xO, yO, width, height, rigidity, hidden, ExpandInPlace);

	return NewPanel;
	}

CPanel *CPanel::AddInternalPanelRelativeToOrigin(int DeltaX, int DeltaY, int width, int height, bool hidden, bool ExpandInPlace)
	{
	int xO = m_xO + DeltaX;
	int yO = m_yO + DeltaY;

	CPanel *NewPanel = AddInternalPanel(xO, yO, width, height, m_Rigidity, hidden, ExpandInPlace);

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

void CPanel::SnuglyFitInternalPanel(int PanelIndex, bool ExpandInPlace)
	{
	CPanel *FocusPanel = m_InternalPanels[PanelIndex];

	if (!FocusPanel->IsHidden())
		{

		int LeftEdgeSpace = 0;
		int RightEdgeSpace = 0;
		int TopEdgeSpace = 0;
		int BottomEdgeSpace = 0;

		RECT FocusPanelRect = FocusPanel->GetPanelRect();
		double rigidity = FocusPanel->GetRigidity();

		int DeltaX;
		int DeltaY;

		CPanel *RelevantLeftEdgePanel = NULL;
		CPanel *RelevantRightEdgePanel = NULL;
		CPanel *RelevantTopEdgePanel = NULL;
		CPanel *RelevantBottomEdgePanel = NULL;
		CPanel *OtherPanel;

		double RelativeRigidity;
		double OtherPanelRigidity;

		for (int i = 0; i < m_InternalPanels.GetCount() + 1; i++)
			{
			if (i != PanelIndex)
				{
				if (i < m_InternalPanels.GetCount())
					{
					OtherPanel = m_InternalPanels[i];
					}
				else
					{
					OtherPanel = this;
					}

				if (OtherPanel->IsHidden())
					{
					continue;
					}
				
				DeltaX = OtherPanel->GetXDisplacementToLeftEdge(FocusPanelRect.right);
				if (DeltaX > 0 && DeltaX > RightEdgeSpace)
					{
					RightEdgeSpace = DeltaX;
					RelevantRightEdgePanel = OtherPanel;
					}
			
				DeltaX = OtherPanel->GetXDisplacementToRightEdge(FocusPanelRect.left);
				if (DeltaX > 0 && DeltaX > LeftEdgeSpace)
					{
					LeftEdgeSpace = DeltaX;
					RelevantLeftEdgePanel = OtherPanel;
					}

				DeltaY = OtherPanel->GetYDisplacementToTopEdge(FocusPanelRect.bottom);
				if (DeltaY > 0 && DeltaY > BottomEdgeSpace)
					{
					BottomEdgeSpace = DeltaY;
					RelevantBottomEdgePanel = OtherPanel;
					}

				DeltaY = OtherPanel->GetYDisplacementToBottomEdge(FocusPanelRect.top);
				if (DeltaY > 0 && DeltaY > TopEdgeSpace)
					{
					TopEdgeSpace = DeltaY;
					RelevantTopEdgePanel = OtherPanel;
					}
				}
			}

		if (RelevantLeftEdgePanel != NULL)
			{
			if (ExpandInPlace)
				{
				OtherPanel = RelevantLeftEdgePanel;
				OtherPanelRigidity = OtherPanel->GetRigidity();
				RelativeRigidity = CalculateRelativeRigidity(OtherPanelRigidity, rigidity);

				//  shift left edge LEFT
				FocusPanel->ShiftLeftEdge(int(-1*LeftEdgeSpace*RelativeRigidity));
				//  shift other panel's right edge RIGHT
				OtherPanel->ShiftRightEdge(int(LeftEdgeSpace*(1-RelativeRigidity)));
				}
			else
				{
				//  shift panel LEFT
				FocusPanel->SetOriginX(FocusPanelRect.left - LeftEdgeSpace);
				}
			}

		if (RelevantRightEdgePanel != NULL)
			{
			if (ExpandInPlace)
				{
				OtherPanel = RelevantRightEdgePanel;
				OtherPanelRigidity = OtherPanel->GetRigidity();
				RelativeRigidity = CalculateRelativeRigidity(OtherPanelRigidity, rigidity);

				//  shift right edge RIGHT
				FocusPanel->ShiftRightEdge(int(RightEdgeSpace*RelativeRigidity));
				//  shift other panel's left edge LEFT
				OtherPanel->ShiftLeftEdge(int(-1*RightEdgeSpace*(1-RelativeRigidity)));
				}
			//  when snugly fitting, without in place expansion, we only move panels to the left and up
			}

		if (RelevantBottomEdgePanel != NULL)
			{
			if (ExpandInPlace)
				{
				OtherPanel = RelevantBottomEdgePanel;
				OtherPanelRigidity = OtherPanel->GetRigidity();
				RelativeRigidity = CalculateRelativeRigidity(OtherPanelRigidity, rigidity);

				//  shift bottom edge DOWN
				FocusPanel->ShiftBottomEdge(int(BottomEdgeSpace*RelativeRigidity));
				//  shift other panel's top edge UP
				OtherPanel->ShiftTopEdge(int(-1*BottomEdgeSpace*(1-RelativeRigidity)));
				}
			}

		if (RelevantTopEdgePanel != NULL)
			{
			if (ExpandInPlace)
				{
				OtherPanel = RelevantTopEdgePanel;
				OtherPanelRigidity = OtherPanel->GetRigidity();
				RelativeRigidity = CalculateRelativeRigidity(OtherPanelRigidity, rigidity);

				//  shift top edge UP
				FocusPanel->ShiftTopEdge(int(-1*TopEdgeSpace*RelativeRigidity));
				//  shift other panel's bottom edge DOWN
				OtherPanel->ShiftBottomEdge(int(TopEdgeSpace*(1-RelativeRigidity)));
				}
			else
				{
				//  shift panel UP
				FocusPanel->SetOriginY(FocusPanelRect.top - TopEdgeSpace);
				}
			}
		}
	}

void CPanel::RearrangeAllPanelsToSnuglyFit(int MaxAttempts, bool ExpandInPlace)
	{
	for (int Attempt = 0; Attempt < MaxAttempts; Attempt++)
		{
		for (int i = 0; i < m_InternalPanels.GetCount(); i++)
			{
			if (!m_InternalPanels[i]->IsHidden())
				{
				SnuglyFitInternalPanel(i, ExpandInPlace);
				}
			}
		}
	}

RECT CPanel::GetPanelRect(void)
	{
	RECT rc;

	rc.left = m_xO;
	rc.top = m_yO;
	rc.right = m_Width;
	rc.bottom = m_Height;

	return rc;
	}

RECT CPanel::GetScaledInnerRect(double scale)
	{
	RECT rc;

	rc.left = int(m_xO + m_Width*(1 - scale)*0.5);
	rc.top = int(m_yO + m_Height*(1 - scale)*0.5);
	rc.right = int(m_Width*scale);
	rc.bottom = int(m_Height*scale);

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
	TArray <CSubSession *> RelevantSessions;

	if (!IsEmpty() && !IsHidden())
		{
		if (IsPointInRect(x, y, GetPanelRect()))
			{
			RelevantSessions.Insert(m_AssociatedSession);
			}
		}

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		RelevantSessions.Insert(m_InternalPanels[i]->ReturnSessionsContainingPoint(x, y));
		}

	return RelevantSessions;
	}

int CPanel::GetInternalPanelIndex(CPanel *Panel)
	{
	int DefaultIndex = -1;	//  not found in m_InternalPanels

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		if (m_InternalPanels[i] == Panel)
			{
			return i;
			}
		}

	return DefaultIndex;
	}

void CPanel::HideInternalPanel(int PanelIndex)
	{
	//  hide the panel first so other panels can take its space during rearrangement
	m_InternalPanels[PanelIndex]->SetHiddenFlag(true);
	RearrangeAllPanelsToSnuglyFit(3, false);
	}

void CPanel::ShowInternalPanel(int PanelIndex)
	{
	CPanel *FocusPanel = m_InternalPanels[PanelIndex];
	//  keep the panel hidden until space has been made for it
	MakeSpaceForPanel(FocusPanel, false);
	FocusPanel->SetHiddenFlag(false);
	}

void CPanel::Hide(void)
	{
	if (m_ParentPanel != NULL)
		{
		int RelevantPanelIndex = m_ParentPanel->GetInternalPanelIndex(this);
		m_ParentPanel->HideInternalPanel(RelevantPanelIndex);
		}
	}

void CPanel::Show(void)
	{
	if (m_ParentPanel != NULL)
		{
		int RelevantPanelIndex = m_ParentPanel->GetInternalPanelIndex(this);
		m_ParentPanel->ShowInternalPanel(RelevantPanelIndex);
		}
	}
