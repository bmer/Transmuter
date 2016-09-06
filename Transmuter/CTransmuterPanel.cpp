//	BasicPanelContents.cpp
//
//	Definitions of the basic sessions declared in BasicPanelContents.h
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#include "PreComp.h"

//  =======================================================================

CTransmuterPanel::CTransmuterPanel (CString sName, CHumanInterface &HI, CTransmuterModel &model, int iWidth, int iHeight) : CFloatContainer(sName, HI, iWidth, iHeight),
m_pTitlePanel(NULL),
m_Model(model)
	{
	}

void CTransmuterPanel::CreateTitlePanel(CString sName, CString sTitleText, int iRelativeOriginX, int iRelativeOriginY, const CG16bitFont * pFont, CG32bitPixel rgbTextColor, CG32bitPixel rgbBackgroundColor)
	{
	m_pTitlePanel = new CTitlePanel(sName, *g_pHI, PanelRect.GetWidth(), 40, sTitleText);
	PlacePanel(m_pTitlePanel, iRelativeOriginX, iRelativeOriginY);
	}

void CTransmuterPanel::SetTitleWidth(int iWidth)
	{
	if (m_pTitlePanel != NULL)
		{
		m_pTitlePanel->PanelRect.SetWidth(iWidth);
		}
	}

void CTransmuterPanel::SetTitleHeight(int iHeight)
	{
	if (m_pTitlePanel != NULL)
		{
		m_pTitlePanel->PanelRect.SetHeight(iHeight);
		}
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

