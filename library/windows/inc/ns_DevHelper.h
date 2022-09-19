#pragma once

#include <Windows.h>
#include <list>
#include "ns_tchar.h"
#include "vector"

class Cns_DevHelper
{
public:

#pragma pack(push,1)

	typedef struct Tag_ng_DEVH_Guid{
		GUID Guid;
		HDEVNOTIFY hDevNotify;
	}ng_DEVH_Guid, *Png_DEVH_Guid, *LPng_DEVH_Guid;

	typedef struct Tag_ng_DEVH_UsbId{
		WORD wVid;		//usb VID
		INT	 nPid;		//usb PID : negative value : no consideration.
		INT	nInf;		//usb interface : negative value : no consideration.
		INT	nRev;		//usb revision : negative value : no consideration.
		VOID *pGeneric;	//generic using pointer
	}ng_DEVH_UsbId, *Png_DEVH_UsbId, *LPng_DEVH_UsbId;

#pragma pack(pop)

	enum ENUM_DevEvent{
		DE_PLUGIN_WANTED_DEV					= 1,
		DE_PLUGOUT_WANTED_DEV,
		DE_PLUGOUT_WANTED_HANDLE,
		DE_PLUGIN_THE_OTHER_DEV,
		DE_PLUGOUT_THE_OTHER_DEV,
		DE_WILL_BE_PLUGOUT_WANTED_DEV,
		DE_WILL_BE_PLUGOUT_THE_OTHER_DEV,
		DE_NO_NEED_EVENT						= 100
	};

public:
	//this function is called on WM_DEVICECHANGE message handler
	static ENUM_DevEvent OnDeviceChangeMsgHandler(
		UINT nEventType,
		DWORD dwData,
		CONST GUID & ClassGuid,
		INT nVID,
		INT nPID,
		INT nRev,
		INT nInf
		);


	static ENUM_DevEvent OnDeviceChangeMsgHandler( 
		UINT nEventType,
		DWORD dwData,
		INT nDev,
		CONST GUID & ClassGuid,
		CONST INT *pnVID,
		CONST INT *pnPID =NULL,
		CONST INT *pnRev =NULL,
		CONST INT *pnInf =NULL
		);

	static ENUM_DevEvent OnDeviceChangeMsgHandler(
		UINT nEventType,
		DWORD dwData,
		std::vector<ng_DEVH_UsbId> & vectorUsbId,
		std::vector<HANDLE> & vectorhDev,
		INT & nIndexVectorhDev,
		_tstring & sInsertedDevName
		);

	//register user device for device-change-notification
	static HDEVNOTIFY RegisterEventDevice( HANDLE hTargetWnd,CONST GUID & InterfaceClassGuid );

	//register user device-handle for device-change-notification
	static HDEVNOTIFY RegisterEventDevice( HANDLE hTargetWnd,HANDLE hDev );

	//unregister device of RegisterEventDevice()
	static BOOL UnRegisterEventDevice( HDEVNOTIFY hNotify );

	//checks whether the given VID & PID exsit in the given device path 
	// if nPID<0 : No consider PID number.
	// if nInf<0 : No consider interface number.
	// if nRev<0 : No consider revision number.
	static BOOL IsWantUSBDevice(
		PTCHAR psDevicePath,
		INT nVID,
		INT nPID = -1,
		INT nInf = -1,
		INT nRev = -1
 		);

	static BOOL ParsingUSBDevicePath(
		PTCHAR pDevicePath,
		PTCHAR psVID,
		PTCHAR psPID = NULL,
		PTCHAR psInf = NULL,
		PTCHAR psRev = NULL
		);

public:
	Cns_DevHelper(void){}
	virtual ~Cns_DevHelper(void){}

private:

	enum WantUSBDeviceMode{
		ModeVidPid,
		ModeVidPidInf,
		ModeVidPidRev,
		ModeVidPidInfRev
	};
};

