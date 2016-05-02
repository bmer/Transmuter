//	PreComp.h

#pragma once

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
#include "TSE.h"
#include "TSUI.h"
#include "Resource.h"
#include "TransmuterModel.h"
#include "BackgroundTasks.h"
#include "TransmuterController.h"
#include "Panel.h"
#include "CCommandParser.h"
#include "CTitlePanel.h"
#include "CTransmuterPanel.h"
#include "CMainSession.h"
#include "CScrollBarContent.h"
#include "CContextContent.h"
#include "CEditorContent.h"
#include "CPreviewContent.h"
#include "CTextContent.h"
#include "CCommandInterfaceContent.h"
#include "UserSettingsDef.h"
#include "SimpleGeometry.h"
#include "SimpleSorting.h"
#include "StringManipulation.h"