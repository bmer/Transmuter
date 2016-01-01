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
class CPanel;
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
	friend CPanel;
	friend CInternalPanels;

	public:
		CPanelRect (CPanel &AssociatedPanel);
		CPanelRect (CPanel &AssociatedPanel, int iOriginX, int iOriginY, int iWidth, int iHeight);
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

		inline void SetWidth (int iWidth) { m_iWidth = iWidth; }
		inline void ChangeWidthBy (int iChange) { m_iWidth += iChange;  }

		inline void SetHeight (int iHeight) { m_iHeight = iHeight; }
		inline void ChangeHeightBy (int iChange) { m_iHeight += iChange;  }

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
		inline CPanel *GetPanel(int iPanelIndex) { return m_aPanels[iPanelIndex]; }
		void ShiftAllOrigins (int iShiftX, int iShiftY);

		void SmoothOut (DWORD dwSmoothType);

		CPanel *AddPanel (int iRelativeOriginX, int iRelativeOriginY, int iWidth, int iHeight, bool bHidden);

		void DeletePanel (int iPanelIndex);

		TArray <IPanelContent *> GetPanelContents (void);
		TArray <IPanelContent *> GetPanelContentsContainingPoint (int x, int y);

		int GetPanelIndex (CPanel *pPanel);

		void HidePanel (CPanel *pPanel);
		void ShowPanel (CPanel *pPanel);
		void HidePanel (int iPanelIndex);
		void ShowPanel (int iPanelIndex);
		void HideAll (void);
		void ShowAll (void);

		void InvalidateAll (void);
		void PaintAllContent (CG32bitImage &Screen, const RECT &rcInvalid);

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
		CPanel (int iOriginX, int iOriginY, int iWidth, int iHeight);
		CPanel (IPanelContent *pAssociatedSession, int iOriginX, int iOriginY, int iWidth, int iHeight);
		~CPanel (void);

		inline void SetParentPanel (CPanel *pPanel) { m_pParentPanel = pPanel; }
		inline CPanel *GetParentPanel (void) { return m_pParentPanel; }

		inline void AssociateSession (IPanelContent *Session) { m_pAssociatedContent = Session; }
		inline IPanelContent *GetAssociatedSession (void) { return m_pAssociatedContent; }

		inline bool ErrorOccurred (void) { return m_bErrorOccurred; }

		void Hide (void);
		void Show (void);
		inline void SetHiddenFlag (bool bHidden) { m_bHidden = bHidden; }
		inline bool IsHidden (void) { return m_bHidden; }

		inline bool IsEmpty (void) { if (m_pAssociatedContent == NULL) { return true; } else { return false; } }

		TArray <IPanelContent *> GetPanelContentsContainingPoint (int x, int y);
		IPanelContent *GetContentContainingPoint (int x, int y);
		void PaintContent (CG32bitImage &Screen, const RECT &rcInvalid);

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

		IPanelContent *m_pAssociatedContent;
		bool m_bHidden;

		bool m_bInvalid;					//  if panel is space is changed, then its made "invalid?"
	};

//TransmuterException UndefinedEdgeError = TransmuterException(CONSTLIT(""));