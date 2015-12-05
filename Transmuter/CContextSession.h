//	CContextSession.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CContextSession;
class CContext;
class CLibrary;
class CContextItemSession;

//  =======================================================================

class CContextSession : public CTransmuterSession
	{
	public:
		CContextSession (CHumanInterface &HI, CPanel &AssociatedPanel);
		~CContextSession (void);

		void LoadDefaultContext (void);
		void LoadGivenContext (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		TArray <CExtension *> m_Extensions;
		TArray <CContextItemSession *> m_ContextItems;
	};

//  =======================================================================

class CLibrary
	{
	public:
		CLibrary (void);

		void ApplyContextFilters(TArray <CContextFilter *> aContextFilters);
	
		void ResetContextFilters(void);

		CContext GetContext(void);

	private:
		TArray <CContextFilter *> m_appliedContextFilters;
	};

class CContextItemSession : public CTransmuterSession
	{
	public:
		CContextItemSession (CHumanInterface &HI, CPanel &AssociatedPanel, TArray <CString> Context);
		~CContextItemSession (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		TArray <CString> Context;
		CTextAreaSession &m_pTextArea;
	};