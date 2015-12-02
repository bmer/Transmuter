//	CMainSession.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CMainSession;

//  =======================================================================

class CMainSession : public IHISession, public CUniverse::INotifications
	{
	public:
		CMainSession (CHumanInterface &HI, CTransmuterModel &model);
		~CMainSession (void);
		//	IHISession virtuals

		//virtual void OnChar (char chChar, DWORD dwKeyData);
		//virtual void OnCleanUp (void);
		//virtual ALERROR OnCommand (const CString &sCmd, void *pData = NULL);
		//virtual ALERROR OnInit (CString *retsError);
		//virtual void OnLButtonDblClick (int x, int y, DWORD dwFlags);
		void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		void OnLButtonUp (int x, int y, DWORD dwFlags);
		//virtual void OnMouseMove (int x, int y, DWORD dwFlags);
		void OnRButtonDown (int x, int y, DWORD dwFlags);
		void OnRButtonUp (int x, int y, DWORD dwFlags);
		//virtual void OnUpdate (bool bTopMost);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		TArray <CTransmuterSession *> m_aSubSessions;
		CTransmuterModel &m_Model;
		CPanel &m_Panel;
	};