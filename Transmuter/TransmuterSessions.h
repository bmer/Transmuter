//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once
#define SCROLL_HORIZONTAL			0
#define SCROLL_VERTICAL				1
#define HEADERBAR_STYLE_TAB			2
#define HEADERBAR_STYLE_FULL		3

//  Class naming convention acronym meanings:
//  CS -- class, session
//  CSM -- class, session, menu item

class CPanel;
class CSLoading;
class CSChild;
class CSHeader;
class CSButton;
class CSError;
class CSScrollBar;
class CSTextArea;
class CSExtensionDetails;
class CSExtensionNavigator;
class CSMExtension;
class CSTransmuter;

//  =======================================================================

class CSLoading : public IHISession
	{
	public:
		CSLoading(CHumanInterface &HI);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
	};

//  =======================================================================

class CSChild : public IHISession
	{
	public:
		CSChild(CHumanInterface &HI, CPanel &AssociatedPanel);
		
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

class CSHeader : public CSChild
	{
	public:
		CSHeader (CHumanInterface &HI, CPanel &AssociatedPanel, CString sPanelName);
		
		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		inline DWORD GetStyle (void) { return m_dwStyle; }
		inline void SetStyle (DWORD dwStyle) { m_dwStyle = dwStyle; }

	private:
		CString m_sText;
		DWORD m_dwStyle;
	};

//  =======================================================================

class CSButton : public CSChild
	{
	public:
		CSButton (CHumanInterface &HI, CPanel &AssociatedPanel);
		CSButton (CHumanInterface &HI, CPanel &AssociatedPanel, double dActiveAreaScale);
		CSButton (CHumanInterface &HI, CPanel &AssociatedPanel, double dActiveAreaScale, CG32bitPixel BGColor);

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		virtual void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		virtual void OnLButtonUp (int x, int y, DWORD dwFlags);

		virtual void OnRButtonDown (int x, int y, DWORD dwFlags);
		virtual void OnRButtonUp (int x, int y, DWORD dwFlags);

		inline virtual void SetBGColor(CG32bitPixel BGColor) { m_rgbBackgroundColor = BGColor; }
		inline virtual void SetTextString(CString TextString) { m_TextString = TextString; m_HasText = true; }

		inline bool CheckIfLPressed(void) { bool ReturnValue = m_IsLPressed; m_IsLPressed = false; return ReturnValue; }
		inline bool CheckIfRPressed(void) { bool ReturnValue = m_IsRPressed; m_IsRPressed = false; return ReturnValue; }

		void UpdateActiveAreaScale(double dNewActiveAreaScale);
		RECT GetActiveAreaRect(void);

	protected:
		CG32bitPixel m_rgbBackgroundColor;

		double m_dActiveAreaScale;

		bool m_HasText;
		CString m_TextString;

		bool m_IsLDown;
		bool m_IsLPressed;

		bool m_IsRDown;
		bool m_IsRPressed;
	};

//  =======================================================================

class CSScrollBar : public CSButton
	{
    	public:
		CSScrollBar(CHumanInterface &HI, CPanel &AssociatedPanel, CPanel &pPanelToScroll);

		//void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

		//void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		//void OnLButtonUp (int x, int y, DWORD dwFlags);

		//void OnRButtonDown (int x, int y, DWORD dwFlags);
		//void OnRButtonUp (int x, int y, DWORD dwFlags);

		inline void SetBGColor(CG32bitPixel BGColor) { m_rgbBackgroundColor = BGColor; }
		inline void SetTextString(CString TextString) { m_TextString = TextString; m_HasText = true; }

		inline void SetScrollBarColor (CG32bitPixel ScrollBarColor) { m_ScrollBarColor = ScrollBarColor; }

		void DetermineVisibility (void);
		void CalculateBarRect (void);
		void CalculateSlideRect (void);
		inline bool IsBarVisible (void) { return m_bVisible; } 

	private:
		DWORD dwBarOrientation;

		CPanel &m_PanelToScroll;
		CG32bitPixel m_ScrollBarColor;

		RECT m_rcSlide;
		RECT m_rcBar;

		bool m_bVisible;
	};

//  =======================================================================

class CSError : public CSChild
	{
	public:
		CSError(CHumanInterface &HI, CPanel &AssociatedPanel, CString ErrorString);

		void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CString m_sErrorString;
	};

//  =======================================================================

class CSTextArea : public CSChild
	{
	public:
		CSTextArea (CHumanInterface &HI, CPanel &AssociatedPanel);

		inline const CString &GetText (void) { return m_sText; }
		inline void SetBackgroundColor (CG32bitPixel rgbColor) { m_rgbBackgroundColor = rgbColor; }
		inline void SetBorderRadius (int iRadius) { m_iBorderRadius = iRadius; }
		inline void SetTextColor (CG32bitPixel rgbColor) { m_rgbTextColor = rgbColor; }
		inline void SetCursor (int iLine, int iCol = 0) { m_iCursorLine = iLine; m_iCursorPos = iCol; }
		inline void SetEditable (bool bEditable = true) { m_bEditable = bEditable; }
		inline void SetFont (const CG16bitFont *pFont) { m_pFont = pFont; m_cxJustifyWidth = 0; }
		inline void SetFontTable (const IFontTable *pFontTable) { m_pFontTable = pFontTable; }
		inline void SetLineSpacing (int cySpacing) { m_cyLineSpacing = cySpacing; m_cxJustifyWidth = 0; }
		inline void SetPadding (int iPadding) { m_rcPadding.left = iPadding; m_rcPadding.top = iPadding; m_rcPadding.right = iPadding; m_rcPadding.bottom = iPadding; }
		void SetRichText (const CString &sRTF);
		inline void SetStyles (DWORD dwStyles) { m_dwStyles = dwStyles; m_cxJustifyWidth = 0; }
		void SetText (const CString &sText);

		int Justify (void);
		int Justify (const RECT &rcRect);
		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
		void UpdateTicker (void) { m_iTick++; }

	private:
		RECT CalcTextRect (const RECT &rcRect);
		RECT CalcTextRect (void);
		void FormatRTF (const RECT &rcRect);
		void PaintRTF (CG32bitImage &Dest, const RECT &rcRect);
		void PaintText (CG32bitImage &Dest, const RECT &rcRect);

		CString m_sText;						//	Text text to draw
		CString m_sRTF;							//	Rich text to draw (only if m_sText is blank)

		bool m_bEditable;						//	TRUE if editable
		DWORD m_dwStyles;						//	AlignmentStyles
		RECT m_rcPadding;						//	Padding around the text
		int m_cyLineSpacing;					//	Extra spacing between lines
		int m_iBorderRadius;
		const CG16bitFont *m_pFont;
		CG32bitPixel m_rgbTextColor;
		CG32bitPixel m_rgbBackgroundColor;

		bool m_bRTFInvalid;						//	TRUE if we need to format rich text
		CTextBlock m_RichText;					//	Rich text to draw (only if m_sText is blank)
		const IFontTable *m_pFontTable;			//	For rich text

		TArray<CString> m_Lines;				//	Justified lines of text
		int m_cxJustifyWidth;					//	Width (in pixels) for which m_Lines
												//		was justified.

		int m_iTick;							//	Cursor tick
		int m_iCursorLine;						//	Cursor position (-1 = no cursor)
		int m_iCursorPos;						//	Position in line
	};


//  =======================================================================

class CSExtensionDetails : public CSChild
	{
	public:
		CSExtensionDetails(CHumanInterface &HI, CPanel &AssociatedPanel);

		//void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CExtension m_Extension;
	};

//  =======================================================================

class CSMExtension : public CSChild
	{
	public:
		CSMExtension (CHumanInterface &HI, CPanel &AssociatedPanel, CExtension *Extension);
		~CSMExtension (void);
		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CExtension &m_Extension;
		CSButton *m_Button;
		CSTextArea *m_TextArea;
	};

class CSExtensionNavigator : public CSChild
	{
	public:
		CSExtensionNavigator (CHumanInterface &HI, CPanel &AssociatedPanel, TArray <CExtension *> Extensions);
		~CSExtensionNavigator(void);

		void CreateExtensionNavigatorMenuItems (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void DrawTitleBar (CG32bitImage &Screen);

	private:
		TArray <CExtension *> m_Extensions;
		TArray <CSMExtension *> m_NavigatorMenuItems;
		int m_iMenuSlotHeight;
		int m_iHeaderBarHeight;
	};


//  =======================================================================

class CSTransmuter : public IHISession, public CUniverse::INotifications
	{
	public:
		CSTransmuter (CHumanInterface &HI, CTransmuterModel &model);
		~CSTransmuter (void);
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
		TArray <CSChild *> m_aChildSessions;
		CTransmuterModel &m_Model;
		CPanel &m_Panel;
		int m_IsRButtonDown;
	};