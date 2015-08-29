//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CTransmuterSession::CTransmuterSession (CHumanInterface &HI, CTransmuterModel &model) : IHISession(HI),
	m_Model(model),
	m_Panel(*(new CPanel(0, 0, (HI.GetScreen()).GetPanelRight(), (HI.GetScreen()).GetPanelBottom()))),
	m_IsRButtonDown(0)
	//	CTransmuterSession constructor
	{
	CPanel *EmptyPanel = m_Panel.AddInternalPanelRelativeToOrigin(0, 0, 200, HI.GetScreen().GetPanelBottom(), false, false, false);

	CExtensionNavigator *pExtensionNavigatorSession = new CExtensionNavigator(HI, *EmptyPanel, m_Model.GetExtensionsArray());
	m_SubSessions.Insert(pExtensionNavigatorSession);
	}

CTransmuterSession::~CTransmuterSession(void)
	{
	for (int i = 0; i < m_SubSessions.GetCount(); i++)
		{
		delete m_SubSessions[i];
		}

	delete &m_Panel;
	}

void CTransmuterSession::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{
	TArray <CSubSession *> RelevantSessions = m_Panel.ReturnSessionsContainingPoint(x, y);

	for (int i = 0; i < RelevantSessions.GetCount(); i++)
		{
		RelevantSessions[i]->OnLButtonDown(x, y, dwFlags, retbCapture);
		}
	}

void CTransmuterSession::OnLButtonUp(int x, int y, DWORD dwFlags)
	{
	TArray <CSubSession *> RelevantSessions = m_Panel.ReturnSessionsContainingPoint(x, y);

	for (int i = 0; i < RelevantSessions.GetCount(); i++)
		{
		RelevantSessions[i]->OnLButtonUp(x, y, dwFlags);
		}
	}

void CTransmuterSession::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	TArray <CSubSession *> RelevantSessions = m_Panel.ReturnSessionsContainingPoint(x, y);

	for (int i = 0; i < RelevantSessions.GetCount(); i++)
		{
		RelevantSessions[i]->OnRButtonDown(x, y, dwFlags);
		}
	}

void CTransmuterSession::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	TArray <CSubSession *> RelevantSessions = m_Panel.ReturnSessionsContainingPoint(x, y);

	for (int i = 0; i < RelevantSessions.GetCount(); i++)
		{
		RelevantSessions[i]->OnRButtonUp(x, y, dwFlags);
		}
	}

void CTransmuterSession::UpdateSubSessionsList(void)
	{
	}

void CTransmuterSession::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//	paint the background
	Screen.Fill(0, 0, Screen.GetPanelRight(), Screen.GetPanelBottom(), CG32bitPixel(32, 32, 32));

	//	make sure m_SubSessions is up-to-date with any error sessions
	UpdateSubSessionsList();

	//	call paint functions of all subsessions
	for (int i = 0; i < m_SubSessions.GetCount(); i++)
		{
		m_SubSessions[i]->OnPaint(Screen, rcInvalid);
		}
	}