//	CMainSession.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CLoadingSession;
class IPanel;
class CTransmutercontent;
class CContextPanel;
class CCommandPanel;
class CMainSession;

//  =======================================================================
//  Presents loading screen.
class CLoadingSession : public IHISession
	{
	public:
		CLoadingSession (CHumanInterface &HI);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
	};

//  =======================================================================

//  =======================================================================

class CMainSession : public IHISession, public CUniverse::INotifications
	{
	public:
		CMainSession (CHumanInterface &HI, CTransmuterModel &model);
		~CMainSession (void);
		//	IHISession virtuals

		//virtual void OnCleanUp (void);
		virtual ALERROR OnCommand (const CString &sCmd, void *pData = NULL);
		//virtual ALERROR OnInit (CString *retsError);
		//virtual void OnLButtonDblClick (int x, int y, DWORD dwFlags);
		void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		void OnLButtonUp (int x, int y, DWORD dwFlags);
		//virtual void OnMouseMove (int x, int y, DWORD dwFlags);
		void OnRButtonDown (int x, int y, DWORD dwFlags);
		void OnRButtonUp (int x, int y, DWORD dwFlags);
		void OnKeyDown (int iVirtKey, DWORD dwKeyData);
		void OnKeyUp (int iVirtKey, DWORD dwKeyData);
		void OnChar (char chChar, DWORD dwKeyData);
		//virtual void OnUpdate (bool bTopMost);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		//CSplitContainer *m_pContextEditorPanel;
		CArrayContainer *m_pContextEditorPanel;
		CContextPanel *m_pContextPanel;
		CCommandPanel *m_pCommandPanel;
		TArray <IPanel *> m_aPanels;

		IPanel *m_pCapture;

		IPanel *m_pFocusPanel;

		CTransmuterModel &m_Model;
		CArrayContainer m_Panel;

		int m_iSeparatorWidth;
	};

