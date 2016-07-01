//	CContextPanel.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once
class CContextEntry;
class CContextEntryArray;
class CContextEntryPanel;
class CContext;
class CContextPanel;

class CContextEntry
	{
	public:
		CContextEntry (CContextEntry *pParentEntry, CString sDisplayText, int iLevel, bool bCollapsed, DWORD dwUNID);

		inline void SetLevel (int iLevel) { m_iLevel = iLevel; }
		inline int GetLevel (void) { return m_iLevel; }

		inline void SetCollapseStatus (bool bCollapsed) { m_bCollapsed = bCollapsed; }
		inline bool GetCollapseStatus (void) { return m_bCollapsed; }
		bool IsParentCollapsed (void);

		inline CContextEntry *GetParent (void) { return m_pParentEntry; }

		inline CString GetDisplayText (void) { return m_sDisplayText; }
		inline DWORD GetUNID (void) { return m_dwUNID; }
		inline CString GetUNIDAsString (void) { return m_sUNID; }
		

	private:
		CContextEntry *m_pParentEntry;
		CString m_sDisplayText;
		int m_iLevel;
		bool m_bCollapsed;
		DWORD m_dwUNID;
		CString m_sUNID;
	};

//  =======================================================================

class CContextEntryPanel : public CTransmuterPanel
	{
	public:
		CContextEntryPanel (CString sID, CHumanInterface &HI, IPanel &AssociatedPanel, CTransmuterModel &model, CContextEntry &refAssociatedEntry);

		inline CContextEntry &GetAssociatedEntry (void) { return m_refAssociatedEntry; }
	private:					  
		CContextEntry &m_refAssociatedEntry;
	};

//  =======================================================================

class CContextEntryArray : public TArray <CContextEntry *>
	{
	public:
		inline CContextEntryArray (void) { ; }
		inline CContextEntryArray (const CContextEntryArray &refCtxEntryArray) { Copy(refCtxEntryArray); }
		inline ~CContextEntryArray (void) { CleanUp(); }

		void Insert (const CContextEntryArray &refCtxObjArray);
		CContextEntry *Insert (const CContextEntry &refCtxObj);
		inline CContextEntryArray &operator=(const CContextEntryArray &refCtxEntryArray) { CleanUp(); Copy(refCtxEntryArray); return *this; }

	private:
		void CleanUp (void);
		void Copy (const CContextEntryArray &refCtxEntryArray);
	};

//  =======================================================================

class CContextualizer
	{
	public:
		CContextualizer (CHumanInterface &HI, TArray<CExtension*> AllExtensions);
		~CContextualizer (void);

		void ApplyQuery (CString sQuery);

		inline CString GetDescription (void) { return m_sContextDescription; }

		CContextEntryArray DetermineContextEntries (CString sQuery);
		CContextEntryArray *GetCurrentContextEntries (void);
		void ChangeToLastContextInHistory (void);
		void ChangeToNextContextInHistory (void);

		void CleanUpHistory (int iNewHistoryLength);

	private:
		CHumanInterface &m_HI;
		inline void SetDescription (CString sContextDescription) { m_sContextDescription = sContextDescription; }
		CContextEntryArray CreateContextObjectListForExtension (CExtension *Extension);
		CString m_sContextDescription;
		TArray <CExtension *> &m_AllExtensions;
		
		int m_iCurrentContextIndex = 0;
		TArray <CString> m_aQueryHistory;
		TArray <CContextEntryArray> m_aContextObjectListHistory;
	};

//  =======================================================================

class CContextPanel : public CTransmuterPanel
	{
	private:
		CExtensionCollection &m_ExtensionCollection;
		const CG16bitFont *m_pFont;
		CG32bitPixel m_rgbFontColor;
		TArray <int> m_bLoadedContextObjectPanelIndices;

	public:
		CContextPanel (CString sID, CHumanInterface &HI, IPanel &AssociatedPanel, CTransmuterModel &model);
		~CContextPanel (void);
		
		CContextualizer m_Contextualizer;

		void LoadContext (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
	};

//  =======================================================================

