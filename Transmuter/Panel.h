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

class CPanelRect;
class CSChild;
class CPanel;
class CInternalPanels;

using namespace std;

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
	friend CPanel;
	friend CInternalPanels;

	public:
		CPanelRect (CPanel &AssociatedPanel);
		CPanelRect (CPanel &AssociatedPanel, int iOriginX, int iOriginY, int iHeight, int iWidth);
		CPanelRect (CPanel &AssociatedPanel, RECT rc);

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

		inline void ChangeHeight (int iHeight) { m_iHeight = iHeight; }
		inline void ChangeWidth (int iWidth) { m_iWidth = iWidth; }

	private:
		CPanel &m_AssociatedPanel;
		int m_iOriginX;
		int m_iOriginY;
		int m_iHeight;
		int m_iWidth;
	};

//  =======================================================================

class CInternalPanels
	{
	friend CPanel;
	friend CPanelRect;

	public:
		CInternalPanels (CPanel &ParentPanel);
		~CInternalPanels (void);

		inline int GetCount (void) { return m_aPanels.GetCount(); }
		inline TArray <CPanel *> GetPanels (void) {return m_aPanels; }

		void SetPanelEdgeAt (int iPanelIndex, DWORD dwEdge, int iLocation);
		void ShiftAllOrigins (int iShiftX, int iShiftY);

		void SmoothOut (DWORD dwSmoothType);

		CPanel *AddPanel (int iRelativeOriginX, int iRelativeOriginY, int iWidth, int iHeight, bool bHidden);

		void DeletePanel (int iPanelIndex);

		TArray <CSChild *> GetAssociatedSessions (void);
		TArray <CSChild *> GetSessionsContainingPoint (int x, int y);

		int GetPanelIndex (CPanel *pPanel);

		void HidePanel (CPanel *pPanel);
		void ShowPanel (CPanel *pPanel);
		void HidePanel (int iPanelIndex);
		void ShowPanel (int iPanelIndex);
		void HideAll (void);
		void ShowAll (void);

		void InvalidateAll (void);
		void PaintAll (CG32bitImage &Screen, const RECT &rcInvalid);

	protected:
		TArray <int> SortByPanelRectEdgeLocation (DWORD dwEdge);

	private:
		CPanel &m_ParentPanel;
		TArray <CPanel *> m_aPanels;
	};

class CPanel 
	{
	friend class CPanelRect;
	friend class CInternalPanels;

	public:
		CPanel (void);

		void InitScreenArea (void);
		void InitScreenArea (RECT rcArea);
		
		inline void SetParentPanel(CPanel *pPanel) { m_pParentPanel = pPanel; }
		inline CPanel *GetParentPanel(void) { return m_pParentPanel; }

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

		CPanelRect PanelRect;
		CInternalPanels InternalPanels;

		void SetViewOffset (int iOffsetX, int iOffsetY);

		inline int GetViewOffsetX (void) { return m_iViewOffsetX; }
		inline int GetViewOffsetY (void) { return m_iViewOffsetY; }

		void FitChildrenExactly (void);

	private:
		int m_iViewOffsetX;
		int m_iViewOffsetY;

		CPanel *m_pParentPanel;
		
		bool m_bErrorOccurred;
		CString m_sErrorString;

		CSChild *m_pAssociatedSession;

		int m_bFocus;
		bool m_bHidden;

		bool m_bInvalid;					//  if panel is space is changed, then its made "invalid?"
	};