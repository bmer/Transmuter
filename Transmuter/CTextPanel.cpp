//	CTextContent.cpp
//
//	CTextContent class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"
#define RGB_CURSOR								(CG32bitPixel(255,255,255))

SDocumentPaintCtx::SDocumentPaintCtx(void) : Screen(NULL),
x(-1),
y(-1)
	{

	}

//  =======================================================================

CDocumentCursor::CDocumentCursor(void) : m_bIsPlaced(false)
	{
	}

CDocumentCursor::CDocumentCursor(int iLine, int iRun, int iChar) : m_iLine(iLine),
	m_iRun(iRun),
	m_iChar(iChar),
	m_bIsPlaced(false)
	{
	}

//  =======================================================================

CTextRun::CTextRun (void) : m_sText(CONSTLIT("")),
	m_bEditable(false),
	m_pFont(&(g_pHI->GetVisuals().GetFont(fontConsoleMediumHeavy))),
	m_rgbTextColor(CG32bitPixel(255, 255, 255)),
	m_rgbBackgroundColor(CG32bitPixel::Null()),
	m_bInvalid(true),
	m_iRunWidth(0),
	m_iRunHeight(0)
	{
	m_rcPadding.left = 0;
	m_rcPadding.top = 0;
	m_rcPadding.right = 0;
	m_rcPadding.bottom = 0;
	}

CTextRun::CTextRun (CString sText) : m_sText(sText),
	m_bEditable(false),
	m_pFont(&(g_pHI->GetVisuals().GetFont(fontConsoleMediumHeavy))),
	m_rgbTextColor(CG32bitPixel(255, 255, 255)),
	m_rgbBackgroundColor(CG32bitPixel::Null()),
	m_bInvalid(true),
	m_iRunWidth(0),
	m_iRunHeight(0)
	{
	m_rcPadding.left = 0;
	m_rcPadding.top = 0;
	m_rcPadding.right = 0;
	m_rcPadding.bottom = 0;
	}

void CTextRun::InsertAt(int iInsertPosition, CString sText)
//	Say we want to insert "black" inside the string "hellokitty",
//  then we do InsertAt(5, CONSTLIT("black")), because 5 is the position of 
//	character 'k' in "hellokitty".
	{
	int iOriginalLength = m_sText.GetLength();
	int iInsertionLength = sText.GetLength();

	CString sHead = strSubString(m_sText, 0, iInsertPosition);
	CString sTail = strSubString(m_sText, iInsertPosition, iOriginalLength - iInsertPosition);

	m_sText = strCat(strCat(sHead, sText), sTail);
	UpdateMetrics();

	m_bInvalid = true;
	}

int CTextRun::DeleteAt(int iStartPosition, int iNumDeletions)
	{
	int iNumOrigChars = m_sText.GetLength();

	int iEndPosition = iStartPosition - iNumDeletions;

	if (iEndPosition < 0)
		{
		iEndPosition = 0;
		}

	CString sHead = strSubString(m_sText, 0, iEndPosition);
	CString sTail = strSubString(m_sText, iStartPosition, -1);
	
	m_sText = strCat(sHead, sTail);
	int iNumFinalChars = m_sText.GetLength();

	UpdateMetrics();

	m_bInvalid = true;

	return iNumOrigChars - iNumFinalChars;
	}

void CTextRun::Paint(SDocumentPaintCtx &refPaintCtx)
	{
	refPaintCtx.Screen->DrawText(refPaintCtx.x, refPaintCtx.y, *m_pFont, m_rgbTextColor, m_sText);
	}

void CTextRun::UpdateMetrics(void)
	{
	m_iRunWidth = m_pFont->MeasureText(m_sText, &m_iRunHeight);
	}

void CTextRun::SetText (const CString &sText)
	{ 
	m_sText = sText; 
	Invalidate();
	}



//	================================================================================

CTextLine::CTextLine (void) : m_iLineWidth(0), 
	m_iLineHeight(0)
	{
	CreateNewRun();
	}

void CTextLine::AppendLine(const CTextLine &refTextLine)
	{
	for (int i = 0; i < refTextLine.GetCount(); i++)
		{
		TArray<CTextRun *>::Insert(new CTextRun(*refTextLine[i]));
		}
	}

CTextRun *CTextLine::CreateNewRun(void)
	{
	CTextRun *pNewRun = new CTextRun("");
	Insert(pNewRun);
	return pNewRun;
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
	pRelevantRun->InsertAt(iChar, sText);
	}

int CTextLine::DeleteText(int iRun, int iChar, int iNumDeletions)
	{
	CTextRun *pRelevantRun = GetAt(iRun);
	return pRelevantRun->DeleteAt(iChar, iNumDeletions);
	}

