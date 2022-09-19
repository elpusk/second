#include "StdAfx.h"
#include "ns_winusb.h"
#include <usb100.h>
#include <setupapi.h>
#include <Strsafe.h>
#include <algorithm>

#pragma comment(lib, "winusb.lib")
#pragma comment(lib, "setupapi.lib")

Cns_Winusb::Cns_Winusb(void) 
{
}

Cns_Winusb::~Cns_Winusb(void)
{
	try{

		// delete all item.
		std::map<HANDLE, WinUsbInfo*>::iterator iterMap;

		for( iterMap = m_MapDev.begin(); iterMap != m_MapDev.end(); iterMap++ ){

			if( iterMap->second->pPipeInfos )
				delete [] iterMap->second->pPipeInfos;

			if( iterMap->second->InterfaceHandle ){
				WinUsb_Free( iterMap->second->InterfaceHandle );
			}

			delete( iterMap->second );

			CloseHandle( iterMap->first );
		}//end for

	}
	catch(...){

	}
	//
}

// BufLen : unit - TCHAR
size_t Cns_Winusb::GetPath( LPGUID InterfaceGuid, TCHAR *DevicePath, size_t BufLen)
{
	BOOL bIng = TRUE;
	int  nIndex = 0;
	size_t nPathBuffer =BufLen ;
	TCHAR *psPath = DevicePath;
	size_t nPath;
	size_t nOffset =0;
	HRESULT hr;

	do{

		bIng = GetPath( InterfaceGuid, nIndex, &(psPath[nOffset]), nPathBuffer );

		if( bIng ){
			//success detects device.
			nIndex++;

			//nPath : not including the terminating null character
			hr = StringCchLength( &(psPath[nOffset]), nPathBuffer, &nPath );
			if(FAILED(hr))	{
				return 0;
			}

			nPathBuffer -= (nPath+1);
			nOffset += (nPath+1);
		}
	}while( bIng );

	if( nIndex > 0 ){
		psPath[nOffset] = NULL;	//make multi string
		return (nOffset+1);
	}
	else{
		return 0;
	}
}

// BufLen : unit - TCHAR
BOOL Cns_Winusb::GetPath( LPGUID InterfaceGuid, INT nIndex, TCHAR *DevicePath, size_t BufLen)
{
	BOOL bResult = FALSE;
	HDEVINFO deviceInfo;
	SP_DEVICE_INTERFACE_DATA interfaceData;
	ULONG length;
	ULONG requiredLength=0;
	HRESULT hr;
	DWORD dwResult;

	if( nIndex< 0 )
		return FALSE;//index value greater then equal zero.

	// [1]
	deviceInfo = SetupDiGetClassDevs(InterfaceGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if( INVALID_HANDLE_VALUE == deviceInfo )
		return FALSE;

	// [2]
	interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	bResult = SetupDiEnumDeviceInterfaces(deviceInfo, NULL, InterfaceGuid,	nIndex, &interfaceData);
	if( !bResult ){
		dwResult = GetLastError();
		SetupDiDestroyDeviceInfoList( deviceInfo );
		return FALSE;
	}

	// [3]
	SetupDiGetDeviceInterfaceDetail(deviceInfo,	&interfaceData,	NULL, 0, &requiredLength, NULL);

	PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, requiredLength);
	if(NULL == detailData){
		SetupDiDestroyDeviceInfoList(deviceInfo);
		return FALSE;
	}

	detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	length = requiredLength;
	bResult = SetupDiGetDeviceInterfaceDetail( deviceInfo,&interfaceData,detailData,length,&requiredLength,NULL );
	if( bResult ){
		// [4]
		hr = StringCchCopy(DevicePath, BufLen, detailData->DevicePath);
		if(FAILED(hr))	{
			bResult = FALSE;
		}
	}

	SetupDiDestroyDeviceInfoList(deviceInfo);
	LocalFree(detailData);
	return bResult;
}


