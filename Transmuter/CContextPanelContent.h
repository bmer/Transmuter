//	CContextPanelContent.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CContextPanelContent;
class CContext;
class CContextFilter;
class CContextItemSession;

//  =======================================================================

class CContextPanelContent : public CTransmuterPanelContent
	{
	public:
		CContextPanelContent (CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
		~CContextPanelContent (void);

		void CreateContextItemSessions(void);
		void SetHeaderContent(int headerWidth, int headerHeight, CString headerText);
		void UpdateHeaderContent(CString headerText);

		void LoadDefinedContext (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CExtensionCollection &m_ExtensionCollection;
		CContext *m_currentContext;
		CContext m_defaultContext;
		TArray <CContext *> m_definedContexts;
	};

//  =======================================================================

class CContext
	{
	public:
		CContext (TArray <CExtension *> m_aExtensions);
		~CContext (void);

		void ApplyContextFilters (TArray <CContextFilter *> aContextFilters);
	
		void ResetContextFilters (void);
		
		void CreateContextItemSessions (CContextPanelContent & ContextSession);
		
		inline void SetContextDescription (CString sContextDescription) { m_sContextDescription = sContextDescription;  }
		inline CString GetContextDescription (void) { m_sContextDescription;  }
	private:
		CString m_sContextDescription;
		TArray <CExtension *> &m_aExtensions;
		TArray <CContextFilter *> m_savedContextFilters;
		TArray <CContextFilter *> m_appliedContextFilters;
		TArray <CContextItemSession *> m_aContextItemSessions;
		
	};

//  =======================================================================

class CContextFilter
	{
	public:
		CContextFilter (void);

		void initContextFilterFromString (CString sContextFilterStr);

	private:
		CString m_sContextFilterStr;
	};

//  =======================================================================

class CContextItemSession : public CTransmuterPanelContent
	{
	public:
		CContextItemSession(CString sExtensionName, CHumanInterface & HI, CPanel & AssociatedPanel, CTransmuterModel & model);

		CContextItemSession (CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);

	private:

	};