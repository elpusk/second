#pragma once

#include <Windows.h>

class Cns_ShareMem
{
public:
	static const INT C_nNAME_MAX_SIZE	=	256;

#pragma pack(push,1)

	typedef	struct ShareMemTag{
		BOOL bFirst;					//the first
		HANDLE hMapObj;				// [out] file mapping object.
		DWORD dwSize;				// [in] the size of memory 
		TCHAR sName[C_nNAME_MAX_SIZE];		// [in] Name of map object
	}ShareMem, *PShareMem, *LPShareMem;

#pragma pack(pop)

	static LPVOID CreateMem( size_t size, HANDLE *phMap = NULL ,CONST TCHAR *sName = NULL );
	static VOID DeleteMem( LPVOID pShareMem, CONST HANDLE hMap = NULL );
	static UINT GetNameOfMem( LPVOID pShareMem, TCHAR *sName = NULL );
	static UINT GetSizeofMem( LPVOID pShareMem );
	static BOOL IsFirst( LPVOID pShareMem );

public:
	Cns_ShareMem();
	explicit Cns_ShareMem( DWORD dwSize, CONST TCHAR *sName = NULL );
	virtual ~Cns_ShareMem();

	LPVOID Create( DWORD dwSize, CONST TCHAR *sName = NULL );
	VOID Delete();

	//getter
	HANDLE GetFileMapHandle()
	{
		return m_ShareMem.hMapObj;
	}

	LPVOID GetMemAddress()
	{
		return m_pMem;
	}

	DWORD GetMemSize()
	{
		return m_ShareMem.dwSize;
	}

	VOID GetName( LPTSTR sName );

private:
	ShareMem m_ShareMem;
	LPVOID m_pMem;
	BOOL m_bIsCreate;
};

