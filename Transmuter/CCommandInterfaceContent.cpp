#include "PreComp.h"

CCommandInterfaceContent::CCommandInterfaceContent(CString sName, CHumanInterface & HI, CPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterPanelContent(sName, HI, AssociatedPanel, model),
	m_Terminal(CONSTLIT("CLI Text Area"), HI, AssociatedPanel, model)
	{
	CPanel &refAssociatedPanel = this->GetAssociatedPanel();
	SetHeaderContent(40, CONSTLIT("Command Line Interface"));
	}
CCommandInterfaceContent::~CCommandInterfaceContent(void)
	{
	delete m_HeaderPanelContent;
	}

void CCommandInterfaceContent::OnKeyDown(int iVirtKey, DWORD dwKeyData)
	{
	}

void CCommandInterfaceContent::OnKeyUp(int iVirtKey, DWORD dwKeyData)
	{
	}

void CCommandInterfaceContent::SetHeaderContent(int iHeaderHeight, CString sHeaderText)
	{
	CPanel *pHeaderPanel = this->GetAssociatedPanel().InternalPanels.AddPanel(0, 0, this->GetAssociatedPanel().PanelRect.GetWidth(), iHeaderHeight, false);
	m_HeaderPanelContent = new CHeaderPanelContent(this->m_sName, sHeaderText, *g_pHI, *pHeaderPanel, *this);
	}

void CCommandInterfaceContent::UpdateHeaderContent(CString sHeaderText)
	{
	}

void CCommandInterfaceContent::OnPaint(CG32bitImage & Screen, const RECT & rcInvalid)
	{
	//  may remove panel outlining in future
	DrawPanelOutline(Screen);

	if (m_HeaderPanelContent != NULL)
		{
		m_HeaderPanelContent->OnPaint(Screen, rcInvalid);
		}
	}
