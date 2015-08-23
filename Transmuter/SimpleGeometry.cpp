//	SimpleGeometry.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

bool IsPointInRect(int px, int py, int rx, int ry, int w, int h)
	{
	return ((rx <= px <= rx + w) && (ry <= py <= ry + h));
	}

bool IsRect1InRect2(int r1x, int r1y, int w1, int h1, int r2x, int r2y, int w2, int h2)
	{
	if (IsPointInRect(r1x, r1y, r2x, r2y, w2, h2))
		{
		return TRUE;
		}
	else if (IsPointInRect(r1x + w1, r1y, r2x, r2y, w2, h2))
		{
		return TRUE;
		}
	else if (IsPointInRect(r1x, r1y + h1, r2x, r2y, w2, h2))
		{
		return TRUE;
		}
	else if (IsPointInRect(r1x + w1, r1y + h1, r2x, r2y, w2, h2))
		{
		return TRUE;
		}
	else
		{
		return FALSE;
		}
	}