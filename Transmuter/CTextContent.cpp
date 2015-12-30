//	CTextContent.cpp
//
//	CTextContent class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"
#define RGB_CURSOR								(CG32bitPixel(255,255,255))

CTextContent::CTextContent (CString sName, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterPanelContent(sName, HI, AssociatedPanel, model)
	{
	}

void CTextContent::OnPaint(CG32bitImage & Screen, const RECT & rcInvalid)
	{
	}

//  =======================================================================

CDocumentCursor::CDocumentCursor(void)
	{
	}

CDocumentCursor::CDocumentCursor(int iLine, int iRun, int iChar) : m_iLine(iLine),
	m_iRun(iRun),
	m_iChar(iChar)
	{
	}

int CDocumentCursor::GetCursorPos(void)
	{
	return 0;
	}

void CDocumentCursor::UpdateCursorPos(void)
	{
	}

//  =======================================================================

CTextRun::CTextRun (CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterPanelContent("", HI, AssociatedPanel, model),
	m_bEditable(false),
	m_dwStyles(alignLeft),
	m_cyLineSpacing(0),
	m_iBorderRadius(0),
	m_pFont(NULL),
	m_rgbTextColor(CG32bitPixel(255, 255, 255)),
	m_rgbBackgroundColor(CG32bitPixel::Null()),
	m_bRTFInvalid(true),
	m_pFontTable(NULL),
	m_cxJustifyWidth(0),
	m_iTick(0),
	m_iCursorLine(-1),
	m_iCursorPos(0)
	{
	m_rcPadding.left = 0;
	m_rcPadding.top = 0;
	m_rcPadding.right = 0;
	m_rcPadding.bottom = 0;
	}

RECT CTextRun::CalcTextRect (const RECT &rcRect)

//	CalcTextRect
//
//	Calculates the text rect given the control rect

	{
	RECT rcText = rcRect;

	rcText.left += m_rcPadding.left;
	rcText.top += m_rcPadding.top;
	rcText.right -= m_rcPadding.right;
	rcText.bottom -= m_rcPadding.bottom;

	return rcText;
	}

RECT CTextRun::CalcTextRect (void)

//	CalcTextRect
//
//	Calculates the text rect given the control rect

	{
	return CalcTextRect(GetAssociatedPanel().PanelRect.GetAsRect());
	}

int CTextRun::Justify (const RECT &rcRect)

//	Justify
//
//	Justify the text and return the height (in pixels)

	{
	RECT rcText = CalcTextRect(rcRect);

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
	else
		return 0;
	}

int CTextRun::Justify (void)

//	Justify
//
//	Justify the text and return the height (in pixels)

	{
	return Justify(GetAssociatedPanel().PanelRect.GetAsRect());
	}

void CTextRun::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)

//	Paint
//
//	Handle paint

	{
	RECT rcText = CalcTextRect();
	RECT rcRect = GetAssociatedPanel().PanelRect.GetAsRect();

	//	Paint the background

	if (m_iBorderRadius > 0)
		CGDraw::RoundedRect(Screen, rcRect.left, rcRect.top, RectWidth(rcRect), RectHeight(rcRect), m_iBorderRadius, m_rgbBackgroundColor);
	else
		Screen.Fill(rcRect.left, rcRect.top, RectWidth(rcRect), RectHeight(rcRect), m_rgbBackgroundColor);

	//	Paint the editable box

	if (m_bEditable)
		{
		CG32bitPixel rgbBorderColor = CG32bitPixel::Blend(CG32bitPixel(0, 0, 0), m_rgbTextColor, (BYTE)128);
		CGDraw::RectOutlineDotted(Screen, rcRect.left, rcRect.top, RectWidth(rcRect), RectHeight(rcRect), rgbBorderColor);
		}

	//	Paint the content

	if (!m_sText.IsBlank())
		PaintText(Screen, rcText);
	}

void CTextRun::SetText (const CString &sText)
	{ 
	m_sText = sText; 
	m_cxJustifyWidth = 0; 
	GetAssociatedPanel().Invalidate();
	}

void CTextRun::PaintText (CG32bitImage &Dest, const RECT &rcRect)

//	PaintText
//
//	Paint plain text

	{
	//	Paint the text

	if (m_pFont)
		{
		//	If we haven't justified the text for this size, do it now

		if (m_cxJustifyWidth != RectWidth(rcRect))
			{
			m_cxJustifyWidth = RectWidth(rcRect);
			m_Lines.DeleteAll();
			m_pFont->BreakText(m_sText, m_cxJustifyWidth, &m_Lines, CG16bitFont::SmartQuotes);
			}

		//	Compute the rect within which we draw the text

		RECT rcText = rcRect;
		if (m_bEditable)
			{
			int iVSpacing = (RectHeight(rcRect) - m_pFont->GetHeight()) / 2;
			rcText.left += iVSpacing;
			rcText.right -= iVSpacing;
			rcText.top += iVSpacing;
			rcText.bottom -= iVSpacing;
			}

		//	Clip to text rect

		RECT rcOldClip = Dest.GetClipRect();
		Dest.SetClipRect(rcText);

		//	Figure out how many lines fit in the rect

		int iMaxLineCount = RectHeight(rcText) / m_pFont->GetHeight();

		//	If there are too many lines, and we're editable, start at the end

		int iStart = 0;
		if (m_bEditable && iMaxLineCount < m_Lines.GetCount())
			iStart = m_Lines.GetCount() - iMaxLineCount;

		//	Paint each line

		int x = rcText.left;
		int y = rcText.top;
		for (int i = iStart; i < m_Lines.GetCount(); i++)
			{
			CString sLine = m_Lines[i];

			//	Trim the last space in the line, if necessary

			char *pPos = sLine.GetASCIIZPointer();
			if (sLine.GetLength() > 0 && pPos[sLine.GetLength() - 1] == ' ')
				sLine = strTrimWhitespace(sLine);

			//	Alignment

			int xLine;
			if (m_dwStyles & alignCenter)
				{
				int cxWidth = m_pFont->MeasureText(sLine);
				xLine = x + (RectWidth(rcText) - cxWidth) / 2;
				}
			else if (m_dwStyles & alignRight)
				{
				int cxWidth = m_pFont->MeasureText(sLine);
				xLine = x + (RectWidth(rcRect) - cxWidth);
				}
			else
				xLine = x;

			Dest.DrawText(xLine, y, *m_pFont, m_rgbTextColor, sLine);

			//	Next

			y += m_pFont->GetHeight() + m_cyLineSpacing;
			if (y >= rcText.bottom)
				break;
			}

		//	Paint the cursor

		if (m_bEditable && m_iCursorLine >= iStart)
			{
			int cxPos = (m_iCursorLine < m_Lines.GetCount() ? m_pFont->MeasureText(CString(m_Lines[m_iCursorLine].GetASCIIZPointer(), m_iCursorPos, true)) : 0);
			int y = rcText.top + (m_iCursorLine - iStart) * (m_pFont->GetHeight() + m_cyLineSpacing);
			int x = rcText.left;
			if (m_dwStyles & alignCenter)
				{
				int cxWidth = (m_iCursorLine < m_Lines.GetCount() ? m_pFont->MeasureText(m_Lines[m_iCursorLine]) : 0);
				x += ((RectWidth(rcText) - cxWidth) / 2) + cxPos;
				}
			else if (m_dwStyles & alignRight)
				{
				int cxWidth = (m_iCursorLine < m_Lines.GetCount() ? m_pFont->MeasureText(m_Lines[m_iCursorLine]) : 0);
				x += (RectWidth(rcText) - cxWidth) + cxPos;
				}
			else
				x += cxPos;

			if (((m_iTick / 30) % 2) > 0)
				{
				Dest.Fill(x, y, 2, m_pFont->GetHeight(), RGB_CURSOR);
				}
			}

		//	Restore clip

		Dest.SetClipRect(rcOldClip);
		}
	}
