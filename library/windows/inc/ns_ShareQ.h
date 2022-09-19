#pragma once

#include <Windows.h>
#include "ns_ShareMem.h"
#include "ns_ShareQ.h"
#include "ns_Etc.h"
#include "ns_tchar.h"
#include <time.h>
#include "ns_new.h"

using namespace std;

template <typename T>
class Cns_ShareQ
{
public:
	static const INT C_nMAX_SIZE_QUEUE	=	1024;
	static const INT C_nDEF_SIZE_QUEUE = 512;

private:
	class CShareQItemBase:public  Cns_New
	{
	public:
		using Cns_New::operator new;
		using Cns_New::operator delete;
		//basic
		static void* operator new ( size_t size ) throw(std::bad_alloc)
		{
			if( size == 0 )
				size = 8;
			//
			void *p;

			while(true){
				p= Cns_ShareMem::CreateMem( size );//::operator new(size);
				if( p )
					return p;

				std::new_handler globalHandler = std::set_new_handler(0);
				std::set_new_handler( globalHandler );

				if( globalHandler )
					(*globalHandler)();
				else
					throw std::bad_alloc();
			}//end while
		} 

		static void operator delete (void *p) throw()
		{	
			Cns_ShareMem::DeleteMem( p );
		}

		static void* operator new [] (size_t size ) throw(std::bad_alloc)
		{
			if( size == 0 )
				size = 8;
			//
			void *p;

			while(true){
				p= Cns_ShareMem::CreateMem( size );//::operator new(size);
				if( p )
					return p;

				std::new_handler globalHandler = std::set_new_handler(0);
				std::set_new_handler( globalHandler );

				if( globalHandler )
					(*globalHandler)();
				else
					throw std::bad_alloc();
			}//end while
		} 

		static void operator delete [] (void *p ) throw()
		{	
			Cns_ShareMem::DeleteMem( p );
		}

		//displacement
		static void* operator new (size_t size, LPCTSTR sName,HANDLE & hMap ) throw(std::bad_alloc)
		{
			if( size == 0 )
				size = 8;
			//
			void *p;

			while(true){
				p= Cns_ShareMem::CreateMem( size,&hMap,sName );//::operator new(size);
				if( p )
					return p;

				std::new_handler globalHandler = std::set_new_handler(0);
				std::set_new_handler( globalHandler );

				if( globalHandler )
					(*globalHandler)();
				else
					throw std::bad_alloc();
			}//end while
		} 

		static void operator delete (void *p, LPCTSTR sName,HANDLE & hMap ) throw()
		{	
			Cns_ShareMem::DeleteMem( p,hMap );
		}

		static void* operator new [] (size_t size, LPCTSTR sName,HANDLE & hMap ) throw(std::bad_alloc)
		{
			if( size == 0 )
				size = 8;
			//
			void *p;

			while(true){
				p= Cns_ShareMem::CreateMem( size,&hMap,sName );//::operator new(size);
				if( p )
					return p;

				std::new_handler globalHandler = std::set_new_handler(0);
				std::set_new_handler( globalHandler );

				if( globalHandler )
					(*globalHandler)();
				else
					throw std::bad_alloc();
			}//end while
		} 

		static void operator delete [] (void *p, LPCTSTR sName,HANDLE & hMap ) throw()
		{	
			Cns_ShareMem::DeleteMem( p,hMap );
		}

	public:
		CShareQItemBase(){}
		virtual ~CShareQItemBase(){}

	public:
		INT m_nRear;	//will be inserted index of array
		INT m_nFront;	//will be removed index of array
		INT m_nSize;

		T m_Array[C_nDEF_SIZE_QUEUE];
	};

public:
	explicit Cns_ShareQ( LPCTSTR sName ) :
	m_hMap(NULL)
	{
		m_pArray = new(sName,m_hMap) CShareQItemBase;
		_ASSERT( m_pArray != NULL );

		BOOL bFirst = Cns_ShareMem::IsFirst( m_pArray );

		if( bFirst ){
			m_pArray->m_nRear = m_pArray->m_nFront = 0;
			m_pArray->m_nSize = 0;
		}

		_tstring Name = sName;
		Name += _T("_MUTEXT_");
		m_hSyncMutex = Cns_Etc::CreateMutex( FALSE, FALSE, Name.c_str() );
		_ASSERT( m_hSyncMutex != NULL );

		Name = sName;
		Name += _T("_EVENT_");
		m_hItemExistEvent = ::OpenEvent( EVENT_MODIFY_STATE, FALSE, Name.c_str() );
		if( m_hItemExistEvent == NULL ){
			if( GetLastError()==ERROR_FILE_NOT_FOUND ){
				
				//the second create-try
				m_hItemExistEvent=::CreateEvent( NULL, TRUE, FALSE, Name.c_str() );
				_ASSERT( m_hItemExistEvent != NULL );
			}
			else{
				_ASSERT( FALSE );//DON'T HERE YOUR CODE.
			}
		}
		//

	}