HANDLE Cns_Winusb::Open(  LPCTSTR DevicePath, BOOL bSync /*= TRUE*/ )
{
	HANDLE hDev = NULL;
	BOOL bResult;
	ULONG length;

	if( DevicePath == NULL )
		return FALSE;

	hDev = CreateFile( 
		DevicePath,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,	
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL
		);
	if( INVALID_HANDLE_VALUE != hDev ){

		if( m_MapDev.find( hDev ) != m_MapDev.end() ){
			return hDev;	//already 
		}

		WINUSB_INTERFACE_HANDLE  InterfaceHandle;
		UCHAR Speed;
		USB_INTERFACE_DESCRIPTOR InterfaceDescriptor;
		PWINUSB_PIPE_INFORMATION pPipeInfos = NULL;

		if( WinUsb_Initialize( hDev, &InterfaceHandle ) ){

			length = sizeof(UCHAR);
			bResult = WinUsb_QueryDeviceInformation( InterfaceHandle, DEVICE_SPEED, &length, &Speed );

			if( bResult ){
				bResult = WinUsb_QueryInterfaceSettings( InterfaceHandle, 0, &InterfaceDescriptor);
				if( bResult ){

					pPipeInfos = new WINUSB_PIPE_INFORMATION[InterfaceDescriptor.bNumEndpoints];
					_ASSERT( pPipeInfos != NULL );

					for( int i=0; i < InterfaceDescriptor.bNumEndpoints; i++){
						bResult = WinUsb_QueryPipe( InterfaceHandle, 0, (UCHAR) i, &pPipeInfos[i]);

						if( !bResult ){
							break;//exit for
						}
					}//end for
				}
			}

			if( bResult ){

				WinUsbInfo* pInfo = new WinUsbInfo;
				_ASSERT( pInfo != NULL );
				memcpy( &pInfo->InterfaceDescriptor, &InterfaceDescriptor, sizeof( InterfaceDescriptor ) );
				pInfo->InterfaceHandle = InterfaceHandle;
				pInfo->pPipeInfos = pPipeInfos;
				pInfo->Speed = Speed;

				m_MapDev[hDev] = pInfo;
			}
			else{
				CloseHandle( hDev );
				hDev = NULL;

				delete [] pPipeInfos;
			}
		}
		else{
			CloseHandle( hDev );
			hDev = NULL;
		}
	}
	
	return hDev;
}

BOOL Cns_Winusb::Close(  HANDLE hDev  )
{
	if( hDev == NULL )
		return FALSE;

	try{

		std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

		if( iterMap != m_MapDev.end() ){
			if( iterMap->second->pPipeInfos ){
				delete [] iterMap->second->pPipeInfos;
			}

			if( iterMap->second->InterfaceHandle ){
				WinUsb_Free( iterMap->second->InterfaceHandle );
			}

			delete iterMap->second;

			CloseHandle( hDev );
			m_MapDev.erase( hDev );
		}
		else
			return FALSE;
	}
	catch(...){
		return FALSE;
	}

	return TRUE;

}

BOOL Cns_Winusb::ControlTransfer(  HANDLE hDev, CONST PWINUSB_SETUP_PACKET pSetupPacket, ULONG nData /*=0*/, PUCHAR pData /*=NULL*/)
{
	if( pSetupPacket == NULL )
		return FALSE;
	//
	std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

	if( iterMap == m_MapDev.end() )
		return FALSE;
	//

	ULONG bytesReturned;
	BOOL bResult = WinUsb_ControlTransfer( iterMap->second->InterfaceHandle, *pSetupPacket, pData, nData, &bytesReturned, NULL);

	return bResult;
}

BOOL Cns_Winusb::Write(  HANDLE hDev, ULONG nData, PUCHAR pData, UCHAR  PipeID /*= C_cInvalid*/ )
{
	UCHAR cId = C_cInvalid;

	std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

	if( iterMap == m_MapDev.end() )
		return FALSE;

	if( PipeID == C_cInvalid ){

		for( int i = 0; i< iterMap->second->InterfaceDescriptor.bNumEndpoints; i++ ){

			if( IsBulkOutPipeType( &iterMap->second->pPipeInfos[i] ) ){
				cId = iterMap->second->pPipeInfos[i].PipeId;
				break;
			}

		}//end for
	}
	else{
		cId = PipeID;
	}

	if( cId == C_cInvalid )
		return FALSE;
	//
	ULONG bytesWritten;
	DWORD dwResult;
	INT nRetry = C_nRetry;

	while( !WinUsb_WritePipe( iterMap->second->InterfaceHandle, cId, pData, nData, &bytesWritten, NULL ) ){
		
		 dwResult = GetLastError();

		nRetry--;
		if( nRetry <= 0 ){
			return FALSE;
		}

		//retry
		Reset( hDev,cId );	//reset port
	}

	return TRUE;
}

