//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CElasticPanel::CElasticPanel(int xO, int yO, int SpaceWidth, int SpaceHeight) :
	m_xO(xO),
	m_yO(yO),
	m_SpaceWidth(SpaceWidth),
	m_SpaceHeight(SpaceHeight),
	m_SeparatorExists(FALSE),
	m_SeparatorFactor(0),
	m_SeparatorLocation(0),
	m_ErrorOccurred(FALSE),
	m_ErrorString(CONSTLIT("")),
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
	m_AssociatedSession(NULL)
	{
	bool PanelSettingsValid = CheckPanelSettingsValidity();

	if (PanelSettingsValid)
		{
		m_SeparatorExists = TRUE;
		m_ErrorOccurred = FALSE;
		m_ErrorString = CONSTLIT("");

		CreateSubPanels(SeparatorDirection, SeparatorLocation);
		}
	else
		{
		m_SeparatorExists = FALSE;
		m_ErrorOccurred = TRUE;
		m_ErrorString = CONSTLIT("Panel validity check failed during construction.");

		m_SubEPAlpha = NULL;
		m_SubEPBeta = NULL;
		}
	}

CElasticPanel::~CElasticPanel(void)
	{
	if (m_SubEPAlpha != NULL)
		{
		delete m_SubEPAlpha;
		}
	
	if (m_SubEPBeta != NULL)
		{
		delete m_SubEPBeta;
		}
	};

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
	if (m_SeparatorExists == FALSE && m_AssociatedSession == NULL)
		{
		return TRUE;
		}
	else
		{
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

	CElasticPanel *TopLeafPanel = LeafPanels[0];
	if (SeparatorDirection == 0)
		{
		if (SpaceHeight < 0)
			return TopLeafPanel->SplitLeafAndReturnEmptyPanel(SeparatorDirection, int(SpaceHeight*0.5));
		else
			return TopLeafPanel->SplitLeafAndReturnEmptyPanel(SeparatorDirection, SpaceHeight);
		}
	else
		{
		if (SpaceWidth < 0)
			return TopLeafPanel->SplitLeafAndReturnEmptyPanel(SeparatorDirection, int(SpaceWidth*0.5));
		else
			return TopLeafPanel->SplitLeafAndReturnEmptyPanel(SeparatorDirection, SpaceWidth);
		}
	};

void CElasticPanel::AssociateSession(CSubSession *Session)
	{
	if (IsEmptyPanel())
		{
		m_AssociatedSession = Session;
		}
	else
		{
		m_ErrorOccurred = TRUE;
		m_ErrorString = CONSTLIT("Failed to associate given session to panel.");
		}
	}

void CElasticPanel::CreateSubPanels(int SeparatorDirection, int SeparatorLocation)
	{
	if (m_SeparatorExists)
		{
		m_ErrorOccurred = TRUE;
		m_ErrorString = CONSTLIT("Tried to create sub-panels for a panel that already has a separator.");
		}
	else
		{
		m_SeparatorExists = TRUE;
		m_SeparatorDirection = SeparatorDirection;
		m_SeparatorLocation = SeparatorLocation;
		
		if (SeparatorDirection == 0)
			{
			m_SeparatorFactor = double(SeparatorLocation)/m_SpaceHeight;
			m_SubEPAlpha = new CElasticPanel(m_xO, m_yO, m_SpaceWidth, m_yO + m_SeparatorLocation);
			m_SubEPBeta = new CElasticPanel(m_xO, m_yO + m_SeparatorLocation, m_SpaceWidth, m_SpaceHeight - m_SeparatorLocation);
			}
		else
			{
			m_SeparatorFactor = double(SeparatorLocation)/m_SpaceWidth;
			m_SubEPAlpha = new CElasticPanel(m_xO, m_yO, m_SeparatorLocation, m_SpaceHeight);
			m_SubEPBeta = new CElasticPanel(m_xO + m_SeparatorLocation, m_yO, m_SpaceWidth - m_SeparatorLocation, m_SpaceHeight);
			}
		}
	}