	virtual ~Cns_ShareQ()
	{
		try{
			if( m_pArray ){
				CShareQItemBase::operator delete(m_pArray,_T("dummy"),m_hMap);
				m_pArray = NULL;
			}

			if( m_hSyncMutex ){
				CloseHandle( m_hSyncMutex );
			}

			if( m_hItemExistEvent ){
				CloseHandle( m_hItemExistEvent );
			}
		}
		catch(...){

		}
	}

	//getter
	UINT GetName( TCHAR *sName = NULL )
	{
		return Cns_ShareMem::GetNameOfMem( m_pArray, sName );
	}
	UINT GetMemorySize()
	{
		return Cns_ShareMem::GetSizeofMem( m_pArray );
	}
	INT GetSize(){	return m_nSize;	};

	BOOL IsEmpty()
	{
		BOOL bResult;

		if( !EnterMutex() )
			return FALSE;

		if( m_pArray->m_nRear == m_pArray->m_nFront )
			bResult = TRUE;
		else
			bResult = FALSE;

		LeaveMutex();

		return bResult;
	}

	BOOL IsFull()
	{
		BOOL bResult;

		if( !EnterMutex() )
			return FALSE;

		if( m_pArray->m_nFront == (m_pArray->m_nRear+1)%C_nDEF_SIZE_QUEUE )
			bResult = TRUE;
		else
			bResult = FALSE;

		LeaveMutex();

		return bResult;
	}

	VOID Empty()
	{
		if( !EnterMutex() )
			return FALSE;

		m_pArray->m_nRear = m_pArray->m_nFront = 0;
		m_pArray->m_nSize = 0;

		BOOL bResult = ::ResetEvent( m_hItemExistEvent );
		_ASSERT( bResult == TRUE );
		LeaveMutex();
	}

	BOOL EnQueue( T & Item )
	{
		if( IsFull() )
			return FALSE;

		if( !EnterMutex() )
			return FALSE;

		m_pArray->m_nRear = (m_pArray->m_nRear+1) % C_nDEF_SIZE_QUEUE;
		m_pArray->m_Array[m_pArray->m_nRear] = Item;

		m_pArray->m_nSize++;

		BOOL bResult = ::SetEvent( m_hItemExistEvent );
		_ASSERT( bResult == TRUE );

		LeaveMutex();

		return TRUE;
	}

	BOOL Dequeue( T & Item )
	{
		if( IsEmpty() )
			return FALSE;
		//
		if( !EnterMutex() )
			return FALSE;

		m_pArray->m_nFront = (m_pArray->m_nFront+1) % C_nDEF_SIZE_QUEUE;
		Item = m_pArray->m_Array[m_pArray->m_nFront];

		m_pArray->m_nSize--;

		if( m_pArray->m_nSize == 0 ){
			BOOL bResult = ::ResetEvent( m_hItemExistEvent );
			_ASSERT( bResult == TRUE );
		}

		LeaveMutex();

		return TRUE;
	}

	BOOL Peek( T & Item )
	{
		if( IsEmpty() )
			return FALSE;
		//
		if( !EnterMutex() )
			return FALSE;

		INT nFront = (m_pArray->m_nFront+1) % C_nDEF_SIZE_QUEUE;
		Item = m_pArray->m_Array[nFront];

		LeaveMutex();

		return TRUE;
	}

	HANDLE GetItemExistEvent()
	{
		return m_hItemExistEvent;
	}

private:
	CShareQItemBase *m_pArray;

	HANDLE m_hSyncMutex;

	HANDLE m_hItemExistEvent;	//this event is signed , if item exsit in queue

	HANDLE m_hMap;	//share map handle
	
private:
	Cns_ShareQ();

	BOOL EnterMutex( DWORD dwTimeout = INFINITE)
	{
		DWORD dwWaitResult = WaitForSingleObject( m_hSyncMutex, dwTimeout );

		if( WAIT_OBJECT_0 != dwWaitResult )
			return FALSE;

		return TRUE;
	}

	BOOL LeaveMutex()
	{
		return ReleaseMutex( m_hSyncMutex );
	}

};

