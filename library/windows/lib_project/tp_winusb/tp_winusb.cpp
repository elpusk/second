// tp_winusb.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include "ns_winusb.h"
#include <iostream>
#include "ns_tchar.h"

#pragma comment(lib, "ns_winusb.lib")

// {2BE1AD16-11B1-448D-9987-0B5520899DB7}
#define	DEV_GUID	\
{ 0x2be1ad16, 0x11b1, 0x448d, { 0x99, 0x87, 0xb, 0x55, 0x20, 0x89, 0x9d, 0xb7 } }

//{ff646f80-8def-11d2-9449-00105a075f6b}
#define EL_PINPAD_CLASS_GUID	\
	{ 0xff646f80, 0x8def, 0x11d2, { 0x94, 0x49, 0x00, 0x10, 0x5a, 0x07, 0x5f, 0x6b } }

//{b35924d6-3e16-4a9e-9782-5524a4b79bac
//{a8b865dd-2e3d-4094-ad97-e593a70c75d6}
#define	WINUSB_GUID	\
	{ 0xa8b865dd, 0x2e3d, 0x4094, { 0xad, 0x97, 0xe5, 0x93, 0xa7, 0x0c, 0x75, 0xd6 } }

#define KICPIN_ADevice_CLASS_GUID \
	{ 0xd33b1f4b, 0xdf5b, 0x4437, { 0x9b, 0xfd, 0xc1, 0xad, 0x86, 0x95, 0x33, 0x78 } }

//{a5dcbf10-6530-11d2-901f-00c04fb951ed}
#define ALL_USB_CLASS_GUID \
	{ 0xa5dcbf10, 0x6530, 0x11d2, { 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed } }

//ELTEK_RF_GUID = "{CD757B44-2948-4c9f-B837-A71B3C1855A6}"
#define ELTEK_RF_GUID \
	{ 0xCD757B44, 0x2948, 0x4c9f, { 0xB8, 0x37, 0xA7, 0x1B, 0x3C, 0x18, 0x55, 0xA6 } }

using namespace std;

/////////////////
// global
//GUID devguid = DEV_GUID;
GUID devguid = ELTEK_RF_GUID;
Cns_Winusb wusb;
HANDLE ghDev = NULL;

TCHAR sPath[256];