BOOL Cns_Winusb::Read(  HANDLE hDev, ULONG nData, PUCHAR pData, UCHAR  PipeID /*= C_cInvalid*/ )
{
	UCHAR cId = C_cInvalid;

	std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

	if( iterMap == m_MapDev.end() )
		return FALSE;

	if( PipeID == C_cInvalid ){

		for( int i = 0; i< iterMap->second->InterfaceDescriptor.bNumEndpoints; i++ ){

			if( IsBulkInPipeType( &iterMap->second->pPipeInfos[i] ) ){
				cId = iterMap->second->pPipeInfos[i].PipeId;
				break;
			}

		}//end for
	}
	else{
		cId = PipeID;
	}

	if( cId == C_cInvalid )
		return FALSE;
	//
	ULONG bytesRead;
	DWORD dwResult;
	INT nRetry = C_nRetry;

	while( !WinUsb_ReadPipe( iterMap->second->InterfaceHandle, cId, pData, nData, &bytesRead, NULL ) ){
		 dwResult = GetLastError();

		nRetry--;
		if( nRetry <= 0 ){
			return FALSE;
		}

		//retry
		Reset( hDev, cId );	//reset port
	}//end while

	return TRUE;
}

BOOL Cns_Winusb::Reset(  HANDLE hDev, UCHAR  PipeID /*= C_cInvalid*/ )
{
	UCHAR cId = C_cInvalid;
	UCHAR cOutId = C_cInvalid;
	UCHAR cInId = C_cInvalid;

	std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

	if( iterMap == m_MapDev.end() )
		return FALSE;

	if( PipeID == C_cInvalid ){

		for( int i = 0; i< iterMap->second->InterfaceDescriptor.bNumEndpoints; i++ ){

			if( IsBulkOutPipeType( &iterMap->second->pPipeInfos[i] ) ){

				if( cOutId == C_cInvalid )
					cOutId = iterMap->second->pPipeInfos[i].PipeId;
			}

			if( IsBulkInPipeType( &iterMap->second->pPipeInfos[i] ) ){
				if( cInId == C_cInvalid )
					cInId = iterMap->second->pPipeInfos[i].PipeId;
			}

		}//end for
	}
	else{
		cId = PipeID;
	}

	if( cId == C_cInvalid && cOutId == C_cInvalid && cInId == C_cInvalid )
		return FALSE;

	if( cId != C_cInvalid )
		return WinUsb_ResetPipe( iterMap->second->InterfaceHandle, cId );

	if( cOutId != C_cInvalid ){
		if( !WinUsb_ResetPipe( iterMap->second->InterfaceHandle, cOutId ) )
			return FALSE;
	}
	if( cInId != C_cInvalid ){
		return WinUsb_ResetPipe( iterMap->second->InterfaceHandle, cInId );
	}

	return TRUE;
}

BOOL Cns_Winusb::SetPipePolicy(  HANDLE hDev, UCHAR PipeId, ULONG PolicyType, ULONG ValueLength, PVOID Value )
{
	std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

	if( iterMap == m_MapDev.end() )
		return FALSE;

	return WinUsb_SetPipePolicy( iterMap->second->InterfaceHandle, PipeId, PolicyType, ValueLength, Value );
}

BOOL Cns_Winusb::GetPipePolicy(  HANDLE hDev, UCHAR PipeId, ULONG PolicyType, PULONG pValueLength, PVOID Value )
{
	std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

	if( iterMap == m_MapDev.end() )
		return FALSE;

	return WinUsb_GetPipePolicy( iterMap->second->InterfaceHandle, PipeId, PolicyType, pValueLength, Value );
}

HANDLE Cns_Winusb::GetInterfaceHandle( HANDLE hDev )
{	
	std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

	if( iterMap == m_MapDev.end() )
		return NULL;

	return iterMap->second->InterfaceHandle;
}

PUSB_INTERFACE_DESCRIPTOR Cns_Winusb::GetInterfaceDescriptor( HANDLE hDev )
{
	std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

	if( iterMap == m_MapDev.end() )
		return NULL;

	return &iterMap->second->InterfaceDescriptor;
}

UCHAR Cns_Winusb::GetDeviceSpeed( HANDLE hDev )
{
	std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

	if( iterMap == m_MapDev.end() )
		return 0;

	return iterMap->second->Speed;	
}

PWINUSB_PIPE_INFORMATION Cns_Winusb::GetPipeInfo(  HANDLE hDev , INT nPipe )
{
	std::map<HANDLE, WinUsbInfo*>::iterator iterMap = m_MapDev.find( hDev );

	if( iterMap == m_MapDev.end() )
		return 0;

	if( nPipe<0 )
		return NULL;

	if( nPipe >= iterMap->second->InterfaceDescriptor.bNumEndpoints )
		return NULL;
	//

	if( iterMap->second->pPipeInfos == NULL )
		return NULL;

	return &iterMap->second->pPipeInfos[nPipe];
}
