//	CCommandInterfaceContent.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.
#pragma once

class CTransmuterPanel;
class CTextContent;
class CCommandInterface;

class CCommandInterface : public CTransmuterPanel
	{
	public:
		CCommandInterface (CString sName, CHumanInterface &HI, CTransmuterModel &model, int iOriginX, int iOriginY, int iWidth, int iHeight);
		~CCommandInterface (void);

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
