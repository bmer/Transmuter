//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CTransmuterSession::CTransmuterSession (CHumanInterface &HI, CTransmuterModel &model) : IHISession(HI),
	m_Model(model),
	m_ElasticPanelling(0, 0, (HI.GetScreen()).GetWidth(), (HI.GetScreen()).GetHeight()),
	m_IsRButtonDown(0)
	//	CTransmuterSession constructor
	{
	CElasticPanel &EmptyPanel = *(m_ElasticPanelling.RequestEmptyPanel(-1, -1, -1));

	CExtensionNavigator *pExtensionNavigatorSession = new CExtensionNavigator(HI, EmptyPanel, m_Model.GetExtensionsArray());
	m_SubSessions.Insert(pExtensionNavigatorSession);
	}

CTransmuterSession::~CTransmuterSession(void)
	{
	for (int i = 0; i < m_SubSessions.GetCount(); i++)
		{
		delete m_SubSessions[i];
		}
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

void CTransmuterSession::UpdateSubSessionsList(void)
	{
	TArray <CSubSession *> ErrorSessions = m_ElasticPanelling.CreateErrorSessions(m_HI);
	m_SubSessions.Insert(ErrorSessions);
	}

void CTransmuterSession::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//	paint the background
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(32, 32, 32));

	//	make sure m_SubSessions is up-to-date with any error sessions
	UpdateSubSessionsList();

	//	call paint functions of all subsessions
	for (int i = 0; i < m_SubSessions.GetCount(); i++)
		{
		m_SubSessions[i]->OnPaint(Screen, rcInvalid);
		}
	}