//	CCommandInterfaceContent.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.
#pragma once

class CTextContent;
class CInputContent;
class COutputcontent;
class CCommandInterfaceContent;

//class CInputContent : public CTransmuterContent
//	{
//	public:
//		CInputContent (CString sID, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
//
//		void OnKeyDown (int iVirtKey, DWORD dwKeyData);
//		void OnKeyUp (int iVirtKey, DWORD dwKeyData);
//
//		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
//
//	private:
//		CTextContent m_InputText;
//	};
//
//class COutputContent : public CTransmuterContent
//	{
//	public:
//		COutputContent (CString sID, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
//
//		void OnPaint (CG32bitImage &Screen, const RECT&rcInvalid);
//
//	private:
//		CTextContent m_OutputText;
//	};

class CCommandInterfaceContent : public CTransmuterContent
	{
	public:
		CCommandInterfaceContent (CString sID, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
		~CCommandInterfaceContent (void);

		void OnKeyDown(int iVirtKey, DWORD dwKeyData);
		void OnKeyUp (int iVirtKey, DWORD dwKeyData);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		inline CTextContent *GetInputContent(void) { return m_pInputContent; }
		inline CTextContent *GetOutputContent(void) { return m_pOutputContent; }

	private:
		int m_iCurrentLine;
		CTextContent *m_pInputContent;
		CTextContent *m_pOutputContent;
	};
