//	CLoadingSession.cpp
//
//	CSTransmuter class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CSLoading::CSLoading (CHumanInterface &HI) : IHISession(HI)
	//	CSTransmuter constructor
	{
	}

void CSLoading::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(64, 64, 64));
	
	CG32bitPixel TextColor = CG32bitPixel(255, 255, 255);

	const CVisualPalette &VI = g_pHI->GetVisuals();
	const CG16bitFont &font = VI.GetFont(fontConsoleMediumHeavy);

	Screen.DrawText(Screen.GetWidth()/2, Screen.GetHeight()/2, font, TextColor, CONSTLIT("Loading..."));
	}

