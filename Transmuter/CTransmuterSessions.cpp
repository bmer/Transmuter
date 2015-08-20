//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CLoadingSession::CLoadingSession (CHumanInterface &HI) : IHISession(HI)
	//	CTransmuterSession constructor
	{
	}

void CLoadingSession::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(64, 64, 64));
	
	CG32bitPixel TextColor = CG32bitPixel(255, 255, 255);

	const CVisualPalette &VI = g_pHI->GetVisuals();
	const CG16bitFont &font = VI.GetFont(fontConsoleMediumHeavy);

	Screen.DrawText(Screen.GetWidth()/2, Screen.GetHeight()/2, font, TextColor, CONSTLIT("Loading..."));
	}

//  =======================================================================

CSubSession::CSubSession(CHumanInterface &HI) : IHISession(HI),
	m_HeadingFont((g_pHI->GetVisuals()).GetFont(fontConsoleMediumHeavy)),
	m_HeadingColor(CG32bitPixel(160, 160, 160)),
	m_BorderColor(CG32bitPixel(64, 64, 64))
	{
	}

void CSubSession::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	}

//  =======================================================================

CExtensionNavigator::CExtensionNavigator(CHumanInterface &HI, TArray <CExtension *> Extensions, CElasticPanel &EmptyPanel) : CSubSession(HI),
	m_Extensions(Extensions),
	m_AssociatedPanel(EmptyPanel)
	{
	}

void CExtensionNavigator::CreateExtensionMenuItems(void)
//	GetExtensionNames
//
//	Loops over CExtension objects in array and gets associated name strings.
	{
	TArray<CString> ExtensionNames;

	for (int i = 0; i < m_Extensions.GetCount(); i++)
		{
		const CString &ExtensionName = m_Extensions[i]->GetName();
		ExtensionNames.Insert(ExtensionName);
		};
	}

void CExtensionNavigator::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	Screen.DrawText(m_AssociatedPanel.GetOriginX() + 10, m_AssociatedPanel.GetOriginY() + 10, m_HeadingFont, m_HeadingColor, CONSTLIT("Extension Navigator"));
	}


//  =======================================================================

CExtensionDetails::CExtensionDetails(CHumanInterface &HI) : CSubSession(HI)
	{
	}

//  =======================================================================
CElasticPanel::CElasticPanel(int xO, int yO, int SpaceWidth, int SpaceHeight) :
	m_xO(xO),
	m_yO(yO),
	m_SpaceWidth(SpaceWidth),
	m_SpaceHeight(SpaceHeight),
	m_SeparatorExists(FALSE),
	m_SeparatorFactor(0),
	m_SeparatorLocation(0),
	m_ErrorOccurred(FALSE),
	m_Error(CONSTLIT("")),
	m_SubEPAlpha(NULL),
	m_SubEPBeta(NULL),
	m_AssociatedSession(NULL)
	{
	};

CElasticPanel::CElasticPanel(int xO, int yO, int SpaceWidth, int SpaceHeight, int SeparatorDirection, int SeparatorLocation) :
	m_xO(xO),
	m_yO(yO),
	m_SpaceWidth(SpaceWidth),
	m_SpaceHeight(SpaceHeight),
	m_SeparatorDirection(SeparatorDirection),
	m_SeparatorLocation(SeparatorLocation),
	m_AssociatedSession(NULL)
	{
	m_SeparatorLocation = SeparatorLocation;
	
	if (m_SeparatorDirection == 0)
		{
		m_SeparatorFactor = double(m_SeparatorLocation)/m_SpaceHeight;
		}
	else
		{
		m_SeparatorFactor = double(m_SeparatorLocation)/m_SpaceWidth;
		}

	bool PanelSettingsValid = CheckPanelSettingsValidity();

	if (PanelSettingsValid)
		{
		m_SeparatorExists = TRUE;
		m_ErrorOccurred = FALSE;
		m_Error = CONSTLIT("");

		CreateSubPanels();
		}
	else
		{
		m_SeparatorExists = FALSE;
		m_ErrorOccurred = TRUE;
		m_Error = CONSTLIT("Panel validity check failed during construction.");

		m_SubEPAlpha = NULL;
		m_SubEPBeta = NULL;
		}
	}

