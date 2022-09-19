#pragma once

#include <windows.h>
#include "ns_tchar.h"

class Cns_Common{

public:
	static const _tstring &GetPathEvtMsg()
	{
		static _tstring sValue(_T("%ProgramFiles%\\Elpusk\\EvtMsg\\"));
		return sValue;
	}

public:
	~Cns_Common(){};

private:
	Cns_Common();
	Cns_Common( const Cns_Common & );
	Cns_Common & operator=( const Cns_Common & );
};
