//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

bool IsPointInRect(int px, int py, RECT rc);
bool IsRect1InRect2(RECT rc1, RECT rc2);
TArray <bool> DoesRect1ClashRect2(RECT rc1, RECT rc2);
int GetSharedTopBottomEdgeLength(RECT *rcTop, RECT *rcBot);
int GetSharedLeftRightEdgeLength(RECT *rcLeft, RECT *rcRight);
RECT ScaleRect(double dScale, RECT rc);