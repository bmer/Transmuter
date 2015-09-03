//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once
#define SCROLL_HORIZONTAL		0
#define SCROLL_VERTICAL			1

//  Class naming convention acronym meanings:
//  CS -- class, session
//  CSM -- class, session, menu item

class CPanel;
class CSLoading;
class CSChild;
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

class CSButton : public CSChild
	{
	public:
		CSButton(CHumanInterface &HI, CPanel &AssociatedPanel);
		CSButton(CHumanInterface &HI, CPanel &AssociatedPanel, RECT rcRelativeButton);
		CSButton(CHumanInterface &HI, CPanel &AssociatedPanel, RECT rcRelativeButton, CG32bitPixel BGColor);

		virtual void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

		virtual void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		virtual void OnLButtonUp (int x, int y, DWORD dwFlags);

		virtual void OnRButtonDown (int x, int y, DWORD dwFlags);
		virtual void OnRButtonUp (int x, int y, DWORD dwFlags);

		inline virtual void SetBGColor(CG32bitPixel BGColor) { m_rgbBackColor = BGColor; }
		inline virtual void SetTextString(CString TextString) { m_TextString = TextString; m_HasText = true; }

		inline bool CheckIfLPressed(void) { bool ReturnValue = m_IsLPressed; m_IsLPressed = false; return ReturnValue; }
		inline bool CheckIfRPressed(void) { bool ReturnValue = m_IsRPressed; m_IsRPressed = false; return ReturnValue; }

		void UpdateRelativeButtonRect(RECT rc);

	protected:
		CG32bitPixel m_rgbBackColor;

		RECT m_rcActive;
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

		inline void SetBGColor(CG32bitPixel BGColor) { m_rgbBackColor = BGColor; }
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

class CSTextArea : public CSChild, public CGTextArea
	{
	public:
		CSTextArea (CHumanInterface &HI, CPanel &AssociatedPanel);

		int JustifyInPanel (void);
		void PaintInPanel (CG32bitImage &Dest);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
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
		TArray <CSChild *> m_aSubSessions;
		CTransmuterModel &m_Model;
		CPanel &m_Panel;
		int m_IsRButtonDown;
	};