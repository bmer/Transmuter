//	BasiCTransmuterPanelContents.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class IPanel;
class CTitlePanel;

class CTransmuterPanel : public IPanel
	{
	public:
		CTransmuterPanel(CString sPanelName, CHumanInterface &HI, CTransmuterModel &model);

		void CreateTitlePanel (CString sPanelName, CString sTitleText, int iWidth, int iHeight, const CG16bitFont *pFont=&(g_pHI->GetVisuals().GetFont(fontConsoleMediumHeavy)), CG32bitPixel rgbTextColor=CG32bitPixel(255,255,255), CG32bitPixel rgbBackgroundColor=CG32bitPixel(140,140,140));

		void UpdateTitleText (CString sText);
		inline CString GetTitleText (void) { if (m_pTitlePanel != NULL) { return m_pTitlePanel->GetText(); } }
		void UpdateTitleFont (const CG16bitFont *pFont);
		void UpdateTitleTextColor (CG32bitPixel rgbColor);
		void UpdateTitleBackgroundColor (CG32bitPixel rgbColor);

		inline CTransmuterModel &GetModel (void) { return m_model; }

		virtual void OnPanelPaint (CG32bitImage &Screen, const RECT &rcInvalid) { ; }

	protected:
		CTransmuterModel &m_model;
		CTitlePanel *m_pTitlePanel;
	};
