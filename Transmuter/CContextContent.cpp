//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"


CContextContent::CContextContent(CString sName, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterPanelContent(sName, HI, AssociatedPanel, model),
	m_ExtensionCollection(m_model.GetExtensionCollection()),
	m_pFont(&((g_pHI->GetVisuals()).GetFont(fontConsoleMediumHeavy))),
	m_rgbFontColor(CG32bitPixel(255, 255, 255)),
	m_Context(m_ExtensionCollection.GetAllExtensions())
	{
	CPanel &refAssociatedPanel = this->GetAssociatedPanel();
	SetHeaderContent(refAssociatedPanel.PanelRect.GetWidth(), 40, CONSTLIT("The Universe"));
	}

CContextContent::~CContextContent (void)
	{
	delete m_HeaderPanelContent;
	}

void CContextContent::SetHeaderContent(int iHeaderWidth, int iHeaderHeight, CString sHeaderText)
	{
	CPanel *pHeaderPanel = this->GetAssociatedPanel().InternalPanels.AddPanel(0, 0, iHeaderWidth, iHeaderHeight, FALSE);
	m_HeaderPanelContent = new CHeaderPanelContent(this->m_sName, m_Context.GetDescription(), *g_pHI, *pHeaderPanel, *this);
	}

void CContextContent::UpdateHeaderContent(CString sHeaderText)
	{
	}

void CContextContent::LoadLastDefinedContextInHistory(void)
	{
	}

void CContextContent::LoadNextDefinedContextInHistory(void)
	{
	}

void CContextContent::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//  may remove panel outlining in future
	DrawPanelOutline(Screen);

	int iBottomOfHeader = 0;
	if (m_HeaderPanelContent != NULL)
		{
		m_HeaderPanelContent->OnPaint(Screen, rcInvalid);
		iBottomOfHeader += m_HeaderPanelContent->GetAssociatedPanel().PanelRect.GetHeight();
		}

	CContextObjectArray *pContextItems = m_Context.GetCurrentContextObjectList();
	int iIndentWidth = 10;
	int iThisPanelLeftEdge = this->GetAssociatedPanel().PanelRect.GetEdgePosition(EDGE_LEFT);
	if (pContextItems != NULL)
		{
		CContextObject *CurrentCtxObj;
		int iCurrentYOffset = 0;
		for (int i = 0; i < pContextItems->GetCount(); i++)
			{
			CurrentCtxObj = pContextItems->GetAt(i);

#ifdef DEBUG
			int iLevel = CurrentCtxObj->GetLevel();
			CString sDisplayText = CurrentCtxObj->GetDisplayText();
			bool bIsParentCollapsed = CurrentCtxObj->IsParentCollapsed();
			bool bCollapseStatus = CurrentCtxObj->GetCollapseStatus();

			if (bIsParentCollapsed == false && iLevel > 0)
				{
				if (CurrentCtxObj->GetParent() != NULL)
					{
					CString sParentDisplayText = CurrentCtxObj->GetParent()->GetDisplayText();
					Screen.DrawText(iThisPanelLeftEdge + (1 + CurrentCtxObj->GetLevel())*iIndentWidth, iBottomOfHeader + iCurrentYOffset, *m_pFont, CG32bitPixel(255, 0, 0), sParentDisplayText);
					iCurrentYOffset += 15;
					}
				}
#endif

			if (CurrentCtxObj->GetLevel() == 0 || CurrentCtxObj->IsParentCollapsed() == false)
				{
				Screen.DrawText(iThisPanelLeftEdge + (1 + CurrentCtxObj->GetLevel())*iIndentWidth, iBottomOfHeader + iCurrentYOffset, *m_pFont, m_rgbFontColor, CurrentCtxObj->GetDisplayText());
				iCurrentYOffset += 15;
				}
			}
		}

	}

//  =======================================================================

