#include "StdAfx.h"
#include "ns_Etc.h"
#include "ns_tchar.h"
#include <stdio.h>
#include <vector>

#if !defined(SM_SERVERR2)
#define SM_SERVERR2						89
#endif

////dynamic exported function prototype.
//GetNativeSystemInfo(WinXP or vista)
typedef VOID (WINAPI *PGNSI)(LPSYSTEM_INFO);


Cns_Etc::Cns_Etc(void)
{
}


Cns_Etc::~Cns_Etc(void)
{
}


VOID Cns_Etc::CreateRandomName( LPTSTR psName,INT nSeed0,INT nSeed1 )
{
	if( psName==NULL )
		return;
	//
	_stprintf( psName,_T("TTR_UNIQUE_NAME_IS_%X_%X_"),nSeed0,nSeed1 );
}

HANDLE Cns_Etc::CreateUniqueEvent( LPTSTR psOutEventName,INT nSeed )
{
	HANDLE hEvent;
	DWORD dwResult;

	INT i=0;

	if( psOutEventName==NULL )
		return NULL;
	//
	CreateRandomName( psOutEventName,nSeed,GetTickCount() );

	do{
		hEvent=::CreateEvent(NULL,TRUE,FALSE,(LPCTSTR)psOutEventName );
		if( hEvent==NULL ){//fail
			//already exist another type named-object.
		}
		else{//success
			dwResult=GetLastError();
			if( dwResult!=ERROR_ALREADY_EXISTS ){
				return hEvent;		
			}
			//here! already exist same type named-object.
			CloseHandle( hEvent );
		}
		
		i++;
		CreateRandomName( psOutEventName,nSeed+i*379,GetTickCount() );
		
	}while( i<50 );//end do-while

	return NULL;
}

HANDLE Cns_Etc::CreateUniqueMutex( LPTSTR psOutMutexName,INT nSeed )
{
	HANDLE hMutex;
	DWORD dwResult;

	INT i=0;

	if( psOutMutexName==NULL )
		return NULL;
	//
	CreateRandomName( psOutMutexName,nSeed,GetTickCount() );

	do{
		hMutex=::CreateMutex(NULL,FALSE,(LPCTSTR)psOutMutexName );
		if( hMutex==NULL ){//fail
			//already exist another type named-object.
		}
		else{//success
			dwResult=GetLastError();
			if( dwResult!=ERROR_ALREADY_EXISTS ){
				return hMutex;		
			}
			//here! already exist same type named-object.
			CloseHandle( hMutex );
		}
		
		i++;
		CreateRandomName( psOutMutexName,nSeed+i*438,GetTickCount() );
		
	}while( i<50 );//end do-while

	return NULL;
}

HANDLE Cns_Etc::CreateMutex(	BOOL bInheritHandle, BOOL bInitialOwner, LPCTSTR lpName /*=NULL*/ )
{
	HANDLE hNewMutex=NULL;
	
	if( lpName ){
		//the first open-try
		hNewMutex=::OpenMutex( MUTEX_ALL_ACCESS,bInheritHandle,lpName );
		if( hNewMutex==NULL ){
			if( GetLastError()==ERROR_FILE_NOT_FOUND ){
				
				//the second create-try
				hNewMutex=::CreateMutex( NULL,bInitialOwner,lpName );
				if( hNewMutex==NULL )
					return hNewMutex;//CreateMutex-error
			}
			else
				return hNewMutex;//OpenMutex-error
		}
		else{
			/*
			::DuplicateHandle(
				GetCurrentProcess(), 
				hNewMutex, 
				GetCurrentProcess(),
				&hMutexDup, 
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS);
			*/
		}
	}
	else{
		hNewMutex=::CreateMutex( NULL,bInitialOwner,lpName );
	}
	
	return hNewMutex;
}

//////////////////////////////////


HANDLE Cns_Etc::CreateOneAppMutex(LPCTSTR lpName,BOOL bOwnerShip)
{
    HANDLE hMutex = NULL;

	if( lpName )
    	hMutex = ::CreateMutex(NULL, bOwnerShip, lpName);   // Create mutex with given name.......
    else{
		return hMutex;//return mutex handle
     }

	switch(GetLastError()){
		case ERROR_SUCCESS:
			// Mutex created successfully. There is no instances running
        	break;
		case ERROR_ALREADY_EXISTS:
			// Mutex already exists so there is a running instance of our app.
			hMutex = NULL;
			break;
		default:
			// Failed to create mutex by unkniown reason
			break;
	}//end switch

    return hMutex;//return mutex handle
}

