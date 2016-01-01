//	CContextContent.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CContextContent;
class CContext;
class CContextObject;

class CContextObject
	{
	public:
		CContextObject (CContextObject *ParentContextObj, CString sDisplayText, int iLevel, bool bCollapsed);

		inline void SetLevel (int iLevel) { m_iLevel = iLevel; }
		inline int GetLevel (void) { return m_iLevel; }

		inline void SetCollapseStatus (bool bCollapsed) { m_bCollapsed = bCollapsed; }
		inline bool GetCollapseStatus (void) { return m_bCollapsed; }
		bool IsParentCollapsed (void);

		inline CContextObject *GetParent (void) { return m_ParentCtxObj; }

		inline CString GetDisplayText (void) { return m_sDisplayText; }

	private:
		CString m_sDisplayText;
		int m_iLevel;
		bool m_bCollapsed;
		CContextObject *m_ParentCtxObj;
	};

//  =======================================================================

class CContextObjectArray : public TArray <CContextObject *>
	{
	public:
		inline CContextObjectArray (void) { ; }
		inline CContextObjectArray (const CContextObjectArray &refCtxObjArray) { Copy(refCtxObjArray); }
		inline ~CContextObjectArray (void) { CleanUp(); }

		void Insert (const CContextObjectArray &refCtxObjArray);
		CContextObject *Insert (const CContextObject &refCtxObj);
		inline CContextObjectArray &operator=(const CContextObjectArray &refCtxObjArray) { CleanUp(); Copy(refCtxObjArray); return *this; }

	private:
		void CleanUp (void);
		void Copy (const CContextObjectArray &refCtxObjArray);
	};

//  =======================================================================

class CContext
	{
	public:
		CContext (TArray <CExtension *> m_AllExtensions);
		~CContext (void);

		void ApplyQuery (CString sQuery);

		inline CString GetDescription (void) { return m_sContextDescription; }

		CContextObjectArray DetermineContextObjectList (CString sQuery);
		CContextObjectArray *GetCurrentContextObjectList (void);
		void ChangeToLastContextInHistory (void);
		void ChangeToNextContextInHistory (void);

		void CleanUpHistory (int iNewHistoryLength);

	private:
		inline void SetDescription (CString sContextDescription) { m_sContextDescription = sContextDescription; }
		CContextObjectArray CreateContextObjectListForExtension (CExtension *Extension);
		CString m_sContextDescription;
		TArray <CExtension *> &m_AllExtensions;
		
		int m_iCurrentContextIndex = 0;
		TArray <CString> m_aQueryHistory;
		TArray <CContextObjectArray> m_aContextObjectListHistory;
	};

//  =======================================================================

class CContextContent : public CTransmuterContent
	{
	private:
		CExtensionCollection &m_ExtensionCollection;
		const CG16bitFont *m_pFont;
		CG32bitPixel m_rgbFontColor;

	public:
		CContextContent (CString sID, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
		~CContextContent (void);
		
		CContext m_Context;

		void LoadLastDefinedContextInHistory (void);
		void LoadNextDefinedContextInHistory (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
	};

//  =======================================================================

