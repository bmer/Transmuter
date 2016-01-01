//	CTextContent.cpp
//
//	CTextContent class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"
#define RGB_CURSOR								(CG32bitPixel(255,255,255))

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

CTextRun::CTextRun (void) : m_sText(CONSTLIT("")),
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
	m_iCursorPos(0),
	m_bInvalid(true)
	{
	m_rcPadding.left = 0;
	m_rcPadding.top = 0;
	m_rcPadding.right = 0;
	m_rcPadding.bottom = 0;
	}

CTextRun::CTextRun (CString sText) : m_sText(sText),
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
	m_iCursorPos(0),
	m_bInvalid(true)
	{
	m_rcPadding.left = 0;
	m_rcPadding.top = 0;
	m_rcPadding.right = 0;
	m_rcPadding.bottom = 0;
	}

CString CTextRun::GetTruncatedStrCopy(CString sInput, int iNewLength)
	{
	if (sInput.GetLength() < iNewLength + 1)
		{
		return sInput;
		}
	else
		{
		CString sTruncated = CString (sInput);
		sTruncated.Truncate(iNewLength);

		return sTruncated;
		}
	}


void CTextRun::InsertAt(int iPosition, CString sText)
	{
	int iOriginalLength = m_sText.GetLength();
	int iInsertionLength = sText.GetLength();

	int iNewLength = iOriginalLength + iInsertionLength;

	CString sTruncated = GetTruncatedStrCopy(sText, iPosition + 1);
	s
	}

RECT CTextRun::CalcTextRect (const RECT &rcCtrlRect)

//	CalcTextRect
//
//	Calculates the text rect given the control rect

	{
	RECT rcText = rcCtrlRect;

	rcText.left += m_rcPadding.left;
	rcText.top += m_rcPadding.top;
	rcText.right -= m_rcPadding.right;
	rcText.bottom -= m_rcPadding.bottom;

	return rcText;
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

void CTextRun::SetText (const CString &sText)
	{ 
	m_sText = sText; 
	m_cxJustifyWidth = 0; 
	Invalidate();
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

//	================================================================================

void CTextLine::ConcatenateLines(const CTextLine &refTextLine)
	{
	for (int i = 0; i < refTextLine.GetCount(); i++)
		{
		TArray<CTextRun *>::Insert(new CTextRun(*refTextLine[i]));
		}
	}

CTextRun *CTextLine::AddNewRun(const CTextRun &refTextRun)
	{
	CTextRun *pNewTextRun = new CTextRun(refTextRun);
	TArray<CTextRun *>::Insert(pNewTextRun);
	return pNewTextRun;
	}

void CTextLine::InsertText(int iRun, int iChar, CString sText)
	//	Callers should guarantee that iRun and iChar are sensical.
	//	(In other words, use 
	//	CTextDocument::InsertText(CDocumentCursor &refIOCursor, CString sText), 
	//  never this method)
	{
	CTextRun *pRelevantRun = GetAt(iRun);
	pRelevantRun->InsertText(iChar, sText);
	}

void CTextLine::CleanUp(void)
	{
	for (int i = 0; i < GetCount(); i++)
		{
		delete GetAt(i);
		}
	DeleteAll();
	}

void CTextLine::Copy(const CTextLine &refTextLine)
	{
	InsertEmpty(refTextLine.GetCount());
	for (int i = 0; i < refTextLine.GetCount(); i++)
		{
		GetAt(i) = new CTextRun(*refTextLine[i]);
		}
	}

//	================================================================================

CTextContent::CTextContent (CString sID, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model) : CTransmuterContent(sID, HI, AssociatedPanel, model)
	{
	}

void CTextContent::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)

	//	Paint
	//
	//	Handle paint

	{
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
	}

void CTextContent::OnContentChar(char chChar, DWORD dwKeyData)
	{

	}

inline CDocumentCursor &CTextDocument::CreateNewCursor(int iLine, int iRun, int iChar)
	{
	if (m_Lines.GetCount() == 0)
		{
		CTextLine *pNewTextLine = new CTextLine();
		AddNewLine(*pNewTextLine);
		}
	CDocumentCursor *pNewDocumentCursor = new CDocumentCursor(iLine, iRun, iChar);

	return *pNewDocumentCursor;
	}
