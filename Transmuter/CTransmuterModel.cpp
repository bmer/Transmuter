//	CTransmuterModel.cpp
//
//	CTransmuterModel class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

const DWORD UNID_DEFAULT_CAMPAIGN =	DEFAULT_ADVENTURE_EXTENSION_UNID;

CTransmuterModel::CTransmuterModel (CHumanInterface &HI, CTransmuterController *pController) :
		m_HI(HI),
		m_pController(pController)

//	CTransmuterModel constructor

	{
	}

bool CTransmuterModel::Init (const SInitDesc &Ctx, CString *retsError)

//	Init
//
//	Initialize

	{
	//	Save file path

	//m_SaveFileFolders = Ctx.SaveFileFolders;

	////	Make sure the first path (default) exists so we can save files there.

	//if (m_SaveFileFolders.GetCount() == 0)
	//	{
	//	if (retsError) *retsError = CONSTLIT("No save file path defined.");
	//	return false;
	//	}

	//if (!pathExists(m_SaveFileFolders[0]))
	//	{
	//	if (!pathCreate(m_SaveFileFolders[0]))
	//		{
	//		if (retsError) *retsError = strPatternSubst(CONSTLIT("Cannot create default save file path: %s."), m_SaveFileFolders[0]);
	//		return false;
	//		}
	//	}

	//	Other folders

	m_sCollectionFolder = Ctx.sCollectionFolder;
	m_ExtensionFolders = Ctx.ExtensionFolders;

	//	Options

	m_bDebugMode = Ctx.bDebugMode;
	m_bForceTDB = Ctx.bForceTDB;

	return true;
	}

bool CTransmuterModel::InitBackground (CString *retsError)

//	InitBackground
//
//	Initialize the universe in the background.

	{
	//	Load the universe

	if (!LoadUniverse(retsError))
		return false;

	return true;
	}

void CTransmuterModel::InvokeCode (ICCItem *pCode)

//	InvokeCode
//
//	Invoke code from a command

	{
	CCodeChainCtx Ctx;

	//	Run code

	ICCItem *pResult = Ctx.Run(pCode);
	if (pResult->IsError())
		kernelDebugLogMessage(pResult->GetStringValue());

	Ctx.Discard(pResult);
	}

bool CTransmuterModel::LoadUniverse (CString *retsError)

//	LoadUniverse
//
//	Load the universe

	{
	try
		{
		//	Make sure the universe know about our various managers

		m_Universe.SetDebugMode(m_bDebugMode);
		m_Universe.SetSoundMgr(&m_HI.GetSoundMgr());

		//	Load the Transcendence Data Definition file that describes the universe.

		CUniverse::SInitDesc Ctx;
		// Ctx.sFilespec = CONSTLIT("");
		Ctx.sFilespec = CONSTLIT("A:\\transdev\\Transcendence\\TransCore\\Transcendence.xml");
		Ctx.sCollectionFolder = m_sCollectionFolder;
		Ctx.ExtensionFolders = m_ExtensionFolders;
		Ctx.pHost = m_pController;
		Ctx.bDebugMode = m_bDebugMode;
		Ctx.dwAdventure = UNID_DEFAULT_CAMPAIGN;
		Ctx.bDefaultExtensions = true;
		Ctx.bForceTDB = m_bForceTDB;

		if (m_Universe.Init(Ctx, retsError) != NOERROR)
			return false;

		return true;
		}
	catch (...)
		{
		if (retsError) *retsError = CONSTLIT("Crash loading universe.");
		return false;
		}
	}

TArray<CExtension *> CTransmuterModel::GetExtensionsArray()
//	GetExtensionsArray
//
//	Gets all extensions in universe's extension collection, in array format.
	{
	CExtensionCollection &ExtensionCollection = m_Universe.GetExtensionCollection();

	TArray<CExtension *> ExtensionsArray = ExtensionCollection.GetAllExtensions();

	return ExtensionsArray;
	}



TArray<CDesignTable> CTransmuterModel::GetExtensionDesignTables()
//	GetExtensionNames
//
//	Loops over CExtension objects in array and gets associated name strings.
	{
	TArray<CExtension *> ExtensionsArray = GetExtensionsArray();

	int x = ExtensionsArray.GetCount();

	TArray<CDesignTable> DesignTables;

	for (int i = 0; i < ExtensionsArray.GetCount(); i++)
		{
		CDesignTable ExtensionDesignTable = ExtensionsArray[i]->GetDesignTypes();
		DesignTables.Insert(ExtensionDesignTable);
		};

	return DesignTables;
	}