bool CElasticPanel::CheckPanelSettingsValidity(void)
	{
	bool IsSeparatorWithinSpace;

	//  if separator is oriented parallel to x-direction
	if (m_SeparatorDirection == 0)
		{
		IsSeparatorWithinSpace = (m_SeparatorLocation >= m_yO) && (m_SeparatorLocation <= m_yO + m_SpaceHeight);
		}
	else  //  UI is two dimensional, thus if not oriented paralell to x-direction, then must be oriented parallel to y-direction
		{
		IsSeparatorWithinSpace = (m_SeparatorLocation >= m_xO) && (m_SeparatorLocation <= m_xO + m_SpaceWidth);
		}

	return IsSeparatorWithinSpace;
	};

bool CElasticPanel::IsEmptyPanel(void)
	{
	if (m_SeparatorExists)
		{
		return FALSE;
		}
	else
		{
		if (m_AssociatedSession == NULL)
			return TRUE;
		else
			return FALSE;
		}
	}

TArray <CElasticPanel *> CElasticPanel::GetAllLeafPanels(void)
	{
	TArray <CElasticPanel *> LeafPanels;

	if (!m_SeparatorExists)
		{
		LeafPanels.Insert(this);
		return LeafPanels;
		}
	else
		{
		//  insert Beta first (bottom, or right hand side panel)
		//  this is because EmptyPanels will usually be used with Pop(), which will remove last element
		LeafPanels.Insert(m_SubEPBeta->GetAllLeafPanels());
		LeafPanels.Insert(m_SubEPAlpha->GetAllLeafPanels());
		}

	return LeafPanels;
	};

bool CElasticPanel::MatchesRequirements(int RequiredSpaceWidth, int RequiredSpaceHeight)
	{
	if ((m_SpaceWidth >= RequiredSpaceWidth) && (m_SpaceHeight >= RequiredSpaceHeight))
		return TRUE;
	else
		return FALSE;
	}

CElasticPanel *CElasticPanel::RequestEmptyPanel(int SpaceWidth, int SpaceHeight, int SeparatorDirection)
	{
	TArray <CElasticPanel *> LeafPanels = GetAllLeafPanels();
	int iNumLeafPanels = LeafPanels.GetCount();

	for (int i = iNumLeafPanels - 1; i > -1; i--)
		{
		if (LeafPanels[i]->IsEmptyPanel())
			{
				return LeafPanels[i];
			}
		}

	CElasticPanel *TopLeafPanel = LeafPanels.Pop();
	if (SeparatorDirection == 0)
		{
		return TopLeafPanel->SplitLeafAndReturnEmptyPanel(SeparatorDirection, int(SpaceHeight*0.5));
		}
	else
		{
		return TopLeafPanel->SplitLeafAndReturnEmptyPanel(SeparatorDirection, int(SpaceWidth*0.5));
		}
	};

CElasticPanel *CElasticPanel::AssociateSession(CSubSession *Session)
	{
	if (IsEmptyPanel())
		{
		m_AssociatedSession = Session;
		Session->AssociatePanel(this);

		return this;
		}
	else
		{
		return NULL;
		}

	}

void CElasticPanel::CreateSubPanels(void)
	{
	if (m_SeparatorDirection == 0)
		{
		m_SubEPAlpha = &CElasticPanel(m_xO, m_yO, m_SpaceWidth, m_yO + m_SeparatorLocation);
		m_SubEPBeta = &CElasticPanel(m_xO, m_yO + m_SeparatorLocation, m_SpaceWidth, m_SpaceHeight - m_SeparatorLocation);
		}
	else
		{
		m_SubEPAlpha = &CElasticPanel(m_xO, m_yO, m_SeparatorLocation, m_SpaceHeight);
		m_SubEPBeta = &CElasticPanel(m_xO + m_SeparatorLocation, m_yO, m_SpaceWidth - m_SeparatorLocation, m_SpaceHeight);
		}
	}

