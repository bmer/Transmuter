//	CTextContent.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

class CTextRun;
class CTextCursor;
class CTextContent;

#pragma once
class CTextRun
	{
	public:
		CTextRun (CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);

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

		int Justify (void);
		int Justify (const RECT &rcRect);

		// void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
		void UpdateTicker (void) { m_iTick++; }

	private:
		//  text in this run (all formatting is applied throughout run)
		CString m_sText;

		//  calculate boundary rectangle
		RECT CalcTextRect (const RECT &rcRect);
		RECT CalcTextRect (void);

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

		CTextDocument m_Document;
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

	private:
		void CleanUp (void);
		void Copy (const CTextLine &refTextLine);
	};

//  =======================================================================

class CTextDocument :
	{
	public:
		inline CTextDocument (void) { ; }
		inline CTextDocument (const CTextDocument &refTextDocument) { Copy(refTextDocument); }
		inline ~CTextDocument (void) { CleanUp(); }

		void ConcatenateDocuments (const CTextDocument &refTextDocument);
		CTextRun *AddNewLine (const CTextLine &refTextLine);

		// Set Style

		// void InsertText(Cursor &ioCursor, CString String)

	private:
		TArray <CTextLine> m_Lines;
		void CleanUp (void);
		void Copy (const CTextDocument &refTextDocument);
	};

//  =======================================================================

class CTextContent : public CTransmuterPanelContent
	{
	public:
		CTextContent (CString sName, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		CTextDocument m_Document;
		TArray <CDocumentCursor> m_Cursors;
	};

//class CTextContent : public CTransmuterPanelContent
//	{
//	public:
//		CTextContent (CString sName, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
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