//check given event is existing or not.......
BOOL Cns_Etc::AvailableEvent( LPCTSTR sEventName )
{
	HANDLE hEvent;
	DWORD dwResult;
	BOOL bResult;
	
	hEvent=::OpenEvent( SYNCHRONIZE,FALSE,sEventName );

	if( hEvent ){
		CloseHandle( hEvent );//decrease reference counter	
		bResult=TRUE;
	}
	else{
		dwResult=GetLastError();
		if( dwResult==ERROR_FILE_NOT_FOUND ){
			//Not found event
			bResult=FALSE;
			
		}
		else{//opening event error
			bResult=FALSE;
		}
	}
	return bResult;
}

//check given mutex is existing or not.......
BOOL Cns_Etc::AvailableMutex( LPCTSTR sMutexName )
{
	HANDLE hMutex;
	DWORD dwResult;
	BOOL bResult;
	
	hMutex=::OpenMutex( SYNCHRONIZE,FALSE,sMutexName );

	if( hMutex ){
		CloseHandle( hMutex );//decrease reference counter	
		bResult=TRUE;
	}
	else{
		dwResult=GetLastError();
		if( dwResult==ERROR_FILE_NOT_FOUND ){
			//Not found event
			bResult=FALSE;
			
		}
		else{//opening event error
			bResult=FALSE;
		}
	}
	return bResult;
}

INT Cns_Etc::ConvertToDigit( LPCTSTR szNumber, INT nType)
{
	LPCTSTR szTemp = szNumber;
	INT nData = 0;
	INT nPow = 1;
	INT ii, nLen;

	nLen = _tcslen(szNumber);

	for ( ii = (nLen-1); ii >= 0; ii-- ){
		switch ( nType ){
			case 2:
				if ( szTemp[ii] < _T('0') || szTemp[ii] > _T('1') )
					return 0; // 잘못된 2진수
				nData += (szTemp[ii] - _T('0')) * nPow;
				nPow *= 2;
				break;
			case 8:
				if ( szTemp[ii] < _T('0') || szTemp[ii] > _T('8') )
					return 0; // 잘못된 8진수
				nData += (szTemp[ii] - _T('0')) * nPow;
				nPow *= 8;
				break;
			case 16:
				if ( (szTemp[ii]  < _T('0') && szTemp[ii] > _T('9')) &&
					 ((szTemp[ii] < _T('A') && szTemp[ii] > _T('F')) ||
					  (szTemp[ii] < _T('a') && szTemp[ii] > _T('f')))){
					return 0; // 잘못된 16진수
				}
				if ( szTemp[ii] >= _T('0') && szTemp[ii] <= _T('9') ){
					nData += (szTemp[ii] - _T('0')) * nPow;
				}
				if ( szTemp[ii] >= _T('A') && szTemp[ii] <= _T('F') ){
					nData += (szTemp[ii] - _T('A') + 10) * nPow;
				}
				if ( szTemp[ii] >= _T('a') && szTemp[ii] <= _T('f') ){
					nData += (szTemp[ii] - _T('a') + 10) * nPow;
				}

				nPow *= 16;
				break;
		}
	}
	return nData;
}


//Get abs-path from base path and relative path
VOID Cns_Etc::GetAbsPathFromRelPath( LPTSTR psOutAbs,INT nOutAbs, LPCTSTR psInBase, LPCTSTR psInRel)
{
	LPTSTR pCur;

	if( psOutAbs==NULL )
		return;

	//
	pCur = _tgetcwd(NULL, 0);	//save current dirct

	_tchdir( psInBase );	//change current directory
	_tfullpath( psOutAbs, psInRel, nOutAbs );

	_tchdir( pCur );			//recover current directory
	free( pCur );

}

