//	CFloatContainer.cpp
//
//	Copyright (c) 2016 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"
#include "CFloatContainer.h"

CSplitContainer::CSplitContainer(CString sName, CHumanInterface &HI, int iWidth, int iHeight) : CContainer(sName, HI, iWidth, iHeight)
	{
	SetSeparatorPositionFactor(-1.0);
	}

bool CSplitContainer::PanelShouldBePainted(int iPanelIndex)
	{
	bool bNoZeroDimensions;
	
	if (iPanelIndex == 0)
		{
		if (m_pLeafPanel0 == NULL)
			{
			return false;
			}
		
		CPanelRect &ThisPanelRect = m_pLeafPanel0->PanelRect;
		bNoZeroDimensions = ThisPanelRect.HasZeroDimension();
		return (!m_pLeafPanel0->IsHidden() && bNoZeroDimensions);
		}
	else if (iPanelIndex == 1)
		{
		if (m_pLeafPanel1 == NULL)
			{
			return false;
			}

		CPanelRect &ThisPanelRect = m_pLeafPanel1->PanelRect;
		bNoZeroDimensions = ThisPanelRect.HasZeroDimension();
		return (!m_pLeafPanel1->IsHidden() && bNoZeroDimensions);
		}

	return false;
	}
void CSplitContainer::SmoothOut (void)
	{
	bool PaintPanel0 = PanelShouldBePainted(0);
	bool PaintPanel1 = PanelShouldBePainted(1);

	if (!PaintPanel0 && !PaintPanel1)
		{
		// nothing to do
		return;
		}

	if (PaintPanel0 && PaintPanel1)
		{
		// we have to paint two panels
		SetSeparatorPositionFactor(m_fSeparatorPosFactor);
		if (m_SplitDirn == V)
			{
			int iPanel0Height = m_pLeafPanel0->PanelRect.GetHeight();
			int iPanel1Height = m_pLeafPanel1->PanelRect.GetHeight();
			float fHeightRatio = float(iPanel0Height) / float(iPanel1Height);

			int iAvailableSpace = PanelRect.GetHeight() - m_iSeparatorThickness;

			int iNewPanel0Height = int(iAvailableSpace*fHeightRatio);

			m_pLeafPanel0->PanelRect.SetHeight(iNewPanel0Height);
			m_pLeafPanel0->PanelRect.SetWidth(PanelRect.GetWidth());
			m_pLeafPanel0->PanelRect.SetOrigin(PanelRect.GetOriginX(), PanelRect.GetOriginY());

			m_pLeafPanel1->PanelRect.SetHeight(iAvailableSpace - iNewPanel0Height);
			m_pLeafPanel1->PanelRect.SetWidth(PanelRect.GetWidth());
			m_pLeafPanel1->PanelRect.SetOrigin(PanelRect.GetOriginX(), m_iSeparatorPos + m_iSeparatorThickness);
			}
		else
			{
			int iPanel0Width = m_pLeafPanel0->PanelRect.GetWidth();
			int iPanel1Width = m_pLeafPanel1->PanelRect.GetWidth();
			float fWidthRatio = float(iPanel0Width) / float(iPanel1Width);

			int iAvailableSpace = PanelRect.GetWidth() - m_iSeparatorThickness;

			int iNewPanel0Width = int(iAvailableSpace*fWidthRatio);

			m_pLeafPanel0->PanelRect.SetHeight(iNewPanel0Width);
			m_pLeafPanel0->PanelRect.SetWidth(PanelRect.GetHeight());
			m_pLeafPanel0->PanelRect.SetOrigin(PanelRect.GetOriginX(), PanelRect.GetOriginY());

			m_pLeafPanel1->PanelRect.SetHeight(iAvailableSpace - iNewPanel0Width);
			m_pLeafPanel1->PanelRect.SetWidth(PanelRect.GetHeight());
			m_pLeafPanel1->PanelRect.SetOrigin(m_iSeparatorPos + m_iSeparatorThickness, PanelRect.GetOriginY());
			}
		}
	else if (PaintPanel0)
		{

		}


	if (m_pLeafPanel0 == NULL || m_pLeafPanel0->IsHidden())
		{
		if (m_pLeafPanel1 == NULL || m_pLeafPanel1->IsHidden())
			{
			return;
			}
		else
			{
			m_pLeafPanel1->PanelRect.SetOrigin(PanelRect.GetOriginX(), PanelRect.GetOriginY());
			m_pLeafPanel1->PanelRect.SetHeight(PanelRect.GetHeight());
			m_pLeafPanel1->PanelRect.SetWidth(PanelRect.GetWidth());
			m_iSeparatorPos = -1;
			}
		}
	else
		{
		if (m_pLeafPanel1 == NULL || m_pLeafPanel1->IsHidden())
			{
			m_pLeafPanel0->PanelRect.SetOrigin(PanelRect.GetOriginX(), PanelRect.GetOriginY());
			m_pLeafPanel0->PanelRect.SetHeight(PanelRect.GetHeight());
			m_pLeafPanel0->PanelRect.SetWidth(PanelRect.GetWidth());
			m_iSeparatorPos = -1;
			}
		else
			{
			if (m_SplitDirn == H)
				{
				m_iSeparatorPos = int(m_fSeparatorPosFactor*PanelRect.GetWidth());

				m_pLeafPanel0->PanelRect.SetOrigin(PanelRect.GetOriginX(), PanelRect.GetOriginY());
				m_pLeafPanel0->PanelRect.SetWidth(PanelRect.GetWidth() - m_iSeparatorPos);
				m_pLeafPanel0->PanelRect.SetHeight(PanelRect.GetHeight());

				m_pLeafPanel1->PanelRect.SetOrigin(m_iSeparatorPos + m_iSeparatorThickness, PanelRect.GetOriginY());
				m_pLeafPanel1->PanelRect.SetWidth(PanelRect.GetWidth() - m_pLeafPanel0->PanelRect.GetWidth() - m_iSeparatorThickness);
				m_pLeafPanel1->PanelRect.SetHeight(PanelRect.GetHeight());
				}
			else if (m_SplitDirn == V)
				{
				m_iSeparatorPos = int(m_fSeparatorPosFactor*PanelRect.GetHeight());

				m_pLeafPanel0->PanelRect.SetOrigin(PanelRect.GetOriginX(), PanelRect.GetOriginY());
				m_pLeafPanel0->PanelRect.SetWidth(PanelRect.GetWidth());
				m_pLeafPanel0->PanelRect.SetHeight(PanelRect.GetHeight() - m_iSeparatorPos);

				m_pLeafPanel1->PanelRect.SetOrigin(PanelRect.GetOriginX(), m_iSeparatorPos + m_iSeparatorThickness);
				m_pLeafPanel1->PanelRect.SetWidth(PanelRect.GetWidth());
				m_pLeafPanel1->PanelRect.SetHeight(PanelRect.GetHeight() - m_pLeafPanel0->PanelRect.GetHeight() - m_iSeparatorThickness);
				}
			}
		}
	}

