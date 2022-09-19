#include "StdAfx.h"
#include "ns_ShareMem.h"
#include "ns_Etc.h"
#include "ns_tchar.h"
#include <time.h>

#pragma comment(lib, "ns_etc.lib")

Cns_ShareMem::Cns_ShareMem()
{
	m_ShareMem.hMapObj = NULL;
	m_pMem = NULL;
	m_ShareMem.dwSize = 0;
	m_bIsCreate = FALSE;
}

Cns_ShareMem::Cns_ShareMem( DWORD dwSize, CONST TCHAR *sName /*=NULL*/ )
{
	m_ShareMem.hMapObj = NULL;
	m_pMem = NULL;
	m_ShareMem.dwSize = 0;
	m_bIsCreate = FALSE;

	LPVOID lpMem = Create( dwSize, sName );

	_ASSERT( lpMem != NULL );
}


Cns_ShareMem::~Cns_ShareMem()
{
	try{
		Delete();
	}
	catch(...){

	}
}


LPVOID Cns_ShareMem::Create( DWORD dwSize, CONST TCHAR *sName /*=NULL*/ )
{
	HANDLE hMapObj=NULL;
	LPVOID pvShared=NULL;
	BOOL bFirst;
	LPTSTR lpMemName=NULL;
	TCHAR sMemName[C_nNAME_MAX_SIZE];

	if( dwSize == 0 )
		return NULL;

	if( sName == NULL ){
		srand( (unsigned)time( NULL ) );

		Cns_Etc::CreateRandomName( sMemName, (INT)GetTickCount(), rand() );
	}
	else
		_tcscpy( sMemName, sName );
		
	// Create a named file mapping object.
	hMapObj = CreateFileMapping(
				INVALID_HANDLE_VALUE,  // Use paging file
				NULL,                 // No security attributes
				PAGE_READWRITE,       // Read/Write access
				0,                    // Mem Size: high 32 bits
				dwSize,          // Mem Size: low 32 bits
				sMemName);   // Name of map object
	if( hMapObj!=NULL ){
		// Determine if this is the first create of the file mapping.
		bFirst=(ERROR_ALREADY_EXISTS != GetLastError());

		// Now get a pointer to the file-mapped shared memory.
		pvShared = MapViewOfFile(
					hMapObj,        // File Map obj to view
					FILE_MAP_WRITE,   // Read/Write access
					0,                // high: map from beginning
					0,                // low:
					0);               // default: map entire file
		if( pvShared != NULL ){

			m_ShareMem.hMapObj=hMapObj;//save map handle for release mapfile
			m_ShareMem.dwSize = dwSize;
			m_pMem=pvShared;

			if( bFirst ){
				m_bIsCreate=TRUE;
				memset( pvShared, 0, dwSize);
				_tcscpy( m_ShareMem.sName, sMemName );
			}
			else
				m_bIsCreate=FALSE;

			return m_pMem;
		}
		else{
			CloseHandle(hMapObj);
			return NULL;
		}
		

	}
	else
		return NULL;
}

VOID Cns_ShareMem::Delete()
{
	if( m_pMem ){

		if( UnmapViewOfFile( m_pMem ) ){
			CloseHandle( m_ShareMem.hMapObj );
		}

		m_pMem = NULL;
		m_bIsCreate = FALSE;
		m_ShareMem.hMapObj = NULL;
		m_ShareMem.dwSize = 0;
	}
}

VOID Cns_ShareMem::GetName( LPTSTR sName )
{
	_ASSERT( sName != NULL && m_ShareMem.sName[0] != NULL );
	_tcscpy( sName, m_ShareMem.sName );
}


LPVOID Cns_ShareMem::CreateMem( size_t size, HANDLE *phMap /* = NULL */, CONST TCHAR *sName /*=NULL*/ )
{
	HANDLE hMapObj=NULL;
	UCHAR *pvShared=NULL;
	BOOL bFirst;
	LPTSTR lpMemName=NULL;
	TCHAR sMemName[C_nNAME_MAX_SIZE];
	ShareMem share;
	LPShareMem pshare;

	if( size == 0 )
		return NULL;

	if( sName == NULL ){
		srand( (unsigned)time( NULL ) );

		Cns_Etc::CreateRandomName( sMemName, (INT)GetTickCount(), rand() );
	}
	else
		_tcscpy( sMemName, sName );
		
	size += sizeof share;

	// Create a named file mapping object.
	hMapObj = CreateFileMapping(
				INVALID_HANDLE_VALUE,  // Use paging file
				NULL,                 // No security attributes
				PAGE_READWRITE,       // Read/Write access
				0,                    // Mem Size: high 32 bits
				size,          // Mem Size: low 32 bits
				sMemName);   // Name of map object
	if( hMapObj!=NULL ){
		// Determine if this is the first create of the file mapping.
		bFirst=(ERROR_ALREADY_EXISTS != GetLastError());

		// Now get a pointer to the file-mapped shared memory.
		pvShared = (UCHAR*)MapViewOfFile(
					hMapObj,        // File Map obj to view
					FILE_MAP_WRITE,   // Read/Write access
					0,                // high: map from beginning
					0,                // low:
					0);               // default: map entire file
		if( pvShared != NULL ){

			if( phMap )
				*phMap= hMapObj;

			if( bFirst ){
				memset( pvShared, 0, size);

				share.bFirst = TRUE;
				share.hMapObj=hMapObj;//save map handle for release mapfile
				share.dwSize = size;
				_tcscpy( share.sName, sMemName );
				memcpy( pvShared, &share, sizeof share ); 
			}
			else{
				pshare = (LPShareMem)pvShared;
				pshare->bFirst = FALSE;
			}

			pvShared += sizeof share;
			return pvShared;
		}
		else{
			CloseHandle(hMapObj);
			return NULL;
		}
		

	}
	else{
		DWORD dwResult = GetLastError();
		return NULL;
	}
}

VOID Cns_ShareMem::DeleteMem( LPVOID pShareMem,CONST HANDLE hMap /*=NULL*/  )
{
	ShareMem share;
	UCHAR *pMem;

	if( pShareMem ){

		pMem = (UCHAR*)pShareMem;
		pMem -= sizeof share;
		memcpy( &share, pMem, sizeof share ); 

		if( UnmapViewOfFile( pMem ) ){

			if( hMap )
				CloseHandle( hMap );
			else
				CloseHandle( share.hMapObj );
		}
	}
}

UINT Cns_ShareMem::GetNameOfMem( LPVOID pShareMem, TCHAR *sName /*= NULL*/ )
{
	if( pShareMem == NULL )
		return 0;
	//
	ShareMem share;
	UCHAR *pMem = (UCHAR*)pShareMem;

	pMem -= sizeof share;
	memcpy( &share, pMem, sizeof share ); 

	if( sName ){
		_tcscpy( sName, share.sName );
	}
	return (UINT)_tcslen( share.sName );
}

UINT Cns_ShareMem::GetSizeofMem( LPVOID pShareMem )
{
	if( pShareMem == NULL )
		return 0;
	//
	ShareMem share;
	UCHAR *pMem = (UCHAR*)pShareMem;

	pMem -= sizeof share;
	memcpy( &share, pMem, sizeof share ); 

	return (UINT)share.dwSize;
}

BOOL Cns_ShareMem::IsFirst( LPVOID pShareMem )
{
	_ASSERT( pShareMem != NULL );
	//
	ShareMem share;
	UCHAR *pMem = (UCHAR*)pShareMem;

	pMem -= sizeof share;
	memcpy( &share, pMem, sizeof share ); 

	return (UINT)share.bFirst;

}