//Get abs-path from base path and relative path in given Module as base
VOID Cns_Etc::GetAbsPathFromRelPathInModule( LPTSTR psOutAbs,INT nOutAbs, HMODULE hModule, LPCTSTR psInRel)
{
	TCHAR sModuleFN[_MAX_PATH];
	TCHAR Drive[_MAX_DRIVE];
	TCHAR Dir[_MAX_DIR];
   	TCHAR Fname[_MAX_FNAME];
	TCHAR Ext[_MAX_EXT];
	TCHAR sBaseFolder[_MAX_PATH];

	if( psOutAbs==NULL )
		return;
	if( hModule == NULL )
		return;

	//
	if( GetModuleFileName( hModule,sModuleFN,_MAX_PATH )>0 ){
		//generate realtive from folder to abs folder
		_tsplitpath( sModuleFN,Drive,Dir,Fname,Ext );
		_tmakepath( sBaseFolder,Drive,Dir,NULL,NULL );

		GetAbsPathFromRelPath( psOutAbs,nOutAbs, sBaseFolder, psInRel);
	}

}

//Exist or absent file ?
BOOL Cns_Etc::IsExistFile( LPCTSTR psFilePath )
{
	BOOL bResult = TRUE;
	HANDLE hfile;

	if( psFilePath ){

		hfile=CreateFile(psFilePath,GENERIC_READ,FILE_SHARE_READ, NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
		if( hfile==INVALID_HANDLE_VALUE ){

			bResult = FALSE;
		}
		else{
			CloseHandle(hfile);
		}

	}
	else
		bResult = FALSE;
	
	return bResult;
}


//get positive integer or zero from inf file's key
//if return negative, may be error
INT Cns_Etc::GetPosZeroIntegerFromInfKey( LPCTSTR psSection,LPCTSTR psKey,LPCTSTR psInfFile)
{
	INT nResult = -1;
	DWORD dwResult =0;
	TCHAR strBuf[_MAX_PATH];
#ifdef UNICODE 
	CHAR strTmp[2*(_MAX_PATH+1)];
#endif

	if( psSection!=NULL && psKey!=NULL && psInfFile!=NULL ){


		dwResult=GetPrivateProfileString( psSection,psKey,_T("-1"),strBuf,_MAX_PATH,psInfFile);
		if( dwResult>0 ){
		//
		#ifdef UNICODE
			wcstombs(strTmp, (const wchar_t *)strBuf, sizeof(strTmp)); 
			nResult=atoi( strTmp );
		#else
			nResult=atoi( strBuf );
		#endif
		}

	}

	return nResult;
}


//get string value from inf file's key
BOOL Cns_Etc::GetStringFromInfKey( LPTSTR psVal,LPCTSTR psSection,LPCTSTR psKey,LPCTSTR psInfFile)
{
	BOOL bResult = FALSE;
	DWORD dwResult =0;
	TCHAR strBuf[_MAX_PATH];

	if( psVal!=NULL && psSection!=NULL && psKey!=NULL && psInfFile!=NULL ){

		dwResult=GetPrivateProfileString( psSection,psKey,_T("-1"),strBuf,_MAX_PATH,psInfFile);
		if( dwResult>0 ){
			//
			_tcscpy( psVal,strBuf );
			bResult = TRUE;
		}
	}

	return bResult;
}


//return Cutted digit as nWith
//if  nSrc = 234 & nWidth = 2, return 34
INT Cns_Etc::GetCuttedDigit( INT nSrc,INT nWidth )
{
	INT nResult = 0;
	double dSrc;
	INT nTemp;
	double dDiv;
	INT nMul;
	INT i;
	double dRemover;
	BOOL bIsPositive;
	

	if( nSrc!=0 ){

		if( nWidth==0 ){
			nResult = nSrc;//no cut in width is zero case.......
		}
		else if( nWidth<17 && nWidth>0 ){
			
			dDiv = 1.0;
			nMul = 1;

			if( nSrc<0 ){
				bIsPositive = FALSE;

				nSrc = -nSrc;	//covert to positive
			}
			else
				bIsPositive = TRUE;

			//
			dRemover = dSrc = (double)nSrc;

			//generate cutter
			for( i=0; i<nWidth; i++ ){
				dDiv *= 10.0;
				nMul *= 10;
			}//end for
			 
			dRemover = dRemover / dDiv;
			nTemp = (INT)dRemover;

			nTemp *= nMul;
			dRemover = (double)nTemp;
				
			//cut hight digit
			dSrc = dSrc - dRemover;
			nResult = (INT)dSrc;

			if( !bIsPositive ){
				//recover sign
				nResult = -nResult;

			}

		}

	}

	return nResult;

}


BOOL Cns_Etc::CreatePath(LPCTSTR sCreatePath)
{
	DWORD attr;
	INT pos;
	BOOL bResult = TRUE;

	_tstring sPath = sCreatePath;

	// Check for trailing slash:
	pos = sPath.find_last_of(_T("\\"));
	if (sPath.length() == pos + 1){	// last character is "\"

		sPath.resize(pos);
	}

	// Look for existing object:
	attr = GetFileAttributes(sPath.c_str());
	if (0xFFFFFFFF == attr){	// doesn't exist yet - create it!

		pos = sPath.find_last_of(_T("\\"));
		if (0 < pos){
			// Create parent dirs:
			bResult = CreatePath(sPath.substr(0, pos).c_str());
		}
		// Create node:
		bResult = bResult && CreateDirectory(sPath.c_str(), NULL);
	}
	else if (FILE_ATTRIBUTE_DIRECTORY != attr){	// object already exists, but is not a dir
		SetLastError(ERROR_FILE_EXISTS);
		bResult = TRUE;
	}

	return bResult;
}


BOOL Cns_Etc::IsExistRecursive( LPCTSTR sFindFileName, LPCTSTR sSearchPath, BOOL bRecursive,BOOL bIsFolder )
{
	BOOL next;
	HANDLE FileSearching;
	WIN32_FIND_DATA FindFileData;
	_tstring stemp, stemp1;
	BOOL bFindOK = FALSE;
	_tstring sPathFind = sSearchPath;

	// Add slash to path if it absent
	if(sPathFind[sPathFind.length()]!= _T('\\')){
		sPathFind+=_T("\\");
	}

	// search for any file
	stemp=sPathFind + _T("*.*");

	//open handle
	FileSearching=FindFirstFile(
		stemp.c_str(),    // pointer to name of file to search for
		&FindFileData     // pointer to returned information
		);

	if(next=(FileSearching  != INVALID_HANDLE_VALUE)){
		do{

			if( bRecursive ){
				//call self  if find directory(FILE_ATTRIBUTE_DIRECTORY)
				if((FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes) &&
					_tcscmp(FindFileData.cFileName,_T(".")) &&
					_tcscmp(FindFileData.cFileName,_T(".."))){
					//
					stemp1 = sPathFind+FindFileData.cFileName;
					bFindOK = IsExistRecursive (sFindFileName, stemp1.c_str(),bRecursive,bIsFolder);
				}
			}

			/////////////////////////
			// to do.
			
			if( bIsFolder ){
				//search is folder
				if( FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes ){

					if( _tcscmp( sFindFileName,FindFileData.cFileName )==0 ){
						bFindOK = TRUE;
					}
				}
			}
			else{
				//file
				if( !(FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes) ){
					if( _tcscmp( sFindFileName,FindFileData.cFileName )==0 ){
						bFindOK = TRUE;
					}
				}
			}
			
			if( bFindOK ){
				FindClose(FileSearching);
				return bFindOK;
			}


			next=FindNextFile( FileSearching,&FindFileData );	// do search

			//do not freeze form till search continuing
			//Application->ProcessMessages();
		}while(next);

	}
	
	FindClose(FileSearching);
	//Search in the subdir - end
	return bFindOK;
}

VOID Cns_Etc::GetFoundList( std::list<_tstring> &listFoundFile,LPCTSTR sSearchFileName,LPCTSTR sSearchPath,BOOL bRecursive,BOOL bIsFolder )
{
	BOOL next;
	HANDLE FileSearching;
	WIN32_FIND_DATA FindFileData;
	_tstring stemp,stemp1;
	_tstring sFindFileName, sPathFind;
	//std::list<_tstring>  *listFoundFile = (std::list<_tstring> *)plistFoundFile;

	sFindFileName = sSearchFileName;
	sPathFind = sSearchPath;

	// Add slash to path if it absent
	if(sPathFind[sPathFind.length()-1]!= _T('\\')){
		sPathFind+=_T("\\");
	}

	// search for any file
	stemp=sPathFind + sFindFileName;

	//open handle
	FileSearching=FindFirstFile(
		stemp.c_str(),    // pointer to name of file to search for
		&FindFileData     // pointer to returned information
		);

	if(next=(FileSearching  != INVALID_HANDLE_VALUE)){
		do{

			if( bRecursive ){
				//call self  if find directory(FILE_ATTRIBUTE_DIRECTORY)
				if((FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes) &&
					_tcscmp(FindFileData.cFileName,_T(".")) &&
					_tcscmp(FindFileData.cFileName,_T(".."))){
					//
					stemp1 = sPathFind+FindFileData.cFileName;
					GetFoundList( listFoundFile, sFindFileName.c_str() ,stemp1.c_str(),bRecursive,bIsFolder);
				}
			}

			/////////////////////////
			// to do.
			
			if( bIsFolder ){
				//search is folder
				if( FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes ){

					listFoundFile.push_back( FindFileData.cFileName );
				}
			}
			else{
				//file
				if( !(FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes) ){

					listFoundFile.push_back( FindFileData.cFileName );
				}
			}
			

			next=FindNextFile( FileSearching,&FindFileData );	// do search

			//do not freeze form till search continuing
			//Application->ProcessMessages();
		}while(next);

	}
	
	FindClose(FileSearching);
	//Search in the subdir - end
}

FARPROC Cns_Etc::GetProcAddress( HMODULE hModule, LPCTSTR lpctProcName )
{
	INT nNeeded;
	FARPROC pFun = NULL;
	CHAR *lpszAnsiFn;

#if defined(_UNICODE) || defined(UNICODE)
	nNeeded = WideCharToMultiByte( CP_ACP,0,lpctProcName,-1,NULL,0,NULL,NULL );
	lpszAnsiFn = new CHAR[nNeeded];

	if( lpszAnsiFn ){
		WideCharToMultiByte( CP_ACP,0,lpctProcName,-1,lpszAnsiFn,nNeeded,NULL,NULL );
		pFun = ::GetProcAddress( hModule,lpszAnsiFn );
	}

	delete( lpszAnsiFn );
#else
	pFun = ::GetProcAddress( hModule,lpctProcName);
#endif

	return pFun;
}


Cns_Etc::OsVersion Cns_Etc::GetCurOSVersion()
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	BOOL bOsVersionInfoEx;

	HKEY hKey;
	TCHAR szProductType[80];
	DWORD dwBufLen=80;
	LONG lRet;
	Cns_Etc::OsVersion Result = OSVersionInvalied;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) ){
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if( !GetVersionEx( (OSVERSIONINFO *) &osvi) ) 
			return Result;
	}

	// Use GetProcAddress to avoid load issues on Windows 2000
	pGNSI = (PGNSI)GetProcAddress(	GetModuleHandle( _T("kernel32.dll")),_T("GetNativeSystemInfo") );
	if(NULL != pGNSI)
		pGNSI(&si);


	switch (osvi.dwPlatformId){
		// Test for the Windows NT product family.
		case VER_PLATFORM_WIN32_NT:
			// Test for the specific product.
			if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 ){

				if( osvi.dwMinorVersion == 0 ){

					if( osvi.wProductType == VER_NT_WORKSTATION )
						Result=OSVersionWinVista;
					else
						Result=OSVersionWin2008;
				}
				else if( osvi.dwMinorVersion == 1 ){

					if( osvi.wProductType == VER_NT_WORKSTATION )
						Result=OSVersionWin7;
					else
						Result=OSVersionWin2008R2;
				}
			}
			else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 ){

				if( GetSystemMetrics(SM_SERVERR2) )
					Result=OSVersionWin2003; //Microsoft Windows Server 2003 R2
				else if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
					Result=OSVersionWinXp;//Microsoft Windows XP Professional x64 Edition
				else
					Result=OSVersionWin2003;//Microsoft Windows Server 2003
			}
			else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
				Result=OSVersionWinXp;//Microsoft Windows XP
			else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
				Result=OSVersionWin2000;//Microsoft Windows 2000
			else if( osvi.dwMajorVersion <= 4 )
				Result=OSVersionWinNT;//Microsoft Windows NT

			// Test for specific product on Windows NT 4.0 SP6 and later.
			if( bOsVersionInfoEx && pGNSI ){
				// Test for the workstation type.
				if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture!=PROCESSOR_ARCHITECTURE_AMD64){
					/*
					if( osvi.dwMajorVersion == 4 ) TraceOnly( 0,"Workstation 4.0 " );
					else if( osvi.wSuiteMask & VER_SUITE_PERSONAL )	TraceOnly( 0,"Home Edition " );
					else TraceOnly( 0,"Professional " );
					*/
				}// Test for the server type.
				else if( osvi.wProductType == VER_NT_SERVER || osvi.wProductType == VER_NT_DOMAIN_CONTROLLER ){
					if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==2){
						if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 ){
							/*
							if( osvi.wSuiteMask & VER_SUITE_DATACENTER )	TraceOnly( 0,"Datacenter Edition for Itanium-based Systems" );
							else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )	TraceOnly( 0,"Enterprise Edition for Itanium-based Systems" );
							*/
						}
						else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 ){
							/*
							if( osvi.wSuiteMask & VER_SUITE_DATACENTER )	TraceOnly( 0,"Datacenter x64 Edition " );
							else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )	TraceOnly( 0,"Enterprise x64 Edition " );
							else TraceOnly( 0,"Standard x64 Edition " );
							*/
						}
						else{
							/*
							if( osvi.wSuiteMask & VER_SUITE_DATACENTER )	TraceOnly( 0,"Datacenter Edition " );
							else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )	TraceOnly( 0,"Enterprise Edition " );
							else if ( osvi.wSuiteMask == VER_SUITE_BLADE )	TraceOnly( 0,"Web Edition " );
							else	TraceOnly( 0,"Standard Edition " );
							*/
						}
					}
					else if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==0){
						/*
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )	TraceOnly( 0,"Datacenter Server " );
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )	TraceOnly( 0,"Advanced Server " );
						else	TraceOnly( 0,"Server " );
						*/
					}
					else{  // Windows NT 4.0 
						/*
						if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )	TraceOnly( 0,"Server 4.0, Enterprise Edition " );
						else	TraceOnly( 0,"Server 4.0 " );
						*/
					}
				}
			}
			else{// Test for specific product on Windows NT 4.0 SP5 and earlier

				lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
						_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
						0, KEY_QUERY_VALUE, &hKey );
				if( lRet != ERROR_SUCCESS )
					return Result;

				lRet = RegQueryValueEx( hKey, _T("ProductType"), NULL, NULL,
						(LPBYTE) szProductType, &dwBufLen);
				RegCloseKey( hKey );

				if( (lRet != ERROR_SUCCESS) || (dwBufLen > 80) )
					return Result;
				/*
				if( lstrcmpi( "WINNT", szProductType) == 0 )	TraceOnly( 0,"Workstation " );
				if( lstrcmpi( "LANMANNT", szProductType) == 0 )	TraceOnly( 0,"Server " );
				if( lstrcmpi( "SERVERNT", szProductType) == 0 )	TraceOnly( 0,"Advanced Server " );
				TraceOnly( 0,"%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion );
				*/
			}

			// Display service pack (if any) and build number.
			if( osvi.dwMajorVersion == 4 && lstrcmpi( osvi.szCSDVersion, _T("Service Pack 6") ) == 0 ){ 

				// Test for SP6 versus SP6a.
				lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
						_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),
						0, KEY_QUERY_VALUE, &hKey );
				/*
				if( lRet == ERROR_SUCCESS )	TraceOnly( 0,"Service Pack 6a (Build %d)\n",osvi.dwBuildNumber & 0xFFFF );         
				else{ // Windows NT 4.0 prior to SP6a
					TraceOnly( 0,"%s (Build %d)\n",osvi.szCSDVersion,osvi.dwBuildNumber & 0xFFFF);
				}
				*/

				RegCloseKey( hKey );
			}
			else{ // not Windows NT 4.0 
				//TraceOnly( 0,"%s (Build %d)\n",osvi.szCSDVersion,osvi.dwBuildNumber & 0xFFFF);
			}
			break;
		// Test for the Windows Me/98/95.
		case VER_PLATFORM_WIN32_WINDOWS:

			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0){
				//TraceOnly ( 0,"Microsoft Windows 95 ");
				Result=OSVersionWin95;
				/*
				if (osvi.szCSDVersion[1]=='C' || osvi.szCSDVersion[1]=='B')
					TraceOnly( 0,"OSR2 " );
				*/
			} 

			if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10){
				//TraceOnly( 0,"Microsoft Windows 98 ");
				Result=OSVersionWin98;
				/*
				if( osvi.szCSDVersion[1]=='A' || osvi.szCSDVersion[1]=='B')
					TraceOnly( 0,"SE " );
				*/
			}

			if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90){
				//TraceOnly( 0,"Microsoft Windows Millennium Edition\n");
				Result=OSVersionWinMe;
			} 
			break;
		case VER_PLATFORM_WIN32s:
			//TraceOnly( 0,"Microsoft Win32s\n");
			break;
	}//end switch
	return Result; 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// return Locale Identifier Constants and Strings
