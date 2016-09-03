//	CTextContent.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

struct SDocumentPaintCtx;
class CTextRun;
class CTextLine;
class CDocumentCursor;
class CTextDocument;
class CTextContent;

struct SDocumentPaintCtx {
	SDocumentPaintCtx (void);

	CG32bitImage *Screen;
	int x;
	int y;
	};

class CTextRun
	{
	friend class CTextLine;
	public:
		CTextRun (void);
		CTextRun (CString sText);

		void InsertAt (int iPosition, CString sText);
		int DeleteAt (int iStartPosition, int iNumDeletions);

		inline const CString &GetText (void) { return m_sText; }
		inline void SetBackgroundColor (CG32bitPixel rgbColor) { m_rgbBackgroundColor = rgbColor; }
		inline void SetEditable (bool bEditable = true) { m_bEditable = bEditable; }
		inline void SetFont (const CG16bitFont *pFont) { m_pFont = pFont; }
		inline void SetPadding (int iPadding) { m_rcPadding.left = iPadding; m_rcPadding.top = iPadding; m_rcPadding.right = iPadding; m_rcPadding.bottom = iPadding; }
		void SetText (const CString &sText);

		inline void Invalidate (void) { m_bInvalid = true; }
		inline int GetHeight (void) { return m_iRunHeight; }
		inline int GetWidth (void) { return m_iRunWidth; }

		void Paint(SDocumentPaintCtx &refPaintCtx);

	protected:
		void UpdateMetrics (void);

	private:
		//  text in this run (all formatting is applied throughout run)
		CString m_sText;

		bool m_bEditable;						//	TRUE if editable

		//  style options
		RECT m_rcPadding;						//	Padding around the text
		const CG16bitFont *m_pFont;
		CG32bitPixel m_rgbTextColor;
		CG32bitPixel m_rgbBackgroundColor;

		int m_bInvalid;

		int m_iRunWidth;
		int m_iRunHeight;
	};

//  =======================================================================

class CTextLine : private TArray <CTextRun *>
	{
	public:
		CTextLine (void);
		inline CTextLine (const CTextLine &refTextLine) : m_iLineWidth(0), m_iLineHeight(0) { Copy(refTextLine); }
		inline ~CTextLine (void) { CleanUp(); }

		void AppendLine (const CTextLine &refTextLine);
		CTextRun *CreateNewRun(void);
		CTextRun *AddNewRun (const CTextRun &refTextRun);
		inline CTextLine &operator=(const CTextLine &refTextLine) { CleanUp(); Copy(refTextLine); return *this; }

		void InsertText (int iRun, int iChar, CString sText);
		int DeleteText (int iRun, int iChar, int iNumDeletions);

		inline int GetRunCount (void) { return GetCount(); }
		inline CTextRun *GetRunAt (int iIndex) { return GetAt(iIndex); }

		inline int GetHeight (void) { return m_iLineHeight; }
		inline int GetWidth (void) { return m_iLineWidth; }

		void Paint(SDocumentPaintCtx &refPaintCtx);

		CString GetAsPlainText (void);

	private:
		void UpdateMetrics (void);
		void CleanUp (void);
		void Copy (const CTextLine &refTextLine);

		int m_iLineWidth;
		int m_iLineHeight;
	};

//  =======================================================================

class CDocumentCursor
	{
	friend class CTextDocument;

	public:
		CDocumentCursor (void);
		CDocumentCursor (int iLine, int iRun, int iChar);

		inline int GetLinePos (void) { return m_iLine; }
		inline int GetRunPos (void) { return m_iRun; }
		inline int GetCharPos (void) { return m_iChar; }
		
		inline void SetLinePos (int iPos) { m_iLine = iPos; InvalidatePlacement(); }
		inline void SetRunPos (int iPos) { m_iRun = iPos; InvalidatePlacement(); }
		inline void SetCharPos (int iPos) { m_iChar = iPos; InvalidatePlacement(); }

		inline void MoveLinePos (int iShift) { m_iLine += iShift; InvalidatePlacement(); }
		inline void MoveRunPos (int iShift) { m_iRun += iShift; InvalidatePlacement(); }
		inline void MoveCharPos (int iShift) { m_iChar += iShift; InvalidatePlacement(); }

		inline bool GetPlacementStatus (void) { return m_bIsPlaced; }

	protected:
		inline void ValidatePlacement (void) { m_bIsPlaced = true; }
		inline void InvalidatePlacement (void) { m_bIsPlaced = false;  }

	private:
		int m_iLine;
		int m_iRun;
		int m_iChar;

		bool m_bIsPlaced;
	};

//  =======================================================================

class CTextDocument
	{
	public:
		CTextDocument (void);
		~CTextDocument (void);

		void PlaceCursor (CDocumentCursor &refCursor);
		CDocumentCursor CreateNewCursor (int iLine, int iRun, int iChar);
		// (document) int Justify (const RECT &rcRect);

		int InsertText(CDocumentCursor &refIOCursor, CString sText);
		int DeleteText(CDocumentCursor &refIOCursor, int iNumDeletions);
		void ClearText(CDocumentCursor &refIOCursor);

		void Paint(SDocumentPaintCtx &refPaintCtx); 
		CString GetAsPlainText(void);
		// inline TArray <CDocumentCursor *> &GetCursors(void) { return m_Cursors; }
		// Set Style

	protected:
		void CreateNewLine (void);

	private:
		TArray <CTextLine> m_Lines;
		CTextContent *m_pParentContent;
	};

//  =======================================================================

class CTextContent : public CTransmuterPanel
	{
	public:
		CTextContent (CString sName, CHumanInterface &HI, CTransmuterModel &model, int iWidth, int iHeight, bool bEditable=false, bool bCommandInput=false);

		~CTextContent (void);

		void OnContentPaint (CG32bitImage &Screen, const RECT &rcInvalid);
		void OnPanelKeyDown (int iVirtKey, DWORD dwKeyData);
		void OnPanelChar (char chChar, DWORD dwKeyData);

		void WriteText (CString sText);

	private:
		CString m_InputBuffer;
		CDocumentCursor m_IOCursor;
		CTextDocument m_Document;

		bool m_bEditable;
		bool m_bCommandInput;
	};

