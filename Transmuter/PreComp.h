//	PreComp.h

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif

#include <windows.h>
#include <ddraw.h>
#include <iostream>
#include <exception>
#include "Alchemy.h"
#include "JPEGUtil.h"
#include "DirectXUtil.h"
#include "XMLUtil.h"
#include "Resource.h"
#include "TransmuterController.h"
#include "CPanel.h"
#include "CMainSession.h"
#include "BasicPanelContents.h"
#include "CScrollBarContent.h"
#include "CContextContent.h"
#include "CEditorPanelContent.h"
#include "CPreviewPanelContent.h"
#include "CTextContent.h"
#include "CCommandInterfaceContent.h"
#include "UserSettingsDef.h"
#include "SimpleGeometry.h"
#include "SimpleSorting.h"