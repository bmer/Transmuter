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

class CSubSession;
class CPanel;

//  =======================================================================

class CPanel 
	{
	public:
		CPanel (void);
		CPanel (RECT rcPanel);
		CPanel (int iLeft, int iTop, int iRight, int iBottom);
		CPanel (RECT rcPanel, double dRigidity);
		CPanel (int iLeft, int iTop, int iRight, int iBottom, double dRigidity);

		~CPanel (void);

		void SetPanelEdgeLocation (DWORD dwEdge, int iLocation);
		void ShiftPanelEdgeLocation (DWORD dwEdge, int iShift);
		int GetPanelEdgeLocation (DWORD dwEdge);

		inline int GetPanelWidth (void) { return RectWidth(m_rcPanel); }
		inline int GetPanelHeight (void) { return RectHeight(m_rcPanel); }
		inline double GetRigidity (void) { return m_dRigidity; }

		inline void SetParentPanel(CPanel *pPanel) { m_pParentPanel = pPanel; }
		inline CPanel *GetParentPanel(void) { return m_pParentPanel; }

		TArray <int> SortInternalPanelsByEdgeLocation (DWORD dwFlag);

		inline void UpdateNumInternalPanels (void) { m_iNumInternalPanels = m_aInternalPanels.GetCount(); }
		inline void SetAsFixed (int iRelativeLeft, int iRelativeTop ) { if (m_pParentPanel != NULL) { m_bFixed = true; m_iRelativeLeft = iRelativeLeft; m_iRelativeTop = iRelativeTop; } };
		inline bool IsFixed (void) { return m_bFixed; }
		inline int GetFixedDeltaLeft (void) { return m_iRelativeLeft; }
		inline int GetFixedDeltaTop (void) { return m_iRelativeTop; }
		inline void RemoveAsFixed (void) { m_bFixed = false; }
		
		void SmoothOutInternalPanels(bool bExpandInPlace, DWORD dwSmoothType);

		CPanel* AddInternalPanel (int iLeft, int iTop, int iRight, int iBottom, double dRigidity, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent);
		CPanel* AddInternalPanel (int iLeft, int iTop, int iRight, int iBottom, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent);
		CPanel* AddInternalPanelRelativeToOrigin (int iDeltaX, int iDeltaY, int iWidth, int iHeight, double dRigidity, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent);
		CPanel* AddInternalPanelRelativeToOrigin (int iDeltaX, int iDeltaY, int iWidth, int iHeight, bool bHidden, bool bExpandInPlace, bool bFixedRelativeToParent);

		inline TArray <CPanel *> GetInternalPanels (void) { return m_aInternalPanels; }

		inline void AssociateSession(CSubSession *Session) { m_pAssociatedSession = Session; }
		inline CSubSession *GetAssociatedSession(void) { return m_pAssociatedSession; }

		TArray <CSubSession *> GetInternalPanelSessions (void);

		inline int GetXDisplacementToLeftEdge(int x) { return (x - m_rcPanel.left); }
		inline int GetXDisplacementToRightEdge(int x) { return (x - m_rcPanel.right); }
		inline int GetYDisplacementToTopEdge(int y) { return (y - m_rcPanel.top); }
		inline int GetYDisplacementToBottomEdge(int y) { return (y - m_rcPanel.bottom); }

		inline void FocusOnInternalPanel (int iPanelIndex) { m_aInternalPanels[iPanelIndex]->m_bFocus = 1; }
		inline void RemoveFocusFromInternalPanel (int iPanelIndex) { m_aInternalPanels[iPanelIndex]->m_bFocus = 0; }

		inline RECT GetPanelRect (void) { return m_rcPanel; }
		RECT GetScaledInnerRect (double scale);

		inline bool ErrorOccurred (void) { return m_bErrorOccurred; }

		int GetInternalPanelIndex (CPanel *pPanel);
		void HideInternalPanel (int iPanelIndex);
		void ShowInternalPanel (int iPanelIndex);

		void Hide (void);
		void Show (void);
		inline void SetHiddenFlag (bool bHidden) { m_bHidden = bHidden; }
		inline bool IsHidden (void) { return m_bHidden; }

		inline bool IsEmpty (void) { if (m_pAssociatedSession == NULL) { return true; } else { return false; } }
		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		TArray <CSubSession *> ReturnSessionsContainingPoint (int x, int y);

	private:
		RECT m_rcPanel;

		double m_dRigidity;					//  dRigidity of panel

		CPanel *m_pParentPanel;
		TArray <CPanel *> m_aInternalPanels;
		int m_iNumInternalPanels;

		bool m_bFixed;
		int m_iRelativeLeft;
		int m_iRelativeTop;

		bool m_bErrorOccurred;
		CString m_sErrorString;

		CSubSession *m_pAssociatedSession;

		int m_bFocus;
		bool m_bHidden;
	};

double CalculateRelativeRigidity(double rigidity1, double rigidity2);