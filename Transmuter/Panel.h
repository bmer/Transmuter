//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

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
		void SetPanelSpace (int SpaceWidth, int SpaceHeight);
		int GetOriginX (void) { return m_xO; }
		int GetOriginY (void) { return m_yO; }
		int GetSpaceWidth (void) { return m_SpaceWidth; }
		int GetSpaceHeight (void) { return m_SpaceHeight; }

		CPanel* AddInternalPanel (int xO, int yO, int SpaceWidth, int SpaceHeight, double Rigidity);
		CPanel* AddInternalPanel (int xO, int yO, int SpaceWidth, int SpaceHeight);

		TArray <CPanel *> GetInternalPanels (void);

		void AssociateSession(CSubSession *Session);

		TArray <CSubSession *> GetInternalPanelSessions (void);

		void ExpandInternalPanels (void);

		void FocusOnInternalPanel (int PanelIndex);
		void RemoveFocusFromInternalPanel (int PanelIndex);

		TArray <CSubSession *> CreateErrorSessions(CHumanInterface &HI);

		TArray <int> GetRectDefinition(void);

		inline bool ErrorOccurred(void) {return m_ErrorOccurred; }

	private:
		int m_xO;							//  top left corner x-coordinate
		int m_yO;							//	top left corner y-coordinate
		int m_SpaceWidth;					//  width of panel
		int m_SpaceHeight;					//	height of panel

		TArray <CPanel *> InternalPanels;

		bool m_ErrorOccurred;
		CString m_ErrorString;

		CSubSession *m_AssociatedSession;

		bool m_IsFocus;
	};

