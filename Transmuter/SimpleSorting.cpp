//	SimpleGeometry.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

int PickPivot(int FirstValue, int MiddleValue, int LastValue)
	{
	if (FirstValue < MiddleValue)
		{
		if (MiddleValue < LastValue)
			{
			return 1;
			}
		else
			{
			return 2;
			}
		}
	else
		{
		if (FirstValue < LastValue)
			{
			return 0;
			}
		else
			{
			return 2;
			}
		}
	}
TArray <int> QuickSortIntegerArray(TArray <int> IntegerArray)
	{
	int ArrayLength = IntegerArray.GetCount();

	if (!(ArrayLength > 0))
		{
		return IntegerArray;
		}

	int FirstValue = IntegerArray[0];
	int MiddleValue = IntegerArray[ArrayLength/2];
	int LastValue = IntegerArray[ArrayLength - 1];

	int PivotNumber = PickPivot(FirstValue, MiddleValue, LastValue);

	int PivotIndex;
	int PivotValue;

	if (PivotNumber = 0)
		{
		PivotIndex = 0;
		PivotValue = FirstValue;
		}
	else if (PivotNumber = 2)
		{
		PivotIndex = (ArrayLength/2);
		PivotValue = MiddleValue;
		}
	else
		{
		PivotIndex = ArrayLength - 1;
		PivotValue = LastValue;
		}

	TArray <int> LesserThanPivotValue;
	TArray <int> GreaterThanPivotValue;

	int CurrentIndexValue;

	for (int i = 0; i < ArrayLength; i++)
		{
		if (i != PivotIndex)
			{
			CurrentIndexValue = IntegerArray[i];
			if (CurrentIndexValue < PivotValue)
				{
				LesserThanPivotValue.Insert(CurrentIndexValue);
				}
			else
				{
				GreaterThanPivotValue.Insert(CurrentIndexValue);
				}
			}
		}

	LesserThanPivotValue = QuickSortIntegerArray(LesserThanPivotValue);
	GreaterThanPivotValue = QuickSortIntegerArray(GreaterThanPivotValue);

	TArray <int> SortedIntegerArray;
	SortedIntegerArray.Insert(LesserThanPivotValue);
	SortedIntegerArray.Insert(PivotValue);
	SortedIntegerArray.Insert(GreaterThanPivotValue);

	return SortedIntegerArray;
	}