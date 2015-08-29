//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CLoadingSession;
class CPanel;
class CSubSession;
class CExtensionDetails;
class CExtensionNavigator;
class CExtensionMenuItem;
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
		
		virtual void OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture) {};
		virtual void OnLButtonUp(int x, int y, DWORD dwFlags) {};
		virtual void OnRButtonDown(int x, int y, DWORD dwFlags) {};
		virtual void OnRButtonUp(int x, int y, DWORD dwFlags) {};

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid) {};

		void DrawPanelOutline (CG32bitImage &Screen);

		inline CPanel &GetAssociatedPanel(void) { return m_AssociatedPanel; }

	protected:
		const CG16bitFont &m_HeadingFont;
		CG32bitPixel m_HeadingColor;
		CG32bitPixel m_BorderColor;
		int m_PanelOutlineWidth;
		CG32bitPixel m_PanelOutlineColor;
		CPanel &m_AssociatedPanel;
	};

//  =======================================================================

class CButton : public CSubSession
	{
	public:
		CButton(CHumanInterface &HI, CPanel &AssociatedPanel);

		void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

		void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		void OnLButtonUp (int x, int y, DWORD dwFlags);

		void OnRButtonDown (int x, int y, DWORD dwFlags);
		void OnRButtonUp (int x, int y, DWORD dwFlags);

		inline void SetBGColor(CG32bitPixel BGColor) { m_BGColor = BGColor; }
		inline void SetTextString(CString TextString) { m_TextString = TextString; m_HasText = true; }

		inline bool CheckIfLPressed(void) { bool ReturnValue = m_IsLPressed; m_IsLPressed = false; return ReturnValue; }
		inline bool CheckIfRPressed(void) { bool ReturnValue = m_IsRPressed; m_IsRPressed = false; return ReturnValue; }

	private:
		CG32bitPixel m_BGColor;

		bool m_HasText;
		CString m_TextString;

		bool m_IsLDown;
		bool m_IsLPressed;

		bool m_IsRDown;
		bool m_IsRPressed;
	};

//  =======================================================================

class CError : public CSubSession
	{
	public:
		CError(CHumanInterface &HI, CPanel &AssociatedPanel, CString ErrorString);

		void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CString m_sErrorString;
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

class CExtensionMenuItem : public CSubSession
	{
	public:
		CExtensionMenuItem (CHumanInterface &HI, CPanel &AssociatedPanel, CExtension *Extension);
		~CExtensionMenuItem (void);
		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CExtension &m_Extension;
		CButton *m_Button;
	};

class CExtensionNavigator : public CSubSession
	{
	public:
		CExtensionNavigator (CHumanInterface &HI, CPanel &AssociatedPanel, TArray <CExtension *> Extensions);
		~CExtensionNavigator(void);

		void CreateExtensionNavigatorMenuItems (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void DrawTitleBar (CG32bitImage &Screen);

	private:
		TArray <CExtension *> m_Extensions;
		TArray <CExtensionMenuItem *> m_NavigatorMenuItems;
		int m_MenuSlotHeight;
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
		void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		void OnLButtonUp (int x, int y, DWORD dwFlags);
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
		CPanel &m_Panel;
		int m_IsRButtonDown;
	};