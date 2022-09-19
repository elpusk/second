#pragma once

#include <Windows.h>
#include <list>
#include "ns_tchar.h"

class Cns_Etc
{
public:
	enum OsVersion{
		OSVersionInvalied = -1,
		OSVersionWin95,
		OSVersionWin98,
		OSVersionWinNT,
		OSVersionWinMe,
		OSVersionWin2000,
		OSVersionWinXp,
		OSVersionWin2003,
		OSVersionWinVista,
		OSVersionWin2008,
		OSVersionWin7,
		OSVersionWin2008R2
	};

public:
	static VOID CreateRandomName( LPTSTR psName,INT nSeed0,INT nSeed1 );

	static HANDLE CreateUniqueEvent( LPTSTR psOutEventName,INT nSeed );

	static HANDLE CreateUniqueMutex( LPTSTR psOutMutexName,INT nSeed );

	static HANDLE CreateMutex(	BOOL bInheritHandle, BOOL bInitialOwner, LPCTSTR lpName = NULL );

	static HANDLE CreateOneAppMutex(LPCTSTR lpName,BOOL bOwnerShip);
	static BOOL AvailableEvent( LPCTSTR sEventName );
	static BOOL AvailableMutex( LPCTSTR sMutexName );
	static INT ConvertToDigit( LPCTSTR szNumber, INT nType);
	static VOID GetAbsPathFromRelPath( LPTSTR psOutAbs,INT nOutAbs, LPCTSTR psInBase, LPCTSTR psInRel);
	static VOID GetAbsPathFromRelPathInModule( LPTSTR psOutAbs,INT nOutAbs, HMODULE hModule, LPCTSTR psInRel);
	static BOOL IsExistFile( LPCTSTR psFilePath );
	static INT GetPosZeroIntegerFromInfKey( LPCTSTR psSection,LPCTSTR psKey,LPCTSTR psInfFile);
	static BOOL GetStringFromInfKey( LPTSTR psVal,LPCTSTR psSection,LPCTSTR psKey,LPCTSTR psInfFile);
	static INT GetCuttedDigit( INT nSrc,INT nWidth );

	static BOOL CreatePath( LPCTSTR sCreatePath);

	static BOOL IsExistRecursive( LPCTSTR sFindFileName, LPCTSTR sSearchPath,BOOL bRecursive,BOOL bIsFolder );

	static VOID GetFoundList( std::list<_tstring> &listFoundFile,LPCTSTR sSearchFileName,LPCTSTR sSearchPath,BOOL bRecursive,BOOL bIsFolder );
	
	static FARPROC GetProcAddress( HMODULE hModule, LPCTSTR lpctProcName );
	
	static OsVersion GetCurOSVersion();
	static INT GetOSDefaultLanguage();
	static VOID Delay( LONGLONG llTime );
	static INT MakeMultiString( INT nCnt,LPTSTR szMultiStr, ... );
	static INT GetStringFromMultiString( std::list<_tstring> &listStr,LPCTSTR szMultiStr );
	static void GetLastErrorString( _tstring & sError );

public:
	Cns_Etc(void);
	virtual ~Cns_Etc(void);
};

