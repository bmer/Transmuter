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
		void SetPanelSpace (int width, int height);

		inline int SetOriginX (int xO) { m_xO = xO; }
		inline int SetOriginY (int yO) { m_yO = yO; }
		inline int SetPanelWidth (int width) { m_Width = width; }
		inline int SetPanelHeight (int height) { m_Height = height; }

		inline int GetOriginX (void) { return m_xO; }
		inline int GetOriginY (void) { return m_yO; }
		inline int GetPanelWidth (void) { return m_Width; }
		inline int GetPanelHeight (void) { return m_Height; }

		inline double GetPanelRigidity (void) { return m_Rigidity; }

		CPanel* AddInternalPanel (int xO, int yO, int width, int height, double rigidity);
		CPanel* AddInternalPanel (int xO, int yO, int width, int height);

		TArray <CPanel &> GetInternalPanels (void);

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
		int m_Width;					//  width of panel
		int m_Height;					//	height of panel

		double m_Rigidity;					//  rigidity of panel

		CPanel *m_ParentPanel;
		TArray <CPanel &> m_InternalPanels;

		bool m_ErrorOccurred;
		CString m_ErrorString;

		CSubSession *m_AssociatedSession;

		bool m_IsFocus;
	};

