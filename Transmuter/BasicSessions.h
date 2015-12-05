//	BasicTransmuterSessions.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#define SCROLL_HORIZONTAL			0
#define SCROLL_VERTICAL				1
#define HEADERBAR_STYLE_TAB			2
#define HEADERBAR_STYLE_FULL		3

class CPanel;
class CLoadingSession;
class CTransmuterSession;
class CHeaderSession;
class CButtonSession;
class CTextAreaSession;
class CMainSession;

//  =======================================================================
//  Presents loading screen.
class CLoadingSession : public IHISession
	{
	public:
		CLoadingSession(CHumanInterface &HI);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
	};

//  =======================================================================

//  Class that all Transmuter sessions derive from.	Note that every Transmuter
//  session has the capacity to be a button, so there is no need to have a separate
//  "Button" session.

class CTransmuterSession : public IHISession
	{
	public:
		CTransmuterSession(CString sSessionName, CHumanInterface &HI, CPanel &AssociatedPanel);

		//  This destructor takes care of:
		//		* deleting the associated HeaderSession, if it is not NULL
		~CTransmuterSession (void);

		//  The following functions have an empty definition by default, which will
		//  probably overridden in classes descending from CTransmuterSession
		virtual void SetHeader (void) {};
		virtual void UpdateHeader (void) {};

		virtual void OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture) {};
		virtual void OnLButtonUp(int x, int y, DWORD dwFlags) {};
		virtual void OnRButtonDown(int x, int y, DWORD dwFlags) {};
		virtual void OnRButtonUp(int x, int y, DWORD dwFlags) {};

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid) {};

		//  Return the associated panel, so that it may be manipulated (e.g. moved)
		inline CPanel &GetAssociatedPanel(void) { return m_AssociatedPanel; }

		//  This function draws an outline around the panel containing this
		//  CTransmuterSession object, and is mainly meant to be used for debugging.
		void DrawPanelOutline (CG32bitImage &Screen);
	protected:
		//  All TransmuterSessions must *always* have an associated panel
		CPanel &m_AssociatedPanel;

		//  TransmuterSessions may, or may not have associated header sessions
		//  If m_HeaderSession == NULL, then a header does not exist, else it does
		CHeaderSession *m_HeaderSession;

		//  TransumterSessions have a name string -- it must be supplied
		//  initializing a TransmuterSession object
		const CString m_sSessionName;

		//  TransumterSessions can draw outlines around themselves, if necessary,
		//  (this is mostly meant to be used for debugging purposes)
		CG32bitPixel m_rgbPanelOutlineColor;

		//  TransmuterSessions have a background color
		CG32bitPixel m_rgbBackgroundColor;
	};

//  =======================================================================
//  HeaderSessions display relevant information regarding the panel at the top;
//  for example, the panel name. Critically, HeaderSessions allow interaction which
//  might affect their associated session. CHeaderSession should be derived from in 
//  order to specialize them for the particular session they are intended for. 

class CHeaderSession : public CTransmuterSession
	{
	public:
		CHeaderSession (CString sParentSessionName, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterSession &AssociatedSession);
		~CHeaderSession (void);
		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CG16bitFont m_font;
		CG32bitPixel m_rgbTextColor;
		CG32bitPixel m_rgbBackgroundColor;
		CString m_sPanelName;
	};