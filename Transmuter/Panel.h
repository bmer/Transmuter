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

		TArray <CPanel *> GetInternalPanels (void);

		bool MatchesRequirements(int RequiredSpaceWidth, int RequiredSpaceHeight);
		void AssociateSession(CSubSession *Session);

		TArray <CSubSession *> GetAllAssociatedSessions (void);

		void ResizePanels (void);

		void SetFocus (void) { m_IsFocus = TRUE; }
		void RemoveFocus (void) { m_IsFocus = FALSE; }

		TArray <CSubSession *> CreateErrorSessions(CHumanInterface &HI);

		TArray <int> GetRectCoords(void);

		inline bool ErrorOccurred(void) {return m_ErrorOccurred; }

	private:
		int m_xO;							//  top left corner x-coordinate
		int m_yO;							//	top left corner y-coordinate
		int m_SpaceWidth;					//  width of panel
		int m_SpaceHeight;					//	height of panel

		bool m_SeparatorExists;
		bool m_MaintainSeparatorFactor;
		int m_SeparatorDirection;			//  0 for separator parallel to x-direction, 1 for y-direction
		double m_SeparatorFactor;			//  fraction of SpaceWidth or SpaceHeight, depending on direction
		int m_SeparatorLocation;			//	SeparatorFactor*SpaceWidth or SeparatorFactor*SpaceHeight depending on direction
		
		bool m_ErrorOccurred;
		CString m_ErrorString;

		CElasticPanel *m_SubEPAlpha;
		CElasticPanel *m_SubEPBeta;

		CSubSession *m_AssociatedSession;

		bool m_IsFocus;
	};

