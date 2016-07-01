//	TransmuterController.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.


#pragma once

class CTransmuterPanel;
class CScrollBarPanel;

class CScrollBarPanel : public CTransmuterPanel
	{
	public:
		CScrollBarPanel(CString sID, CHumanInterface &HI, IPanel &AssociatedPanel, CTransmuterModel &model, IPanel &pPanelToScroll);

		//void OnPaint(CG32bitImage &Screen, const RECT &rcInvalid);

		//void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		//void OnLButtonUp (int x, int y, DWORD dwFlags);

		//void OnRButtonDown (int x, int y, DWORD dwFlags);
		//void OnRButtonUp (int x, int y, DWORD dwFlags);

		inline void SetBackgroundColor(CG32bitPixel BGColor) { m_rgbBackgroundColor = BGColor; }

		inline void SetScrollBarColor (CG32bitPixel ScrollBarColor) { m_ScrollBarColor = ScrollBarColor; }

		void DetermineVisibility (void);
		void CalculateBarRect (void);
		void CalculateSlideRect (void);
		inline bool IsBarVisible (void) { return m_bVisible; }

	private:
		CG32bitPixel m_rgbBackgroundColor;
		DWORD dwBarOrientation;

		IPanel &m_PanelToScroll;
		CG32bitPixel m_ScrollBarColor;

		RECT m_rcSlide;
		RECT m_rcBar;

		bool m_bVisible;
	};


