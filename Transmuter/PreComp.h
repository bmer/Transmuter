// Optimization during compile tile -- what it does is that you put you 

//	PreComp.h

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <windows.h>
#include <ddraw.h>
#include "Alchemy.h"
#include "JPEGUtil.h"
#include "DirectXUtil.h"
#include "XMLUtil.h"
#include "Resource.h"
#include "Transmuter.h"
#include "TransmuterSession.h"
#include "UserSettingsDef.h"