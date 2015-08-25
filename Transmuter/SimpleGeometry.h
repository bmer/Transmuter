//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

bool IsPointInRect(int px, int py, int rx, int ry, int w, int h);
bool IsPointInRect(int px, int py, TArray <int> RectDefinition);
bool IsRect1InRect2(int r1x, int r1y, int w1, int h1, int r2x, int r2y, int w2, int h2);
bool IsRect1InRect2(TArray<int> Rect1Definition, TArray <int> Rect2Definition);
TArray <bool> DoesRect1ClashRect2(int r1x, int r1y, int w1, int h1, int r2x, int r2y, int w2, int h2);
