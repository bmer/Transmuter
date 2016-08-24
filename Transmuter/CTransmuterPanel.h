//	CTransmuterPanel.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class IPanel;
class CTitlePanel;

class CTransmuterPanel : public CArrayContainer
	{
	public:
		CTransmuterPanel(CString sName, CHumanInterface &HI, CTransmuterModel &model, int iWidth=0, int iHeight=0);

		inline CTitlePanel *GetTitlePanel (void) { return m_pTitlePanel; }
		void CreateTitlePanel (CString sName, CString sTitleText, int iRelativeOriginX=0, int iRelativeOriginY=0, const CG16bitFont *pFont=&(g_pHI->GetVisuals().GetFont(fontConsoleMediumHeavy)), CG32bitPixel rgbTextColor=CG32bitPixel(255,255,255), CG32bitPixel rgbBackgroundColor=CG32bitPixel(140,140,140));
		void SetTitleWidth (int iWidth);
		void SetTitleHeight (int iHeight);
		void UpdateTitleText (CString sText);
		inline CString GetTitleText (void) { if (m_pTitlePanel != NULL) { return m_pTitlePanel->GetText(); } }
		void UpdateTitleFont (const CG16bitFont *pFont);
		void UpdateTitleTextColor (CG32bitPixel rgbColor);
		void UpdateTitleBackgroundColor (CG32bitPixel rgbColor);

		inline CTransmuterModel &GetModel (void) { return m_Model; }

	protected:
		CTransmuterModel &m_Model;
		CTitlePanel *m_pTitlePanel;
	};
