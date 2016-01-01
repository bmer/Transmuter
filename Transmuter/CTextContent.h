//	CTextContent.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

class CTextRun;
class CTextLine;
class CDocumentCursor;
class CTextDocument;
class CTextContent;

#pragma once
class CTextRun
	{
	public:
		CTextRun (void);
		CTextRun (CString sText);

		CString GetTruncatedStrCopy(CString sInput, int iNewLength);

		void InsertAt (int iPosition, CString sText);
		void DeleteBetween (int iStartPosition, int iDeletePosition);

		inline const CString &GetText (void) { return m_sText; }
		inline void SetBackgroundColor (CG32bitPixel rgbColor) { m_rgbBackgroundColor = rgbColor; }
		inline void SetBorderRadius (int iRadius) { m_iBorderRadius = iRadius; }
		inline void SetTextColor (CG32bitPixel rgbColor) { m_rgbTextColor = rgbColor; }
		inline void SetCursor (int iLine, int iCol = 0) { m_iCursorLine = iLine; m_iCursorPos = iCol; }
		inline void SetEditable (bool bEditable = true) { m_bEditable = bEditable; }
		inline void SetFont (const CG16bitFont *pFont) { m_pFont = pFont; m_cxJustifyWidth = 0; }
		inline void SetFontTable (const IFontTable *pFontTable) { m_pFontTable = pFontTable; }
		inline void SetLineSpacing (int cySpacing) { m_cyLineSpacing = cySpacing; m_cxJustifyWidth = 0; }
		inline void SetPadding (int iPadding) { m_rcPadding.left = iPadding; m_rcPadding.top = iPadding; m_rcPadding.right = iPadding; m_rcPadding.bottom = iPadding; }
		inline void SetStyles (DWORD dwStyles) { m_dwStyles = dwStyles; m_cxJustifyWidth = 0; }
		void SetText (const CString &sText);

		int Justify (const RECT &rcRect);

		// void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
		void UpdateTicker (void) { m_iTick++; }

		inline void Invalidate (void) { m_bInvalid = true; }

	private:
		//  text in this run (all formatting is applied throughout run)
		CString m_sText;

		//  calculate boundary rectangle
		RECT CalcTextRect (const RECT &rcCtrlRect);

		//  format RTF
		void PaintText (CG32bitImage &Dest, const RECT &rcRect);

		bool m_bEditable;						//	TRUE if editable

		//  style options
		DWORD m_dwStyles;						//	AlignmentStyles
		RECT m_rcPadding;						//	Padding around the text
		int m_cyLineSpacing;					//	Extra spacing between lines
		int m_iBorderRadius;
		const CG16bitFont *m_pFont;
		CG32bitPixel m_rgbTextColor;
		CG32bitPixel m_rgbBackgroundColor;

		bool m_bRTFInvalid;						//	TRUE if we need to format rich text
		CTextBlock m_RichText;					//	Rich text to draw (only if m_sText is blank)
		const IFontTable *m_pFontTable;			//	For rich text

		TArray<CString> m_Lines;				//	Justified lines of text
		int m_cxJustifyWidth;					//	Width (in pixels) for which m_Lines
												//		was justified.
				 
		int m_iTick;							//	Cursor tick
		int m_iCursorLine;						//	Cursor position (-1 = no cursor)
		int m_iCursorPos;						//	Position in text run

		int m_bInvalid;
	};

//  =======================================================================

class CTextLine : private TArray <CTextRun *>
	{
	public:
		inline CTextLine (void) { ; }
		inline CTextLine (const CTextLine &refTextLine) { Copy(refTextLine); }
		inline ~CTextLine (void) { CleanUp(); }

		void ConcatenateLines (const CTextLine &refTextLine);
		CTextRun *AddNewRun (const CTextRun &refTextRun);
		inline CTextLine &operator=(const CTextLine &refTextLine) { CleanUp(); Copy(refTextLine); return *this; }

		void InsertText (int iRun, int iChar, CString sText);
	private:
		void CleanUp (void);
		void Copy (const CTextLine &refTextLine);
	};

//  =======================================================================


class CDocumentCursor
	{
	public:
		CDocumentCursor (void);
		CDocumentCursor (int iLine, int iRun, int iChar);

		int GetCursorPos (void);
		void UpdateCursorPos (void);

	private:
		int m_iLine;
		int m_iRun;
		int m_iChar;
	};

//  =======================================================================

