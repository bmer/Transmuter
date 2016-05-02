//	BasiCTransmuterPanelContents.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#define SCROLL_HORIZONTAL			0
#define SCROLL_VERTICAL				1
#define HEADERBAR_STYLE_TAB			2
#define HEADERBAR_STYLE_FULL		3

class IPanel;
class CTransmuterContent;
class CHeaderContent;
class CTextContent;
class CMainSession;
class CSeparatorContent;

class CTransmuterContent : public IPanelContent
	{
	public:
		CTransmuterContent(CString sContentName, CHumanInterface &HI, IPanel &AssociatedPanel, CTransmuterModel &model);

		//  This destructor takes care of:
		//		* deleting the associated HeaderPanelContent, if it is not NULL
		~CTransmuterContent (void);

		inline CHeaderContent *GetHeaderPanelContent (void) { m_pHeaderContent; }
		void SetHeaderContent (CString sID, CString sHeaderText, int iWidth, int iHeight, const CG16bitFont *pFont=&(g_pHI->GetVisuals().GetFont(fontConsoleMediumHeavy)), CG32bitPixel rgbTextColor=CG32bitPixel(255,255,255), CG32bitPixel rgbBackgroundColor=CG32bitPixel(140,140,140));
		void UpdateHeaderText (CString sHeaderText);
		void UpdateHeaderFont (const CG16bitFont *pFont);
		void UpdateHeaderTextColor (CG32bitPixel rgbColor);
		void UpdateHeaderBackgroundColor (CG32bitPixel rgbColor);

		inline CTransmuterModel &GetModel (void) { return m_model; }
		inline CString GetStringID (void) { return m_sID; }

		// virtual CString ExecuteCommand (CString sCommand, void *pData=NULL);
	protected:
		//  TrasnmuterPanelContents have a name string -- it must be supplied
		//  initializing a TransmuterPanelContent object
		const CString m_sID;

		//  The TransmuterModel. 
		CTransmuterModel &m_model;
	};

//  =======================================================================
//  HeaderPanelContents display relevant information regarding the panel at the top;
//  for example, the panel name. Critically, HeaderPanelContents allow interaction which
//  might affect their associated session. CHeaderContent should be derived from in 
//  order to specialize them for the particular session they are intended for. 

class CHeaderContent : public IPanelContent
	{
	public:
		CHeaderContent (CString sID, CString sHeaderText, CHumanInterface &HI, IPanel &AssociatedPanel, CTransmuterContent &AssociatedContent);
		~CHeaderContent (void);

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);

		inline void SetHeaderText (CString sText) { m_sHeaderText = sText;  }
		inline CString GetHeaderText (void) { return m_sHeaderText; }
		inline void SetHeaderTextFont (const CG16bitFont *pFont) { m_pFont = pFont; }
		inline void SetHeaderTextColor (CG32bitPixel rgbColor) { m_rgbTextColor = rgbColor;  }
		inline void SetBackgroundColor (CG32bitPixel rgbColor) { m_rgbBackgroundColor = rgbColor; }

	private:
		CString m_sHeaderText;
		const CG16bitFont *m_pFont;
		CG32bitPixel m_rgbTextColor;
		CG32bitPixel m_rgbBackgroundColor;
	};

class CSeparatorContent : public IPanelContent
	{
	public:
		CSeparatorContent (CString sID, CHumanInterface &HI, IPanel &AssociatedPanel, bool bHorizontal);
		~CSeparatorContent (void);

		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
		inline void SetBackgroundColor (CG32bitPixel rgbColor) { m_rgbBackgroundColor = rgbColor; }

		inline void SetWidth (int iVisualWidth) { GetAssociatedPanel().PanelRect.SetWidth(iVisualWidth); }
		inline void SetHeight (int iVisualHeight) { GetAssociatedPanel().PanelRect.SetHeight(iVisualHeight); }
		
		inline void OnLButtonDown(int x, int y, DWORD dwFlags, bool *retbCapture) { m_rgbBackgroundColor = CG32bitPixel(255, 255, 255);  m_iLastX = x; m_iLastY = y; *retbCapture = true; }
		inline void OnLButtonUp(int x, int y, DWORD dwFlags) { if (m_bHorizontal) { GetAssociatedPanel().PanelRect.ShiftOrigin(x - m_iLastX, 0); } else { GetAssociatedPanel().PanelRect.ShiftOrigin(0, y - m_iLastY); } m_rgbBackgroundColor = CG32bitPixel(90, 90, 90); }

		inline int GetLeftEdgePosition (void) { return GetAssociatedPanel().PanelRect.GetEdgePosition(0); }
		inline int GetTopEdgePosition (void) { return GetAssociatedPanel().PanelRect.GetEdgePosition(1); }
		inline int GetRightEdgePosition (void) { return GetAssociatedPanel().PanelRect.GetEdgePosition(2); }
		inline int GetBottomEdgePosition (void) { return GetAssociatedPanel().PanelRect.GetEdgePosition(3); }

	private:
		int m_iLastX;
		int m_iLastY;

		bool m_bHorizontal; // if not horizontal, then is vertical
		CG32bitPixel m_rgbBackgroundColor;
	};