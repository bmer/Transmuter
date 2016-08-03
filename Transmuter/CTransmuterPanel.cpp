//	BasicPanelContents.cpp
//
//	Definitions of the basic sessions declared in BasicPanelContents.h
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#include "PreComp.h"

//  =======================================================================

CTransmuterPanel::CTransmuterPanel (CString sName, CHumanInterface &HI, CTransmuterModel &model, int iWidth, int iHeight) : IPanel(sName, HI, iWidth, iHeight),
m_pTitlePanel(NULL),
m_Model(model)
	{
	}
CTransmuterPanel::CTransmuterPanel(CString sName, CHumanInterface & HI, CTransmuterModel & model, int iOriginX, int iOriginY, int iWidth, int iHeight) : IPanel(sName, HI, iOriginX, iOriginY, iWidth, iHeight),
m_pTitlePanel(NULL),
m_Model(model)
	{
	}
;

void CTransmuterPanel::CreateTitlePanel(CString sName, CString sTitleText, int iRelativeOriginX, int iRelativeOriginY, int iWidth, int iHeight, const CG16bitFont * pFont, CG32bitPixel rgbTextColor, CG32bitPixel rgbBackgroundColor)
	{
	m_pTitlePanel = new CTitlePanel(sName, *g_pHI, iWidth, iHeight, sTitleText);
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

