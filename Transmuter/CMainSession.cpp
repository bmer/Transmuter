//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CLoadingSession::CLoadingSession (CHumanInterface &HI) : IHISession(HI)
//	CLoadingSession constructor
	{
	}

void CLoadingSession::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(64, 64, 64));

	CG32bitPixel TextColor = CG32bitPixel(255, 255, 255);

	const CVisualPalette &VI = g_pHI->GetVisuals();
	const CG16bitFont &font = VI.GetFont(fontConsoleMediumHeavy);

	Screen.DrawText(Screen.GetWidth() / 2, Screen.GetHeight() / 2, font, TextColor, CONSTLIT("Loading..."));
	}

//  ==============================================================================

CMainSession::CMainSession (CHumanInterface &HI, CTransmuterModel &model) : IHISession(HI),
	m_Model(model),
	m_Panel(0, 0, HI.GetScreen().GetWidth(), HI.GetScreen().GetHeight()),
	//	CMainSession constructor
	{
	CPanel *pEmptyPanel = m_Panel.InternalPanels.AddPanel(0, 0, 200, m_Panel.PanelRect.GetHeight(), false);

	CContextPanelContent *pExtensionNavigatorSession = new CContextPanelContent(HI, *pEmptyPanel, m_Model.GetExtensionsArray());
	m_aSubSessions.Insert(pExtensionNavigatorSession);
	}

CMainSession::~CMainSession(void)
	{
	m_aSubSessions.DeleteAll();
	}

void CMainSession::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{															   
	TArray <IPanelContent *> aRelevantContents = m_Panel.GetPanelContentsContainingPoint(x, y);

	for (int i = 0; i < aRelevantContents.GetCount(); i++)
		{
		aRelevantContents[i]->OnLButtonDown(x, y, dwFlags, retbCapture);
		}
	}

void CMainSession::OnLButtonUp(int x, int y, DWORD dwFlags)
	{
	TArray <IPanelContent *> aRelevantContents = m_Panel.GetPanelContentsContainingPoint(x, y);

	for (int i = 0; i < aRelevantContents.GetCount(); i++)
		{
		aRelevantContents[i]->OnLButtonUp(x, y, dwFlags);
		}
	}

void CMainSession::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	TArray <IPanelContent *> aRelevantContents = m_Panel.GetPanelContentsContainingPoint(x, y);

	for (int i = 0; i < aRelevantContents.GetCount(); i++)
		{
		aRelevantContents[i]->OnRButtonDown(x, y, dwFlags);
		}
	}

void CMainSession::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	TArray <IPanelContent *> aRelevantContents = m_Panel.GetPanelContentsContainingPoint(x, y);

	for (int i = 0; i < aRelevantContents.GetCount(); i++)
		{
		aRelevantContents[i]->OnRButtonUp(x, y, dwFlags);
		}
	}

void CMainSession::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//	paint the background GRAY: 32, 32, 32
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(32, 32, 32));

	RECT rcClip;
	//	call paint functions of all subsessions
	for (int i = 0; i < m_aSubSessions.GetCount(); i++)
		{
		CTransmuterPanelContent *pSubSession = m_aSubSessions[i];
		rcClip = (m_aSubSessions[i]->GetAssociatedPanel()).PanelRect.GetAsRect();
		Screen.SetClipRect(rcClip);
		m_aSubSessions[i]->OnPaint(Screen, rcInvalid);
		}
	Screen.ResetClipRect();
	} 