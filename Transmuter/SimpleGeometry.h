//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

bool IsPointInRect(int px, int py, RECT rc);
bool IsPointInRect (int px, int py, int iLeft, int iTop, int iWidth, int iHeight);
bool IsRect1InRect2(RECT rc1, RECT rc2);
TArray <bool> DoesRect1ClashRect2(RECT rc1, RECT rc2);
int GetSharedTopBottomEdgeLength(RECT *rcTop, RECT *rcBot);
int GetSharedLeftRightEdgeLength(RECT *rcLeft, RECT *rcRight);
RECT ScaleRect(double dScale, RECT rc);
RECT ScaleRect(double dScale, int iLeft, int iTop, int iWidth, int iHeight);
RECT MakeRect (int iLeft, int iTop, int iWidth, int iHeight);
RECT MakeRect (RECT rcReference, int iRelativeLeft, int iRelativeTop, int iWidth, int iHeight);
