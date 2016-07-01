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
	m_pCapture(NULL),
	m_iSeparatorWidth(5)
	//	CMainSession constructor
	{
	int iContextPanelWidth = 300;
	int iContextPanelHeight = 600;

	// slider -- should have a parent

	// main background panel is m_Panel

	//  Initializing context panel
	IPanel *pEmptyPanelForContextContent = m_Panel.InternalPanels.AddPanel(0, 0, iContextPanelWidth, iContextPanelHeight, false);
	m_pContextPanelContent = new CContextContent(CONSTLIT("Context"), HI, *pEmptyPanelForContextContent, m_Model);
	m_aContent.Insert(m_pContextPanelContent);

	IPanel *pEmptyPanelForContextEditorSeparator = m_Panel.InternalPanels.AddPanel(iContextPanelWidth, 0, 10, iContextPanelHeight, false);
	m_pContextEditorSeparatorContent = new CSeparatorContent(CONSTLIT("ConextSeparator"), HI, *pEmptyPanelForContextEditorSeparator, true);
	m_aContent.Insert(m_pContextEditorSeparatorContent);
	m_pContextPanelContent->SetRightSeparator(m_pContextEditorSeparatorContent);

	int iCommandLineWidth = m_Panel.PanelRect.GetWidth();
	int iCommandLineHeight = m_Panel.PanelRect.GetHeight() - iContextPanelHeight - 10;

	IPanel *pEmptyPanelForCLISeparator = m_Panel.InternalPanels.AddPanel(0, iContextPanelHeight, iCommandLineWidth, 10, false);
	m_pCLISeparator = new CSeparatorContent(CONSTLIT("CLISeparator"), HI, *pEmptyPanelForCLISeparator, false);
	m_aContent.Insert(m_pCLISeparator);

	//  Initializing command line interface
	
	IPanel *pEmptyPanelForCLI = m_Panel.InternalPanels.AddPanel(0, iContextPanelHeight + 10, iCommandLineWidth, iCommandLineHeight, false);
	m_pCommandInterfaceContent = new CCommandInterfaceContent(CONSTLIT("Command Line Interface"), HI, *pEmptyPanelForCLI, m_Model);
	m_aContent.Insert(m_pCommandInterfaceContent);
	m_pCommandInterfaceContent->SetTopSeparator(m_pCLISeparator);
	m_pContextPanelContent->SetBottomSeparator(m_pCLISeparator);

	CTextContent *pInputContent = m_pCommandInterfaceContent->GetInputContent();
	pInputContent->SetController(this);

	// Initializing text editor 
	}

CMainSession::~CMainSession(void)
	{
	//for (int i = 0; i < m_aContent.GetCount(); i++)
	//	{
	//	delete m_aContent[i];
	//	}
	delete m_pContextPanelContent;
	delete m_pCommandInterfaceContent;
	}

ALERROR CMainSession::OnCommand(const CString &sCmd, void *pData)
	{
	CTextContent *pOutputContent = m_pCommandInterfaceContent->GetOutputContent();
	pOutputContent->WriteText(strCat(">> ", sCmd));

	TArray <CString> aParsedCmd = CCommandParser::ParseStr(sCmd);
	CString *pCommandToken = &aParsedCmd[0];

	if (strEquals(*pCommandToken, CONSTLIT("open")))
		{										                 
		// m_pContextPanelContent->m_Context.ApplyQuery(aParsedCmd[1]);
		}
	else
		{
		pOutputContent->WriteText("unknown command\n");
		}
	return ALERROR();
	}

void CMainSession::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{
	IPanelContent *pRelevantContent = m_Panel.GetContentContainingPoint(x, y);

	if (pRelevantContent != NULL)
		{
		if (pRelevantContent->GetCaptureStatus())
			{
			m_pCapture = pRelevantContent;
			}
		pRelevantContent->OnLButtonDown(x, y, dwFlags, retbCapture);
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
		IPanelContent *pRelevantContent = m_Panel.GetContentContainingPoint(x, y);

		if (pRelevantContent != NULL)
			{
			pRelevantContent->OnLButtonUp(x, y, dwFlags);

			if (pRelevantContent->IsLClicked())
				{
				if (m_pFocusContent != NULL)
					{
					m_pFocusContent->RemoveFocus();
					}
				m_pFocusContent = pRelevantContent->SetFocus();
				}
			}
		}
	}

void CMainSession::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	IPanelContent *pRelevantContent = m_Panel.GetContentContainingPoint(x, y);

	if (pRelevantContent != NULL)
		{
		pRelevantContent->OnRButtonDown(x, y, dwFlags);
		}
	}

void CMainSession::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	IPanelContent *pRelevantContent = m_Panel.GetContentContainingPoint(x, y);

	if (pRelevantContent != NULL)
		{
		pRelevantContent->OnRButtonUp(x, y, dwFlags);

		if (pRelevantContent->IsRClicked())
			{
			if (m_pFocusContent != NULL)
				{
				m_pFocusContent->RemoveFocus();
				}
			m_pFocusContent = pRelevantContent->SetFocus();
			}
		}
	}

void CMainSession::OnKeyDown(int iVirtKey, DWORD dwKeyData)
	{
	if (m_pFocusContent != NULL)
		{
		m_pFocusContent->OnKeyDown(iVirtKey, dwKeyData);
		}
	}

void CMainSession::OnKeyUp(int iVirtKey, DWORD dwKeyData)
	{
	if (m_pFocusContent != NULL)
		{
		m_pFocusContent->OnKeyUp(iVirtKey, dwKeyData);
		}
	}

void CMainSession::OnChar(char chChar, DWORD dwKeyData)
	{
	if (m_pFocusContent != NULL)
		{
		m_pFocusContent->OnChar(chChar, dwKeyData);
		}
	}

void CMainSession::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//	paint the background GRAY: 32, 32, 32
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(32, 32, 32));

	RECT rcClip;
	//	call paint functions of all subsessions
	for (int i = 0; i < m_aContent.GetCount(); i++)
		{
		IPanelContent *pContent = m_aContent[i];
		rcClip = (pContent->GetAssociatedPanel()).PanelRect.GetAsRect();
		Screen.SetClipRect(rcClip);
		// should only be called if bOverlay
		pContent->UpdateEdgePositionsFromSeparators();
		pContent->OnPaint(Screen, rcInvalid);
		}
	Screen.ResetClipRect();

	// now do overlay stuff	 -- loop again, but call OnPaint if bOverlay == True
	} 