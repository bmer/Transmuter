//	CSTransmuter.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CSTransmuter::CSTransmuter (CHumanInterface &HI, CTransmuterModel &model) : IHISession(HI),
	m_Model(model),
	m_Panel(*(new CPanel(0, 0, (HI.GetScreen()).GetWidth(), (HI.GetScreen()).GetHeight()))),
	m_IsRButtonDown(0)
	//	CSTransmuter constructor
	{
	CPanel *pEmptyPanel = m_Panel.InternalPanels.Add(0, 0, 200, m_Panel.ScreenArea.GetHeight(), false, false);

	CSExtensionNavigator *pExtensionNavigatorSession = new CSExtensionNavigator(HI, *pEmptyPanel, m_Model.GetExtensionsArray());
	m_aChildSessions.Insert(pExtensionNavigatorSession);
	}

CSTransmuter::~CSTransmuter(void)
	{
	for (int i = 0; i < m_aChildSessions.GetCount(); i++)
		{
		delete m_aChildSessions[i];
		}

	delete &m_Panel;
	}

void CSTransmuter::OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture)
	{
	TArray <CSChild *> RelevantSessions = m_Panel.GetSessionsContainingPoint(x, y);

	for (int i = 0; i < RelevantSessions.GetCount(); i++)
		{
		RelevantSessions[i]->OnLButtonDown(x, y, dwFlags, retbCapture);
		}
	}

void CSTransmuter::OnLButtonUp(int x, int y, DWORD dwFlags)
	{
	TArray <CSChild *> RelevantSessions = m_Panel.GetSessionsContainingPoint(x, y);

	for (int i = 0; i < RelevantSessions.GetCount(); i++)
		{
		RelevantSessions[i]->OnLButtonUp(x, y, dwFlags);
		}
	}

void CSTransmuter::OnRButtonDown(int x, int y, DWORD dwFlags)
	{
	TArray <CSChild *> RelevantSessions = m_Panel.GetSessionsContainingPoint(x, y);

	for (int i = 0; i < RelevantSessions.GetCount(); i++)
		{
		RelevantSessions[i]->OnRButtonDown(x, y, dwFlags);
		}
	}

void CSTransmuter::OnRButtonUp(int x, int y, DWORD dwFlags)
	{
	TArray <CSChild *> RelevantSessions = m_Panel.GetSessionsContainingPoint(x, y);

	for (int i = 0; i < RelevantSessions.GetCount(); i++)
		{
		RelevantSessions[i]->OnRButtonUp(x, y, dwFlags);
		}
	}

void CSTransmuter::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
	{
	//	paint the background
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(32, 32, 32));

	RECT rcClip;
	//	call paint functions of all subsessions
	for (int i = 0; i < m_aChildSessions.GetCount(); i++)
		{
		CSChild *pSubSession = m_aChildSessions[i];
		rcClip = (m_aChildSessions[i]->GetAssociatedPanel()).ScreenArea.GetViewOffsetRect();
		Screen.SetClipRect(rcClip);
		m_aChildSessions[i]->OnPaint(Screen, rcInvalid);
		}
	Screen.ResetClipRect();
	} 