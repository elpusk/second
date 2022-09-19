#pragma once

#include "ns_tchar.h"
#include <list>
#include <Windows.h>
#include <vector>

//the space of ns device.
namespace ns_device
{

class Cns_device
{
public:
	typedef	vector<BYTE>	bufType;

public:
	Cns_device(void);
	virtual ~Cns_device(void);

	virtual size_t GetDevList( list<_tstring> & listDevPath )=0;

	virtual HANDLE Open( const _tstring & szDevPath, bool bOverlap = false )=0;
	virtual bool Close( HANDLE hDev )=0;

	virtual size_t Write( HANDLE hDev, bufType &buffer, size_t nData, bool bOverlap = false  )=0;
	virtual size_t Read( HANDLE hDev, bufType &buffer, size_t nData, bool bOverlap = false  )=0;

private:
	Cns_device( const Cns_device & );

};

///////////////////////////
}//the end of ns_device
