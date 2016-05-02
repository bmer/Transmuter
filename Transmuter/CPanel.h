//	TransmuterPanelContent.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once
#define EDGE_LEFT			0
#define EDGE_TOP			1
#define EDGE_RIGHT			2
#define EDGE_BOTTOM			3

#define SMOOTH_UPDOWN		4
#define SMOOTH_LEFTRIGHT	5

// class TransmuterException;
class CPanelRect;
class IPanelContent;
class IPanel;
class CInternalPanels;

// using namespace std;

//class CMessageException: public exception
//	{
//	public: 
//		CMessageException (const char* ExceptionMessage);
//
//		virtual const char* what() const throw() { return m_ExceptionMessage; }
//
//	private:
//		const char* m_ExceptionMessage;
//	};

class CPanelRect
	{
	friend IPanel;
	friend CInternalPanels;

	public:
		CPanelRect (IPanel &AssociatedPanel);
		CPanelRect (IPanel &AssociatedPanel, int iOriginX, int iOriginY, int iWidth, int iHeight);
		CPanelRect (IPanel &AssociatedPanel, RECT rc);

		inline int GetOriginX (void) { return m_iOriginX; }
		inline int GetOriginY (void) { return m_iOriginY; }
		inline int GetHeight (void) { return m_iHeight; }
		inline int GetWidth (void) { return m_iWidth; }

		void MakeTopLeftOrigin(void);

		void SetOrigin (int iOriginX, int iOriginY);
		void ShiftOrigin (int iDeltaX, int iDeltaY);
		RECT GetAsRect (void);
		void SetWithRect (RECT rc);
		
		int GetEdgePosition (DWORD dwEdge);

		void SetEdgePosition (DWORD dwEdge, int iPosition);
		void ShiftEdgePosition (DWORD dwEdge, int iShift);

		inline void SetWidth (int iWidth) { m_iWidth = iWidth; }
		inline void ChangeWidthBy (int iChange) { m_iWidth += iChange;  }

		inline void SetHeight (int iHeight) { m_iHeight = iHeight; }
		inline void ChangeHeightBy (int iChange) { m_iHeight += iChange;  }

	private:
		IPanel &m_AssociatedPanel;
		int m_iOriginX;
		int m_iOriginY;
		int m_iHeight;
		int m_iWidth;
	};

//  =======================================================================

