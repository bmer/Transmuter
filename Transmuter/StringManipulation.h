//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

TArray <CString> SplitString (CString *pInputStr, CString sSplitToken, int iNumSplits=-1);
CString StripHeadWhitespace (CString sInputStr);
CString StripTailWhitespace (CString sInputStr);