void CTextLine::Paint(SDocumentPaintCtx &refPaintCtx)
	{
	CTextRun *pRelevantRun;
	UpdateMetrics();

	for (int i = 0; i < GetCount(); i++)
		{
		pRelevantRun = GetAt(i);
		pRelevantRun->Paint(refPaintCtx);
		
		refPaintCtx.x += pRelevantRun->GetWidth();
		}
	}

CString CTextLine::GetAsPlainText(void)
	{
	CString sPlainText = CString(CONSTLIT(""));

	for (int i = 0; i < GetCount(); i++)
		{
		sPlainText.Append(GetAt(i)->GetText());
		}

	return sPlainText;
	}

void CTextLine::UpdateMetrics(void)
	{
	CTextRun *pRun;
	int iMaxRunHeight = 0;
	int iCurrentRunHeight;

	for (int i = 0; i < GetCount(); i++)
		{
		pRun = GetAt(i);
		pRun->UpdateMetrics();

		iCurrentRunHeight = pRun->GetHeight();
		if (iCurrentRunHeight > iMaxRunHeight)
			{
			iMaxRunHeight = iCurrentRunHeight;
			}
		m_iLineWidth += pRun->GetWidth();
		}

	m_iLineHeight = iMaxRunHeight;
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
	CleanUp();

	InsertEmpty(refTextLine.GetCount());
	for (int i = 0; i < refTextLine.GetCount(); i++)
		{
		GetAt(i) = new CTextRun(*refTextLine[i]);
		}
	}

//	================================================================================

CTextContent::CTextContent (CString sName, CHumanInterface &HI, CTransmuterModel &model, int iWidth, int iHeight, bool bEditable, bool bCommandInput) : CTransmuterPanel(sName, HI, model, iWidth, iHeight),
m_bEditable(bEditable),
m_bCommandInput(bCommandInput)
	{
	m_IOCursor.SetCharPos(0);
	m_IOCursor.SetRunPos(0);
	m_IOCursor.SetLinePos(0);
	m_Document.PlaceCursor(m_IOCursor);
	}

CTextContent::~CTextContent(void)
	{
	}

void CTextContent::OnPanelPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	SDocumentPaintCtx PaintCtx = SDocumentPaintCtx();
	PaintCtx.x = PanelRect.GetOriginX();
	PaintCtx.y = PanelRect.GetOriginY();
	PaintCtx.Screen = &Screen;

	m_Document.Paint(PaintCtx);
	}

void CTextContent::OnPanelKeyDown(int iVirtKey, DWORD dwKeyData)
	{
	if (m_bEditable || m_bCommandInput)
		{
		if (iVirtKey == VK_BACK)
			{
			m_Document.DeleteText(m_IOCursor, 1);
			}
		else if (iVirtKey == VK_RETURN)
			{
			if (GetController() != NULL)
				{
				if (m_bCommandInput)
					{
					GetController()->HICommand(m_Document.GetAsPlainText());
					m_Document.ClearText(m_IOCursor);
					}
				// make new line
				}
			}
		}
	}

void CTextContent::OnPanelChar(char chChar, DWORD dwKeyData)
	{
	int iChar = int(chChar);
	if (m_bEditable && iChar > 31 && iChar < 127)
		{
		CString sInsertionText = CString(&chChar);
		m_Document.InsertText(m_IOCursor, sInsertionText);
		}
	}

void CTextContent::WriteText(CString sText)
	{
	m_Document.PlaceCursor(m_IOCursor);
	m_Document.InsertText(m_IOCursor, sText);
	}

CTextDocument::CTextDocument(void)
	{
	CreateNewLine();
	}

CTextDocument::~CTextDocument(void)
	{
	}

void CTextDocument::CreateNewLine(void)
	{
	m_Lines.Insert(CTextLine());
	}

void CTextDocument::PlaceCursor(CDocumentCursor &refCursor)
	{
	int iNumLines = m_Lines.GetCount();

	int iCursorLinePos = refCursor.GetLinePos();
	if (!(iCursorLinePos < iNumLines || iCursorLinePos > -1))
		{
		refCursor.SetLinePos(iNumLines - 1);
		}

	CTextLine &refRelevantLine = m_Lines[refCursor.GetLinePos()];
	int iNumRuns = refRelevantLine.GetRunCount();
	int iCursorRunPos = refCursor.GetRunPos();
	if (!(iCursorRunPos < iNumRuns || iCursorRunPos > -1))
		{
		refCursor.SetRunPos(iNumRuns - 1);
		}

	CTextRun &refRelevantRun = *(refRelevantLine.GetRunAt(refCursor.GetRunPos()));
	int iNumChars = refRelevantRun.GetText().GetLength();
	int iCursorCharPos = refCursor.GetCharPos();
	if (!(iCursorCharPos < iNumChars || iCursorCharPos > -1))
		{
		refCursor.SetCharPos(iNumChars - 1);
		}

	refCursor.ValidatePlacement();
	}