TArray <CSubSession> CElasticPanel::GetAllSessions(void)
	{
	if (m_SeparatorExists)
		{
		//  first we put in all the sessions on the alpha side
		TArray <CSubSession> AllSessions = m_SubEPAlpha->GetAllSessions();

		//	then we get all the sessions from the beta side
		TArray <CSubSession> BetaSessions = m_SubEPBeta->GetAllSessions();

		//  then we put all the beta sessions together with the alpha sessions
		AllSessions.Insert(BetaSessions);

		return AllSessions;
		}
	else
		{
		TArray <CSubSession> AllSessions;
		AllSessions.Insert(*m_AssociatedSession);

		return AllSessions;
		}
	}

void CElasticPanel::ResizePanels(void)
	{
	if (m_SeparatorExists)
		{
		if (m_SeparatorDirection == 0)
			{
			int SeparatorLocation = int(m_SeparatorFactor*m_SpaceHeight);

			m_SubEPAlpha->SetPanelLocation(m_xO, m_yO);
			m_SubEPAlpha->SetSpace(m_SpaceWidth, SeparatorLocation);

			m_SubEPBeta->SetPanelLocation(m_xO, m_yO + SeparatorLocation);
			m_SubEPBeta->SetSpace(m_SpaceWidth, m_SpaceHeight - SeparatorLocation);
			
			}
		else
			{
			int SeparatorLocation = int(m_SeparatorFactor*m_SpaceWidth);

			m_SubEPAlpha->SetPanelLocation(m_xO, m_yO);
			m_SubEPAlpha->SetSpace(SeparatorLocation, m_SpaceHeight);

			m_SubEPBeta->SetPanelLocation(m_xO + SeparatorLocation, m_yO);
			m_SubEPBeta->SetSpace(m_SpaceWidth - SeparatorLocation*m_SpaceWidth, m_SpaceHeight);
			}
		}
	}

void CElasticPanel::SetPanelLocation(int xO, int yO)
	{
	m_xO = xO;
	m_yO = yO;

	ResizePanels();
	}

CElasticPanel* CElasticPanel::SplitLeafAndReturnEmptyPanel(int SeparatorDirection, int SeparatorLocation)
	{
	if (!m_SeparatorExists)
		{
		//	flag existence of newly created separator
		m_SeparatorExists = TRUE;

		CreateSubPanels();

		//  reassociate the associated session to a sub-panel
		if (m_SubEPAlpha->AssociateSession(m_AssociatedSession))
			{
			m_AssociatedSession = NULL;
			}
		else
			{
			m_ErrorOccurred = TRUE;
			m_Error = CONSTLIT("Failed to reassociate sub-session upon creation of new separator.");
			}


		m_SeparatorDirection = SeparatorDirection;

		if (m_SeparatorDirection == 0)
			{
			m_SeparatorFactor = double(SeparatorLocation)/m_SpaceHeight;
			}
		else
			{
			m_SeparatorFactor = double(SeparatorLocation)/m_SpaceWidth;
			}

		ResizePanels();

		return m_SubEPBeta;
		}
	else
		{
		m_ErrorOccurred = TRUE;
		m_Error = CONSTLIT("Tried to split a non-leaf panel.");
		return NULL;
		}
	}

void CElasticPanel::SetSpace(int SpaceWidth, int SpaceHeight)
	{
	}
//  =======================================================================

CTransmuterSession::CTransmuterSession (CHumanInterface &HI, CTransmuterModel &model) : IHISession(HI),
	m_Model(model),
	m_ElasticPanelling(0, 0, (HI.GetScreen()).GetWidth(), (HI.GetScreen()).GetHeight()),
	m_IsRButtonDown(0)
	//	CTransmuterSession constructor
	{
	CElasticPanel &EmptyPanel = *(m_ElasticPanelling.RequestEmptyPanel(-1, -1, -1));

	CExtensionNavigator ExtensionNavigatorSession = CExtensionNavigator(HI, m_Model.GetExtensionsArray(), EmptyPanel);
	m_SubSessions.Insert(&ExtensionNavigatorSession);
	}

void CTransmuterSession::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	m_IsRButtonDown = 1;
	HIInvalidate();
	}

void CTransmuterSession::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	m_IsRButtonDown = 0;
	HIInvalidate();
	}

void CTransmuterSession::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(32, 32, 32));

	TArray <CSubSession> SubSessions = m_ElasticPanelling.GetAllSessions();

	for (int i = 0; i < SubSessions.GetCount(); i++)
		{
		SubSessions[i].OnPaint(Screen, rcInvalid);
		}
	}

