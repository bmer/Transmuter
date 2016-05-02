//	CMainSession.cpp
//
//	CMainSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CContextEntry::CContextEntry(CContextEntry *pParentEntry, CString sDisplayText, int iLevel, bool bCollapsed, DWORD dwUNID) : m_pParentEntry(pParentEntry),
	m_sDisplayText(sDisplayText),
	m_iLevel(iLevel),
	m_bCollapsed(bCollapsed),
	m_dwUNID(dwUNID)
	{
	m_sUNID = strPatternSubst("0x%08x", m_dwUNID);
	}

// ===============================================================

bool CContextEntry::IsParentCollapsed(void)
	{
	if (m_pParentEntry != NULL)
		{
		return m_pParentEntry->GetCollapseStatus();
		}
	else
		{
		return false;
		}
	}

// ===============================================================

CContextEntryContent::CContextEntryContent(CString sID, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model, CContextEntry &refAssociatedEntry) : CTransmuterContent(sID, HI, AssociatedPanel, model),
	m_refAssociatedEntry(refAssociatedEntry)
	{
	}

// ===============================================================

void CContextEntryArray::Insert(const CContextEntryArray & refCtxObjArray)
	{
	for (int i = 0; i < refCtxObjArray.GetCount(); i++)
		{
		TArray<CContextEntry *>::Insert(new CContextEntry(*refCtxObjArray[i]));
		}
	}

// ===============================================================

CContextEntry *CContextEntryArray::Insert(const CContextEntry &refCtxObj)
	{
	CContextEntry *pNewCtxObj = new CContextEntry(refCtxObj);
	TArray<CContextEntry *>::Insert(pNewCtxObj);
	return pNewCtxObj;
	}

// ===============================================================

void CContextEntryArray::CleanUp(void)
	{
	for (int i = 0; i < GetCount(); i++)
		{
		delete GetAt(i);
		}
	DeleteAll();
	}

// ===============================================================

void CContextEntryArray::Copy(const CContextEntryArray &refCtxObjArray)
	{
	InsertEmpty(refCtxObjArray.GetCount());
	for (int i = 0; i < refCtxObjArray.GetCount(); i++)
		{
		GetAt(i) = new CContextEntry(*refCtxObjArray[i]);
		}
	}

// ===============================================================

CContextContent::CContextContent(CString sID, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterContent(sID, HI, AssociatedPanel, model),
	m_ExtensionCollection(m_model.GetExtensionCollection()),
	m_pFont(&((g_pHI->GetVisuals()).GetFont(fontConsoleMediumHeavy))),
	m_rgbFontColor(CG32bitPixel(255, 255, 255)),
	m_Contextualizer(CContextualizer(HI, m_ExtensionCollection.GetAllExtensions()))
	{
	CPanel &refAssociatedPanel = this->GetAssociatedPanel();
	SetHeaderContent(strCat(m_sID, CONSTLIT(".h")), CONSTLIT("Context"), refAssociatedPanel.PanelRect.GetWidth(), 40);
	}

// ===============================================================

CContextContent::~CContextContent (void)
	{
	delete m_pHeaderContent;
	}

// ===============================================================

void CContextContent::LoadContext (void)
	{

	CPanel &refAssociatedPanel = GetAssociatedPanel();
	// delete all existing internal panels dealing with content
	for (int i = 0; i < m_bLoadedContextObjectPanelIndices.GetCount(); i++)
		{
		//// first we must take care to delete the associated content!
		//iThisPanelIndex = m_bLoadedContextObjectPanelIndices[i];
		//delete refAssociatedPanel.InternalPanels.GetPanel(iThisPanelIndex)->GetAssociatedSession();

		// now we can delete the panel itself
		refAssociatedPanel.InternalPanels.DeletePanel(i);
		}
	// deleting all the old stored panel indices
	m_bLoadedContextObjectPanelIndices.DeleteAll();

	CContextEntryArray *pContextEntryList = m_Contextualizer.GetCurrentContextEntries();

	int iBottomOfHeader = 0;
	if (m_pHeaderContent != NULL)
		{
		iBottomOfHeader += m_pHeaderContent->GetAssociatedPanel().PanelRect.GetHeight();
		}

	int iEntryHeight = 15;
	int iIndentWidth = 10;

	int iParentPanelWidth = refAssociatedPanel.PanelRect.GetWidth();
	int iNumEntries = pContextEntryList->GetCount();
	m_bLoadedContextObjectPanelIndices.InsertEmpty(iNumEntries);
	CPanel *pEntryPanel;

	for (int i = 0; i < iNumEntries; i++)
		{
		// create new internal panels, content
		CContextEntry *pEntry = pContextEntryList->GetAt(i);
		pEntryPanel = refAssociatedPanel.InternalPanels.AddPanel(0, iBottomOfHeader + i*iEntryHeight, iParentPanelWidth, iEntryHeight, false);
		m_bLoadedContextObjectPanelIndices[i] = refAssociatedPanel.InternalPanels.GetPanelIndex(pEntryPanel);

		CString sToConcatenate = strCat(CONSTLIT("_"), pEntry->GetUNIDAsString());
		pEntryPanel->AssociateSession(new CContextEntryContent(strCat(GetStringID(), sToConcatenate), GetHI(), *pEntryPanel, GetModel(), *pEntry));
		}
	}

// ===============================================================

void CContextContent::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	bool bFocusStatus = GetFocusStatus();
	if (GetFocusStatus() == true)
		{
		UpdatePanelOutlineColor(CG32bitPixel(255, 0, 0));
		}
	else
		{
		UpdatePanelOutlineColor(CG32bitPixel(255, 255, 255));
		}
	DrawPanelOutline(Screen);/*

	int iBottomOfHeader = 0;
	if (m_pHeaderContent != NULL)
		{
		m_pHeaderContent->OnPaint(Screen, rcInvalid);
		iBottomOfHeader += m_pHeaderContent->GetAssociatedPanel().PanelRect.GetHeight();
		}

	CContextEntryArray *pContextEntries = m_Contextualizer.GetCurrentContextEntries();
	int iIndentWidth = 10;
	int iThisPanelLeftEdge = this->GetAssociatedPanel().PanelRect.GetEdgePosition(EDGE_LEFT);
	if (pContextEntries != NULL)
		{
		CContextEntry *CurrentEntry;
		int iCurrentYOffset = 0;
		for (int i = 0; i < pContextEntries->GetCount(); i++)
			{
			CurrentEntry = pContextEntries->GetAt(i);

#ifdef DEBUG
			int iLevel = CurrentEntry->GetLevel();
			CString sDisplayText = CurrentEntry->GetDisplayText();
			bool bIsParentCollapsed = CurrentEntry->IsParentCollapsed();
			bool bCollapseStatus = CurrentEntry->GetCollapseStatus();

			if (bIsParentCollapsed == false && iLevel > 0)
				{
				if (CurrentEntry->GetParent() != NULL)
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
*/
	}

