//	CTextAreaSession.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

class CTextAreaSession;

#pragma once

//  =======================================================================

class CTextAreaSession : public CTransmuterSession
	{
	public:
		CTextAreaSession (CHumanInterface &HI, CPanel &AssociatedPanel);

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
		void SetRichText (const CString &sRTF);
		inline void SetStyles (DWORD dwStyles) { m_dwStyles = dwStyles; m_cxJustifyWidth = 0; }
		void SetText (const CString &sText);

		int Justify (void);
		int Justify (const RECT &rcRect);
		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
		void UpdateTicker (void) { m_iTick++; }

	private:
		RECT CalcTextRect (const RECT &rcRect);
		RECT CalcTextRect (void);
		void FormatRTF (const RECT &rcRect);
		void PaintRTF (CG32bitImage &Dest, const RECT &rcRect);
		void PaintText (CG32bitImage &Dest, const RECT &rcRect);

		CString m_sText;						//	Text text to draw
		CString m_sRTF;							//	Rich text to draw (only if m_sText is blank)

		bool m_bEditable;						//	TRUE if editable
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
		int m_iCursorPos;						//	Position in line
	};