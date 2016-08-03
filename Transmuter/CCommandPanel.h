//	CCommandPanel.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.
#pragma once

class CTransmuterPanel;
class CTextContent;
class CCommandPanel;

class CCommandPanel : public CTransmuterPanel
	{
	public:
		CCommandPanel (CString sName, CHumanInterface &HI, CTransmuterModel &model, int iWidth, int iHeight);
		~CCommandPanel (void);

		void OnKeyDown(int iVirtKey, DWORD dwKeyData);
		void OnKeyUp (int iVirtKey, DWORD dwKeyData);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		inline CTextContent *GetInputContent(void) { return m_pInputPanel; }
		inline CTextContent *GetOutputContent(void) { return m_pOutputPanel; }

	private:
		int m_iCurrentLine;
		CTextContent *m_pInputPanel;
		CTextContent *m_pOutputPanel;
	};
