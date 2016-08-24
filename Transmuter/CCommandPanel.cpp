#include "PreComp.h"

CCommandPanel::CCommandPanel(CString sName, CHumanInterface &HI, CTransmuterModel &model, int iWidth, int iHeight) : CTransmuterPanel(sName, HI, model, iWidth, iHeight)
	{
	CreateTitlePanel(CreateSubPanelName("TitlePanel"), CONSTLIT("Command Interface"));

	CTransmuterPanel *pIOPanel = new CTransmuterPanel(CreateSubPanelName("IO"), HI, model, PanelRect.GetWidth(), PanelRect.GetHeight() - m_pTitlePanel->PanelRect.GetHeight());
	PlacePanel(pIOPanel, 0, m_pTitlePanel->PanelRect.GetHeight());

	CTextContent *m_pInputPanel = new CTextContent(pIOPanel->CreateSubPanelName(CONSTLIT("InputPanel")), HI, model, pIOPanel->PanelRect.GetWidth(), pIOPanel->PanelRect.GetHeight() - 40);
	pIOPanel->PlacePanel(m_pInputPanel, 0, 0);
	

	m_pOutputPanel = new CTextContent(CreateSubPanelName(CONSTLIT("OutputPanel")), HI, model, pIOPanel->PanelRect.GetWidth(), pIOPanel->PanelRect.GetHeight() - 2 * 40);
	pIOPanel->PlacePanel(m_pOutputPanel, 0, 40);
	}

CCommandPanel::~CCommandPanel(void)
	{
	delete m_pInputPanel;
	delete m_pOutputPanel;
	}

void CCommandPanel::OnKeyDown(int iVirtKey, DWORD dwKeyData)
	{
	}

void CCommandPanel::OnKeyUp(int iVirtKey, DWORD dwKeyData)
	{
	}

void CCommandPanel::OnPaint(CG32bitImage & Screen, const RECT & rcInvalid)
	{
	if (m_pInputPanel != NULL && m_pOutputPanel != NULL)
		{
		m_pInputPanel->OnPaint(Screen, rcInvalid);
		m_pOutputPanel->OnPaint(Screen, rcInvalid);
		}
	}