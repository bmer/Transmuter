//	CCommandPanel.h
//
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.
#pragma once

class CSplitContainer : public CContainer
	{
	public:
		enum ESplitDirns
			{
				V,
				H
			};

		CSplitContainer(CString sName, CHumanInterface &HI, int iWidth = 0, int iHeight = 0);
		bool PlacePanel (IPanel *pPanel, ESplitDirns SplitDirn, int iPanelIndex);

		void SmoothOut(void);

		void FlipPanelLocation (void);

		void inline SetSeparatorPosition (int iSeparatorPos) { m_iSeparatorPos = iSeparatorPos; }
		void inline SetSeparatorPositionFactor (float fSeparatorPosFactor) { m_fSeparatorPosFactor = fSeparatorPosFactor; }
		void inline SetSeparatorThickness (int iSeparatorThickness) { m_iSeparatorThickness = iSeparatorThickness; }
		inline int GetSeparatorPosition (void) { return m_iSeparatorPos; }
		inline float GetSeparatorPositionFactor (void) { return m_fSeparatorPosFactor; }
		inline int GetSeparatorThickness (void) { return m_iSeparatorThickness; }

		void DeletePanel (int iPanelIndex);

		TArray <IPanel *> GetPanelsContainingPoint (int x, int y);
		int GetPanelIndex (IPanel *pPanel);
		void HidePanel (int iPanelIndex);
		void HideAll (void);
		void ShowPanel (int iPanelIndex);
		void ShowAll (void);

		virtual void OnContentPaint (CG32bitImage &Screen, const RECT &rcInvalid) override;
		void PaintSeparator (CG32bitImage &Scren, const RECT &rcInvalid);
		bool PanelShouldBePainted (int iPanelIndex);

	private:
		ESplitDirns m_SplitDirn;
		float m_fSeparatorPosFactor;
		int m_iSeparatorPos;
		int m_iSeparatorThickness;

		IPanel *m_pLeafPanel0;
		IPanel *m_pLeafPanel1;
	};

