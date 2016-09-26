//	Panel.h
//
//	Transmuter
//	Copyright (c) 2016 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#define EDGE_LEFT			0
#define EDGE_TOP			1			
#define EDGE_RIGHT			2
#define EDGE_BOTTOM			3

#define ORG_LIST			0
#define ORG_TREE			1
#define ORG_NONE			2

class CPanelRect;
class CPanelOrganizer;
class IPanel;

class CPanelRect
	{
	friend IPanel;
	friend CPanelOrganizer;

	public:
		CPanelRect (IPanel &AssociatedPanel, int iWidth=0, int iHeight=0);
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

		inline bool HasZeroDimension (void) { return (m_iWidth == 0 || m_iHeight == 0); }

	private:
		IPanel &m_AssociatedPanel;
		int m_iOriginX;
		int m_iOriginY;
		int m_iHeight;
		int m_iWidth;
	};

//  =======================================================================

class IPanel : public IHISession
	{
	friend class CPanelRect;

	public:
		IPanel (CString sName, CHumanInterface &HI, int iWidth=0, int iHeight=0);
		~IPanel (void);

		inline void SetParentPanel (IPanel *pPanel) { m_pParentPanel = pPanel; }
		inline IPanel *GetParentPanel (void) { return m_pParentPanel; }

		inline bool ErrorOccurred (void) { return m_bErrorOccurred; }

		void Hide (void);
		void Show (void);
		inline void SetHiddenFlag (bool bHidden) { m_bHidden = bHidden; }
		inline bool IsHidden (void) { return m_bHidden; }

		void Invalidate (void);

		void SetError (CString sErrorDescription) { m_bErrorOccurred = true; m_sErrorString = sErrorDescription; }

		CPanelRect PanelRect;

		void SetViewOffset (int iOffsetX, int iOffsetY);

		inline int GetViewOffsetX (void) { return m_iViewOffsetX; }
		inline int GetViewOffsetY (void) { return m_iViewOffsetY; }

		//  IHISession virtuals
		virtual void SetHeaderContent (void) {};
		virtual void UpdateHeaderContent (void) {};

		/* put all functions that shouldn't be overriden under one heading, and put the remaining under another*/
		virtual void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture) override;
		virtual void OnContentLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture) {}
		virtual void OnLButtonUp (int x, int y, DWORD dwFlags) override;
		virtual void OnContentLButtonUp (int x, int y, DWORD dwFlags) {}
		virtual void OnLButtonDblClick (int x, int y, DWORD dwFlags) override;
		virtual void OnContentLButtonDblClick (int x, int y, DWORD dwFlags) {}
		virtual void OnRButtonDown (int x, int y, DWORD dwFlags) override;
		virtual void OnContentRButtonDown (int x, int y, DWORD dwFlags) {}
		virtual void OnRButtonUp (int x, int y, DWORD dwFlags) override;
		virtual void OnContentRButtonUp (int x, int y, DWORD dwFlags) {}

		void OnKeyDown (int iVirtKey, DWORD dwKeyData) override;
		void OnKeyUp (int iVirtKey, DWORD dwKeyData) override;
		void OnChar (char chChar, DWORD dwKeyData) override;

		void OnContentKeyDown (int iVirtKey, DWORD dwKeyData) {}
		void OnContentKeyUp (int iVirtKey, DWORD dwKeyData) {}
		void OnContentChar (char chChar, DWORD dwKeyData) {}

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid) override;
		virtual void OnContentPaint (CG32bitImage &Screen, const RECT &rcInvalid) {}
		void UpdateOutlineColorGivenFocusStatus(CG32bitPixel rgbNoFocus = CG32bitPixel(255, 0, 0), CG32bitPixel rgbFocus = CG32bitPixel(255, 255, 255));

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
		inline IHICommand *GetController (void) { return m_pController; }
		inline CHumanInterface &GetHI (void) { return m_HI; }

		inline void SetCaptureStatus (bool bCapture) { m_bCapture = bCapture; }
		inline bool GetCaptureStatus (void) { return m_bCapture; }

		bool IsPlaced (void) { return m_bIsPlaced; }
		CString GetName (void) { return m_sName; }
		CString CreateSubPanelName (CString sSubPanelNamePart);

		void ConfirmPlacement (void) { m_bIsPlaced = true; }

		virtual int GetPanelCount (void) { return 0; }
		virtual TArray <IPanel *> GetPanels (void) { return TArray <IPanel *> (); }
		virtual TArray <IPanel *> GetPanelsContainingPoint (int x, int y) { return TArray <IPanel *> (); }
		virtual int GetPanelIndex (IPanel *pPanel) { return -1; }
		virtual IPanel *GetPanel(int iPanelIndex) { return NULL; }

		virtual void HidePanel (IPanel *pPanel) { }
		virtual void ShowPanel (IPanel *pPanel) { }
		virtual void HidePanel (int iPanelIndex) { }
		virtual void ShowPanel (int iPanelIndex) { }
		virtual void HideAll (void) { }
		virtual void ShowAll (void) { }

		virtual void DeletePanel (int iPanelIndex) {}

		inline bool HasParent (void) { if (m_pParentPanel != NULL) { return true; } return false; }
		inline IPanel *GetParent (void) { return m_pParentPanel; }

	private:
		void PaintBackground (CG32bitImage & Screen);
		void PaintOutline (CG32bitImage & Screen);
		inline void UpdateOutlineColor (CG32bitPixel rgbColor) { m_rgbOutlineColor = rgbColor; }

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

		IHICommand *m_pController;
	};

