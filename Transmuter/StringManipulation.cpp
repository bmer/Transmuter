//	StringManipulation.cpp
//
//	CTextContent class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

TArray <CString> SplitString(CString *pInputStr, CString sSplitToken, int iNumSplits)
	{
	int iSplitTokenLen = sSplitToken.GetLength();

	TArray <CString> aBrokenString;

	if (iSplitTokenLen > 0)
		{
		int iNumSplitTokensFound = 0;
		int iInputStrLen = pInputStr->GetLength();
		char *pInputStrChars = pInputStr->GetPointer();

		CString sSubStr;

		char cSplitTokenHead = sSplitToken.GetPointer()[0];
	    
		bool bTokenStarted = false;

		int i = 0;
		int iLastTokenEndPosition = 0;

		while (i < iInputStrLen && (iNumSplits == -1 || iNumSplitTokensFound + 1 < iNumSplits))
			{
			if (iInputStrLen - i + 1 < iSplitTokenLen)
				{
				break;
				}
			else
				{
				if (pInputStrChars[i] == cSplitTokenHead)
					{
					if (iSplitTokenLen > 1)
						{
						sSubStr = strSubString(*pInputStr, i, iSplitTokenLen);

						if (strCompare(sSubStr, sSplitToken))
							{
							aBrokenString.Insert(strSubString(*pInputStr, iLastTokenEndPosition, i - iLastTokenEndPosition));
							iLastTokenEndPosition = i + iSplitTokenLen;
							iNumSplitTokensFound++;
							}
						}
					else
						{
						aBrokenString.Insert(strSubString(*pInputStr, iLastTokenEndPosition, i - iLastTokenEndPosition));
						iLastTokenEndPosition = i + iSplitTokenLen;
						iNumSplitTokensFound++;
						}
					}
				}
			}
		}

	return aBrokenString;
	}

CString StripHeadWhitespace(CString sInputStr)
	{
	int iOffset = 0;
	char *pStrChars = sInputStr.GetPointer();

	for (int i = 0; i < sInputStr.GetLength(); i++)
		{
		if (pStrChars[i] == ' ')
			{
			iOffset++;
			}
		else
			{
			break;
			}
		}

	return strSubString(sInputStr, iOffset);
	}

CString StripTailWhitespace(CString sInputStr)
	{
	int iOrigLength = sInputStr.GetLength();
	int iNewLength = sInputStr.GetLength();

	char *pStrChars = sInputStr.GetPointer();

	for (int i = iOrigLength - 1; i > -1; i--)
		{
		if (pStrChars[i] == ' ')
			{
			iNewLength--;
			}
		else
			{
			break;
			}
		}

	return strSubString(sInputStr, 0, iNewLength);
	}
