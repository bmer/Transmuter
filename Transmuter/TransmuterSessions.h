//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CLoadingSession;
class CSubSession;
class CExtensionDetails;
class CExtensionNaviagor;
class CElasticPanel;
class CTransmuterSession;


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
		CSubSession(CHumanInterface &HI);

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
		

		void AssociatePanel (CElasticPanel *AssociatedPanel) { m_AssociatedPanel = AssociatedPanel; }

	protected:
		CG16bitFont m_HeadingFont;
		CG32bitPixel m_HeadingColor;
		CG32bitPixel m_BorderColor;
		CElasticPanel *m_AssociatedPanel;
	};

//  =======================================================================

class CExtensionDetails : public CSubSession
	{
	public:
		CExtensionDetails(CHumanInterface &HI);

		//void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CElasticPanel *AssociatedPanel;
		CExtension CurrentExtension;
	};

//  =======================================================================

class CExtensionNavigator : public CSubSession
	{
	public:
		CExtensionNavigator(CHumanInterface &HI, TArray <CExtension *> Extensions, CElasticPanel &EmptyPanel);

		void CExtensionNavigator::CreateExtensionMenuItems(void);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		TArray <CExtension *> m_Extensions;
		CElasticPanel &m_AssociatedPanel;
	};

//  =======================================================================

class CElasticPanel 
	{
	public:
		CElasticPanel (int xO, int yO, int SpaceWidth, int SpaceHeight);
		CElasticPanel (int xO, int yO, int SpaceWidth, int SpaceHeight, int SeparatorDirection, int SeparatorLocation);

		bool CheckPanelSettingsValidity (void);

		void SetPanelLocation (int xO, int yO);
		void SetSpace (int SpaceWidth, int SpaceHeight);
		int GetOriginX(void) { return m_xO; }
		int GetOriginY(void) { return m_yO; }
		int GetSpaceWidth(void) { return m_SpaceWidth; }
		int GetSpaceHeight(void) { return m_SpaceHeight; }

		CElasticPanel* SplitLeafAndReturnEmptyPanel (int SeparatorDirection, int SeparatorLocation);
		int GetSeparatorDirection (void) { return m_SeparatorDirection; }
		int GetSeparatorLocation (void) { return m_SeparatorLocation; }
		void ResetSeparator (void);
		void CreateSubPanels (void);
		void DeleteSeparator (void);

		bool IsEmptyPanel (void);
		TArray <CElasticPanel *> GetAllLeafPanels (void);
		CElasticPanel *RequestEmptyPanel(int SpaceWidth, int SpaceHeight, int SeparatorDirection);
		bool MatchesRequirements(int RequiredSpaceWidth, int RequiredSpaceHeight);
		CElasticPanel *AssociateSession(CSubSession *Session);

		TArray <CSubSession> GetAllSessions (void);

		void ResizePanels (void);

		void SetFocus (void) { m_IsFocus = TRUE; }
		void RemoveFocus (void) { m_IsFocus = FALSE; }

	private:
		int m_xO;							//  top left corner x-coordinate
		int m_yO;							//	top left corner y-coordinate
		int m_SpaceWidth;					//  width of panel
		int m_SpaceHeight;					//	height of panel

		bool m_SeparatorExists;
		int m_SeparatorDirection;			//  0 for separator parallel to x-direction, 1 for y-direction
		double m_SeparatorFactor;			//  fraction of SpaceWidth or SpaceHeight, depending on direction
		int m_SeparatorLocation;			//	SeparatorFactor*SpaceWidth or SeparatorFactor*SpaceHeight depending on direction
		
		bool m_ErrorOccurred;
		CString m_Error;

		CElasticPanel *m_SubEPAlpha;
		CElasticPanel *m_SubEPBeta;

		CSubSession *m_AssociatedSession;

		bool m_IsFocus;
	};
//  =======================================================================

class CTransmuterSession : public IHISession, public CUniverse::INotifications
	{
	public:
		CTransmuterSession (CHumanInterface &HI, CTransmuterModel &model);

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

	private:
		TArray <CSubSession> m_SubSessions;
		CTransmuterModel &m_Model;
		CElasticPanel m_ElasticPanelling;
		int m_IsRButtonDown;
	};