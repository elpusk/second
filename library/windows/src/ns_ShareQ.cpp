/*
#include "StdAfx.h"
#include "ns_ShareQ.h"
#include "ns_Etc.h"
#include "ns_tchar.h"
#include <time.h>

/////////////////////////////////////////////////////////
// ShareQItemBase calss
template <typename T> Cns_ShareQ<T>::CShareQItemBase::CShareQItemBase()
{
}
template <typename T> Cns_ShareQ<T>::CShareQItemBase::~CShareQItemBase()
{
}

template <typename T> void* Cns_ShareQ<T>::CShareQItemBase::operator new (size_t size, LPCTSTR sName )
{
	void *p= Cns_ShareMem::CreateMem( size, sName );//::operator new(size);
	return p; 
} 

template <typename T> void Cns_ShareQ<T>::CShareQItemBase::operator delete (void *p, LPCTSTR sName )
{	
	//::operator delete(p); // return memory to pool
	Cns_ShareMem::DeleteMem( p );
}


template <typename T> void* Cns_ShareQ<T>::CShareQItemBase::operator new [] (size_t size, LPCTSTR sName )
{
	void *p= Cns_ShareMem::CreateMem( size, sName );//::operator new(size);
	return p; 
} 

template <typename T> void Cns_ShareQ<T>::CShareQItemBase::operator delete [] (void *p, LPCTSTR sName )
{	
	//::operator delete [] (p); // return memory to pool
	Cns_ShareMem::DeleteMem( p );
}

//////////////////////////////////////////////////////////////
// Cns_ShareQ

template <typename T> Cns_ShareQ<T>::Cns_ShareQ( LPCTSTR sName )
{
	m_pArray = new(sName) CShareQItemBase;
	_ASSERT( m_pArray != NULL );

	m_nRear = m_nFront = 0;
	m_nSize = 0;
}

 template <typename T> Cns_ShareQ<T>::~Cns_ShareQ()
{
	if( m_pArray ){
		CShareQItemBase::operator delete(m_pArray,NULL);
		m_pArray = NULL;
	}
}

template <typename T> BOOL Cns_ShareQ<T>::EnQueue( T & Item )
{
	if( IsFull() )
		return FALSE;

	m_nRear = (m_nRear+1) % C_nDEF_SIZE_QUEUE;
	m_pArray->m_Array[m_nRear] = Item;

	m_nSize++;
	return TRUE;
}

template <typename T> BOOL Cns_ShareQ<T>::Dequeue( T & Item )
{
	if( IsEmpty() )
		return FALSE;
	//
	m_nFront = (m_nFront+1) % C_nDEF_SIZE_QUEUE;
	Item = m_pArray->m_Array[m_nFront];

	m_nSize--;
	return TRUE;
}

template <typename T> BOOL Cns_ShareQ<T>::Peek( T & Item )
{
	if( IsEmpty() )
		return FALSE;
	//
	INT nFront = (m_nFront+1) % C_nDEF_SIZE_QUEUE;
	Item = m_pArray->m_Array[nFront];

	return TRUE;
}


template <typename T> BOOL Cns_ShareQ<T>::IsEmpty()
{
	if( m_nRear == m_nFront )
		return TRUE;
	else
		return FALSE;
}

template <typename T> BOOL Cns_ShareQ<T>::IsFull()
{
	if( m_nFront == (m_nReae+1)%C_nDEF_SIZE_QUEUE )
		return TRUE;
	else
		return FALSE;
}

template <typename T> VOID Cns_ShareQ<T>::Empty()
{
	m_nRear = m_nFront = 0;
	m_nSize = 0;
}
*/