void Print_Pre()
{
	_tout << _T("=============================") << endl;
	_tout << _T("  starting test ") << endl;
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

void WaitForUser()
{
	TCHAR cIn;
	_tout << _T("Press any key.") << endl;
	_tin >> cIn;
	_tout << _T("Pressed ") << cIn << endl;
}

BOOL test_GetPath()
{
	if( !Cns_Winusb::GetPath( &devguid, sPath, 256 ) ){

		_tout << _T("ERROR : GetPath().\n") <<endl;
		return FALSE;
	}

	_tout << _T("FOUND : ") << sPath << endl;
	return TRUE;
}

BOOL test_Open()
{
	ghDev = wusb.Open( sPath );
	if( ghDev == NULL ){
		_tout << _T("ERROR : Open()") <<endl;
		return FALSE;
	}

	_tout << _T("SUCCESS : open device.") << endl;
	return TRUE;
}

BOOL test_Write_I()
{
	static int nCounter = 0;
	UCHAR sCmd[64];
	memset( sCmd, 0 , 64 );

	sCmd[0] = 'I';

	if( !wusb.Write( ghDev, 64, sCmd ) ){
		_tout << nCounter++ << _T(". ERROR : Write( I )") <<endl;
		return FALSE;
	}

	_tout << nCounter++ << _T(". SUCCESS : Write (I).") << endl;
	return TRUE;
}

BOOL test_Write_X()
{
	UCHAR sCmd[64];
	memset( sCmd, 0 , 64 );
	static int nCounter = 0;

	sCmd[0] = 'X';

	if( !wusb.Write( ghDev, 64, sCmd ) ){
		_tout << nCounter++ << _T(". ERROR : Write( X )") <<endl;
		return FALSE;
	}

	_tout << nCounter++ << _T(". SUCCESS : Write (X).") << endl;
	return TRUE;
}

BOOL test_Read()
{
	UCHAR sRx[64];
	memset( sRx, 0 , 64 );
	static int nCounter = 0;
	TCHAR cFirst;

	if( !wusb.Read( ghDev, 64, sRx ) ){
		_tout << nCounter++ << _T(". ERROR : Read( )") <<endl;
		return FALSE;
	}

	cFirst = (TCHAR)sRx[0];

	_tout << nCounter++ << _T(". SUCCESS : Read ( " ) << cFirst << _T(" ).") << endl;
	return TRUE;

}

BOOL test_Close()
{
	if( !wusb.Close( ghDev ) ){
		_tout << _T("ERROR : Close()") <<endl;
		return FALSE;
	}

	_tout << _T("SUCCESS : close device.") << endl;
	return TRUE;
}


//_tout << _T("") <<endl;
class td{
public:
	static void* operator new (size_t size); 
	static void operator delete (void *p);
	static void* operator new [] (size_t size); 
	static void operator delete [] (void *p);

public:
	td();
	td( int a );
	virtual ~td();

private:
	int m_nPrivate_a;
};


td::td() : m_nPrivate_a(0)
{
	_tout << _T("td()") <<endl;
}

td::td( int a )
{
	m_nPrivate_a = a;
	_tout << _T("td( a )") <<endl;
}

td::~td()
{
	_tout << _T("~td()") <<endl;
}

void* td::operator new (size_t size)
{
	void *p=::operator new(size);

	 _tout << _T("new ") << size << _T(" : ") << p << endl;
	return p; 
} 

void td::operator delete (void *p)
{	
	_tout << _T("delete : ") << p << endl;
	::operator delete(p); // return memory to pool
}


void* td::operator new [] (size_t size)
{
	void *p=::operator new(size);

	 _tout << _T("new [") << size << _T("]: ") << p << endl;
	return p; 
} 

void td::operator delete [] (void *p)
{	
	_tout << _T("delete []: ") << p << endl;
	::operator delete [] (p); // return memory to pool
}

class t1{
public:
	static void* operator new (size_t size)
	{	
		return td::operator new( size );
	}
	static void operator delete (void *p)
	{	
		return td::operator delete(p);
	}

	static void* operator new [] (size_t size)
	{	
		return td::operator new [] ( size );
	}

	static void operator delete [] (void *p)
	{	
		return td::operator delete [] (p);
	}

public:
	t1();
	virtual ~t1();

};

t1::t1()
{
	_tout << _T("t1()") <<endl;
}
t1::~t1()
{
	_tout << _T("~t1()") <<endl;
}

class t2 : public t1
{
public:
	t2() : m_nCounter(0)
	{
		_tout << _T("t2()") <<endl;
	}

	~t2()
	{
		_tout << _T("~t2()") <<endl;
	}

private:
	int m_nCounter;
};

/////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	
	list<char> a;

	td *pb, *pbs;
	pb = new td(3);
	delete pb;
	_tout << _T("====================") <<endl;
	//
	pbs = new td[2];
	delete [] pbs;
	_tout << _T("====================") <<endl;
	

	_tstring sCounter = _T("1");
	INT nCounter;

	/*
	t1 *pt1;
	pt1 = new t1;
	delete pt1;
	_tout << _T("====================") <<endl;

	t2 *pt2 = new t2[1];
	delete [] pt2;
	_tout << _T("====================") <<endl;

	t2 A2;
	_tout << _T("====================") <<endl;

	return 0;
	*/
	///////////////////////////////////////////////////////////////////////////
	//set repeat counter.
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


	Print_Pre();

	if( !test_GetPath() )
		return 0;
	
	if( !test_Open() )
		return 0;

	for( int i=0; i< nCounter; i++ ){
		if( !test_Write_I() )
			return 0;
		if( !test_Read() )
			return 0;

		//WaitForUser();
		//Sleep( 200 );

		if( !test_Write_X() )
			return 0;
	}//end for

	if( !test_Close() )
		return 0;

	Print_Post();

	return 0;
}


