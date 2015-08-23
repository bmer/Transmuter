//	SimpleGeometry.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

bool IsPointInRect(int px, int py, int rx, int ry, int w, int h)
//  IsPointInRect
// 
//  Checks to see if given point is inside given rectangle. 
//  rx and ry are the coordinates of the TOP LEFT HAND CORNER of the rectangle.
	{
	return ((rx < px < rx + w) && (ry < py < ry + h));
	}

bool IsRect1InRect2(int r1x, int r1y, int w1, int h1, int r2x, int r2y, int w2, int h2)
//  IsRect1InRect2
//  
//  Returns TRUE if Rectangle 1 is entirely contained in Rectangle 2, FALSE otherwise.
//  rjx and rjy (where j in {1, 2}) are the coordinates of the TOP LEFT HAND CORNER of a rectangle.
	{
	return IsPointInRect(r1x, r1y, r2x, r2y, w2, h2) &&
		   IsPointInRect(r1x + w1, r1y, r2x, r2y, w2, h2) &&
		   IsPointInRect(r1x, r1y + h1, r2x, r2y, w2, h2) &&
		   IsPointInRect(r1x + w1, r1y + h1, r2x, r2y, w2, h2);
	}

TArray <bool> DoesRect1ClashRect2(int r1x, int r1y, int w1, int h1, int r2x, int r2y, int w2, int h2)
//  DoesRect1ClashRect2
//
//  Checks if any part of Rectangle 1 is in Rectangle 2, by checking if any corner
//  of Rectangle 1 is in Rectangle 2. Returns an array of booleans, where the index
//  corresponds with the corner number, and the value corresponds with TRUE if
//  corner is inside the array; FALSE otherwise.
//
//  Rectangle corner numbering as follows:
//      0-----------1
//      |           |
//      |           |
//      3-----------2

	{
	TArray <bool> CornerStatus;

	for (int i = 0; i < 3; i++)
		{
		CornerStatus[i] = FALSE;
		}

	if (IsPointInRect(r1x, r1y, r2x, r2y, w2, h2))
		{
		CornerStatus[0] = TRUE;
		}

	if (IsPointInRect(r1x + w1, r1y, r2x, r2y, w2, h2))
		{
		CornerStatus[1] = TRUE;
		}


	if (IsPointInRect(r1x + w1, r1y + h1, r2x, r2y, w2, h2))
		{
		CornerStatus[2] = TRUE;
		}

	if (IsPointInRect(r1x, r1y + h1, r2x, r2y, w2, h2))
		{
		CornerStatus[3] = TRUE;
		}
	
	return CornerStatus;
	}