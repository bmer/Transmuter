//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CTextArea::CTextArea (CHumanInterface &HI, CPanel &AssociatedPanel) : CSubSession(HI, AssociatedPanel),
	m_pFont(NULL),
	m_bPlainText(true),
	m_cxJustifyWidth(0),
	m_cyLineSpacing(0),
	m_iBorderThickness(0),
	m_bRTFInvalid(true),
	m_dwStyles(alignLeft),
	m_rgbTextColor(CG32bitPixel(255,255,255)),
	m_rgbBackColor(CG32bitPixel::Null()),
	m_pFontTable(NULL),
	m_iTick(0),
	m_iCursorLine(-1),
	m_iCursorPos(0)
	{
	m_rcPadding.bottom = 0;
	m_rcPadding.left = 0;
	m_rcPadding.right = 0;
	m_rcPadding.top = 0;
	}

CTextArea::CTextArea (CHumanInterface &HI, CPanel &AssociatedPanel, CString sText) : CSubSession(HI, AssociatedPanel),
	m_sText(sText),
	m_pFont(NULL),
	m_bPlainText(true),
	m_cxJustifyWidth(0),
	m_cyLineSpacing(0),
	m_iBorderThickness(0),
	m_bRTFInvalid(true),
	m_dwStyles(alignLeft),
	m_rgbTextColor(CG32bitPixel(255,255,255)),
	m_rgbBackColor(CG32bitPixel::Null()),
	m_pFontTable(NULL),
	m_iTick(0),
	m_iCursorLine(-1),
	m_iCursorPos(0)
	{
	m_rcPadding.bottom = 0;
	m_rcPadding.left = 0;
	m_rcPadding.right = 0;
	m_rcPadding.top = 0;
	}

CTextArea::~CTextArea (void)
	{
	}

void CTextArea::SetEdgePadding (DWORD dwEdge, int iPadding)
	{
	if (dwEdge == EDGE_BOTTOM)
		{
		m_rcPadding.bottom = iPadding;
		}
	else if (dwEdge == EDGE_LEFT)
		{
		m_rcPadding.left = iPadding;
		}
	else if (dwEdge == EDGE_RIGHT)
		{
		m_rcPadding.right = iPadding;
		}
	else if (dwEdge == EDGE_TOP)
		{
		m_rcPadding.right = iPadding;
		}
	}

void CTextArea::FormatForRichText (const RECT &rcRect)

//	FormatRTF
//
//	Make sure we are formatted

	{
	if (m_bRTFInvalid)
		{
		SBlockFormatDesc BlockFormat;

		BlockFormat.cxWidth = RectWidth(rcRect);
		BlockFormat.cyHeight = -1;
		BlockFormat.iHorzAlign = ((m_dwStyles & alignRight) ? alignRight : ((m_dwStyles & alignCenter) ? alignCenter : alignLeft));
		BlockFormat.iVertAlign = alignTop;
		BlockFormat.iExtraLineSpacing = m_cyLineSpacing;

		BlockFormat.DefaultFormat.rgbColor = m_rgbTextColor;
		BlockFormat.DefaultFormat.pFont = m_pFont;

		m_sRichText.InitFromRTF(m_sRichText, *m_pFontTable, BlockFormat);

		m_bRTFInvalid = false;
		}
	}


int CTextArea::Justify (const RECT &rcRect)

//	Justify
//
//	Justify the text and return the height (in pixels)

	{
	RECT rcText = m_AssociatedPanel.GetPanelRect();

	if (!m_sText.IsBlank())
		{
		if (m_pFont == NULL)
			return 0;

		if (m_cxJustifyWidth != RectWidth(rcText))
			{
			m_cxJustifyWidth = RectWidth(rcText);
			m_Lines.DeleteAll();
			m_pFont->BreakText(m_sText, m_cxJustifyWidth, &m_Lines, CG16bitFont::SmartQuotes);
			}

		return m_rcPadding.top + (m_Lines.GetCount() * m_pFont->GetHeight() + (m_Lines.GetCount() - 1) * m_cyLineSpacing) + m_rcPadding.bottom;
		}
	else if (!m_sRichText.IsBlank())
		{
		FormatAsRichText(rcText);

		RECT rcBounds;
		m_RichText.GetBounds(&rcBounds);
		return m_rcPadding.top + RectHeight(rcBounds) + m_rcPadding.bottom;
		}
	else
		return 0;
	}