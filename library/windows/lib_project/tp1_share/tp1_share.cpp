// tp1_share.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include "ns_tchar.h"
#include "ns_ShareQ.h"
#include <algorithm>
#include<locale>
#include<functional>

#undef	TEST_SINGLE
#define	TEST_FULL

#pragma comment(lib, "ns_share.lib")

using namespace std;

Cns_ShareQ<int> Qa( _T("TEST_InQueue") );
Cns_ShareQ<int> Qb( _T("TEST_OutQueue") );

static int gna = 1;
static int gnb = 1;

const DWORD dwSleep = 50;

void Print_Pre()
{
	_tout << _T("=============================") << endl;
	_tout << _T("  t1_share <input> starting test ") << endl;
	_tout << _T("  * ") << __DATE__ << __TIME__ << endl;
	_tout << _T("=============================") << endl;
}

void Print_Post()
{
	_tout << _T("=============================") << endl;
	_tout << _T("  ending test ") << endl;
	_tout << _T("  * ") << __DATE__ << __TIME__ << endl;
	_tout << _T("=============================") << endl;
}

void TestEnqueueQa( int nloop )
{
	for( int i =0 ; i<nloop; i++ ){
		if( !Qa.EnQueue( gna ) ){
			_tout << _T("[") << gna << _T("]  ERROR : TestEnqueueQa ") << endl;
			return;
		}
		else{
			_tout << _T("[") << gna << _T("]  OK : TestEnqueueQa ") << endl;
		}

		gna++;
	}//end for
}

void TestEnqueueQb( int nloop )
{
	for( int i =0 ; i<nloop; i++ ){
		if( !Qb.EnQueue( gnb ) ){
			_tout << _T("[") << gnb << _T("]  ERROR : TestEnqueueQb ") << endl;
			return;
		}
		else{
			_tout << _T("[") << gnb << _T("]  OK : TestEnqueueQb ") << endl;
		}

		gnb++;
	}//end for
}

void TestDequeueQa( int nloop )
{
	int nOut;

	for( int i = 0; i< nloop; i++ ){
		while(1){
			Sleep( dwSleep );

			if( !Qa.IsEmpty() )
				break;//exit while
		}

		if( !Qa.Dequeue( nOut ) ){
			_tout << _T("[") << -100 << _T("]  ERROR : TestDequeueQa ") << endl;
		}
		else{
			_tout << _T("[") << nOut << _T("]  OK : TestDequeueQa ") << endl;
		}
	}//end for
}

void TestDequeueQb( int nloop )
{
	int nOut;

	for( int i = 0; i< nloop; i++ ){
		while(1){
			Sleep( dwSleep );

			if( !Qb.IsEmpty() )
				break;//exit while
		}

		if( !Qb.Dequeue( nOut ) ){
			_tout << _T("[") << -100 << _T("]  ERROR : TestDequeueQb ") << endl;
		}
		else{
			_tout << _T("[") << nOut << _T("]  OK : TestDequeueQb ") << endl;
		}
	}//end for
}

void TestEnFullQa()
{
	for( int i =0 ; i<1024; i++ ){
		if( !Qa.EnQueue( gna ) ){
			_tout << _T("[") << gna << _T("]  ERROR : TestEnFullQa ") << endl;
			return;
		}
		else{
			_tout << _T("[") << gna << _T("]  OK : TestEnFullQa ") << endl;
		}

		gna++;

		if( Qa.IsFull() ){
			return;
		}
	}//end for
}

void TestEnFullQb()
{
	for( int i =0 ; i<1024; i++ ){
		if( !Qb.EnQueue( gnb ) ){
			_tout << _T("[") << gnb << _T("]  ERROR : TestEnFullQb ") << endl;
			return;
		}
		else{
			_tout << _T("[") << gnb << _T("]  OK : TestEnFullQb ") << endl;
		}

		gnb++;

		if( Qb.IsFull() ){
			return;
		}
	}//end for
}

void TestDeFullQa()
{
	int nOut;

	while(1){
		Sleep( dwSleep );

		if( Qa.IsFull() )
			break;//exit while
	}

	while(!Qa.IsEmpty()){

		if( !Qa.Dequeue( nOut ) ){
			_tout << _T("[") << -100 << _T("]  ERROR : TestDeFullQa ") << endl;
		}
		else{
			_tout << _T("[") << nOut << _T("]  OK : TestDeFullQa ") << endl;
		}
	}

}

void TestDeFullQb()
{
	int nOut;

	while(1){
		Sleep( dwSleep );

		if( Qb.IsFull() )
			break;//exit while
	}

	while(!Qb.IsEmpty()){

		if( !Qb.Dequeue( nOut ) ){
			_tout << _T("[") << -100 << _T("]  ERROR : TestDeFullQb ") << endl;
		}
		else{
			_tout << _T("[") << nOut << _T("]  OK : TestDeFullQb ") << endl;
		}
	}

}

struct lt_str_1 : public std::binary_function< _tstring, _tstring, bool>
{
	struct lt_char {

		const std::ctype<TCHAR>& ct;

		lt_char(const std::ctype<TCHAR>& c) : ct(c)
		{
		}

		bool operator()(TCHAR x, TCHAR y) const
		{	
			return ct.toupper(x) < ct.toupper(y);
			//return (ct.toupper(x) == ct.toupper(y));
		}
	};

	std::locale loc;
	const std::ctype<TCHAR>& ct;

	lt_str_1( const std::locale& L = std::locale::classic() )
	: loc(L), ct( std::use_facet<std::ctype<TCHAR> >(loc) )
	{
	}

	bool operator()(const _tstring& x,	const _tstring& y) const
	{
		return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end(), lt_char(ct));
	}
};


//////////////////////////////////////////////////
//  main
//////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	_tstring s1( _T("a21bcD") );
	_tstring s2( _T("A21BCD") );

	lt_str_1 cmp;

	if( !cmp( s1,s2 ) && !cmp( s2,s1 ) )
	{
		_tout << _T("equal") <<endl;
	}
	else{
		_tout << _T("Not equal") <<endl;
	}

	return 0;
	///////////////////////////////////////////////////////////////////////////
	//set repeat counter.
	int nCounter;
	_tstring sCounter=_T("1");

	if( argc > 1 ){
		sCounter = argv[1];
	}
	try{
		nCounter = stoi( sCounter );
	}
	catch(std::invalid_argument e){
		_tout << _T("input integer.") <<endl;
		return 0;
	}

	/////////////
	Print_Pre();

#ifdef	TEST_SINGLE
	for( int i=0; i<nCounter; i++ ){

		TestEnqueueQa( 1 );

		TestDequeueQb( 1 );
	}//end for
#endif	//TEST_SINGLE

#ifdef	TEST_FULL
	for( int i=0; i<nCounter; i++ ){
		TestEnFullQa();
		TestDeFullQb();
	}//end for
#endif		//TEST_FULL

	//////////////
	Print_Post();

	return 0;
}