//  =======================================================================

CContextualizer::CContextualizer(CHumanInterface &HI, TArray<CExtension*> AllExtensions) : m_sContextDescription(CONSTLIT("The Universe")),
	m_AllExtensions(AllExtensions),
	m_iCurrentContextIndex(-1),
	m_HI(HI)
	{
	ApplyQuery(CONSTLIT("universe"));
	}

CContextualizer::~CContextualizer(void)
	{
	}

void CContextualizer::ApplyQuery (CString sQuery)
	{
	m_aContextObjectListHistory.Insert(DetermineContextEntries(sQuery));
	m_aQueryHistory.Insert(sQuery);
	m_iCurrentContextIndex += 1;
	}

CContextEntryArray CContextualizer::DetermineContextEntries(CString sQuery)
	{
	CContextEntryArray NewContextObjectList;

	if (strCompare(sQuery, CONSTLIT("universe")) == 0)
		{
		if (m_aContextObjectListHistory.GetCount() == 0)
			{
			CContextEntryArray  ContextObjListForThisExt;
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

CContextEntryArray *CContextualizer::GetCurrentContextEntries (void)
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

void CContextualizer::ChangeToLastContextInHistory (void)
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

void CContextualizer::ChangeToNextContextInHistory (void)
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

void CContextualizer::CleanUpHistory(int iNewHistoryLength)
	{
	TArray <CContextEntryArray> NewContextObjectListHistory;
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

CContextEntryArray CContextualizer::CreateContextObjectListForExtension (CExtension *Extension)
	{
	CContextEntryArray ContextEntries;

	if (Extension->GetUNID() != 0)
		{
		//  top level entry
		CString sDisplayString = Extension->GetName();
		DWORD dwExtensionUNID = Extension->GetUNID();
		if (sDisplayString.GetLength() == 0)
			{
			sDisplayString = strPatternSubst("0x%08x", dwExtensionUNID);
			}

		CContextEntry ExtensionContextEntry = CContextEntry(NULL, sDisplayString, 0, true, dwExtensionUNID);
		CContextEntry *pExtensionEntry = ContextEntries.Insert(ExtensionContextEntry);

		CDesignTable ExtensionDesignTable = Extension->GetDesignTypes();

		for (int i = 0; i < designCount; i++)
			{
			DesignTypes iType = DesignTypes(i);
			CContextEntry DesignTypeEntry = CContextEntry(pExtensionEntry, CDesignType::GetTypeClassName(iType), 1, true, -1);
			CContextEntry *pDesignTypeEntry = ContextEntries.Insert(DesignTypeEntry);

			CDesignType *CurrentDesignType;
			for (int j = 0; j < ExtensionDesignTable.GetCount(); j++)
				{
				CurrentDesignType = ExtensionDesignTable.GetEntry(j);
				if (CurrentDesignType->GetType() == iType)
					{
					ContextEntries.Insert(CContextEntry(pDesignTypeEntry, CurrentDesignType->GetTypeName(), 2, true, CurrentDesignType->GetUNID()));
					}
				}
			}
		}

	return ContextEntries;
	}

//  =======================================================================