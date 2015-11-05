//	TransmuterSession.h
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

class CSChild;
class CScreenSpace;
class CPanel;
class CInternalPanels;

//  =======================================================================

class CPanelArea
	{
	friend CPanel;
	friend CInternalPanels;

	public:
		CPanelArea (CPanel &AssociatedPanel);
		CPanelArea (CPanel &AssociatedPanel, RECT rcArea);

		inline void SetRect (RECT rcArea) { m_rcArea = rcArea; }
		RECT GetRect (void);

		int GetEdgeLocation (DWORD dwEdge);

		void SetEdgeTo (DWORD dwEdge, int iNewLocation);
		void MoveAreaByEdgeTo (DWORD dwEdge, int iShift);

		inline int CalculateDisplacementToEdge (DWORD dwEdge, int x);
		void ExpandRectToFitChildren(void);

	private:
		CPanel &m_AssociatedPanel;
		RECT m_rcArea;
	};

class CInternalPanels
	{
	friend CPanel;
	friend CPanelArea;

	public:
		CInternalPanels (CPanel &ParentPanel);
		~CInternalPanels (void);

		inline int GetCount (void) { return m_aPanels.GetCount(); }
		inline TArray <CPanel *> GetPanels (void) {return m_aPanels; }

		void SetEdgeAt (int iPanelIndex, DWORD dwEdge, int iLocation);
		void ShiftEdges (DWORD dwEdge, int iShift);

		void SmoothOut (DWORD dwSmoothType);

		CPanel *Add (int iRelativeLeft, int iRelativeTop, int iWidth, int iHeight, bool bHidden, bool bFixed);

		void Remove (int iPanelIndex);

		TArray <CSChild *> GetAssociatedSessions (void);
		TArray <CSChild *> GetSessionsContainingPoint (int x, int y);

		int GetPanelIndex (CPanel *pPanel);

		void HidePanel (CPanel *pPanel);
		void ShowPanel (CPanel *pPanel);
		void HidePanel (int iPanelIndex);
		void ShowPanel (int iPanelIndex);
		void HideAll (void);
		void ShowAll (void);

		void SetViewOffsetX (int iOffset);
		void SetViewOffsetY (int iOffset);

		void Invalidate (void);
		void PaintAll (CG32bitImage &Screen, const RECT &rcInvalid);

	protected:
		TArray <int> SortByScreenAreaEdgeLocation (DWORD dwEdge);

	private:
		CPanel &m_ParentPanel;
		TArray <CPanel *> m_aPanels;
	};

class CPanel 
	{
	friend class CPanelArea;
	friend class CInternalPanels;

	public:
		CPanel (void);
		CPanel (RECT rcScreenSpace);
		CPanel (int iLeft, int iTop, int iWidth, int iHeight);

		void InitScreenArea (void);
		void InitScreenArea (RECT rcArea);
		
		inline void SetParentPanel(CPanel *pPanel) { m_pParentPanel = pPanel; }
		inline CPanel *GetParentPanel(void) { return m_pParentPanel; }

	
		inline bool IsSticky (void) { return m_bSticky; }
		inline void MakeSticky (int iRelativeLeft, int iRelativeTop) { if (m_pParentPanel != NULL) { m_bSticky = true; m_iStickyLeftOffset = iRelativeLeft; m_iStickyTopOffset = iRelativeTop; } else { m_bSticky = false; } };
		inline int GetStickyLeftOffset (void) { return m_iStickyLeftOffset; }
		inline int GetStickyTopOffset (void) { return m_iStickyTopOffset; }
		inline void RemoveSticky (void) { m_bSticky = false; }

		inline void AssociateSession(CSChild *Session) { m_pAssociatedSession = Session; }
		inline CSChild *GetAssociatedSession (void) { return m_pAssociatedSession; }
		TArray <CSChild *> GetAllAssociatedSessions (void);

		inline bool ErrorOccurred (void) { return m_bErrorOccurred; }

		void HideInternalPanel (int iPanelIndex);
		void ShowInternalPanel (int iPanelIndex);

		void Hide (void);
		void Show (void);
		inline void SetHiddenFlag (bool bHidden) { m_bHidden = bHidden; }
		inline bool IsHidden (void) { return m_bHidden; }

		inline void SetFocus (bool bFocus) { m_bFocus = bFocus; }
		inline void RemoveFocus (void) { m_bFocus = false; }

		inline bool IsEmpty (void) { if (m_pAssociatedSession == NULL) { return true; } else { return false; } }

		TArray <CSChild *> GetSessionsContainingPoint (int x, int y);
		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void Invalidate (void);

		void SetError (CString sErrorDescription) { m_bErrorOccurred = true; m_sErrorString = sErrorDescription; }

		CPanelArea ScreenArea;
		CInternalPanels InternalPanels;

		inline void SetViewOffsetX (int iOffset) { m_iViewOffsetX = iOffset; InternalPanels.SetViewOffsetX(iOffset); }
		inline void SetViewOffsetY (int iOffset) { m_iViewOffsetY = iOffset; InternalPanels.SetViewOffsetY(iOffset); }

		inline int GetViewOffsetX (void) { return m_iViewOffsetX; }
		inline int GetViewOffsetY (void) { return m_iViewOffsetY; }

	private:
		int m_iViewOffsetX;
		int m_iViewOffsetY;

		bool m_bSticky;
		int m_iStickyLeftOffset;
		int m_iStickyTopOffset;

		CPanel *m_pParentPanel;
		
		bool m_bErrorOccurred;
		CString m_sErrorString;

		CSChild *m_pAssociatedSession;

		int m_bFocus;
		bool m_bHidden;

		bool m_bInvalid;					//  if panel is space is changed, then its made "invalid?"
	};