class CTextDocument
	{
	public:
		inline CTextDocument (void) { ; }
		~CTextDocument (void);

		inline void AddNewLine (CTextLine &Line) { m_Lines.Insert(Line);  }

		void PlaceCursor (CDocumentCursor &refCursor)
		CDocumentCursor &CreateNewCursor (int iLine, int iRun, int iChar);
		void MoveCursorToEOD (CDocumentCursor &refCursor);
		//inline CDocumentCursor &CreateNewCursor (void) { return CreateNewCursor(0, 0, 0); }

		void InsertText(CDocumentCursor &refIOCursor, CString sText);
		void DeleteChar

		// inline TArray <CDocumentCursor *> &GetCursors(void) { return m_Cursors; }
		// Set Style

	private:
		TArray <CTextLine> m_Lines;
	};

//  =======================================================================

class CTextContent : public CTransmuterContent
	{
	public:
		CTextContent (CString sID, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		void OnContentChar (char chChar, DWORD dwKeyData);

	private:
		CString m_InputBuffer;
		CDocumentCursor m_Cursor;
		CTextDocument m_Document;
	};

//class CTextContent : public CTransmuterContent
//	{
//	public:
//		CTextContent (CString sID, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
//
//		inline const CString &GetText (void) { return m_sText; }
//		inline void SetBackgroundColor (CG32bitPixel rgbColor) { m_rgbBackgroundColor = rgbColor; }
//		inline void SetBorderRadius (int iRadius) { m_iBorderRadius = iRadius; }
//		inline void SetTextColor (CG32bitPixel rgbColor) { m_rgbTextColor = rgbColor; }
//		inline void SetCursor (int iLine, int iCol = 0) { m_iCursorLine = iLine; m_iCursorPos = iCol; }
//		inline void SetEditable (bool bEditable = true) { m_bEditable = bEditable; }
//		inline void SetFont (const CG16bitFont *pFont) { m_pFont = pFont; m_cxJustifyWidth = 0; }
//		inline void SetFontTable (const IFontTable *pFontTable) { m_pFontTable = pFontTable; }
//		inline void SetLineSpacing (int cySpacing) { m_cyLineSpacing = cySpacing; m_cxJustifyWidth = 0; }
//		inline void SetPadding (int iPadding) { m_rcPadding.left = iPadding; m_rcPadding.top = iPadding; m_rcPadding.right = iPadding; m_rcPadding.bottom = iPadding; }
//		void SetRichText (const CString &sRTF);
//		inline void SetStyles (DWORD dwStyles) { m_dwStyles = dwStyles; m_cxJustifyWidth = 0; }
//		void SetText (const CString &sText);
//
//		int Justify (void);
//		int Justify (const RECT &rcRect);
//		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
//		void UpdateTicker (void) { m_iTick++; }
//
//	private:
//		//
//		TArray <TArray <CTextRun>> m_Lines;
//
//		//  text in this run (all formatting is applied throughout run)
//		CString m_sText;
//		CString m_sRTF
//
//		//  calculate boundary rectangle
//		RECT CalcTextRect (const RECT &rcRect);
//		RECT CalcTextRect (void);
//
//		//  format RTF
//		void FormatRTF (const RECT &rcRect);
//		void PaintRTF (CG32bitImage &Dest, const RECT &rcRect);
//		void PaintText (CG32bitImage &Dest, const RECT &rcRect);
//
//		bool m_bEditable;						//	TRUE if editable
//
//												//  style options
//		DWORD m_dwStyles;						//	AlignmentStyles
//		RECT m_rcPadding;						//	Padding around the text
//		int m_cyLineSpacing;					//	Extra spacing between lines
//		int m_iBorderRadius;
//		const CG16bitFont *m_pFont;
//		CG32bitPixel m_rgbTextColor;
//		CG32bitPixel m_rgbBackgroundColor;
//
//		bool m_bRTFInvalid;						//	TRUE if we need to format rich text
//		CTextBlock m_RichText;					//	Rich text to draw (only if m_sText is blank)
//		const IFontTable *m_pFontTable;			//	For rich text
//
//		TArray<CString> m_Lines;				//	Justified lines of text
//		int m_cxJustifyWidth;					//	Width (in pixels) for which m_Lines
//												//		was justified.
//
//		int m_iTick;							//	Cursor tick
//		int m_iCursorLine;						//	Cursor position (-1 = no cursor)
//		int m_iCursorPos;						//	Position in text run
//
//	};