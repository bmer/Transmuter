//	SimpleGeometry.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

int PickPivot (int iFirstValue, int iMiddleValue, int iLastValue)
	{
	if (iFirstValue < iMiddleValue)
		{
		if (iMiddleValue < iLastValue)
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
		if (iFirstValue < iLastValue)
			{
			return 0;
			}
		else
			{
			return 2;
			}
		}
	}


TArray <int> QuickSortIntegerArray (TArray <int> aIntegers)
	{
	int iArrayLength = aIntegers.GetCount();

	if (!(iArrayLength > 0))
		{
		return aIntegers;
		}

	int iFirstValue = aIntegers[0];
	int iMiddleValue = aIntegers[iArrayLength/2];
	int iLastValue = aIntegers[iArrayLength - 1];

	int iPivotSelection = PickPivot(iFirstValue, iMiddleValue, iLastValue);

	int iPivotIndex;
	int iPivotValue;

	if (iPivotSelection = 0)
		{
		iPivotIndex = 0;
		iPivotValue = iFirstValue;
		}
	else if (iPivotSelection = 2)
		{
		iPivotIndex = (iArrayLength/2);
		iPivotValue = iMiddleValue;
		}
	else
		{
		iPivotIndex = iArrayLength - 1;
		iPivotValue = iLastValue;
		}

	TArray <int> aLesserThanPivotValue;
	TArray <int> aGreaterThanPivotValue;

	int iCurrentValue;

	for (int i = 0; i < iArrayLength; i++)
		{
		if (i != iPivotIndex)
			{
			iCurrentValue = aIntegers[i];
			if (iCurrentValue < iPivotValue)
				{
				aLesserThanPivotValue.Insert(iCurrentValue);
				}
			else
				{
				aGreaterThanPivotValue.Insert(iCurrentValue);
				}
			}
		}

	aLesserThanPivotValue = QuickSortIntegerArray(aLesserThanPivotValue);
	aGreaterThanPivotValue = QuickSortIntegerArray(aGreaterThanPivotValue);

	TArray <int> aSortedIntegers;
	aSortedIntegers.Insert(aLesserThanPivotValue);
	aSortedIntegers.Insert(iPivotValue);
	aSortedIntegers.Insert(aGreaterThanPivotValue);

	return aSortedIntegers;
	}

TArray <int> QuickSortIntegerArray (TArray <int> aIntegers, TArray <int> aAuxiliaries)
	{
	int iArrayLength = aIntegers.GetCount();

	if (!(iArrayLength > 0))
		{
		return aIntegers;
		}

	int iFirstValue = aIntegers[0];
	int iMiddleValue = aIntegers[iArrayLength/2];
	int iLastValue = aIntegers[iArrayLength - 1];

	int iPivotSelection = PickPivot(iFirstValue, iMiddleValue, iLastValue);

	int iPivotIndex;
	int iPivotValue;
	int PivotValueAuxiliary;

	if (iPivotSelection = 0)
		{
		iPivotIndex = 0;
		iPivotValue = iFirstValue;
		}
	else if (iPivotSelection = 2)
		{
		iPivotIndex = (iArrayLength/2);
		iPivotValue = iMiddleValue;
		}
	else
		{
		iPivotIndex = iArrayLength - 1;
		iPivotValue = iLastValue;
		}

	PivotValueAuxiliary = aAuxiliaries[iPivotIndex];

	TArray <int> aLesserThanPivotValue;
	TArray <int> aLesserThanPivotValueAuxliary;
	TArray <int> aGreaterThanPivotValue;
	TArray <int> aGreaterThanPivotValueAuxiliary;

	int iCurrentValue;

	for (int i = 0; i < iArrayLength; i++)
		{
		if (i != iPivotIndex)
			{
			iCurrentValue = aIntegers[i];
			if (iCurrentValue < iPivotValue)
				{
				aLesserThanPivotValue.Insert(iCurrentValue);
				aLesserThanPivotValueAuxliary.Insert(aAuxiliaries[i]);
				}
			else
				{
				aGreaterThanPivotValue.Insert(iCurrentValue);
				aGreaterThanPivotValueAuxiliary.Insert(aAuxiliaries[i]);
				}
			}
		}

	aLesserThanPivotValueAuxliary = QuickSortIntegerArray(aLesserThanPivotValue, aLesserThanPivotValueAuxliary);
	aGreaterThanPivotValueAuxiliary = QuickSortIntegerArray(aGreaterThanPivotValue, aGreaterThanPivotValueAuxiliary);

	TArray <int> aSortedAuxliaries;
	aSortedAuxliaries.Insert(aLesserThanPivotValueAuxliary);
	aSortedAuxliaries.Insert(PivotValueAuxiliary);
	aSortedAuxliaries.Insert(aGreaterThanPivotValueAuxiliary);

	return aSortedAuxliaries;
	}