//	CArrayContainer.h
//
//	Copyright (c) 2016 by Kronosaur Productions, LLC. All Rights Reserved.
#pragma once

#define SMOOTH_VERTICAL		0
#define SMOOTH_HORIZONTAL	1

class CArrayContainer : public CContainer
	{
	public:
		CArrayContainer (CString sName, CHumanInterface &HI, int iWidth=0, int iHeight=0);
		
		void SmoothOut (DWORD dwSmoothType);
		bool PlacePanel (IPanel *pPanel, int iRelativeOriginX, int iRelativeOriginY);

		void ChangePanelIndex (int iOldPanelIndex, int iNewPanelIndex);
		void DeletePanel (int iPanelIndex);

		TArray <IPanel *> GetPanelsContainingPoint (int x, int y);
		int GetPanelIndex (IPanel *pPanel);

		void HidePanel (int iPanelIndex);
		void HideAll (void);
		void ShowPanel (int iPanelIndex);
		void ShowAll (void);
	private:
		TArray <int> SortByPanelRectEdgeLocation (DWORD dwEdge);
		TArray <IPanel *> m_Panels;

	};