TArray <CSubSession *> CElasticPanel::GetAllAssociatedSessions(void)
	{
	if (m_SeparatorExists)
		{
		//  first we put in all the sessions on the alpha side
		TArray <CSubSession *> AllSessions = m_SubEPAlpha->GetAllAssociatedSessions();

		//	then we get all the sessions from the beta side
		TArray <CSubSession *> BetaSessions = m_SubEPBeta->GetAllAssociatedSessions();

		//  then we put all the beta sessions together with the alpha sessions
		AllSessions.Insert(BetaSessions);

		return AllSessions;
		}
	else
		{
		TArray <CSubSession *> AllSessions;

		if (m_AssociatedSession != NULL)
			AllSessions.Insert(m_AssociatedSession);

		return AllSessions;
		}
	}

void CElasticPanel::ResizePanels(void)
	{
	if (m_SeparatorExists)
		{
		int SeparatorLocation = m_SeparatorLocation;

		if (m_SeparatorDirection == 0)
			{
			if (m_MaintainSeparatorFactor == TRUE)
				SeparatorLocation = int(m_SeparatorFactor*m_SpaceHeight);

			m_SubEPAlpha->SetPanelLocation(m_xO, m_yO);
			m_SubEPAlpha->SetSpace(m_SpaceWidth, SeparatorLocation);

			m_SubEPBeta->SetPanelLocation(m_xO, m_yO + SeparatorLocation);
			m_SubEPBeta->SetSpace(m_SpaceWidth, m_SpaceHeight - SeparatorLocation);		
			}
		else
			{
			if (m_MaintainSeparatorFactor == TRUE)
				SeparatorLocation = int(m_SeparatorFactor*m_SpaceWidth);

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
		//  Create sub-panels
		CreateSubPanels(SeparatorDirection, SeparatorLocation);

		//  reassociate the associated session to a sub-panel
		m_SubEPAlpha->AssociateSession(m_AssociatedSession);
		if (!(m_SubEPAlpha->ErrorOccurred()))
			{
			m_AssociatedSession = NULL;
			}
		else
			{
			m_ErrorOccurred = TRUE;
			m_ErrorString = CONSTLIT("Failed to reassociate sub-session upon creation of new separator.");
			}

		ResizePanels();

		return m_SubEPBeta;
		}
	else
		{
		m_ErrorOccurred = TRUE;
		m_ErrorString = CONSTLIT("Tried to split a non-leaf panel.");
		return NULL;
		}
	}

void CElasticPanel::SetSpace(int SpaceWidth, int SpaceHeight)
	{
	m_SpaceWidth = SpaceWidth;
	m_SpaceHeight = SpaceHeight;
	}

TArray <CSubSession *> CElasticPanel::CreateErrorSessions(CHumanInterface &HI)
	{
	TArray <CSubSession *> ErrorSessions;

	if (m_ErrorOccurred)
		{
		CErrorSession *ErrorSession = new CErrorSession(HI, *this, m_ErrorString);

		ErrorSessions.Insert(ErrorSession);

		return ErrorSessions;
		}

	if (m_SeparatorExists)
		{
		ErrorSessions.Insert(m_SubEPAlpha->CreateErrorSessions(HI));
		ErrorSessions.Insert(m_SubEPBeta->CreateErrorSessions(HI));
		}
	else
		{
		return ErrorSessions;
		}
	}

TArray <int> CElasticPanel::GetRectCoords(void)
	{
	TArray <int> RectCoords;
	RectCoords.Insert(m_xO);
	RectCoords.Insert(m_yO);
	RectCoords.Insert(m_xO + m_SpaceWidth);
	RectCoords.Insert(m_yO + m_SpaceHeight);

	return RectCoords;
	}
