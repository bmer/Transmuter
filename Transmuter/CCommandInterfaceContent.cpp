#include "PreComp.h"

CCommandInterfaceContent::CCommandInterfaceContent(CString sID, CHumanInterface & HI, IPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterContent(sID, HI, AssociatedPanel, model)
	{
	IPanel &refAssociatedPanel = GetAssociatedPanel();
	SetHeaderContent(strCat(sID, CONSTLIT(".h")), CONSTLIT("Command Line Interface"), refAssociatedPanel.PanelRect.GetWidth(), 40);

	IPanel *pInputPanel = refAssociatedPanel.InternalPanels.AddPanel(0, refAssociatedPanel.PanelRect.GetHeight() - 40, refAssociatedPanel.PanelRect.GetWidth(), 40, false);
	m_pInputContent = new CTextContent(CONSTLIT("Input Content"), HI, *pInputPanel, model, true, true);

	IPanel *pOutputPanel = refAssociatedPanel.InternalPanels.AddPanel(0, 40, refAssociatedPanel.PanelRect.GetWidth(), refAssociatedPanel.PanelRect.GetHeight() - 2 * 40, false);
	m_pOutputContent = new CTextContent(CONSTLIT("Output Content"), HI, *pOutputPanel, model);
	}

CCommandInterfaceContent::~CCommandInterfaceContent(void)
	{
	delete m_pHeaderContent;
	delete m_pInputContent;
	delete m_pOutputContent;
	}

void CCommandInterfaceContent::OnKeyDown(int iVirtKey, DWORD dwKeyData)
	{
	}

void CCommandInterfaceContent::OnKeyUp(int iVirtKey, DWORD dwKeyData)
	{
	}

void CCommandInterfaceContent::OnPaint(CG32bitImage & Screen, const RECT & rcInvalid)
	{
	//  may remove panel outlining in future
#if DEBUG
	bool bFocusStatus = GetFocusStatus();
#endif
	if (GetFocusStatus() == true)
		{
		UpdatePanelOutlineColor(CG32bitPixel(255, 0, 0));
		}
	else
		{
		UpdatePanelOutlineColor(CG32bitPixel(255, 255, 255));
		}
	DrawPanelOutline(Screen);

	if (m_pHeaderContent != NULL)
		{
		m_pHeaderContent->OnPaint(Screen, rcInvalid);
		}

	m_pInputContent->OnPaint(Screen, rcInvalid);
	m_pOutputContent->OnPaint(Screen, rcInvalid);
	}
//
//CInputContent::CInputContent(CString sID, CHumanInterface &HI, IPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterContent (sID, HI, AssociatedPanel, model),
//	m_InputText(sID, HI, AssociatedPanel, model)
//	{
//	}
//
//void CInputContent::OnKeyDown(int iVirtKey, DWORD dwKeyData)
//	{
//	}
//
//void CInputContent::OnKeyUp(int iVirtKey, DWORD dwKeyData)
//	{
//	}
//
//void CInputContent::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
//	{
//#if DEBUG
//	bool bFocusStatus = GetFocusStatus();
//#endif
//	if (GetFocusStatus() == true)
//		{
//		UpdatePanelOutlineColor(CG32bitPixel(255, 0, 0));
//		}
//	else
//		{
//		UpdatePanelOutlineColor(CG32bitPixel(255, 255, 255));
//		}
//	DrawPanelOutline(Screen);
//	}
//
//COutputContent::COutputContent(CString sID, CHumanInterface &HI, IPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterContent(sID, HI, AssociatedPanel, model),
//	m_OutputText(CONSTLIT("Output Text Content"), HI, AssociatedPanel, model)
//	{
//	}
//
//void COutputContent::OnPaint(CG32bitImage &Screen, const RECT &rcInvalid)
//	{
//#if DEBUG
//	bool bFocusStatus = GetFocusStatus();
//#endif
//	if (GetFocusStatus() == true)
//		{
//		UpdatePanelOutlineColor(CG32bitPixel(255, 0, 0));
//		}
//	else
//		{
//		UpdatePanelOutlineColor(CG32bitPixel(255, 255, 255));
//		}
//	DrawPanelOutline(Screen);
//	}
//