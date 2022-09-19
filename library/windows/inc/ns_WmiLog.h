#pragma once

#include <Windows.h>
#include "ns_tchar.h"

class Cns_WmiLog
{
public:
	static Cns_WmiLog *GetService( const _tstring & sLogName, const _tstring & sSrcName,const  _tstring & sMsgDLL,DWORD dwCategoryCount = 0 );
	~Cns_WmiLog();
	bool UnregisterSource();

public:
	void Log( ULONG nEventID,  const _tstring & sTitle, const _tstring & sMsg, WORD wType=EVENTLOG_INFORMATION_TYPE, WORD wCategory=0 );

	inline void LogSuccess( ULONG nEventID, const _tstring & sTitle, const _tstring & sMsg, WORD wCategory=0 )
	{	Log( nEventID, sTitle, sMsg, EVENTLOG_SUCCESS );	}

	inline void LogError( ULONG nEventID, const _tstring & sTitle, const _tstring & sMsg, WORD wCategory=0 )
	{	Log( nEventID, sTitle, sMsg, EVENTLOG_ERROR_TYPE );	}

	inline void LogInfo( ULONG nEventID, const _tstring & sTitle, const _tstring & sMsg, WORD wCategory=0 )
	{	Log( nEventID, sTitle, sMsg, EVENTLOG_INFORMATION_TYPE );	}

	inline void LogWarning( ULONG nEventID, const _tstring & sTitle, const _tstring & sMsg, WORD wCategory=0 )
	{	Log( nEventID, sTitle, sMsg, EVENTLOG_WARNING_TYPE );	}

private:
	Cns_WmiLog();

	bool RegisterSource( const _tstring & sLogName, const _tstring & sSrcName,const  _tstring & sMsgDLL, DWORD dwCategoryCount = 0 );
	

	_tstring m_sSrcName;
	_tstring m_sLogName;

private://don't call these method
	Cns_WmiLog(const Cns_WmiLog &);
	Cns_WmiLog & operator-(const Cns_WmiLog &);
};