bool CSplitContainer::PlacePanel (IPanel *pPanel, ESplitDirns SplitDirn, int iInsertIndex)
	{
	if (SplitDirn != m_SplitDirn)
		{
		return false;
		}

	if (iInsertIndex == 0)
		{
		if (m_pLeafPanel0 != NULL)
			{
			if (m_pLeafPanel1 != NULL)
				{
				return false;
				}
			else
				{
				m_pLeafPanel1 = m_pLeafPanel0;
				m_pLeafPanel0 = pPanel;
				}
			}
		else
			{
			m_pLeafPanel0 = pPanel;
			}
		}
	else if (iInsertIndex == 1)
		{
		if (m_pLeafPanel1 != NULL)
			{
			if (m_pLeafPanel0 != NULL)
				{
				return false;
				}
			else
				{
				m_pLeafPanel0 = m_pLeafPanel1;
				m_pLeafPanel1 = pPanel;
				}
			}
		else
			{
			m_pLeafPanel1 = pPanel;
			}
		}
	else
		{
		return false;
		}

	SmoothOut();
	pPanel->ConfirmPlacement();

	return true;
	}

void CSplitContainer::FlipPanelLocation(void)
	{
	IPanel *temp = m_pLeafPanel1;
	m_pLeafPanel1 = m_pLeafPanel0;
	m_pLeafPanel0 = m_pLeafPanel1;

	SmoothOut();
	}

void CSplitContainer::SetSeparatorPosition(int iSeparatorPos)
	{
	if (m_SplitDirn != N)
		{
		int iSepPosLowerBound;
		int iSepPosUpperBound;
		float fSepPosFactorDenominator;

		if (m_SplitDirn == V)
			{
			iSepPosLowerBound = PanelRect.GetOriginY();
			iSepPosUpperBound = PanelRect.GetEdgePosition(EDGE_BOTTOM) - m_iSeparatorThickness;
			fSepPosFactorDenominator = float(PanelRect.GetHeight() - m_iSeparatorThickness);
			}
		else
			{
			int iSepPosLowerBound = PanelRect.GetOriginX();
			iSepPosUpperBound = PanelRect.GetEdgePosition(EDGE_RIGHT) - m_iSeparatorThickness;
			fSepPosFactorDenominator = float(PanelRect.GetWidth() - m_iSeparatorThickness);
			}

		if (iSepPosLowerBound <= iSeparatorPos && iSeparatorPos <= iSepPosUpperBound)
			{
			m_iSeparatorPos = iSeparatorPos;
			m_fSeparatorPosFactor = float(iSeparatorPos) / fSepPosFactorDenominator;
			}
		}
	}

