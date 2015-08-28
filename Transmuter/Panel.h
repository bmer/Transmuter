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

		void SetPanelOrigin (int xO, int yO);
		void ShiftPanelOrigin (int DeltaX, int DeltaY);
		void SetPanelSpace (int width, int height);

		void SetOriginX (int xO);
		void SetOriginY (int yO);
		void SetPanelWidth (int width);
		void SetPanelHeight (int height);

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

		TArray <int> LeftEdgeLocationInternalPanelSort (void);
		TArray <int> TopEdgeLocationInternalPanelSort (void);
		TArray <int> RightEdgeLocationInternalPanelSort (void);
		TArray <int> BottomEdgeLocationInternalPanelSort (void);

		inline void UpdateNumInternalPanels (void) { m_NumInternalPanels = m_InternalPanels.GetCount(); }
		inline void SetAsFixed (int Relative_xO, int Relative_yO ) { if (m_ParentPanel != NULL) { m_Fixed = true; m_Relative_xO = Relative_xO; m_Relative_yO = Relative_yO; } };
		inline bool IsFixed (void) { return m_Fixed; }
		inline int GetFixedDeltaX (void) { return m_Relative_xO; }
		inline int GetFixedDeltaY (void) { return m_Relative_yO; }
		inline void RemoveAsFixed (void) { m_Fixed = false; }
		
		void SmoothOutInternalPanels(bool bExpandInPlace, bool bUpDownSmooth);

		CPanel* AddInternalPanel (int xO, int yO, int width, int height, double rigidity, bool hidden, bool ExpandInPlace, bool FixedRelativeToParent);
		CPanel* AddInternalPanel (int xO, int yO, int width, int height, bool hidden, bool ExpandInPlace, bool FixedRelativeToParent);
		CPanel* AddInternalPanelRelativeToOrigin (int DeltaX, int DeltaY, int width, int height, double rigidity, bool hidden, bool ExpandInPlace, bool FixedRelativeToParent);
		CPanel* AddInternalPanelRelativeToOrigin (int DeltaX, int DeltaY, int width, int height, bool hidden, bool ExpandInPlace, bool FixedRelativeToParent);

		inline TArray <CPanel *> GetInternalPanels (void) { return m_InternalPanels; }

		inline void AssociateSession(CSubSession *Session) { m_AssociatedSession = Session; }
		inline CSubSession *GetAssociatedSession(void) { return m_AssociatedSession; }

		TArray <CSubSession *> GetInternalPanelSessions (void);

		inline int GetXDisplacementToLeftEdge(int x) { return (x - m_xO); }
		inline int GetXDisplacementToRightEdge(int x) { return (x - (m_xO + m_Width)); }
		inline int GetYDisplacementToTopEdge(int y) { return (y - m_yO); }
		inline int GetYDisplacementToBottomEdge(int y) { return (y - (m_yO + m_Height)); }

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
		int m_NumInternalPanels;

		bool m_Fixed;
		int m_Relative_xO;
		int m_Relative_yO;

		bool m_ErrorOccurred;
		CString m_ErrorString;

		CSubSession *m_AssociatedSession;

		int m_Focus;
		bool m_Hidden;
	};

double CalculateRelativeRigidity(double rigidity1, double rigidity2);