#pragma once

#include <Windows.h>
#include <hidclass.h>

extern "C"
{
	#include <hidsdi.h>
}

#include <setupapi.h>
#include <dbt.h>
#include <list>
#include <map>
#include <memory>

#include "ns_device.h"

using namespace std;

class Cns_Hidusb : public ns_device::Cns_device
{

protected:
	struct HIDINFO{
		USHORT VID;//vender ID
		USHORT PID;//product ID
		TCHAR sPath[_MAX_PATH];//device path
		HIDP_CAPS HidCaps;//HID capabilities
	};

	//device fine mode..
	enum ENUM_FindMode{
		FM_ALL = -1,		//all find usb device
		FM_VID = 0,			//find matched VID device.
		FM_VID_PID,			//find matched VID & PID device.
		FM_VID_PID_INF,		//find matched VID & PID & interface device.
		FM_VID_PID_REV,		//find matched VID & PID & revision device.
		FM_VID_PID_INF_REV	//find matched VID & PID * interface & revision device.
	};

	static CONST GUID CLASS_GUID_USB;			//USB class Guid
	static CONST INT FIND_ALL_DEVICES = -1;

public:
	static CONST UCHAR C_cInvalid = 255;
	static CONST INT C_nRetry = 3;

	static Cns_Hidusb *GetInstance();

	// BufLen : unit - TCHAR
	// return DevicePath is multi-string
	// return including the terminating dual null character
	static size_t GetPath( TCHAR *DevicePath, size_t BufLen);

	// BufLen : unit - TCHAR
	static BOOL GetPath( INT nIndex, TCHAR *DevicePath, size_t BufLen);

	USHORT   GetInputReportByteLength(){		return m_Capabilities.InputReportByteLength;	}
    USHORT   GetOutputReportByteLength(){	return m_Capabilities.OutputReportByteLength;	}
    USHORT   GetFeatureReportByteLength(){	return m_Capabilities.FeatureReportByteLength;	}

	static void setNoDevCapabilities( bool bNoCap = true ){	m_bNoNeedDevCap = bNoCap;	}

	static INT GetInterfaceNumberOnDevicePath( LPCTSTR psDevPath );
	static INT GetPidOnDevicePath( LPCTSTR psDevPath );
	static INT GetVidOnDevicePath( LPCTSTR psDevPath );

	/////////////////////////
	//override functions
	size_t GetDevList( list<_tstring> & listDevPath );
	HANDLE Open( const _tstring & szDevPath, bool bOverlap = false );
	bool Close( HANDLE hDev );
	size_t Write( HANDLE hDev, bufType &buffer, size_t nData, bool bOverlap = false  );
	size_t Read( HANDLE hDev, bufType &buffer, size_t nData, bool bOverlap = false  );

public:
	virtual ~Cns_Hidusb(void);

private:
	Cns_Hidusb(void);
	Cns_Hidusb( const Cns_Hidusb & );	//remove copy construct.......

	static INT Find( 
		list<HIDINFO*> & listDev,
		INT nVID = FIND_ALL_DEVICES,
		INT nPID = FIND_ALL_DEVICES,
		INT nInterface = FIND_ALL_DEVICES,
		INT nRev = FIND_ALL_DEVICES
		);

	static BOOL GetCapabilities( PHIDP_CAPS pCapabilities,HANDLE hDeviceHandle );

	//remove device list
	VOID RemoveDevList();
	VOID RefreshDevList();

private:
	list<HIDINFO*> m_listHids;	//Hid device list
	HIDP_CAPS m_Capabilities;
	
	static bool m_bNoNeedDevCap;

private:
	typedef	map< HANDLE, shared_ptr<OVERLAPPED> >		mapTypeOverLap;
	typedef	pair< HANDLE, shared_ptr<OVERLAPPED> >		ov_pair;
	mapTypeOverLap	m_mapHandleToOverlap;
};