void CSplitContainer::SetSeparatorPositionFactor(float fSeparatorPosFactor)
	{
	m_fSeparatorPosFactor = fSeparatorPosFactor;

	if (m_fSeparatorPosFactor > 0.0)
		{
		if (m_SplitDirn == V)
			{
			m_iSeparatorPos = PanelRect.GetHeight()*m_fSeparatorPosFactor + PanelRect.GetOriginY();
			}
		else if (m_SplitDirn == H)
			{
			m_iSeparatorPos = PanelRect.GetWidth()*m_fSeparatorPosFactor + PanelRect.GetOriginX();
			}
		else
			{
			m_iSeparatorPos = -1;
			}
		}
	}

void CSplitContainer::DeletePanel (int iPanelIndex)
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

TArray <IPanel *> CSplitContainer::GetPanelsContainingPoint (int x, int y)
	{
	TArray <IPanel *> aRelevantPanels;

	if (m_pLeafPanel0 != NULL)
		{
		aRelevantPanels.Insert(m_pLeafPanel0->GetPanelsContainingPoint(x, y));
		}
	if (m_pLeafPanel1 != NULL)
		{
		aRelevantPanels.Insert(m_pLeafPanel1->GetPanelsContainingPoint(x, y));
		}

	return aRelevantPanels;
	}

int CSplitContainer::GetPanelIndex (IPanel *pPanel)
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

	return -1;
	}

void CSplitContainer::HidePanel (int iPanelIndex)
	{
	if (iPanelIndex == 0)
		{
		if (m_pLeafPanel0 != NULL)
			{
			m_pLeafPanel0->SetHiddenFlag(true);
			SmoothOut();
			}
		}
	else if (iPanelIndex == 1)
		{
		if (m_pLeafPanel1 != NULL)
			{
			m_pLeafPanel1->SetHiddenFlag(true);
			SmoothOut();
			}
		}
	}

void CSplitContainer::HideAll (void)
	{
	if (m_pLeafPanel0 != NULL)
		{
		m_pLeafPanel0->SetHiddenFlag(true);
		}
	if (m_pLeafPanel1 != NULL)
		{
		m_pLeafPanel1->SetHiddenFlag(true);
		}
	SmoothOut();
	}

void CSplitContainer::ShowPanel (int iPanelIndex)
	{
	if (iPanelIndex == 0)
		{
		m_pLeafPanel0->SetHiddenFlag(false);
		}
	else if (iPanelIndex == 1)
		{
		m_pLeafPanel1->SetHiddenFlag(false);
		}

	SmoothOut();
	}


void CSplitContainer::ShowAll (void)
	{
	if (m_pLeafPanel0 != NULL)
		{
		m_pLeafPanel0->SetHiddenFlag(false);
		}
	if (m_pLeafPanel1 != NULL)
		{
		m_pLeafPanel1->SetHiddenFlag(false);
		}

	SmoothOut();
	}

void CSplitContainer::OnContentPaint(CG32bitImage &Screen, const RECT & rcInvalid)
	{
	PaintSeparator(Screen, rcInvalid);

	if (m_pLeafPanel0 != NULL)
		{
		m_pLeafPanel0->OnPaint(Screen, rcInvalid);
		}

	if (m_pLeafPanel1 != NULL)
		{
		m_pLeafPanel1->OnPaint(Screen, rcInvalid);
		}
	}

void CSplitContainer::PaintSeparator(CG32bitImage & Screen, const RECT & rcInvalid)
	{
	if (m_iSeparatorPos != -1)
		{
		if (m_SplitDirn == V)
			{
			Screen.Fill(PanelRect.GetOriginX(), m_iSeparatorPos, PanelRect.GetWidth(), m_iSeparatorThickness, CG32bitPixel(100, 100, 100));
			}
		else
			{
			Screen.Fill(m_iSeparatorPos, PanelRect.GetOriginY(), m_iSeparatorThickness, PanelRect.GetHeight(), CG32bitPixel(100, 100, 100));
			}
		}
	}



