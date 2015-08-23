//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CLoadingSession;
class CSubSession;
class CExtensionDetails;
class CExtensionNavigator;
class CExtensionNavigatorMenuItem;
class CTransmuterSession;

//  =======================================================================

class CLoadingSession : public IHISession
	{
	public:
		CLoadingSession(CHumanInterface &HI);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
	};

//  =======================================================================

class CSubSession : public IHISession
	{
	public:
		CSubSession(CHumanInterface &HI, CPanel &AssociatedPanel);

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void DrawPanelOutline (CG32bitImage &Screen);

	protected:
		const CG16bitFont &m_HeadingFont;
		CG32bitPixel m_HeadingColor;
		CG32bitPixel m_BorderColor;
		int m_PanelOutlineWidth;
		CG32bitPixel m_PanelOutlineColor;
		CPanel &m_AssociatedPanel;
	};

//  =======================================================================

class CErrorSession : public CSubSession
	{
	public:
		CErrorSession(CHumanInterface &HI, CPanel &AssociatedPanel, CString ErrorString);

		void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CString m_ErrorString;
	};

//  =======================================================================

class CMenuSession : public CSubSession
	{
	public:
		CMenuSession(CHumanInterface &HI, CPanel &AssociatedPanel);

		void OnPaint(CG32bitImage &Screen, const RECT &rcINvalid);
	};

//  =======================================================================

class CExtensionDetails : public CSubSession
	{
	public:
		CExtensionDetails(CHumanInterface &HI, CPanel &AssociatedPanel);

		//void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CExtension m_Extension;
	};

//  =======================================================================

class CExtensionNavigatorMenuItem : public CSubSession
	{
	public:
		CExtensionNavigatorMenuItem (CHumanInterface &HI, CPanel &AssociatedPanel, CExtension *Extension);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CExtension &m_Extension;
	};

class CExtensionNavigator : public CSubSession
	{
	public:
		CExtensionNavigator (CHumanInterface &HI, CPanel &AssociatedPanel, TArray <CExtension *> Extensions);

		void CreateExtensionNavigatorMenuItems (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void DrawTitleBar (CG32bitImage &Screen);

	private:
		TArray <CExtension *> m_Extensions;
		TArray <CExtensionNavigatorMenuItem> m_NavigatorMenuItems;
		CPanel m_InternalPanelling;
	};


//  =======================================================================

class CTransmuterSession : public IHISession, public CUniverse::INotifications
	{
	public:
		CTransmuterSession (CHumanInterface &HI, CTransmuterModel &model);
		~CTransmuterSession (void);
		//	IHISession virtuals

		//virtual void OnChar (char chChar, DWORD dwKeyData);
		//virtual void OnCleanUp (void);
		//virtual ALERROR OnCommand (const CString &sCmd, void *pData = NULL);
		//virtual ALERROR OnInit (CString *retsError);
		//virtual void OnLButtonDblClick (int x, int y, DWORD dwFlags);
		//virtual void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		//virtual void OnLButtonUp (int x, int y, DWORD dwFlags);
		//virtual void OnMouseMove (int x, int y, DWORD dwFlags);
		void OnRButtonDown (int x, int y, DWORD dwFlags);
		void OnRButtonUp (int x, int y, DWORD dwFlags);
		//virtual void OnUpdate (bool bTopMost);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	protected:
		void UpdateSubSessionsList(void);

	private:
		TArray <CSubSession *> m_SubSessions;
		CTransmuterModel &m_Model;
		CPanel m_ElasticPanelling;
		int m_IsRButtonDown;
	};