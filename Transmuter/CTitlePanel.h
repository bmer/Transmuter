//  TitlePanels display relevant information regarding the panel at the top;
//  for example, the panel name.

#pragma once

class IPanel;
class CTitlePanel;

class CTitlePanel : public IPanel
	{
	public:
		CTitlePanel (CString sName, CHumanInterface &HI, int iWidth, int iHeight, CString sTitleText);

		inline void SetText (CString sText) { m_sTitleText = sText; }
		inline CString GetText (void) { return m_sTitleText; }
		inline void SetTextFont (const CG16bitFont *pFont) { m_pFont = pFont; }
		inline void SetTextColor (CG32bitPixel rgbColor) { m_rgbTextColor = rgbColor; }
		inline void SetBackgroundColor (CG32bitPixel rgbColor) { m_rgbBackgroundColor = rgbColor; }

		void OnContentPaint (CG32bitImage &Screen, const RECT &rcInvalid) override;

	private:
		CString m_sTitleText;
		const CG16bitFont *m_pFont;
		CG32bitPixel m_rgbTextColor;
		CG32bitPixel m_rgbBackgroundColor;
	};