// see : ms-help://MS.W7SDK.1033/MS.W7SDKCOM.1033/intl/intl/language_identifier_constants_and_strings.htm
INT Cns_Etc::GetOSDefaultLanguage()
{
	LONG lDefLanInfo;
	INT nLanguageCode=0;


	if( !SystemParametersInfo( SPI_GETDEFAULTINPUTLANG,0,&lDefLanInfo,0 ) ){
		return 0;	//invalied language code
	}
	//
	lDefLanInfo=lDefLanInfo&0x0000FFFF;//make upper word

	if( lDefLanInfo==0x0812 ){//Korean OS
		//0x0812 is only Windows 95, Windows NT 4.0
		nLanguageCode=LANG_KOREAN;//( 0x0412 == LANG_KOREAN )
	}
	else
		nLanguageCode = lDefLanInfo;


	return nLanguageCode;
}


//time delay  unit : 100nsec
//constant sytle : 1 sec = 10000000LL
VOID Cns_Etc::Delay( LONGLONG llTime )
{
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	liDueTime.QuadPart = -llTime;

	// Create an unnamed waitable timer.
	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (NULL == hTimer)
		return;

	// Set a timer to wait for 10 seconds.
	if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
		return;

	// Wait for the timer.
	WaitForSingleObject(hTimer, INFINITE);

	CloseHandle( hTimer );
}

