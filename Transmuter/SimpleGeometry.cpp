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
	return ((rx <= px) && (px <= rx + w) && (ry <= py) && (py <= ry + h));
	}

bool IsPointInRect(int px, int py, RECT rc)
//  IsPointInRect
// 
//  Checks to see if given point is inside given rectangle. 
//  rx and ry are the coordinates of the TOP LEFT HAND CORNER of the rectangle.
	{
	int rx = rc.left;
	int ry = rc.top;
	int w = rc.right;
	int h = rc.bottom;

	return IsPointInRect(px, py, rx, ry, w, h);
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

bool IsRect1InRect2(RECT rc1, RECT rc2)
//  IsRect1InRect2
//  
//  Returns TRUE if Rectangle 1 is entirely contained in Rectangle 2, FALSE otherwise.
//  rjx and rjy (where j in {1, 2}) are the coordinates of the TOP LEFT HAND CORNER of a rectangle.
	{
	int r1x = rc1.left;
	int r1y = rc1.top;
	int w1 = rc1.right;
	int h1 = rc1.bottom;

	int r2x = rc2.left;
	int r2y = rc2.top;
	int w2 = rc2.right;
	int h2 = rc2.bottom;

	return IsRect1InRect2(r1x, r1y, w1, h1, r2x, r2y, w2, h2);
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

	for (int i = 0; i < 4; i++)
		{
		CornerStatus.Insert(false);
		}

	if (IsPointInRect(r1x, r1y, r2x, r2y, w2, h2))
		{
		CornerStatus[0] = true;
		}

	if (IsPointInRect(r1x + w1, r1y, r2x, r2y, w2, h2))
		{
		CornerStatus[1] = true;
		}


	if (IsPointInRect(r1x + w1, r1y + h1, r2x, r2y, w2, h2))
		{
		CornerStatus[2] = true;
		}

	if (IsPointInRect(r1x, r1y + h1, r2x, r2y, w2, h2))
		{
		CornerStatus[3] = true;
		}
	
	return CornerStatus;
	}

TArray <bool> DoesRect1ClashRect2(RECT rc1, RECT rc2)
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
	int r1x = rc1.left;
	int r1y = rc1.top;
	int w1 = rc1.right;
	int h1 = rc1.bottom;

	int r2x = rc2.left;
	int r2y = rc2.top;
	int w2 = rc2.right;
	int h2 = rc2.bottom;

	return DoesRect1ClashRect2(r1x, r1y, w1, h1, r2x, r2y, w2, h2);
	}