#pragma once

#include <Windows.h>
#include <winusb.h>
#include <list>
#include <map>

class Cns_Winusb
{
private:

	struct WinUsbInfo
	{
		WINUSB_INTERFACE_HANDLE  InterfaceHandle;
		UCHAR Speed;
		USB_INTERFACE_DESCRIPTOR InterfaceDescriptor;
		PWINUSB_PIPE_INFORMATION pPipeInfos;
	};

public:
	static CONST UCHAR C_cInvalid = 255;
	static CONST INT C_nRetry = 3;

	// BufLen : unit - TCHAR
	// return DevicePath is multi-string
	// return including the terminating dual null character
	static size_t GetPath( LPGUID InterfaceGuid, TCHAR *DevicePath, size_t BufLen);

	// BufLen : unit - TCHAR
	static BOOL GetPath( LPGUID InterfaceGuid, INT nIndex, TCHAR *DevicePath, size_t BufLen);


public:
	Cns_Winusb(void);
	virtual ~Cns_Winusb(void);

	HANDLE Open( LPCTSTR DevicePath, BOOL bSync = TRUE );
	BOOL Close( HANDLE hDev );

	BOOL ControlTransfer( HANDLE hDev, CONST PWINUSB_SETUP_PACKET pSetupPacket, ULONG nData = 0, PUCHAR pData = NULL );

	BOOL Write( HANDLE hDev, ULONG nData, PUCHAR pData, UCHAR  PipeID = C_cInvalid );

	BOOL Read( HANDLE hDev, ULONG nData, PUCHAR pData, UCHAR  PipeID = C_cInvalid );

	BOOL Reset( HANDLE hDev, UCHAR  PipeID = C_cInvalid );

	BOOL SetPipePolicy(  HANDLE hDev, UCHAR PipeId, ULONG PolicyType, ULONG ValueLength, PVOID Value );

	BOOL GetPipePolicy(  HANDLE hDev, UCHAR PipeId, ULONG PolicyType, PULONG pValueLength, PVOID Value );

	//getter
	HANDLE GetInterfaceHandle( HANDLE hDev );
	PUSB_INTERFACE_DESCRIPTOR GetInterfaceDescriptor( HANDLE hDev );
	UCHAR GetDeviceSpeed( HANDLE hDev );
	PWINUSB_PIPE_INFORMATION GetPipeInfo(  HANDLE hDev , INT nPipe );

	//
	inline BOOL IsInPipeType( CONST PWINUSB_PIPE_INFORMATION pPipeInfo )
	{
		return ( USB_ENDPOINT_DIRECTION_IN(pPipeInfo->PipeId) );
	}
	inline BOOL IsOutPipeType( CONST PWINUSB_PIPE_INFORMATION pPipeInfo )
	{
		return ( USB_ENDPOINT_DIRECTION_OUT(pPipeInfo->PipeId) );
	}

	inline BOOL IsBulkInPipeType( CONST PWINUSB_PIPE_INFORMATION pPipeInfo )
	{
		return ( pPipeInfo->PipeType == UsbdPipeTypeBulk && USB_ENDPOINT_DIRECTION_IN(pPipeInfo->PipeId) );
	}

	inline BOOL IsBulkOutPipeType( CONST PWINUSB_PIPE_INFORMATION pPipeInfo )
	{
		return ( pPipeInfo->PipeType == UsbdPipeTypeBulk && USB_ENDPOINT_DIRECTION_OUT(pPipeInfo->PipeId) );
	}

	inline BOOL IsInterruptPipeType( CONST PWINUSB_PIPE_INFORMATION pPipeInfo )
	{
		return ( pPipeInfo->PipeType == UsbdPipeTypeInterrupt );
	}

	inline BOOL IsInterruptInPipeType( CONST PWINUSB_PIPE_INFORMATION pPipeInfo )
	{
		return ( pPipeInfo->PipeType == UsbdPipeTypeInterrupt && USB_ENDPOINT_DIRECTION_IN(pPipeInfo->PipeId) );
	}

	inline BOOL IsInterruptOutPipeType( CONST PWINUSB_PIPE_INFORMATION pPipeInfo )
	{
		return ( pPipeInfo->PipeType == UsbdPipeTypeInterrupt && USB_ENDPOINT_DIRECTION_OUT(pPipeInfo->PipeId) );
	}

private:

	std::map<HANDLE, WinUsbInfo*> m_MapDev;

};