CContext::CContext(TArray<CExtension*> AllExtensions) : m_sContextDescription(CONSTLIT("The Universe")),
	m_AllExtensions(AllExtensions),
	m_iCurrentContextIndex(-1)
	{
	ApplyQuery(CONSTLIT("universe"));
	}

CContext::~CContext(void)
	{
	}

void CContext::ApplyQuery (CString sQuery)
	{
	m_aContextObjectListHistory.Insert(DetermineContextObjectList(sQuery));
	m_aQueryHistory.Insert(sQuery);
	m_iCurrentContextIndex += 1;
	}

CContextObjectArray CContext::DetermineContextObjectList(CString sQuery)
	{
	CContextObjectArray NewContextObjectList;

	if (strCompare(sQuery, CONSTLIT("universe")) == 0)
		{
		if (m_aContextObjectListHistory.GetCount() == 0)
			{
			CContextObjectArray  ContextObjListForThisExt;
			for (int i = 0; i < m_AllExtensions.GetCount(); i++)
				{
				ContextObjListForThisExt = CreateContextObjectListForExtension(m_AllExtensions[i]);
				NewContextObjectList.Insert(ContextObjListForThisExt);
				}
			}
		else
			{
			NewContextObjectList = m_aContextObjectListHistory[0];
			}
		}

	return NewContextObjectList;
	}

CContextObjectArray *CContext::GetCurrentContextObjectList (void)
	{
	int iContextCount = m_aContextObjectListHistory.GetCount();
	if (iContextCount > 0)
		{
		return &m_aContextObjectListHistory[m_iCurrentContextIndex];
		}
	else
		{
		return NULL;
		}
	}

void CContext::ChangeToLastContextInHistory (void)
	{
	int iContextCount = m_aContextObjectListHistory.GetCount();
	if (iContextCount > 0)
		{
		if (m_iCurrentContextIndex > 0)
			{
			m_iCurrentContextIndex -= 1;
			}
		}
	}

void CContext::ChangeToNextContextInHistory (void)
	{
	int iContextCount = m_aContextObjectListHistory.GetCount();
	if (iContextCount > 0)
		{
		if (m_iCurrentContextIndex < iContextCount - 1)
			{
			m_iCurrentContextIndex += 1;
			}
		}
	}

void CContext::CleanUpHistory(int iNewHistoryLength)
	{
	TArray <CContextObjectArray> NewContextObjectListHistory;
	TArray <CString> NewQueryHistory;

	int iStartIndex = m_iCurrentContextIndex - iNewHistoryLength - 1;

	//  we always keep the very first element, which corresponds to the entire universe
	NewContextObjectListHistory.Insert(m_aContextObjectListHistory[0]);

	if (iStartIndex > 0)
		{
		for (int i = 0; i < iNewHistoryLength; i++)
			{
			NewContextObjectListHistory.Insert(m_aContextObjectListHistory[iStartIndex + i]);
			}
		}
	else
		{
		if (m_iCurrentContextIndex > 0)
			{
			NewContextObjectListHistory.Insert(m_aContextObjectListHistory[iStartIndex]);
			}
		}
	}

CContextObjectArray CContext::CreateContextObjectListForExtension (CExtension *Extension)
	{
	CContextObjectArray CtxObjList;

	//  top level entry
	CContextObject ExtCtxObj = CContextObject(NULL, Extension->GetName(), 0, true);
	CContextObject *pExtCtxObject = CtxObjList.Insert(ExtCtxObj);
	
	CDesignTable ExtensionDesignTable = Extension->GetDesignTypes();

	for (int i = 0; i < designCount; i++)
		{
		DesignTypes iType = DesignTypes(i);
		CContextObject DesignTypeClassCtxObj = CContextObject(pExtCtxObject, CDesignType::GetTypeClassName(iType), 1, true);
		CContextObject *pDesignTypeClassCtxObj = CtxObjList.Insert(DesignTypeClassCtxObj);

		CDesignType *CurrentDesignType;
		for (int j = 0; j < ExtensionDesignTable.GetCount(); j++)
			{
			CurrentDesignType = ExtensionDesignTable.GetEntry(j);
			if (CurrentDesignType->GetType() == iType)
				{
				CtxObjList.Insert(CContextObject(pDesignTypeClassCtxObj, CurrentDesignType->GetTypeName(), 2, true));
				}
			}
		}

	return CtxObjList;
	}

