//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CLoadingSession;
class CElasticPanel;
class CSubSession;
class CExtensionDetails;
class CExtensionNavigator;
class CExtensionNavigatorMenuItem;
class CTransmuterSession;

//  =======================================================================

class CElasticPanel 
	{
	public:
		CElasticPanel (int xO, int yO, int SpaceWidth, int SpaceHeight);
		CElasticPanel (int xO, int yO, int SpaceWidth, int SpaceHeight, int SeparatorDirection, int SeparatorLocation);

		~CElasticPanel (void);

		bool CheckPanelSettingsValidity (void);

		void SetPanelLocation (int xO, int yO);
		void SetSpace (int SpaceWidth, int SpaceHeight);
		int GetOriginX (void) { return m_xO; }
		int GetOriginY (void) { return m_yO; }
		int GetSpaceWidth (void) { return m_SpaceWidth; }
		int GetSpaceHeight (void) { return m_SpaceHeight; }

		CElasticPanel* SplitLeafAndReturnEmptyPanel (int SeparatorDirection, int SeparatorLocation);
		int GetSeparatorDirection (void) { return m_SeparatorDirection; }
		int GetSeparatorLocation (void) { return m_SeparatorLocation; }
		void ResetSeparator (void);
		void CreateSubPanels (int SeparatorDirection, int SeparatorLocation);
		void DeleteSeparator (void);

		bool IsEmptyPanel (void);
		TArray <CElasticPanel *> GetAllLeafPanels (void);
		CElasticPanel *RequestEmptyPanel(int SpaceWidth, int SpaceHeight, int SeparatorDirection);
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

//  =======================================================================

class CLoadingSession : public IHISession
	{
	public:
		CLoadingSession(CHumanInterface &HI);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
	};

//  =======================================================================

class CSubSession : public IHISession
	{
	public:
		CSubSession(CHumanInterface &HI, CElasticPanel &AssociatedPanel);

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void DrawPanelOutline (CG32bitImage &Screen);

	protected:
		const CG16bitFont &m_HeadingFont;
		CG32bitPixel m_HeadingColor;
		CG32bitPixel m_BorderColor;
		int m_PanelOutlineWidth;
		CG32bitPixel m_PanelOutlineColor;
		CElasticPanel &m_AssociatedPanel;
	};

//  =======================================================================

class CErrorSession : public CSubSession
	{
	public:
		CErrorSession(CHumanInterface &HI, CElasticPanel &AssociatedPanel, CString ErrorString);

		void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CString m_ErrorString;
	};

//  =======================================================================

class CMenuSession : public CSubSession
	{
	public:
		CMenuSession(CHumanInterface &HI, CElasticPanel &AssociatedPanel);

		void OnPaint(CG32bitImage &Screen, const RECT &rcINvalid);
	};

//  =======================================================================

class CExtensionDetails : public CSubSession
	{
	public:
		CExtensionDetails(CHumanInterface &HI, CElasticPanel &AssociatedPanel);

		//void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CExtension m_Extension;
	};

//  =======================================================================

class CExtensionNavigatorMenuItem : public CSubSession
	{
	public:
		CExtensionNavigatorMenuItem (CHumanInterface &HI, CElasticPanel &AssociatedPanel, CExtension *Extension);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CExtension &m_Extension;
	};

class CExtensionNavigator : public CSubSession
	{
	public:
		CExtensionNavigator (CHumanInterface &HI, CElasticPanel &AssociatedPanel, TArray <CExtension *> Extensions);

		void CreateExtensionNavigatorMenuItems (void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void DrawTitleBar (CG32bitImage &Screen);

	private:
		TArray <CExtension *> m_Extensions;
		TArray <CExtensionNavigatorMenuItem> m_NavigatorMenuItems;
		CElasticPanel m_InternalPanelling;
	};


//  =======================================================================

class CTransmuterSession : public IHISession, public CUniverse::INotifications
	{
	public:
		CTransmuterSession (CHumanInterface &HI, CTransmuterModel &model);
		~CTransmuterSession (void);
		//	IHISession virtuals

		//virtual void OnChar (char chChar, DWORD dwKeyData);
		//virtual void OnCleanUp (void);
		//virtual ALERROR OnCommand (const CString &sCmd, void *pData = NULL);
		//virtual ALERROR OnInit (CString *retsError);
		//virtual void OnLButtonDblClick (int x, int y, DWORD dwFlags);
		//virtual void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		//virtual void OnLButtonUp (int x, int y, DWORD dwFlags);
		//virtual void OnMouseMove (int x, int y, DWORD dwFlags);
		void OnRButtonDown (int x, int y, DWORD dwFlags);
		void OnRButtonUp (int x, int y, DWORD dwFlags);
		//virtual void OnUpdate (bool bTopMost);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	protected:
		void UpdateSubSessionsList(void);

	private:
		TArray <CSubSession *> m_SubSessions;
		CTransmuterModel &m_Model;
		CElasticPanel m_ElasticPanelling;
		int m_IsRButtonDown;
	};