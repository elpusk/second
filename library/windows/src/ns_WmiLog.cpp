#include "StdAfx.h"
#include "ns_WmiLog.h"
#include <vector>
#include "ns_warpobject.h"


Cns_WmiLog *Cns_WmiLog::GetService( const _tstring & sLogName, const _tstring & sSrcName,const  _tstring & sMsgDLL, DWORD dwCategoryCount /*=0*/ )
{
	static Cns_WmiLog Log;
	static bool bFirst = true;

	if( bFirst ){

		if( sLogName.empty() || sSrcName.empty() || sMsgDLL.empty() )
			return NULL;

		if( Log.RegisterSource( sLogName, sSrcName, sMsgDLL,dwCategoryCount ) ){
			bFirst= false;
			Log.m_sSrcName = sSrcName;
			Log.m_sLogName = sLogName;
			return  &Log;
		}
		else{
			return NULL;
		}
	}
	else
		return &Log;
}


Cns_WmiLog::Cns_WmiLog()
{

}

Cns_WmiLog::~Cns_WmiLog()
{

}

void Cns_WmiLog::Log( ULONG nEventID, const _tstring & sTitle, const _tstring & sMsg, WORD wType/*=EVENTLOG_INFORMATION_TYPE*/, WORD wCategory/*=0*/ )
{
	vector<LPCTSTR> vStr(2);
	Cns_WarpEvtLogHandle EventSource( ::RegisterEventSource( NULL, m_sSrcName.c_str() ) );

	if( NULL != EventSource.GetHandle() )
	{
		vStr[0] = sTitle.c_str(); //title
		vStr[1] = sMsg.c_str();

		::ReportEvent(
					EventSource.GetHandle(),   // event log handle
					wType,						//EVENTLOG_ERROR_TYPE, // event type
					wCategory,                   // event category
					nEventID,			// event identifier
					NULL,                // no security identifier
					vStr.size(),                   // size of lpszStrings array
					0,                   // no binary data
					&vStr[0],         // array of strings
					NULL);               // no binary data
	}
}

bool Cns_WmiLog::RegisterSource( const _tstring & sLogName, const _tstring & sSrcName,const  _tstring & sMsgDLL, DWORD dwCategoryCount /*=0*/ )
{
	_tstring sSubKey(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\") );
	sSubKey += sLogName;
	sSubKey += _T("\\");
	sSubKey += sSrcName;	//event source name

	HKEY hKey;
	DWORD dwDisposition = 0;

	//create or open registery key
	if( ::RegCreateKeyEx( HKEY_LOCAL_MACHINE, sSubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition ) != ERROR_SUCCESS ){
		return false;
	}

	Cns_WarpRegKeyHandle RegKey( hKey );

	vector<TCHAR> v(_MAX_PATH);
	_tstring sMsg( sMsgDLL );
	sMsg.copy( &v[0], sMsg.size()+1 );

	//set the name of the event message file.
	if( ::RegSetValueEx( RegKey.GetHandle(), _T("EventMessageFile"), 0, REG_EXPAND_SZ, reinterpret_cast<LPBYTE>(&v[0]), (sMsgDLL.size()+1)*sizeof(TCHAR) ) != ERROR_SUCCESS ){
		return false;
	}
	
	// Set the supported event types.
	DWORD dwData = EVENTLOG_SUCCESS | EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE |  EVENTLOG_INFORMATION_TYPE;

	if( ::RegSetValueEx( RegKey.GetHandle(), _T("TypesSupported"), 0, REG_DWORD, (LPBYTE) &dwData, sizeof(DWORD) ) != ERROR_SUCCESS ){
		return false;
	}

	if( dwCategoryCount > 0 ){
		// Set the category message file and number of categories. Here both the categories and messages are   
		// defined in the same message dll.
		if( ::RegSetValueEx( RegKey.GetHandle(), _T("CategoryMessageFile"), 0, REG_EXPAND_SZ, reinterpret_cast<LPBYTE>(&v[0]), (sMsgDLL.size()+1)*sizeof(TCHAR) ) != ERROR_SUCCESS ){
			return false;
		}

		DWORD  dwNum = dwCategoryCount; //This is the count of categories supported
		if( ::RegSetValueEx( RegKey.GetHandle(), _T("CategoryCount"), 0, REG_DWORD, (LPBYTE) &dwNum, sizeof(DWORD) ) != ERROR_SUCCESS ){
			return false;
		}
	}

	return true;
}

bool Cns_WmiLog::UnregisterSource()
{
	_tstring sSubKey(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\") );
	sSubKey += m_sLogName;
	sSubKey += _T("\\");
	sSubKey += m_sSrcName;	//event source name

	HKEY hKey;
	DWORD dwDisposition = 0;

	//create or open registery key
	if( ::RegCreateKeyEx( HKEY_LOCAL_MACHINE, sSubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition ) ){
		return false;
	}

	Cns_WarpRegKeyHandle RegKey( hKey );

	if( ERROR_SUCCESS == ::RegDeleteKey( hKey, sSubKey.c_str() ) ){
		return true;
	}
	else
		return false;
}
