//	CArrayContainer.cpp
//
//	Copyright (c) 2016 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"
#include "CArrayContainer.h"

CSplitContainer::CSplitContainer(CString sName, CHumanInterface &HI, int iWidth, int iHeight) : CContainer(sName, HI, iWidth, iHeight)
	{
	}

void CSplitContainer::SmoothOut (void)
	{
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
			if (m_cSplitDirn == 'h')
				{
				m_iSeparatorPos = int(m_fSeparatorPosFactor*PanelRect.GetWidth());

				m_pLeafPanel0->PanelRect.SetOrigin(PanelRect.GetOriginX(), PanelRect.GetOriginY());
				m_pLeafPanel0->PanelRect.SetWidth(PanelRect.GetWidth() - m_iSeparatorPos);
				m_pLeafPanel0->PanelRect.SetHeight(PanelRect.GetHeight());

				m_pLeafPanel1->PanelRect.SetOrigin(m_iSeparatorPos + m_iSeparatorThickness, PanelRect.GetOriginY());
				m_pLeafPanel1->PanelRect.SetWidth(PanelRect.GetWidth() - m_pLeafPanel0->PanelRect.GetWidth() - m_iSeparatorThickness);
				m_pLeafPanel1->PanelRect.SetHeight(PanelRect.GetHeight());
				}
			else if (m_cSplitDirn == 'v')
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

bool CSplitContainer::PlacePanel (IPanel *pPanel, char cSplitDirn, int iInsertIndex)
	{
	if (cSplitDirn != m_cSplitDirn)
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