inline CDocumentCursor CTextDocument::CreateNewCursor(int iLine, int iRun, int iChar)
	{
	if (m_Lines.GetCount() == 0)
		{
		CreateNewLine();
		}
	CDocumentCursor NewCursor(iLine, iRun, iChar);
	PlaceCursor(NewCursor);

	return NewCursor;
	}

int CTextDocument::InsertText(CDocumentCursor &refIOCursor, CString sText)
	{
	if (refIOCursor.GetPlacementStatus() == false)
		{
		return 0;
		}
	else
		{
		const char *pStrChars = sText.GetPointer();
		bool bIsNewLineCharInString = false;

		for (int i = 0; i < sText.GetLength(); i++)
			{
			if (pStrChars[i] == '\n')
				{
				bIsNewLineCharInString = true;
				break;
				}
			}

		if (bIsNewLineCharInString == true)
			{
			TArray <int> aNewLineCharPositions;
			TArray <int> aLineStartPositions;
			TArray <int> aLineLengths;
			int iNumLines = 0;

			bool bLineStarted = false;

			for (int i = 0; i < sText.GetLength(); i++)
				{
				if (!bLineStarted)
					{
					if (pStrChars[i] == '\n')
						{
						aLineStartPositions.Insert(i);
						aLineLengths.Insert(0);
						iNumLines += 1;
						}
					else
						{
						aLineStartPositions.Insert(i);
						bLineStarted = true;
						}
					}
				else
					{
					if (pStrChars[i] == '\n')
						{
						bool bLineStarted = false;
						aLineLengths.Insert(i - aLineStartPositions[iNumLines]);
						iNumLines += 1;
						}
					}
				}

			for (int i = 0; i < iNumLines; i++)
				{
				m_Lines[refIOCursor.GetLinePos()].InsertText(refIOCursor.GetRunPos(), refIOCursor.GetCharPos(), strSubString(sText, aLineStartPositions[i], aLineLengths[i]));
				CreateNewLine();
				refIOCursor.MoveLinePos(1);
				PlaceCursor(refIOCursor);
				}
			}
		else
			{
			m_Lines[refIOCursor.GetLinePos()].InsertText(refIOCursor.GetRunPos(), refIOCursor.GetCharPos(), sText);
			refIOCursor.SetCharPos(refIOCursor.GetCharPos() + sText.GetLength());
			PlaceCursor(refIOCursor);
			}

		return 1;
		}
	}

int CTextDocument::DeleteText(CDocumentCursor & refIOCursor, int iNumDeletions)
	{
	if (refIOCursor.GetPlacementStatus() == false)
		{
		return 0;
		}
	else
		{
		int iNumDeletionsPerformed = m_Lines[refIOCursor.GetLinePos()].DeleteText(refIOCursor.GetRunPos(), refIOCursor.GetCharPos(), iNumDeletions);
		refIOCursor.SetCharPos(refIOCursor.GetCharPos() - iNumDeletionsPerformed);
		PlaceCursor(refIOCursor);
		return 1;
		}
	}

void CTextDocument::ClearText(CDocumentCursor & refIOCursor)
	{
	m_Lines.DeleteAll();
	CreateNewLine();

	refIOCursor.SetLinePos(0);
	refIOCursor.SetRunPos(0);
	refIOCursor.SetCharPos(0);

	PlaceCursor(refIOCursor);
	}

void CTextDocument::Paint(SDocumentPaintCtx &refPaintCtx)
	{
	CTextLine *pRelevantLine;
	int iOriginX = refPaintCtx.x;

	for (int i = 0; i < m_Lines.GetCount(); i++)
		{
		pRelevantLine = &m_Lines[i];
		pRelevantLine->Paint(refPaintCtx);
		refPaintCtx.x = iOriginX;
		refPaintCtx.y += pRelevantLine->GetHeight();
		}
	}

CString CTextDocument::GetAsPlainText(void)
	{
	CString sPlainText = CString(CONSTLIT(""));

	for (int i = 0; i < m_Lines.GetCount(); i++)
		{
		sPlainText.Append(m_Lines[i].GetAsPlainText());
		sPlainText.Append("\n");
		}

	return sPlainText;
	}


