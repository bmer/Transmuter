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
	m_IsFocus(FALSE)
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
	m_IsFocus(FALSE)
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
	m_IsFocus(FALSE)
	{
	}

CPanel::~CPanel()
	{
	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		delete &m_InternalPanels[i];
		}
	}

void CPanel::SetPanelOrigin (int xO, int yO)
	{
	m_xO = xO;
	m_yO = yO;
	}

void CPanel::SetPanelSpace(int width, int height)
	{
	double WidthDelta = double(width)/m_Width;
	double HeightDelta = double(height)/m_Height;

	m_Width = width;
	m_Height = height;

	for (int i = 0; i < m_InternalPanels.GetCount(); i++)
		{
		CPanel &CurrentPanel = m_InternalPanels[i];
		CurrentPanel.SetPanelSpace(CurrentPanel.GetPanelWidth()*WidthDelta, CurrentPanel.GetPanelHeight()*HeightDelta);
		}
	}

CPanel * CPanel::AddInternalPanel(int xO, int yO, int width, int height, double rigidity)
	{
	CPanel *NewPanel = NULL;

	if (IsRect1InRect2(xO, yO, width, height, m_xO, m_yO, m_Width, m_Height))
		{
		NewPanel = new CPanel(xO, yO, width, height, rigidity);

		for (int i = 0; i < m_InternalPanels.GetCount(); i++)
			{
			CPanel &CurrentInternalPanel = m_InternalPanels[i];
			
			int cxO = CurrentInternalPanel.GetOriginX();
			int cyO = CurrentInternalPanel.GetOriginY();
			int cwidth = CurrentInternalPanel.GetPanelWidth();
			int cheight = CurrentInternalPanel.GetPanelHeight();
			double crigidity = CurrentInternalPanel.GetPanelRigidity();

			TArray <bool> NewRectCornerStatus = DoesRect1ClashRect2(xO, yO, width, height, cxO, cyO, cwidth, cheight);

			double TotalRigidity = rigidity + crigidity;
			double RelativeRigidityNew = rigidity/TotalRigidity;

			if (NewRectCornerStatus[0])
				{
				if (NewRectCornerStatus[1])
					{
					int Delta = xO - cxO;
					width = width + Delta*RelativeRigidityNew;
					CurrentInternalPanel.SetPanelHeight(cheight - Delta*(1 - RelativeRigidityNew));
					}
				}
			else if (NewRectCornerStatus[1])
				{
				}
			else if (NewRectCornerStatus[2])
				{
				}
			else if (NewRectCornerStatus[3])
				{
				}
			}

		m_InternalPanels.Insert(*NewPanel);
		}

	return NewPanel;
	}