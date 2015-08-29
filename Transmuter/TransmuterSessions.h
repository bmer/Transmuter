//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CLoadingSession;
class CPanel;
class CSubSession;
class CButton;
class CError;
class CTextArea;
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
		CButton(CHumanInterface &HI, CPanel &AssociatedPanel, CG32bitPixel BGColor);

		void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

		void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		void OnLButtonUp (int x, int y, DWORD dwFlags);

		void OnRButtonDown (int x, int y, DWORD dwFlags);
		void OnRButtonUp (int x, int y, DWORD dwFlags);

		inline void SetBGColor(CG32bitPixel BGColor) { m_rgbBackColor = BGColor; }
		inline void SetTextString(CString TextString) { m_TextString = TextString; m_HasText = true; }

		inline bool CheckIfLPressed(void) { bool ReturnValue = m_IsLPressed; m_IsLPressed = false; return ReturnValue; }
		inline bool CheckIfRPressed(void) { bool ReturnValue = m_IsRPressed; m_IsRPressed = false; return ReturnValue; }

	private:
		CG32bitPixel m_rgbBackColor;

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

class CTextArea : public CSubSession
	{
	public:
		CTextArea(CHumanInterface &HI, CPanel &AssociatedPanel);
		CTextArea(CHumanInterface &HI, CPanel &AssociatedPanel, CString sText);
		CTextArea(CHumanInterface &HI, CPanel &AssociatedPanel, CString sRichText);

		inline const CString &GetText (void) { return m_sText; }
		inline void SetBackColor (CG32bitPixel rgbColor) { m_rgbBackColor = rgbColor; }
		inline void SetBorderThickness (int iThickness) { m_iBorderThickness = iThickness; }
		inline void SetTextColor (CG32bitPixel rgbColor) { m_rgbTextColor = rgbColor; }
		inline void SetCursor (int iLine, int iCol = 0) { m_iCursorLine = iLine; m_iCursorPos = iCol; }
		inline void SetEditable (bool bEditable = true) { m_bEditable = bEditable; }
		inline void SetFont (const CG16bitFont *pFont) { m_pFont = pFont; m_cxJustifyWidth = 0; }
		inline void SetFontTable (const IFontTable *pFontTable) { m_pFontTable = pFontTable; }
		inline void SetLineSpacing (int cySpacing) { m_cyLineSpacing = cySpacing; m_cxJustifyWidth = 0; }
		inline void SetPadding (int iPadding) { m_rcPadding.left = iPadding; m_rcPadding.top = iPadding; m_rcPadding.right = iPadding; m_rcPadding.bottom = iPadding; }
		inline void SetRichText (const CString &sRTF) { m_sRichText = sRTF; m_sText = NULL_STR; m_bRTFInvalid = true; HIInvalidate(); }
		inline void SetStyles (DWORD dwStyles) { m_dwStyles = dwStyles; m_cxJustifyWidth = 0; }
		inline void SetText (const CString &sText) { m_sText = sText; m_sRichText = NULL_STR; m_cxJustifyWidth = 0; HIInvalidate(); }
		inline void SetPadding (RECT rcPadding) { m_rcPadding = rcPadding; }
		inline RECT GetPadding(RECT rcPadding) { return m_rcPadding; }
		void SetEdgePadding (DWORD dwEdge, int iPadding);

		int Justify (const RECT &rcRect);
		void FormatRichText(void);

		void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);
	private:
		CString m_sText;					//  basic content string
		const CG16bitFont *m_pFont;
		RECT m_rcPadding;					//  padding information -- not really a rectangle

		bool m_bPlainText;					//  true if we want plain text shown, false if we want rich text

		int m_cxJustifyWidth;				//	Width (in pixels) for which m_Lines
											//	was justified.
		int m_cyLineSpacing;

		bool m_bRTFInvalid;					//  true if we need to format rich text
		DWORD m_dwStyles;
		CString m_sRichText;				//  rich text formatted string
		const IFontTable *m_pFontTable;		//  for rich text
		TArray<CString> m_Lines;			//	Justified lines of text

		CG32bitPixel m_rgbBackColor;		//  Background color of text box
		int m_iBorderThickness;
		CG32bitPixel m_rgbTextColor;

		bool m_bEditable;
		int m_iTick;						//	Cursor tick
		int m_iCursorLine;					//	Cursor position (-1 = no cursor)
		int m_iCursorPos;					//	Position in line
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
		int m_iMenuSlotHeight;
		int m_iHeaderBarHeight;
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

	private:
		TArray <CSubSession *> m_aSubSessions;
		CTransmuterModel &m_Model;
		CPanel &m_Panel;
		int m_IsRButtonDown;
	};