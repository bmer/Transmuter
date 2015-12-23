//	CContextPanelContent.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CContextPanelContent;
class CContext;
class CContextObject;

//  =======================================================================

class CContextPanelContent : public CTransmuterPanelContent
	{
	public:
		CContextPanelContent (CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
		~CContextPanelContent (void);
		
		void ApplyContextFilters (TArray <CString> aFilters);

		void SetHeaderContent(int headerWidth, int headerHeight, CString headerText);
		void UpdateHeaderContent(CString headerText);

		void LoadLastDefinedContextInHistory (void);
		void LoadNextDefinedContextInHistory (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CContext m_Context;
		CExtensionCollection &m_ExtensionCollection;
	};

//  =======================================================================

class CContextObject
	{
	public:
		CContextObject (CString sDisplayText);

		inline void SetLevel (int iLevel) { m_iLevel = iLevel; }
		inline int GetLevel (void) { return m_iLevel;  }

		inline void SetCollapseStatus (bool bCollapsed) { m_bCollapsed = bCollapsed;  }
		inline bool GetCollapseStatus (void) { return m_bCollapsed; }

	private:
		CString m_sDisplayText;
		int m_iLevel;
		bool m_bCollapsed;
	};

//  =======================================================================

class CContext
	{
	public:
		CContext (TArray <CExtension *> m_AllExtensions);
		~CContext (void);

		void ApplyQuery (CString sQuery);
		
		inline CString GetDescription (void) { m_sContextDescription;  }

		TArray<CContextObject> GetCurrentContextObjectList (void);
		TArray <CContextObject> GetLastContextObjectListInHistory (void);
		TArray <CContextObject> GetNextContextObjectListInHistory (void);

		void CleanUpHistory (int iNewHistoryLength);

	private:
		inline void SetDescription (CString sContextDescription) { m_sContextDescription = sContextDescription; }

		CString m_sContextDescription;
		TArray <CExtension *> &m_AllExtensions;
		TArray <CString *> m_aQueryHistory;
		TArray <TArray <CContextObject>> m_aContextObjectListHistory;
		
	};

//  =======================================================================