//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CSubSession;
class CPanel;

//  =======================================================================

class CPanel 
	{
	public:
		CPanel (void);
		CPanel (int xO, int yO, int SpaceWidth, int SpaceHeight);
		CPanel (int xO, int yO, int SpaceWidth, int SpaceHeight, double Rigidity);

		~CPanel (void);

		inline void SetPanelOrigin (int xO, int yO) { m_xO = xO; m_yO = yO; }
		inline void ShiftPanelOrigin (int DeltaX, int DeltaY) { m_xO = m_xO + DeltaX; m_yO = m_yO + DeltaY; }
		void SetPanelSpace (int width, int height);

		inline void SetOriginX (int xO) { m_xO = xO; }
		inline void SetOriginY (int yO) { m_yO = yO; }
		inline void SetPanelWidth (int width) { m_Width = width; }
		inline void SetPanelHeight (int height) { m_Height = height; }

		inline int GetOriginX (void) { return m_xO; }
		inline int GetOriginY (void) { return m_yO; }
		inline int GetWidth (void) { return m_Width; }
		inline int GetHeight (void) { return m_Height; }

		inline double GetRigidity (void) { return m_Rigidity; }

		void ShiftTopEdge (int Delta);
		void ShiftRightEdge (int Delta);
		void ShiftBottomEdge (int Delta);
		void ShiftLeftEdge (int Delta);

		inline void SetParentPanel(CPanel *Panel) { m_ParentPanel = Panel; }
		void MakeSpaceForPanel (CPanel *FocusPanel, bool ExpandInPlace);
		CPanel* AddInternalPanel (int xO, int yO, int width, int height, double rigidity, bool hidden, bool ExpandInPlace);
		CPanel* AddInternalPanel (int xO, int yO, int width, int height, bool hidden, bool ExpandInPlace);
		CPanel* AddInternalPanelRelativeToOrigin (int DeltaX, int DeltaY, int width, int height, double rigidity, bool hidden, bool ExpandInPlace);
		CPanel* AddInternalPanelRelativeToOrigin (int DeltaX, int DeltaY, int width, int height, bool hidden, bool ExpandInPlace);

		inline TArray <CPanel *> GetInternalPanels (void) { return m_InternalPanels; }

		inline void AssociateSession(CSubSession *Session) { m_AssociatedSession = Session; }
		inline CSubSession *GetAssociatedSession(void) { return m_AssociatedSession; }

		TArray <CSubSession *> GetInternalPanelSessions (void);

		inline int GetXDisplacementToLeftEdge(int x) { return (x - m_xO); }
		inline int GetXDisplacementToRightEdge(int x) { return (x - (m_xO + m_Width)); }
		inline int GetYDisplacementToTopEdge(int y) { return (y - m_yO); }
		inline int GetYDisplacementToBottomEdge(int y) { return (y - (m_yO + m_Height)); }
		
		void SnuglyFitInternalPanel (int PanelIndex, bool ExpandInPlace);
		void RearrangeAllPanelsToSnuglyFit(int MaxAttempts, bool ExpandInPlace);

		inline void FocusOnInternalPanel (int PanelIndex) { m_InternalPanels[PanelIndex]->m_Focus = 1; }
		inline void RemoveFocusFromInternalPanel (int PanelIndex) { m_InternalPanels[PanelIndex]->m_Focus = 0; }

		RECT GetPanelRect (void);
		RECT GetScaledInnerRect (double scale);

		inline bool ErrorOccurred (void) { return m_ErrorOccurred; }

		int GetInternalPanelIndex (CPanel *Panel);
		void HideInternalPanel (int PanelIndex);
		void ShowInternalPanel (int PanelIndex);

		void Hide(void);
		void Show(void);
		inline void SetHiddenFlag (bool hidden) { m_Hidden = hidden; }
		inline bool IsHidden (void) { return m_Hidden; }

		inline bool IsEmpty(void) { if (m_AssociatedSession == NULL) { return true; } else { return false; } }
		void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

		TArray <CSubSession *> ReturnSessionsContainingPoint(int x, int y);

	private:
		int m_xO;							//  top left corner x-coordinate
		int m_yO;							//	top left corner y-coordinate
		int m_Width;						//  width of panel
		int m_Height;						//	height of panel

		double m_Rigidity;					//  rigidity of panel

		CPanel *m_ParentPanel;
		TArray <CPanel *> m_InternalPanels;

		bool m_ErrorOccurred;
		CString m_ErrorString;

		CSubSession *m_AssociatedSession;

		int m_Focus;
		bool m_Hidden;
	};

double CalculateRelativeRigidity(double rigidity1, double rigidity2);