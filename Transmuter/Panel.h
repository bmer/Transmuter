//	TransmuterPanelContent.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#define EDGE_LEFT			0
#define EDGE_TOP			1
#define EDGE_RIGHT			2
#define EDGE_BOTTOM			3

#define SMOOTH_UPDOWN		0
#define SMOOTH_LEFTRIGHT	1

#define ORG_LIST	0
#define ORG_TREE	1

class CPanelRect;
class CPanelOrganizer;
class IPanel;

class CPanelRect
	{
	friend IPanel;
	friend CPanelOrganizer;

	public:
		CPanelRect (IPanel &AssociatedPanel);
		CPanelRect (IPanel &AssociatedPanel, int iOriginX, int iOriginY, int iWidth, int iHeight);
		CPanelRect (IPanel &AssociatedPanel, int iWidth, int iHeight);
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

		virtual void ShiftAllOrigins (int ShiftX, int iShiftY);

		virtual inline int GetCount (void) { return m_aPanels.GetCount(); }
		virtual inline TArray <IPanel *> GetPanels (void) { return m_aPanels; }

		virtual TArray <IPanel *> GetPanelsContainingPoint (int x, int y);
		virtual int GetPanelIndex (IPanel *pPanel);
		virtual inline IPanel *GetPanel(int iPanelIndex) { return m_aPanels[iPanelIndex]; }

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void HidePanel (IPanel *pPanel);
		void ShowPanel (IPanel *pPanel);
		void HidePanel (int iPanelIndex);
		void ShowPanel (int iPanelIndex);
		void HideAll (void);
		void ShowAll (void);

		void Invalidate (void);

		void SmoothOut (DWORD dwSmoothType);
		bool PlacePanel (IPanel *pPanel, int iRelativeOriginX, int iRelativeOriginY);
		bool PlacePanel (IPanel *pPanel, char cSplitDirn, float fSeparatorPos=0.5, int iSeparatorThickness=5);
		void DeletePanel (int iPanelIndex);

	protected:
		// methods
		TArray <int> SortByPanelRectEdgeLocation (DWORD dwEdge);

		// member variables
		int m_iPanelConfigType;
		char m_cSplitDirn;

		IPanel &m_ParentPanel;
		TArray <IPanel *> m_aPanels;
		TArray <IPanel *> m_aLeafPanels;

		int m_iSeparatorPos;
		int m_iSeparatorThickness;
	};

//  =======================================================================

class IPanel : public IHISession
	{
	friend class CPanelRect;
	friend class CPanelOrganizer;

	public:
		IPanel (CString sContentName, CHumanInterface &HI);
		IPanel (CString sName, CHumanInterface &HI, int iWidth, int iHeight);
		IPanel(CString sName, CHumanInterface & HI, int iOriginX, int iOriginY, int iWidth, int iHeight);
		~IPanel (void);

		void PaintBackground (CG32bitImage & Screen);
		void PaintOutline (CG32bitImage & Screen);
		inline void UpdateOutlineColor (CG32bitPixel rgbColor) { m_rgbOutlineColor = rgbColor; }

		inline void SetParentPanel (IPanel *pPanel) { m_pParentPanel = pPanel; }
		inline IPanel *GetParentPanel (void) { return m_pParentPanel; }

		inline bool ErrorOccurred (void) { return m_bErrorOccurred; }

		void Hide (void);
		void Show (void);
		inline void SetHiddenFlag (bool bHidden) { m_bHidden = bHidden; }
		inline bool IsHidden (void) { return m_bHidden; }

		TArray <IPanel *> GetPanelsContainingPoint (int x, int y);
		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void Invalidate (void);

		void SetError (CString sErrorDescription) { m_bErrorOccurred = true; m_sErrorString = sErrorDescription; }

		CPanelRect PanelRect;
		CPanelOrganizer PanelOrganizer;

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

		// more Panel methods
		inline IPanel *SetFocus (void) { m_bFocus = true; return this; }
		void RemoveFocus (void) { m_bFocus = false; }
		inline bool GetFocusStatus (void) { return m_bFocus; }

		inline void SetController (IHICommand *pController) { m_pController = pController; }
		inline CHumanInterface &GetHI (void) { return m_HI; }

		inline void SetCaptureStatus (bool bCapture) { m_bCapture = bCapture; }
		inline bool GetCaptureStatus (void) { return m_bCapture; }

		bool IsPlaced (void) { return m_bIsPlaced; }
		CString GetName (void) { return m_sName; }
		CString CreateSubPanelName (CString sSubPanelNamePart);

	protected:
		IHICommand *m_pController;

		virtual void OnPanelLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture) { ; }
		virtual void OnPanelLButtonUp (int x, int y, DWORD dwFlags) { ; }
		virtual void OnPanelLButtonDblClick (int x, int y, DWORD dwFlags, bool *retbCapture) { ; }

		virtual void OnPanelRButtonDown (int x, int y, DWORD dwFlags) { ; }
		virtual void OnPanelRButtonUp (int x, int y, DWORD dwFlags) { ; }

		virtual void OnPanelKeyDown (int iVirtKey, DWORD dwKeyData) { ; }
		virtual void OnPanelKeyUp (int iVirtKey, DWORD dwKeyData) { ; }
		virtual void OnPanelChar (char chChar, DWORD dwKeyData) { ; }

		virtual void OnPanelPaint (CG32bitImage &Screen, const RECT &rcInvalid) { ; }

		void ConfirmPlacement (void) { m_bIsPlaced = true; }
	private:
		// methods
		inline bool GetCurrentStatusAndReset (bool &refBool) { bool bCurrentStatus = refBool; refBool = false; return bCurrentStatus; }

		// member variables
		CString m_sName;
		IPanel *m_pParentPanel;

		bool m_bErrorOccurred;
		CString m_sErrorString;
		
		bool m_bHidden;

		int m_iViewOffsetX;
		int m_iViewOffsetY;

		bool m_bLButtonDown;
		bool m_bLClicked;
		bool m_bLDblClicked;

		bool m_bRButtonDown;
		bool m_bRClicked;

		bool m_bFocus;

		CG32bitPixel m_rgbBackgroundColor;
		CG32bitPixel m_rgbOutlineColor;

		bool m_bCapture;

		bool m_bIsPlaced;
	};

