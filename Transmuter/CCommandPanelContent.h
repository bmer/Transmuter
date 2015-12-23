//	CCommandPanelContent.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.
#pragma once

class CCommandPanelContent;

class CCommandPanelContent : public CTransmuterPanelContent
	{
	CCommandPanelContent (CHumanInterface &HI, CPanel &AssociatedPanel, CTransmuterModel &model);
	
	void OnKeyDown(int iVirtKey, DWORD dwKeyData);
	void OnKeyUp (int iVirtKey, DWORD dwKeyData);

	void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

	private:
		int m_iCurrentLine;
		CTextArea m_Terminal;
	};
