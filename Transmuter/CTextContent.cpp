//	CTextContent.cpp
//
//	CTextContent class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"
#define RGB_CURSOR								(CG32bitPixel(255,255,255))

CTextContent::CTextContent (CHumanInterface &HI, CPanel &AssociatedPanel) : CTransmuterPanelContent(HI, AssociatedPanel),
	m_bEditable(false),
	m_dwStyles(alignLeft),
	m_cyLineSpacing(0),
	m_iBorderRadius(0),
	m_pFont(NULL),
	m_rgbTextColor(CG32bitPixel(255,255,255)),
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

RECT CTextContent::CalcTextRect (const RECT &rcRect)

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

RECT CTextContent::CalcTextRect (void)

//	CalcTextRect
//
//	Calculates the text rect given the control rect

	{
	return CalcTextRect(m_AssociatedPanel.PanelRect.GetAsRect());
	}

void CTextContent::FormatRTF (const RECT &rcRect)

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

		m_RichText.InitFromRTF(m_sRTF, *m_pFontTable, BlockFormat);

		m_bRTFInvalid = false;
		}
	}

int CTextContent::Justify (const RECT &rcRect)

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
	else if (!m_sRTF.IsBlank())
		{
		FormatRTF(rcText);

		RECT rcBounds;
		m_RichText.GetBounds(&rcBounds);
		return m_rcPadding.top + RectHeight(rcBounds) + m_rcPadding.bottom;
		}
	else
		return 0;
	}

int CTextContent::Justify (void)

//	Justify
//
//	Justify the text and return the height (in pixels)

	{
	return Justify(m_AssociatedPanel.PanelRect.GetAsRect());
	}

void CTextContent::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)

//	Paint
//
//	Handle paint

	{
	RECT rcText = CalcTextRect();
	RECT rcRect = m_AssociatedPanel.PanelRect.GetAsRect();

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
	else
		PaintRTF(Screen, rcText);
	}

CString CTextContent::Escape (const CString &sText)

//	Escape
//
//	Escapes all reserved characters in sText

	{
	char *pPos = sText.GetASCIIZPointer();
	char *pEndPos = pPos + sText.GetLength();

	CString sResult;
	char *pDest = NULL;

	char *pStart = pPos;
	while (pPos < pEndPos)
		{
		switch (*pPos)
			{
			case '\\':
			case '/':
			case '{':
			case '}':
				{
				//	If necessary, allocate a resulting buffer (note that we can
				//	never be larger than twice the length of the original string).

				if (pDest == NULL)
					pDest = sResult.GetWritePointer(sText.GetLength() * 2);

				//	Write out the string up to now

				char *pSrc = pStart;
				while (pSrc < pPos)
					*pDest++ = *pSrc++;

				//	Write out an escaped version of the character

				*pDest++ = '\\';
				*pDest++ = *pPos;

				pStart = pPos + 1;
				break;
				}
			}

		pPos++;
		}

	//	If necessary write the remaining string

	if (pDest)
		{
		char *pSrc = pStart;
		while (pSrc < pEndPos)
			*pDest++ = *pSrc++;

		//	Truncate

		sResult.Truncate((int)(pDest - sResult.GetPointer()));

		//	Done

		return sResult;
		}

	//	If we didn't need to escape anything then we just return the original
	//	string.

	else
		return sText;
	}

void CTextContent::SetAsRichText (const CString &sText)

//	LoadAsRichText
//
//	Takes either a RTF string (with "{\rtf...") or a plain string and returns
//	a valid RTF string.

	{
	//	If this is already an RTF string, just return it.

	if (strStartsWith(sText, CONSTLIT("{\\rtf")) || strStartsWith(sText, CONSTLIT("{/rtf")))
		SetRichText(sText);

	//	Otherwise, escape the string

	SetRichText(strPatternSubst(CONSTLIT("{\\rtf %s}"), Escape(sText)));
	}

void CTextContent::SetRichText (const CString &sRTF)
	{ 
	m_sRTF = sRTF; 
	m_sText = NULL_STR; 
	m_bRTFInvalid = true; 
	m_AssociatedPanel.Invalidate(); 
	}

void CTextContent::SetText (const CString &sText)
	{ 
	m_sText = sText; 
	m_sRTF = NULL_STR; 
	m_cxJustifyWidth = 0; 
	m_AssociatedPanel.Invalidate(); 
	}

void CTextContent::PaintRTF (CG32bitImage &Dest, const RECT &rcRect)

//	PaintRTF
//
//	Paint RTF

	{
	int i;

	//	Must have a font table

	if (m_pFontTable == NULL)
		return;

	//	Format, if necessary

	FormatRTF(rcRect);

	//	Paint

	for (i = 0; i < m_RichText.GetFormattedSpanCount(); i++)
		{
		const SFormattedTextSpan &Span = m_RichText.GetFormattedSpan(i);

		Span.Format.pFont->DrawText(Dest,
				rcRect.left + Span.x,
				rcRect.top + Span.y,
				Span.Format.rgbColor,
				Span.sText);
		}
	}

void CTextContent::PaintText (CG32bitImage &Dest, const RECT &rcRect)

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