//  =======================================================================

CContextObject::CContextObject(CContextObject *ParentCtxObj, CString sDisplayText, int iLevel, bool bCollapsed) : m_sDisplayText(sDisplayText),
	m_iLevel(iLevel),
	m_bCollapsed(bCollapsed),
	m_ParentCtxObj(ParentCtxObj)
	{
	}

bool CContextObject::IsParentCollapsed (void)
	{
	if (m_ParentCtxObj != NULL) 
		{ 
		return m_ParentCtxObj->GetCollapseStatus(); 
		}
	else 
		{ 
		return false; 
		}
	};

//CSExtensionMenuItem::CSExtensionMenuItem (CHumanInterface &HI, CPanel &AssociatedPanel, CExtension *Extension) : CTransmuterPanelContent(HI, AssociatedPanel),
//	m_Extension(*Extension)
//	{
//	//  button panels should be sticky
//	CPanel *ButtonPanel = m_AssociatedPanel.InternalPanels.AddPanel(0, 0, 40, m_AssociatedPanel.PanelRect.GetHeight(), false);
//	m_Button = new CButtonSession(HI, *ButtonPanel, 0.8, CG32bitPixel(100, 100, 100));
//	ButtonPanel->AssociateSession(m_Button);
//
//	CPanel *TextPanel = m_AssociatedPanel.InternalPanels.AddPanel(40, 0, m_AssociatedPanel.PanelRect.GetWidth() - 40, m_AssociatedPanel.PanelRect.GetHeight(), false);
//	m_TextArea = new CTextContent(HI, *TextPanel);
//	m_TextArea->SetFontTable(&HI.GetVisuals());
//	m_TextArea->SetFont(&HI.GetVisuals().GetFont(fontConsoleMediumHeavy));
//	TextPanel->AssociateSession(m_TextArea);
//
//	m_TextArea->SetPadding(10);
//	m_TextArea->SetAsRichText(Extension->GetName());
//	m_TextArea->Justify();
//
//	}
//
//CSExtensionMenuItem::~CSExtensionMenuItem(void)
//	{
//	delete m_Button;
//	delete m_TextArea;
//	}
//
//void CSExtensionMenuItem::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
//	{
//	if (!m_AssociatedPanel.IsHidden())
//		{
//		if (m_Button->CheckIfLPressed())
//			{
//			m_AssociatedPanel.Hide();
//			}
//		else
//			{
//			DrawPanelOutline(Screen);
//			m_Button->OnPaint(Screen, rcInvalid);
//			m_TextArea->OnPaint(Screen, rcInvalid);
//			}
//		}
//	}

void CContextObjectArray::Insert(const CContextObjectArray & refCtxObjArray)
	{
	for (int i = 0; i < refCtxObjArray.GetCount(); i++)
		{
		TArray<CContextObject *>::Insert(new CContextObject(*refCtxObjArray[i]));
		}
	}

CContextObject *CContextObjectArray::Insert(const CContextObject &refCtxObj)
	{
	CContextObject *pNewCtxObj = new CContextObject(refCtxObj);
	TArray<CContextObject *>::Insert(pNewCtxObj);
	return pNewCtxObj;
	}

void CContextObjectArray::CleanUp(void)
	{
	for (int i = 0; i < GetCount(); i++)
		{
		delete GetAt(i);
		}
	DeleteAll();
	}

void CContextObjectArray::Copy(const CContextObjectArray &refCtxObjArray)
	{
	InsertEmpty(refCtxObjArray.GetCount());
	for (int i = 0; i < refCtxObjArray.GetCount(); i++)
		{
		GetAt(i) = new CContextObject(*refCtxObjArray[i]);
		}
	}
