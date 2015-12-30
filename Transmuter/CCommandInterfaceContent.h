//	CCommandInterfaceContent.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.
#pragma once

class CTextContent;
class CCommandInterfaceContent;

class CCommandInterfaceContent : public CTransmuterPanelContent
	{
	public:
		CCommandInterfaceContent (CString sName, CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
		~CCommandInterfaceContent (void);

		void SetHeaderContent (int iHeight, CString sHeaderText);

		void UpdateHeaderContent(CString sHeaderText);

		void OnKeyDown(int iVirtKey, DWORD dwKeyData);
		void OnKeyUp (int iVirtKey, DWORD dwKeyData);

		void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		int m_iCurrentLine;
		CTextContent m_Terminal;
	};
