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

		inline int SetOriginX (int xO) { m_xO = xO; }
		inline int SetOriginY (int yO) { m_yO = yO; }
		inline int SetPanelWidth (int width) { m_Width = width; }
		inline int SetPanelHeight (int height) { m_Height = height; }

		inline int GetOriginX (void) { return m_xO; }
		inline int GetOriginY (void) { return m_yO; }
		inline int GetWidth (void) { return m_Width; }
		inline int GetHeight (void) { return m_Height; }

		inline double GetRigidity (void) { return m_Rigidity; }

		void ShiftTopEdge (int Delta);
		void ShiftRightEdge (int Delta);
		void ShiftBottomEdge (int Delta);
		void ShiftLeftEdge (int Delta);

		CPanel* AddInternalPanel (int xO, int yO, int width, int height, double rigidity);
		CPanel* AddInternalPanel (int xO, int yO, int width, int height);
		CPanel* AddInternalPanelRelativeToOrigin (int DeltaX, int DeltaY, int width, int height, double rigidity);
		CPanel* AddInternalPanelRelativeToOrigin (int DeltaX, int DeltaY, int width, int height);

		inline TArray <CPanel *> GetInternalPanels (void) { return m_InternalPanels; }

		inline void AssociateSession(CSubSession *Session) { m_AssociatedSession = Session; }
		inline CSubSession *GetAssociatedSession(void) { return m_AssociatedSession; }

		TArray <CSubSession *> GetInternalPanelSessions (void);

		inline int GetXDisplacementToLeftEdge(int x) { return (x - m_xO); }
		inline int GetXDisplacementToRightEdge(int x) { return (x - (m_xO + m_Width)); }
		inline int GetYDisplacementToTopEdge(int y) { return (y - m_yO); }
		inline int GetYDisplacementToBottomEdge(int y) { return (y - (m_yO + m_Height)); }
		
		void ExpandInternalPanel (int PanelIndex);
		void ExpandAllInternalPanels (void);

		inline void FocusOnInternalPanel (int PanelIndex) { m_InternalPanels[PanelIndex]->m_Focus = 1; }
		inline void RemoveFocusFromInternalPanel (int PanelIndex) { m_InternalPanels[PanelIndex]->m_Focus = 0; }

		TArray <int> GetRectDefinition(void);

		inline bool ErrorOccurred(void) { return m_ErrorOccurred; }

		inline bool HidePanel(void) { m_HidePanel = TRUE; }
		inline bool ShowPanel(void) { m_HidePanel = FALSE; }

		inline bool IsEmpty(void) { if (m_AssociatedSession == NULL) { return true; } else { return false; } }
		void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

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
		bool m_HidePanel;
	};

