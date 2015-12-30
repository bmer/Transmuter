//	BasiCTransmuterPanelContents.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#define SCROLL_HORIZONTAL			0
#define SCROLL_VERTICAL				1
#define HEADERBAR_STYLE_TAB			2
#define HEADERBAR_STYLE_FULL		3

class CPanel;
class IPanelContent;
class CTransmuterPanelContent;
class CHeaderPanelContent;
class CTextContent;
class CMainSession;

//  Class that all Transmuter PanelContents derive from. Note that every Transmuter
//  PanelContent has the capacity to be a button, so there is no need to have a separate
//  "Button" PanelContent object.

class IPanelContent : public IHISession
	{
	public: 
		IPanelContent(CString sContentName, CHumanInterface &HI, CPanel &AssociatedPanel);

		//  IHISession virtuals
		virtual void SetHeaderContent (void) {};
		virtual void UpdateHeaderContent (void) {};

		virtual void OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture) {};
		virtual void OnLButtonUp(int x, int y, DWORD dwFlags) {};
		virtual void OnRButtonDown(int x, int y, DWORD dwFlags) {};
		virtual void OnRButtonUp(int x, int y, DWORD dwFlags) {};

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid) {};

		//  Return the associated panel, so that it may be manipulated (e.g. moved)
		inline CPanel &GetAssociatedPanel(void) { return m_AssociatedPanel; }

		//  This function draws an outline around the panel containing this
		//  CTransmuterPanelContent object, and is mainly meant to be used for debugging.
		void DrawPanelOutline (CG32bitImage &Screen);

	private:
		//  All PanelContents must *always* have an associated panel
		CPanel &m_AssociatedPanel;

		//  PanelContents have a background color
		CG32bitPixel m_rgbBackgroundColor;
		//  PanelContents can draw an outline of their Panel with color m_rgbPanelOutlineColor
		CG32bitPixel m_rgbPanelOutlineColor;

		bool m_bLButtonDown;
		bool m_RButtonDown;
	};

class CTransmuterPanelContent : public IPanelContent
	{
	public:
		CTransmuterPanelContent(CString sContentName, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);

		//  This destructor takes care of:
		//		* deleting the associated HeaderPanelContent, if it is not NULL
		~CTransmuterPanelContent (void);

		inline CHeaderPanelContent *GetHeaderPanelContent (void) { m_HeaderPanelContent; }
		//  The following functions have an empty definition by default, which will
		//  probably overridden in classes descending from CTransmuterPanelContent
		virtual void SetHeaderContent (void) {};
		virtual void UpdateHeaderContent (void) {};

		inline CTransmuterModel &GetModel (void) { m_model; }
	protected:
		//  TransmuterPanelContents may, or may not have associated header sessions
		//  If m_HeaderPanelContent == NULL, then a header does not exist, else it does
		CHeaderPanelContent *m_HeaderPanelContent;

		//  TrasnmuterPanelContents have a name string -- it must be supplied
		//  initializing a TransmuterPanelContent object
		const CString m_sName;

		//  The TransmuterModel. 
		CTransmuterModel &m_model;
	};

//  =======================================================================
//  HeaderPanelContents display relevant information regarding the panel at the top;
//  for example, the panel name. Critically, HeaderPanelContents allow interaction which
//  might affect their associated session. CHeaderPanelContent should be derived from in 
//  order to specialize them for the particular session they are intended for. 

class CHeaderPanelContent : public IPanelContent
	{
	public:
		CHeaderPanelContent (CString sName, CString sHeaderText, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterPanelContent &AssociatedSession);
		~CHeaderPanelContent (void);
		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CString m_sHeaderText;
		const CG16bitFont *m_pFont;
		CG32bitPixel m_rgbTextColor;
		CG32bitPixel m_rgbBackgroundColor;
		CString m_sPanelName;
	};