//make multi zero-string with given zero-strings
//return the numebr of data(BYTE) in multistring(including NULL )
INT Cns_Etc::MakeMultiString( INT nCnt,LPTSTR szMultiStr, ... )
{
	LPTSTR pStr = NULL;
	LPTSTR szMultiOut = szMultiStr;
	INT nNumStr = 0;
	INT i;

	va_list argptr;
	va_start(argptr, nCnt);

	//pass second argument
	va_arg( argptr, LPTSTR );

	for( i=0; i<nCnt; i++ ){
		pStr = (LPTSTR)va_arg( argptr, LPCTSTR );

		if( pStr ){

			if( szMultiStr ){
				_tcscpy( szMultiOut, pStr );
				szMultiOut[_tcslen(pStr)] = NULL;

				szMultiOut = szMultiOut + _tcslen(pStr) +1;
			}

			nNumStr = nNumStr + (_tcslen(pStr) +1) * sizeof(TCHAR);
		}
	}//end for

	if( nNumStr >0 ){

		if( szMultiStr ){
			szMultiOut[0] = NULL;	//make multi Zero string
		}
		nNumStr = nNumStr + sizeof(TCHAR);
	}

	return nNumStr;

}

INT Cns_Etc::GetStringFromMultiString( std::list<_tstring> &listStr,LPCTSTR szMultiStr )
{
	LPCTSTR pDest;
	_tstring stemp;
	INT nCount =0;
	INT nOffset = 0;

	if( szMultiStr == NULL )
		return 0;
	else
		listStr.clear();
	//

	while( szMultiStr[nOffset] != NULL ){

		pDest = &(szMultiStr[nOffset]);
		stemp=pDest;
		listStr.push_back( stemp );

		nOffset += stemp.length()+1;//for passing null termination
		nCount++;
	}//while

	return nCount;
}

void Cns_Etc::GetLastErrorString( _tstring & sError )
{
	DWORD dwError = ::GetLastError();

	HLOCAL hLocal = NULL;

	size_t size = ::FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)(&hLocal),
		0,
		NULL
		);

	sError.clear();

	if( hLocal != NULL ){
		TCHAR *sOut = (TCHAR*)hLocal;
		sError.assign( &sOut[0], size );
		::LocalFree( hLocal );
	}
}