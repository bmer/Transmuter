//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CTransmuterSession::CTransmuterSession (CHumanInterface &HI, CTransmuterModel &model) : IHISession(HI),
	m_Model(model)
	//	CTransmuterSession constructor
	{
	}

void CTransmuterSession::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	CUniverse &universe = m_Model.GetUniverse();

	TArray<CString> ExtensionNames = m_Model.GetExtensionNames();

	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(0, 102, 102));
	
	CG32bitPixel textColor = CG32bitPixel(255, 255, 255);

	const CVisualPalette &VI = g_pHI->GetVisuals();
	const CG16bitFont &font = VI.GetFont(fontConsoleMediumHeavy);

	for (int i = 0; i < ExtensionNames.GetCount(); i++) 
		{
		Screen.DrawText(10, 10 + i*20, font, textColor, ExtensionNames[i]);
		};

	}

//	====================================================================

CTextEditorTabSession::CTextEditorTabSession (CHumanInterface &HI, CTextEditorSession &parentTextEditorSession) : IHISession(HI),
		m_parentTextEditorSession(parentTextEditorSession)
	//	CTransmuterSession constructor
	{
	}

//	====================================================================

CTextEditorSession::CTextEditorSession (CHumanInterface &HI, CTransmuterSession &mainTransmuterSession) : IHISession(HI),
		m_mainTransmuterSession(mainTransmuterSession)
	//	CTransmuterSession constructor
	{
	}

//	====================================================================

CNavigationSession::CNavigationSession (CHumanInterface &HI, CTransmuterSession &mainTransmuterSession) : IHISession(HI),
		m_mainTransmuterSession(mainTransmuterSession)
	//	CTransmuterSession constructor
	{
	}

//	====================================================================

CCommandRibbonSession::CCommandRibbonSession (CHumanInterface &HI, CTransmuterSession &mainTransmuterSession) : IHISession(HI),
		m_mainTransmuterSession(mainTransmuterSession)
	//	CTransmuterSession constructor
	{
	}
