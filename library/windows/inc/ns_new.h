///////////////////////////////////////////////////////////////////////////
//	the base class of user-defintion new & delete operator
//	designed by Seong-Sig, Yang
//
#pragma once

class Cns_New{

public:

	//basic new & delete operator
	static void * operator new(std::size_t size) throw(std::bad_alloc)
	{
		return ::operator new( size );
	}

	static void operator delete( void *p ) throw()
	{
		::operator delete(p);
	}

	//displacement new & delete opertor
	static void * operator new(std::size_t size, void *pBase )throw()
	{
		return ::operator new( size,pBase );
	}

	static void operator delete( void *p, void *pBase) throw()
	{
		::operator delete( p,pBase );
	}

	//no throw new & delete operator
	static void * operator new(std::size_t size, const std::nothrow_t & nt ) throw()
	{
		return ::operator new( size, nt );
	}

	static void operator delete( void *p, const std::nothrow_t & nt ) throw()
	{
		::operator delete( p);
	}

};

