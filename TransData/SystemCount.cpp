//	SystemCount.cpp
//
//	Counts system in a game

#include <stdio.h>

#include <windows.h>
#include <ddraw.h>
#include "Alchemy.h"
#include "XMLUtil.h"
#include "TransData.h"

typedef TSortMap<DWORD, int> CSystemTypeTable;

struct SSysNodeDesc
	{
	int iLevel;
	CString sNodeID;
	CSystemTypeTable Table;
	};

void GenerateSystemCount (CUniverse &Universe, CXMLElement *pCmdLine)
	{
	int i, j, k;

	//	Options

	int iSystemSample = pCmdLine->GetAttributeIntegerBounded(CONSTLIT("count"), 1, -1, 1);
	bool bLogo = !pCmdLine->GetAttributeBool(CONSTLIT("noLogo"));
	bool bAll = pCmdLine->GetAttributeBool(CONSTLIT("all"));

	//	Additional columns

	TArray<CString> Cols;
	for (i = 0; i < pCmdLine->GetAttributeCount(); i++)
		{
		CString sAttrib = pCmdLine->GetAttributeName(i);

		if (!IsMainCommandParam(sAttrib)
				&& !strEquals(sAttrib, CONSTLIT("count"))
				&& !strEquals(sAttrib, CONSTLIT("systemcount")))
			{
			CString sValue = pCmdLine->GetAttribute(i);
			
			if (!strEquals(sValue, CONSTLIT("true")))
				Cols.Insert(strPatternSubst(CONSTLIT("%s:%s"), sAttrib, sValue));
			else
				Cols.Insert(sAttrib);
			}
		}

	//	Generate systems for multiple games

	TSortMap<CString, SSysNodeDesc> NodeTable;

	for (i = 0; i < iSystemSample; i++)
		{
		if (bLogo)
			printf("pass %d...\n", i+1);

		//	Initialize the game

		CString sError;
		if (Universe.InitGame(0, &sError) != NOERROR)
			{
			printf("%s\n", sError.GetASCIIZPointer());
			return;
			}

		for (j = 0; j < Universe.GetTopologyNodeCount(); j++)
			{
			CTopologyNode *pNode = Universe.GetTopologyNode(j);
			if (pNode->IsEndGame())
				continue;

			//	Create a sort string for this system

			CString sSort = strPatternSubst(CONSTLIT("%02d-%s"), pNode->GetLevel(), pNode->GetID());

			//	Get the table

			bool bNew;
			SSysNodeDesc *pResult = NodeTable.SetAt(sSort, &bNew);
			if (bNew)
				{
				pResult->iLevel = pNode->GetLevel();
				pResult->sNodeID = pNode->GetID();
				}

			//	Add the entry

			int *pCount = pResult->Table.SetAt(pNode->GetSystemTypeUNID(), &bNew);
			if (bNew)
				*pCount = 1;
			else
				*pCount += 1;
			}

		Universe.Reinit();
		}

	//	Header

	printf("Node\tLevel\tSystemType\tCount");
	for (i = 0; i < Cols.GetCount(); i++)
		printf("\t%s", Cols[i].GetASCIIZPointer());
	printf("\n");

	//	Output all rows

	for (i = 0; i < NodeTable.GetCount(); i++)
		{
		for (j = 0; j < NodeTable[i].Table.GetCount(); j++)
			{
			int iCount = NodeTable[i].Table[j];
			int iCountWhole = (iCount / iSystemSample);
			int iCountFrac = 1000 * (iCount % iSystemSample) / iSystemSample;

			printf("%s\t%d\t0x%08x\t%d.%03d",
					NodeTable[i].sNodeID.GetASCIIZPointer(),
					NodeTable[i].iLevel,
					NodeTable[i].Table.GetKey(j),
					iCountWhole,
					iCountFrac);

			CSystemType *pSystemType = Universe.FindSystemType(NodeTable[i].Table.GetKey(j));
			if (pSystemType)
				{
				for (k = 0; k < Cols.GetCount(); k++)
					{
					CString sValue = pSystemType->GetDataField(Cols[k]);
					printf("\t%s", sValue.GetASCIIZPointer());
					}
				}

			printf("\n");
			}
		}
	}

