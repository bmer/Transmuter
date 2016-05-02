//	BasicPanelContents.cpp
//
//	Definitions of the basic sessions declared in BasicPanelContents.h
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#include "PreComp.h"

//  =======================================================================

CTransmuterPanel::CTransmuterPanel (CString sPanelName, CHumanInterface &HI, CTransmuterModel &model, int iWidth, int iHeight) : IPanel(sPanelName, HI, iWidth, iHeight),
m_pTitlePanel(NULL),
m_model(model)
	{
	}
CTransmuterPanel::CTransmuterPanel(CString sPanelName, CHumanInterface & HI, CTransmuterModel & model, int iOriginX, int iOriginY, int iWidth, int iHeight) : IPanel(sPanelName, HI, iOriginX, iOriginY, iWidth, iHeight),
m_pTitlePanel(NULL),
m_model(model)
	{
	}
;

void CTransmuterPanel::CreateTitlePanel(CString sPanelName, CString sTitleText, int iRelativeOriginX, int iRelativeOriginY, int iWidth, int iHeight, const CG16bitFont * pFont, CG32bitPixel rgbTextColor, CG32bitPixel rgbBackgroundColor)
	{
	m_pTitlePanel = new CTitlePanel(sPanelName, *g_pHI, iWidth, iHeight, sTitleText);
	PanelOrganizer.PlacePanel(m_pTitlePanel, iRelativeOriginX, iRelativeOriginY);
	}

void CTransmuterPanel::UpdateTitleText(CString sHeaderText)
	{
	if (m_pTitlePanel != NULL)
		{
		m_pTitlePanel->SetText(sHeaderText);
		}
	}

void CTransmuterPanel::UpdateTitleFont(const CG16bitFont *pFont)
	{
	if (m_pTitlePanel != NULL)
		{
		m_pTitlePanel->SetTextFont(pFont);
		}
	}

void CTransmuterPanel::UpdateTitleTextColor(CG32bitPixel rgbColor)
	{
	if (m_pTitlePanel != NULL)
		{
		m_pTitlePanel->SetTextColor(rgbColor);
		}
	}

void CTransmuterPanel::UpdateTitleBackgroundColor(CG32bitPixel rgbColor)
	{
	if (m_pTitlePanel != NULL)
		{
		m_pTitlePanel->SetBackgroundColor(rgbColor);
		}
	}

