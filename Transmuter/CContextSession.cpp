//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"


CContextPanelContent::CContextPanelContent(CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterPanelContent(CONSTLIT("context view"), HI, AssociatedPanel, model),
	m_ExtensionCollection(m_model.GetExtensionCollection()),
	m_defaultContext(m_ExtensionCollection.GetAllExtensions())
	{
	CPanel &refAssociatedPanel = this->GetAssociatedPanel();
	SetHeaderContent(refAssociatedPanel.PanelRect.GetWidth(), refAssociatedPanel.PanelRect.GetHeight(), CONSTLIT("The Universe"));
	}

CContextPanelContent::~CContextPanelContent (void)
	{
	delete &m_defaultContext;
	m_definedContexts.DeleteAll();
	delete m_HeaderPanelContent;
	}

void CContext::CreateContextItemSessions (CContextPanelContent &refContextSession)
	{
	int iItemWidth = refContextSession.GetAssociatedPanel().PanelRect.GetWidth();
	int iItemHeight = 40;
	CPanel *pNewItemPanel;
	CContextItemSession *pNewCtxItemSessn;
	CExtension *pExtension;

	if (refContextSession.GetHeaderPanelContent() == NULL)
		{
		refContextSession.SetHeaderContent(refContextSession.GetAssociatedPanel().PanelRect.GetWidth(), 40, m_sContextDescription);
		}

	int iItemYOffset = refContextSession.GetHeaderPanelContent.GetAssociatedPanel().PanelRect.GetHeight();
	
	int iNumExtensions = m_aExtensions.GetCount();
	for (int i = 0; i < iNumExtensions; i++)
		{
		pExtension = m_aExtensions[i];
		pNewItemPanel = refContextSession.GetAssociatedPanel().InternalPanels.AddPanel(0, iItemYOffset, iItemWidth, iItemHeight, FALSE);
		pNewCtxItemSessn = new CContextItemSession(pExtension->GetName(), *g_pHI, *pNewItemPanel, refContextSession.GetModel());
		m_aContextItemSessions.Insert(pNewCtxItemSessn);
		iItemYOffset += iItemHeight;
		};
	}

void CContextPanelContent::SetHeaderContent(int headerWidth, int headerHeight, CString headerText)
	{
	int iHeaderWidth = this->GetAssociatedPanel().PanelRect.GetWidth();

	CPanel *pHeaderPanel = this->GetAssociatedPanel().InternalPanels.AddPanel(0, 0, iHeaderWidth, 40, FALSE);
	m_HeaderPanelContent = new CHeaderPanelContent(m_currentContext->GetContextDescription(), *g_pHI, *pHeaderPanel, *this);
	}

void CContextPanelContent::UpdateHeaderContent(CString headerText)
	{
	}

void CContextPanelContent::LoadDefinedContext(void)
	{
	}

void CContextPanelContent::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//  may remove panel outlining in future
	DrawPanelOutline(Screen);

	if (m_HeaderPanelContent != NULL)
		{
		m_HeaderPanelContent->OnPaint(Screen, rcInvalid);
		}

	m_currentContext->Paint(Screen, rcInvalid);
	}

//  =======================================================================


CSExtensionDetails::CSExtensionDetails (CHumanInterface &HI, CPanel &AssociatedPanel) : CTransmuterPanelContent(HI, AssociatedPanel)
	{
	}


//  =======================================================================

CSExtensionMenuItem::CSExtensionMenuItem (CHumanInterface &HI, CPanel &AssociatedPanel, CExtension *Extension) : CTransmuterPanelContent(HI, AssociatedPanel),
	m_Extension(*Extension)
	{
	//  button panels should be sticky
	CPanel *ButtonPanel = m_AssociatedPanel.InternalPanels.AddPanel(0, 0, 40, m_AssociatedPanel.PanelRect.GetHeight(), false);
	m_Button = new CButtonSession(HI, *ButtonPanel, 0.8, CG32bitPixel(100, 100, 100));
	ButtonPanel->AssociateSession(m_Button);

	CPanel *TextPanel = m_AssociatedPanel.InternalPanels.AddPanel(40, 0, m_AssociatedPanel.PanelRect.GetWidth() - 40, m_AssociatedPanel.PanelRect.GetHeight(), false);
	m_TextArea = new CTextAreaPanelContent(HI, *TextPanel);
	m_TextArea->SetFontTable(&HI.GetVisuals());
	m_TextArea->SetFont(&HI.GetVisuals().GetFont(fontConsoleMediumHeavy));
	TextPanel->AssociateSession(m_TextArea);

	m_TextArea->SetPadding(10);
	m_TextArea->SetAsRichText(Extension->GetName());
	m_TextArea->Justify();

	}

CSExtensionMenuItem::~CSExtensionMenuItem(void)
	{
	delete m_Button;
	delete m_TextArea;
	}

void CSExtensionMenuItem::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	if (!m_AssociatedPanel.IsHidden())
		{
		if (m_Button->CheckIfLPressed())
			{
			m_AssociatedPanel.Hide();
			}
		else
			{
			DrawPanelOutline(Screen);
			m_Button->OnPaint(Screen, rcInvalid);
			m_TextArea->OnPaint(Screen, rcInvalid);
			}
		}
	}

CContextItemSession::CContextItemSession(CString sExtensionName, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterPanelContent(sExtensionName, HI, AssociatedPanel, model)
	{
	}