class CPanelOrganizer
	{
	friend IPanel;
	friend CPanelRect;

	public:
		CPanelOrganizer (IPanel &ParentPanel);
		virtual ~CPanelOrganizer (void);

		virtual int GetCount (void);
		virtual TArray <IPanel *> GetPanels (void);
		virtual inline IPanel *GetPanel(int iPanelIndex) { return m_aPanels[iPanelIndex]; }

		virtual void ShiftAllOrigins (int ShiftX, int iShiftY);

		virtual TArray <IPanelContent *> GetPanelContents (void);
		virtual TArray <IPanelContent *> GetPanelContentsContainingPoint (int x, int y);

		virtual int GetPanelIndex (IPanel *pPanel);
		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		inline int GetCount (void) { return m_aPanels.GetCount(); }
		inline TArray <IPanel *> GetPanels (void) { return m_aPanels; }

		TArray <IPanelContent *> GetPanelContents (void);
		TArray <IPanelContent *> GetPanelContentsContainingPoint (int x, int y);

		int GetPanelIndex (IPanel *pPanel);

		void HidePanel (IPanel *pPanel);
		void ShowPanel (IPanel *pPanel);
		void HidePanel (int iPanelIndex);
		void ShowPanel (int iPanelIndex);
		void HideAll (void);
		void ShowAll (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void SmoothOut (DWORD dwSmoothType);
		IPanel *AddPanel(int iRelativeOriginX, int iRelativeOriginY, int iWidth, int iHeight, bool bHidden, CString sPanelConfiguration);
		void DeletePanel (int iPanelIndex);

		TArray <IPanel *> Split (CString sSplitType, int iSeparatorPos);
		void ReverseSplit (int iPanelIndex);
		void ReverseSplit (IPanel *pPanel);

	protected:
		char m_cPanelConfigType;
		TArray <int> SortByPanelRectEdgeLocation (DWORD dwEdge);
		IPanel &m_ParentPanel;
		TArray <IPanel *> m_aPanels;
		TArray <IPanel *> m_aLeafPanels;
	};

//  =======================================================================

class IPanel : public IHISession
	{
	friend class CPanelRect;
	friend class CPanelOrganizer;

	public:
		IPanel (void);
		IPanel (int iOriginX, int iOriginY, int iWidth, int iHeight, CString sPanelConfiguration);
		IPanel (IPanelContent *pAssociatedSession, int iOriginX, int iOriginY, int iWidth, int iHeight, CString sPanelConfiguration);
		~IPanel (void);

		inline void SetParentPanel (IPanel *pPanel) { m_pParentPanel = pPanel; }
		inline IPanel *GetParentPanel (void) { return m_pParentPanel; }

		inline void AssociateContent (IPanelContent *Content) { m_pAssociatedContent = Content; }
		inline IPanelContent *GetAssociatedContent (void) { return m_pAssociatedContent; }

		inline bool ErrorOccurred (void) { return m_bErrorOccurred; }

		void Hide (void);
		void Show (void);
		inline void SetHiddenFlag (bool bHidden) { m_bHidden = bHidden; }
		inline bool IsHidden (void) { return m_bHidden; }

		inline bool IsEmpty (void) { if (m_pAssociatedContent == NULL) { return true; } else { return false; } }

		TArray <IPanelContent *> GetPanelContentsContainingPoint (int x, int y);
		IPanelContent *GetContentContainingPoint (int x, int y);
		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void Invalidate (void);

		void SetError (CString sErrorDescription) { m_bErrorOccurred = true; m_sErrorString = sErrorDescription; }

		CPanelRect PanelRect;
		CPanelOrganizer *InternalPanels;

		void SetViewOffset (int iOffsetX, int iOffsetY);

		inline int GetViewOffsetX (void) { return m_iViewOffsetX; }
		inline int GetViewOffsetY (void) { return m_iViewOffsetY; }

		void FitChildrenExactly (void);

		//  IHISession virtuals
		virtual void SetHeaderContent (void) {};
		virtual void UpdateHeaderContent (void) {};

		virtual void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		virtual void OnLButtonUp (int x, int y, DWORD dwFlags);
		virtual void OnLButtonDblClick (int x, int y, DWORD dwFlags, bool *retbCapture);

		virtual void OnRButtonDown (int x, int y, DWORD dwFlags);
		virtual void OnRButtonUp (int x, int y, DWORD dwFlags);

		void OnKeyDown (int iVirtKey, DWORD dwKeyData);
		void OnKeyUp (int iVirtKey, DWORD dwKeyData);
		void OnChar (char chChar, DWORD dwKeyData);

		inline bool IsLButtonDown (void) { return m_bLButtonDown; }
		inline bool IsLClicked (void) { return m_bLClicked; }
		inline bool IsLDblClicked (void) { return m_bLDblClicked; }

		inline bool IsRButtonDown (void) { return m_bRButtonDown; }
		inline bool IsRClicked (void) { return m_bRClicked; }

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid) { ; }

		// more Panel functions
		void DrawPanelOutline (CG32bitImage &Screen);
		inline void UpdatePanelOutlineColor (CG32bitPixel rgbColor) { m_rgbPanelOutlineColor = rgbColor; }

		inline IPanelContent *SetFocus (void) { m_bFocus = true; return this; }
		void RemoveFocus (void) { m_bFocus = false; }
		inline bool GetFocusStatus (void) { return m_bFocus; }

		inline void SetController (IHICommand *pController) { m_pController = pController; }
		inline CHumanInterface &GetHI (void) { return m_HI; }

		inline void SetCaptureStatus (bool bCapture) { m_bCapture = bCapture; }
		inline bool GetCaptureStatus (void) { return m_bCapture; }

	protected:
		IHICommand *m_pController;

		virtual void OnContentLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture) { ; }
		virtual void OnContentLButtonUp (int x, int y, DWORD dwFlags) { ; }
		virtual void OnContentLButtonDblClick (int x, int y, DWORD dwFlags, bool *retbCapture) { ; }

		virtual void OnContentRButtonDown (int x, int y, DWORD dwFlags) { ; }
		virtual void OnContentRButtonUp (int x, int y, DWORD dwFlags) { ; }

		virtual void OnContentKeyDown (int iVirtKey, DWORD dwKeyData) { ; }
		virtual void OnContentKeyUp (int iVirtKey, DWORD dwKeyData) { ; }
		virtual void OnContentChar (char chChar, DWORD dwKeyData) { ; }

	private:
		int m_iViewOffsetX;
		int m_iViewOffsetY;

		int m_iSeparatorPos;
		int m_iSeparatorWidth;

		IPanel *m_pParentPanel;

		bool m_bErrorOccurred;
		CString m_sErrorString;

		IPanelContent *m_pAssociatedContent;
		bool m_bHidden;

		inline bool GetCurrentStatusAndReset (bool &refBool) { bool bCurrentStatus = refBool; refBool = false; return bCurrentStatus; }

		bool m_bLButtonDown;
		bool m_bLClicked;
		bool m_bLDblClicked;

		bool m_bRButtonDown;
		bool m_bRClicked;

		bool m_bFocus;

		//  Panels have a background color
		CG32bitPixel m_rgbBackgroundColor;
		//  Panels can draw an outline around themselves with color m_rgbPanelOutlineColor
		CG32bitPixel m_rgbPanelOutlineColor;

		bool m_bCapture;

		CHeader *m_pHeader;
		CScrollBar *m_pScrollBar;
	};

//TransmuterException UndefinedEdgeError = TransmuterException(CONSTLIT(""));