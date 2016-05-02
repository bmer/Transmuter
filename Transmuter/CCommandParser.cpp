//	BasicPanelContents.cpp
//
//	Definitions of the basic sessions declared in BasicPanelContents.h
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

#pragma once

CCommandParser::CCommandParser(void)
	{
	};

TArray <CString> CCommandParser::ParseStr(CString sStr)
	{
	TArray <CString> aParseResult;

	sStr = StripHeadWhitespace(sStr);
	sStr = StripTailWhitespace(sStr);

	TArray <CString> aTokens = SplitString(&sStr, " ", 1);

	return aTokens;
	}
