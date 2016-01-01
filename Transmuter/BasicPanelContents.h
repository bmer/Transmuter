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
class CTransmuterContent;
class CHeaderContent;
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

		virtual void OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture);
		virtual void OnLButtonUp(int x, int y, DWORD dwFlags);
		virtual void OnLButtonDblClick(int x, int y, DWORD dwFlags, bool *retbCapture);

		virtual void OnRButtonDown(int x, int y, DWORD dwFlags);
		virtual void OnRButtonUp(int x, int y, DWORD dwFlags);

		void OnKeyDown (int iVirtKey, DWORD dwKeyData);
		void OnKeyUp (int iVirtKey, DWORD dwKeyData);
		void OnChar (char chChar, DWORD dwKeyData);
 
		inline bool IsLButtonDown (void) { return m_bLButtonDown; }
		inline bool IsLClicked (void) { return m_bLClicked; }
		inline bool IsLDblClicked (void) { return m_bLDblClicked; }

		inline bool IsRButtonDown (void) { return m_bRButtonDown; }
		inline bool IsRClicked (void) { return m_bRClicked;  }

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid) { ; }

		//  Return the associated panel, so that it may be manipulated (e.g. moved)
		inline CPanel &GetAssociatedPanel (void) { return m_AssociatedPanel; }

		//  This function draws an outline around the panel containing this
		//  CTransmuterContent object, and is mainly meant to be used for debugging.
		void DrawPanelOutline (CG32bitImage &Screen);
		inline void UpdatePanelOutlineColor (CG32bitPixel rgbColor) { m_rgbPanelOutlineColor = rgbColor;  }

		inline IPanelContent *SetFocus (void) { m_bFocus = true; return this; }
		void RemoveFocus (void) { m_bFocus = false;  }
		inline bool GetFocusStatus (void) { return m_bFocus; }

	protected:
		virtual void OnContentLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture) {};
		virtual void OnContentLButtonUp (int x, int y, DWORD dwFlags) { ; }
		virtual void OnContentLButtonDblClick (int x, int y, DWORD dwFlags, bool *retbCapture) { ; }

		virtual void OnContentRButtonDown (int x, int y, DWORD dwFlags) { ; }
		virtual void OnContentRButtonUp (int x, int y, DWORD dwFlags) { ; }

		void OnContentKeyDown (int iVirtKey, DWORD dwKeyData) { ; }
		void OnContentKeyUp (int iVirtKey, DWORD dwKeyData) { ; }
		void OnContentChar (char chChar, DWORD dwKeyData) { ; }

	private:
		inline bool GetCurrentStatusAndReset (bool &refBool) { bool bCurrentStatus = refBool; refBool = false; return bCurrentStatus; }

		bool m_bLButtonDown;
		bool m_bLClicked;
		bool m_bLDblClicked;

		bool m_bRButtonDown;
		bool m_bRClicked;

		bool m_bFocus;

		//  All PanelContents must *always* have an associated panel
		CPanel &m_AssociatedPanel;

		//  PanelContents have a background color
		CG32bitPixel m_rgbBackgroundColor;
		//  PanelContents can draw an outline of their Panel with color m_rgbPanelOutlineColor
		CG32bitPixel m_rgbPanelOutlineColor;
	};

class CTransmuterContent : public IPanelContent
	{
	public:
		CTransmuterContent(CString sContentName, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);

		//  This destructor takes care of:
		//		* deleting the associated HeaderPanelContent, if it is not NULL
		~CTransmuterContent (void);

		inline CHeaderContent *GetHeaderPanelContent (void) { m_HeaderContent; }
		void SetHeaderContent (CString sID, CString sHeaderText, int iWidth, int iHeight, const CG16bitFont *pFont=&(g_pHI->GetVisuals().GetFont(fontConsoleMediumHeavy)), CG32bitPixel rgbTextColor=CG32bitPixel(255,255,255), CG32bitPixel rgbBackgroundColor=CG32bitPixel(140,140,140));
		void UpdateHeaderText (CString sHeaderText);
		void UpdateHeaderFont (const CG16bitFont *pFont);
		void UpdateHeaderTextColor (CG32bitPixel rgbColor);
		void UpdateHeaderBackgroundColor (CG32bitPixel rgbColor);

		inline CTransmuterModel &GetModel (void) { m_model; }

		// virtual CString ExecuteCommand (CString sCommand, void *pData=NULL);
	protected:
		//  TransmuterPanelContents may, or may not have associated header sessions
		//  If m_HeaderContent == NULL, then a header does not exist, else it does
		CHeaderContent *m_HeaderContent;

		//  TrasnmuterPanelContents have a name string -- it must be supplied
		//  initializing a TransmuterPanelContent object
		const CString m_sID;

		//  The TransmuterModel. 
		CTransmuterModel &m_model;
	};

//  =======================================================================
//  HeaderPanelContents display relevant information regarding the panel at the top;
//  for example, the panel name. Critically, HeaderPanelContents allow interaction which
//  might affect their associated session. CHeaderContent should be derived from in 
//  order to specialize them for the particular session they are intended for. 

class CHeaderContent : public IPanelContent
	{
	public:
		CHeaderContent (CString sID, CString sHeaderText, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterContent &AssociatedSession);
		~CHeaderContent (void);

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		inline void SetHeaderText (CString sText) { m_sHeaderText = sText;  }
		inline CString GetHeaderText (void) { return m_sHeaderText; }
		inline void SetHeaderTextFont (const CG16bitFont *pFont) { m_pFont = pFont; }
		inline void SetHeaderTextColor (CG32bitPixel rgbColor) { m_rgbTextColor = rgbColor;  }
		inline void SetBackgroundColor (CG32bitPixel rgbColor) { m_rgbBackgroundColor = rgbColor; }

	private:
		CString m_sHeaderText;
		const CG16bitFont *m_pFont;
		CG32bitPixel m_rgbTextColor;
		CG32bitPixel m_rgbBackgroundColor;
	};