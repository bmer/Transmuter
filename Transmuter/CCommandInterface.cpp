#include "PreComp.h"

CCommandInterface::CCommandInterface(CString sPanelName, CHumanInterface &HI, CTransmuterModel &model, int iOriginX, int iOriginY, int iWidth, int iHeight) : CTransmuterPanel(sID, HI, model, iOriginX, iOriginY, iWidth, iHeight)
	{
	CreateTitlePanel(CreateSubPanelName("TitlePanel"), CONSTLIT("Command Interface"), PanelRect.GetWidth(), 40);

	CTransmuterPanel *pIOPanel = new CTransmuterPanel(CreateSubPanelName("IO"), HI, model, PanelRect.GetWidth(), PanelRect.GetHeight() - m_pTitlePanel->PanelRect.GetHeight());
	PanelOrganizer.PlacePanel(pIOPanel, 0, m_pTitlePanel->PanelRect.GetHeight());

	CTextContent *m_pInputPanel = new CTextContent(pIOPanel->CreateSubPanelName(CONSTLIT("InputPanel")), HI, model, pIOPanel->PanelRect.GetWidth(), pIOPanel->PanelRect.GetHeight() - 40);
	IPanel *pInputPanel = pIOPanel->PanelOrganizer.PlacePanel(m_pInputPanel, 0, 0);
	

	m_pOutputPanel = new CTextContent(CreateSubPanelName(CONSTLIT("OutputPanel")), HI, model);
	IPanel *pOutputPanel = refAssociatedPanel.InternalPanels.AddPanel(0, 40, refAssociatedPanel.PanelRect.GetWidth(), refAssociatedPanel.PanelRect.GetHeight() - 2 * 40, false);
	
	}

CCommandInterface::~CCommandInterface(void)
	{
	delete m_pInputContent;
	delete m_pOutputContent;
	}

void CCommandInterface::OnKeyDown(int iVirtKey, DWORD dwKeyData)
	{
	}

void CCommandInterface::OnKeyUp(int iVirtKey, DWORD dwKeyData)
	{
	}

void CCommandInterface::OnPaint(CG32bitImage & Screen, const RECT & rcInvalid)
	{
	m_pInputPanel->OnPanelPaint(Screen, rcInvalid);
	m_pOutputPanel->OnPanelPaint(Screen, rcInvalid);
	}