//	SimpleGeometry.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

bool IsPointInRect(int px, int py, RECT rc)
//  IsPointInRect
// 
//  Checks to see if given point is inside given rectangle. 
//  rx and ry are the coordinates of the TOP LEFT HAND CORNER of the rectangle.
	{
	int rx0 = rc.left;
	int ry0 = rc.top;
	int rx1 = rc.right;
	int ry1 = rc.bottom;

	return ((rx0 <= px) && (px <= rx1) && (ry0 <= py) && (py <= ry1));
	}

bool IsRect1InRect2(RECT rc1, RECT rc2)
//  IsRect1InRect2
//  
//  Returns TRUE if Rectangle 1 is entirely contained in Rectangle 2, FALSE otherwise.
//  rjx and rjy (where j in {1, 2}) are the coordinates of the TOP LEFT HAND CORNER of a rectangle.
	{
	int r1x0 = rc1.left;
	int r1y0 = rc1.top;
	int r1x1 = rc1.right;
	int r1y1 = rc1.bottom;

	return (IsPointInRect(r1x0, r1y0, rc2) &&
			IsPointInRect(r1x0, r1y1, rc2) &&
			IsPointInRect(r1x1, r1y1, rc2) &&
			IsPointInRect(r1x1, r1y0, rc2));
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
	int r1x0 = rc1.left;
	int r1y0 = rc1.top;
	int r1x1 = rc1.right;
	int r1y1 = rc1.bottom;

	int r2x0 = rc2.left;
	int r2y0 = rc2.top;
	int r2x1 = rc2.right;
	int r2y1 = rc2.bottom;

	TArray <bool> CornerStatus;

	for (int i = 0; i < 4; i++)
		{
		CornerStatus.Insert(false);
		}

	if (IsPointInRect(r1x0, r1y0, rc2))
		{
		CornerStatus[0] = true;
		}

	if (IsPointInRect(r1x1, r1y0, rc2))
		{
		CornerStatus[1] = true;
		}

	if (IsPointInRect(r1x1, r1y1, rc2))
		{
		CornerStatus[2] = true;
		}

	if (IsPointInRect(r1x0, r1y1, rc2))
		{
		CornerStatus[3] = true;
		}
	
	return CornerStatus;
	}

int GetSharedTopBottomEdgeLength(RECT *rc1, RECT *rc2)
//	GetSharedTopBottomEdgeLength
//
//	Check to see if two rectangles, one on top of the other,
//  share boundaries in such a way that the bottom edge of rcTop touches the top edge of rcBot.
//  If boundaries are shared, return the overlapping length, else return 0 (since 0 length is shared).
	{
	RECT *rcTop;
	RECT *rcBot;
	
	//  determine which rectangle is "on top" of the other
	if (rc1->bottom == rc2->top)
		{
		rcTop = rc1;
		rcBot = rc2;
		}
	else if (rc2->bottom == rc1->top)
		{
		rcTop = rc2;
		rcBot = rc1;
		}
	else // the two rectangles don't have have y-coordinates that allow for a shared edge
		{
		return 0;
		}

	// are the rectangles completely misaligned in the x-direction?
	if ((rcTop->right <= rcBot->left) || (rcTop->left >= rcBot->right))
		{
		return 0;
		}
	else // there is a shared edge, and we now have to determine its length
		{
		if (rcTop->left < rcBot->left)
			{
			if (rcTop->right < rcBot->right)
				{
				return (rcTop->right - rcBot->left);
				}
			else
				{
				return (rcBot->right - rcBot->left);
				}
			}
		else
			{
			if (rcTop->right < rcBot->right)
				{
				return (rcTop->right - rcTop->left);
				}
			else
				{
				return (rcBot->right - rcTop->left);
				}
			}
		}
	}

int GetSharedLeftRightEdgeLength(RECT *rc1, RECT *rc2)
//	GetSharedRightLeftLength
//
//	Check to see if two rectangles, one (rcLeft) supposed to be to the left of another (rcRight),
//  share boundaries in such a way that the right edge of rcLeft touches the left edge of rcLeft.
//  If boundaries are shared, return the overlapping length, else return 0 (since 0 length is shared).
	{
	// do the two rectangles have x-coordinates that allow for a shared edge?
	RECT *rcLeft;
	RECT *rcRight;
	
	//  determine which rectangle is "to the left" of the other
	if (rc1->right == rc2->left)
		{
		rcLeft = rc1;
		rcRight = rc2;
		}
	else if (rc2->right == rc1->left)
		{
		rcLeft = rc2;
		rcRight = rc1;
		}
	else // the two rectangles don't have have x-coordinates that allow for a shared edge
		{
		return 0;
		}

	// are the rectangles completely misaligned in the y-direction?
	if ((rcLeft->bottom <= rcRight->top) || (rcLeft->top >= rcRight->bottom))
		{
		return 0;
		}
	else // there is a shared edge, and we now have to determine its length
		{
		if (rcLeft->top < rcLeft->top)
			{
			if (rcLeft->bottom < rcLeft->bottom)
				{
				return (rcLeft->bottom - rcLeft->top);
				}
			else
				{
				return (rcLeft->bottom - rcLeft->top);
				}
			}
		else
			{
			if (rcLeft->bottom < rcLeft->bottom)
				{
				return (rcLeft->bottom - rcLeft->top);
				}
			else
				{
				return (rcLeft->bottom - rcLeft->top);
				}
			}
		}
	}