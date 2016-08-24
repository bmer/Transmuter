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
	m_Panel(CONSTLIT("Main"), HI, HI.GetScreen().GetWidth(), HI.GetScreen().GetHeight()),
	m_pCapture(NULL),
	m_iSeparatorWidth(5)
	//	CMainSession constructor
	{
	// main background panel is m_Panel

	int iContextPanelWidth = 300;
	int iContextPanelHeight = 600;

	// Initializing ContextEditor panel
	m_pContextEditorPanel = new CSplitContainer(CONSTLIT("ContextEditor"), HI, m_Panel.PanelRect.GetWidth(), iContextPanelHeight);
	m_Panel.PlacePanel(m_pContextEditorPanel, 'h', 0);

	//  Initializing context panel
	m_pContextPanel = new CContextPanel(CONSTLIT("Context"), HI, m_Model, iContextPanelWidth, iContextPanelHeight);
	m_pContextEditorPanel->PlacePanel(m_pContextPanel, 'v', 0);
	m_aPanels.Insert(m_pContextPanel);

	//  Initializing command line interface
	int iCommandPanelWidth = m_Panel.PanelRect.GetWidth();
	int iCommandPanelHeight = m_Panel.PanelRect.GetHeight() - iContextPanelHeight;
	m_pCommandPanel = new CCommandPanel(CONSTLIT("CLI"), HI, m_Model, iCommandPanelWidth, iCommandPanelHeight);
	m_Panel.PlacePanel(m_pCommandPanel, 'h', 1);
	m_aPanels.Insert(m_pCommandPanel);
	}

CMainSession::~CMainSession(void)
	{
	//for (int i = 0; i < m_aPanels.GetCount(); i++)
	//	{
	//	delete m_aPanels[i];
	//	}
	delete m_pContextPanel;
	delete m_pCommandPanel;
	}

ALERROR CMainSession::OnCommand(const CString &sCmd, void *pData)
	{
	CTextContent *pOutputContent = m_pCommandPanel->GetOutputContent();
	pOutputContent->WriteText(strCat(">> ", sCmd));

	TArray <CString> aParsedCmd = CCommandParser::ParseStr(sCmd);
	CString *pCommandToken = &aParsedCmd[0];

	if (strEquals(*pCommandToken, CONSTLIT("open")))
		{										                 
		// m_pContextPanel->m_Context.ApplyQuery(aParsedCmd[1]);
		}
	else
		{
		pOutputContent->WriteText("unknown command\n");
		}
	return ALERROR();
	}

void CMainSession::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{
	TArray<IPanel *> aRelevantPanels = m_Panel.GetPanelsContainingPoint(x, y);

	if (aRelevantPanels.GetCount() != 0)
		{
		IPanel *pRelevantPanel = aRelevantPanels[aRelevantPanels.GetCount() - 1];
		if (pRelevantPanel->GetCaptureStatus())
			{
			m_pCapture = pRelevantPanel;
			}
		pRelevantPanel->OnLButtonDown(x, y, dwFlags, retbCapture);
		}
	}

void CMainSession::OnLButtonUp(int x, int y, DWORD dwFlags)
	{
	if (m_pCapture != NULL)
		{
		m_pCapture->OnLButtonUp(x, y, dwFlags);
		m_pCapture = NULL;
		}
	else
		{
		TArray<IPanel *> aRelevantPanels = m_Panel.GetPanelsContainingPoint(x, y);

		if (aRelevantPanels.GetCount() != 0)
			{
			IPanel *pRelevantPanel = aRelevantPanels[aRelevantPanels.GetCount() - 1];
			pRelevantPanel->OnLButtonUp(x, y, dwFlags);

			if (pRelevantPanel->IsLClicked())
				{
				if (m_pFocusPanel != NULL)
					{
					m_pFocusPanel->RemoveFocus();
					}
				m_pFocusPanel = pRelevantPanel->SetFocus();
				}
			}
		}
	}

void CMainSession::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	TArray<IPanel *> aRelevantPanels = m_Panel.GetPanelsContainingPoint(x, y);

	if (aRelevantPanels.GetCount() != NULL)
		{
		IPanel *pRelevantPanel = aRelevantPanels[aRelevantPanels.GetCount() - 1];
		pRelevantPanel->OnRButtonDown(x, y, dwFlags);
		}
	}

void CMainSession::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	TArray <IPanel *> aRelevantPanels = m_Panel.GetPanelsContainingPoint(x, y);

	if (aRelevantPanels.GetCount() != 0)
		{
		IPanel *pRelevantPanel = aRelevantPanels[aRelevantPanels.GetCount() - 1];

		pRelevantPanel->OnRButtonUp(x, y, dwFlags);

		if (pRelevantPanel->IsRClicked())
			{
			if (m_pFocusPanel != NULL)
				{
				m_pFocusPanel->RemoveFocus();
				}
			m_pFocusPanel = pRelevantPanel->SetFocus();
			}
		}
	}

void CMainSession::OnKeyDown(int iVirtKey, DWORD dwKeyData)
	{
	if (m_pFocusPanel != NULL)
		{
		m_pFocusPanel->OnKeyDown(iVirtKey, dwKeyData);
		}
	}

void CMainSession::OnKeyUp(int iVirtKey, DWORD dwKeyData)
	{
	if (m_pFocusPanel != NULL)
		{
		m_pFocusPanel->OnKeyUp(iVirtKey, dwKeyData);
		}
	}

void CMainSession::OnChar(char chChar, DWORD dwKeyData)
	{
	if (m_pFocusPanel != NULL)
		{
		m_pFocusPanel->OnChar(chChar, dwKeyData);
		}
	}

void CMainSession::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//	paint the background GRAY: 32, 32, 32
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(32, 32, 32));

	RECT rcClip;
	//	call paint functions of all subsessions
	for (int i = 0; i < m_aPanels.GetCount(); i++)
		{
		IPanel *pPanel = m_aPanels[i];
		rcClip = pPanel->PanelRect.GetAsRect();
		Screen.SetClipRect(rcClip);
		// should only be called if bOverlay
		pPanel->OnPaint(Screen, rcInvalid);
		}
	Screen.ResetClipRect();

	// now do overlay stuff	 -- loop again, but call OnPaint if bOverlay == True
	} 