#include "StdAfx.h"
#include "ns_hidusb.h"
#include <string.h>
#include <ns_Etc.h>

#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "ns_device.lib")

////{a5dcbf10-6530-11d2-901f-00c04fb951ed}
CONST GUID Cns_Hidusb::CLASS_GUID_USB = { 0xa5dcbf10, 0x6530, 0x11d2, { 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed } };
bool Cns_Hidusb::m_bNoNeedDevCap = false;

Cns_Hidusb::Cns_Hidusb(void)
{
}

Cns_Hidusb::~Cns_Hidusb(void)
{
	if( !m_mapHandleToOverlap.empty() ){

		for( mapTypeOverLap::iterator iter = m_mapHandleToOverlap.begin(); iter != m_mapHandleToOverlap.end(); ++iter ){

			if( iter->second->hEvent )
				::CloseHandle( iter->second->hEvent );

		}//end for
	}

	RemoveDevList();
}

Cns_Hidusb *Cns_Hidusb::GetInstance()
{
	static Cns_Hidusb hid;
	return &hid;
}

// BufLen : unit - TCHAR
// return DevicePath is multi-string
// return including the terminating dual null character
size_t Cns_Hidusb::GetPath( TCHAR *DevicePath, size_t BufLen)
{
	size_t nDev = 0;
	size_t nOffset =0;
	list<_tstring> listDevPath;

	GetInstance()->GetDevList( listDevPath );

	if( listDevPath.empty() )
		return nOffset;

	list<_tstring>::iterator iter = listDevPath.begin();

	for( ; iter != listDevPath.end(); ++iter ){
		nOffset += iter->size();
		nOffset++;	//add null
	}//end for

	nOffset++;//make multi string

	if( DevicePath ){

		if( BufLen < nOffset ){
			return 0;
		}
		//
		nOffset = 0;

		for( iter = listDevPath.begin(); iter != listDevPath.end(); ++iter ){
			_tcscpy( &DevicePath[nOffset], iter->c_str() );
			nOffset += iter->size();
			nOffset++;	//add null
		}//end for

		DevicePath[nOffset] = NULL;
		nOffset++;//make multi string
	}

	return nOffset;
}


// BufLen : unit - TCHAR
BOOL Cns_Hidusb::GetPath( INT nIndex, TCHAR *DevicePath, size_t BufLen)
{
	if( DevicePath == NULL )
		return FALSE;
	list<_tstring> listDevPath;

	GetInstance()->GetDevList( listDevPath );

	if( listDevPath.size() >= nIndex )
		return FALSE;
	//
	list<_tstring>::iterator iter = listDevPath.begin();

	for( int i=0; i<nIndex; i++ ){
		++iter;
	}//end for

	if( BufLen > iter->size() )
		_tcscpy(  DevicePath, iter->c_str() );
	else
		return FALSE;

	return TRUE;
}

//pHidInfos[OUT] : return found device infomation
//return the number of detected devices.
//if nVID==0 && nPID==0 , find all HID deivce 
INT Cns_Hidusb::Find( 
						 list<HIDINFO*> & listDev,
						 INT nVID/*=0*/,INT nPID/*=0*/,
						 INT nInterface/*=-1*/,
						 INT nRev/*=-1*/
						 )
{
	HIDD_ATTRIBUTES Attributes;
	DWORD DeviceUsage;
	SP_DEVICE_INTERFACE_DATA devInfoData;
	INT nMemberIndex = 0;
	BOOL lResult;
	GUID HidGuid;
	HIDP_CAPS Capabilities;
	HANDLE hHid = INVALID_HANDLE_VALUE;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetailData=NULL;
	HDEVINFO hDevInfo;
	DWORD dwLength=0;
	DWORD dwRequiredSize;
	INT nDetected=0;
	BOOL bDected = FALSE;
	INT i=0;
	INT nCurInterface=0;
	HIDINFO *pHidInfo = NULL;

	list<HIDINFO*>::iterator devitor = listDev.begin();

	Cns_Hidusb::ENUM_FindMode nMatchMode = FM_ALL;		//-1 : all devices
							//0 : matching VID.
							//1	: matching VID & PID.
							//2 : matching VID & PID & INTERFACE
							//3 : matching VID & PID & REV
							//4 : matching VID & PID & INTERFACE & REV

	/////////////////////
	//set matching mode.
	if( nVID <= FIND_ALL_DEVICES ){
		nMatchMode = FM_ALL;//-1 : all device
	}
	else if( nPID <= FIND_ALL_DEVICES ){
		nMatchMode = FM_VID;//0 : matching VID.
	}
	else{
		if( nInterface <= FIND_ALL_DEVICES ){

			if( nRev <= FIND_ALL_DEVICES ){
				nMatchMode = FM_VID_PID;//1	: matching VID & PID.
			}
			else{
				nMatchMode = FM_VID_PID_REV;//3 : matching VID & PID & REV
			}
		}
		else{

			if( nRev <= FIND_ALL_DEVICES ){
				nMatchMode = FM_VID_PID_INF;//2 : matching VID & PID & INTERFACE
			}
			else{
				nMatchMode = FM_VID_PID_INF_REV;//4 : matching VID & PID & INTERFACE & REV
			}
		}
	}

	//Get HID GUID
	HidD_GetHidGuid(&HidGuid);	
	
	//Get device information set of installed HID-Devices
	hDevInfo=SetupDiGetClassDevs( &HidGuid,NULL,NULL,DIGCF_PRESENT|DIGCF_INTERFACEDEVICE );
		
	devInfoData.cbSize = sizeof(devInfoData);

	//find wanted device.
	nMemberIndex = 0;

	for( nMemberIndex = 0 ; TRUE ;nMemberIndex++ ){
		// 1.
		lResult=SetupDiEnumDeviceInterfaces(
			hDevInfo, 0, &HidGuid, nMemberIndex, &devInfoData
			);

		if( lResult == FALSE )
			break;//exit for;//NO more device.

		// 2.
		lResult = SetupDiGetDeviceInterfaceDetail(
			hDevInfo, &devInfoData, NULL, 0, &dwLength, NULL
			);

		pDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(dwLength);
		pDetailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		lResult = SetupDiGetDeviceInterfaceDetail(
			hDevInfo, &devInfoData, pDetailData, dwLength, &dwRequiredSize, NULL
			);
		if( lResult == FALSE )
			break;//exit for

		// 3.
		hHid = CreateFile(
			pDetailData->DevicePath, 0,
			FILE_SHARE_READ|FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING, 0, NULL
			);
		if( hHid == INVALID_HANDLE_VALUE ){
			//Free the memory used by the detailData structure (no longer needed).
			free(pDetailData);
			continue;
		}

		// 4.
		Attributes.Size = sizeof(Attributes);

		if( m_bNoNeedDevCap ){
			Attributes.VendorID = GetVidOnDevicePath( pDetailData->DevicePath );
			Attributes.ProductID = GetPidOnDevicePath( pDetailData->DevicePath );
		}
		else{
			lResult = HidD_GetAttributes(hHid,&Attributes);
			if( lResult == FALSE )
				break;//exit for
		}
		
		//5.
		bDected = FALSE;

		if( Attributes.VendorID != 0 ){
			switch( nMatchMode ){
				case FM_ALL://-1 : all device
					bDected = TRUE;
					break;
				case FM_VID://0 : matching VID.
					if( Attributes.VendorID == (USHORT)nVID )
						bDected = TRUE;
					break;
				case FM_VID_PID://1	: matching VID & PID.
					if( Attributes.VendorID == (USHORT)nVID 
						&& Attributes.ProductID == (USHORT)nPID
						)
						bDected = TRUE;
					break;
				case FM_VID_PID_INF://2 : matching VID & PID & INTERFACE
					nCurInterface = GetInterfaceNumberOnDevicePath( pDetailData->DevicePath );

					if( Attributes.VendorID == (USHORT)nVID 
						&& Attributes.ProductID == (USHORT)nPID
						&& nCurInterface == nInterface
						)
						bDected = TRUE;
					break;
				case FM_VID_PID_REV://3 : matching VID & PID & REV
					if( Attributes.VendorID == (USHORT)nVID 
						&& Attributes.ProductID == (USHORT)nPID
						&& Attributes.VersionNumber == (USHORT)nRev
						)
						bDected = TRUE;
					break;
				case FM_VID_PID_INF_REV://4 : matching VID & PID & INTERFACE & REV
					nCurInterface = GetInterfaceNumberOnDevicePath( pDetailData->DevicePath );

					if( Attributes.VendorID == (USHORT)nVID 
						&& Attributes.ProductID == (USHORT)nPID
						&& Attributes.VersionNumber == (USHORT)nRev
						&& nCurInterface == nInterface
						)
						bDected = TRUE;
					break;

			}//end switch
		}

		if( bDected ){

			if( m_bNoNeedDevCap ){
				pHidInfo = new HIDINFO();
			
				if( pHidInfo ){//copy path
					_tcscpy( pHidInfo->sPath,pDetailData->DevicePath );
					::memset( &(pHidInfo->HidCaps),0,sizeof(HIDP_CAPS) );
					pHidInfo->VID=Attributes.VendorID;
					pHidInfo->PID=Attributes.ProductID;

					listDev.push_back( pHidInfo );
					nDetected++;
				}
			}
			else{
				// save detect device information.
				//Get the device's capablities.
				GetCapabilities( &Capabilities,hHid );

				// Find out if the device is a system mouse or keyboard.
				DeviceUsage = (Capabilities.UsagePage * 256) + Capabilities.Usage;

				if ((DeviceUsage != 0x102) && (DeviceUsage != 0x106)){

					pHidInfo = new HIDINFO();
					if( pHidInfo ){//copy path
						_tcscpy( pHidInfo->sPath,pDetailData->DevicePath );
						memcpy( &(pHidInfo->HidCaps),&Capabilities,sizeof(HIDP_CAPS) );
						pHidInfo->VID=Attributes.VendorID;
						pHidInfo->PID=Attributes.ProductID;

						listDev.push_back( pHidInfo );
						nDetected++;
					}
				
				}
				else{
					//current deivce is mouse or keyboard... then 
					//Windows 2000 and Windows XP don't allow applications
					//to directly request Input reports from or 
					//write Output reports to these devices.
				}
			}
		}//if( bDected )

		CloseHandle(hHid);

		//Free the memory used by the detailData structure (no longer needed).
		free(pDetailData);

	}//end for

	//Free the memory reserved for hDevInfo by SetupDiClassDevs.
	SetupDiDestroyDeviceInfoList(hDevInfo);

	return nDetected;
}

//Get interface number on HID-device path
INT Cns_Hidusb::GetInterfaceNumberOnDevicePath( LPCTSTR psDevPath )
{
	INT nInterface=0;
	TCHAR *pdest=NULL;
	INT i;
	TCHAR sInterface[5];
	LPTSTR psSrc=NULL;


	if( psDevPath==NULL )
		return -1;	//Not found interface.

	psSrc=_tcslwr( _tcsdup( psDevPath ) );

	pdest=_tcsstr( psSrc, _T("mi_") );	//mi_XX ==> xx is interface number( interface number is started from zero-base )

	if( pdest==NULL ){
		if( psSrc )
			free( psSrc );

		return -1;	//Not found interface.
	}
	//
	
	memset( sInterface,0,5*sizeof(TCHAR) );//reset buffer.
	pdest=pdest+3*sizeof(TCHAR);	//increase pointer number to interface number position.

	//get interface number string
	for( i=0; i<5; i++ ){
		if( pdest[i]<_T('0') || pdest[i]>_T('9') )
			break;//exit for
		//
		sInterface[i]=pdest[i];
	
	}//end for

	if( i==0 ){
		if( psSrc )
			free( psSrc );
		return -1;	//Not found interface.
	}

	//convert string to digit.
	nInterface=Cns_Etc::ConvertToDigit( (LPCTSTR)sInterface, 16);

	if( psSrc )
		free( psSrc );

	return nInterface;
}

//Get PID on HID-device path
INT Cns_Hidusb::GetPidOnDevicePath( LPCTSTR psDevPath )
{
	INT nInterface=0;
	TCHAR *pdest=NULL;
	INT i;
	TCHAR sPid[8];
	LPTSTR psSrc=NULL;


	if( psDevPath==NULL )
		return -1;	//Not found interface.

	psSrc=_tcslwr( _tcsdup( psDevPath ) );

	pdest=_tcsstr( psSrc, _T("pid_") );	//pid_XXXX ==> xxxx hexcimal is product ID.

	if( pdest==NULL ){
		if( psSrc )
			free( psSrc );

		return -1;	//Not found interface.
	}
	//
	
	memset( sPid,0,8*sizeof(TCHAR) );//reset buffer.
	pdest= &pdest[4];	//increase pointer number to interface number position.

	//get interface number string
	for( i=0; i<4; i++ ){
		sPid[i]=pdest[i];
	}//end for

	if( i==0 ){
		if( psSrc )
			free( psSrc );
		return -1;	//Not found interface.
	}

	//convert string to digit.
	nInterface=Cns_Etc::ConvertToDigit( (LPCTSTR)sPid, 16);

	if( psSrc )
		free( psSrc );

	return nInterface;
}

//Get VID on HID-device path
INT Cns_Hidusb::GetVidOnDevicePath( LPCTSTR psDevPath )
{
	INT nInterface=0;
	TCHAR *pdest=NULL;
	INT i;
	TCHAR sVid[8];
	LPTSTR psSrc=NULL;


	if( psDevPath==NULL )
		return 0;	//Not found interface.

	psSrc=_tcslwr( _tcsdup( psDevPath ) );

	pdest=_tcsstr( psSrc, _T("vid_") );	//vid_XXXX ==> xxxx hexcimal is vender ID.

	if( pdest==NULL ){
		if( psSrc )
			free( psSrc );

		return 0;	//Not found interface.
	}
	//
	
	memset( sVid,0,8*sizeof(TCHAR) );//reset buffer.
	pdest=&pdest[4];	//increase pointer number to interface number position.

	//get interface number string
	for( i=0; i<4; i++ ){
		sVid[i]=pdest[i];
	}//end for

	if( i==0 ){
		if( psSrc )
			free( psSrc );
		return 0;	//Not found interface.
	}

	//convert string to digit.
	nInterface=Cns_Etc::ConvertToDigit( (LPCTSTR)sVid, 16);

	if( psSrc )
		free( psSrc );

	return nInterface;
}

BOOL Cns_Hidusb::GetCapabilities( PHIDP_CAPS pCapabilities,HANDLE hDeviceHandle )
{
	//Get the Capabilities structure for the device.
	PHIDP_PREPARSED_DATA	PreparsedData;
	BOOL bResult;

	/*
	API function: HidD_GetPreparsedData
	Returns: a pointer to a buffer containing the information about the device's capabilities.
	Requires: A handle returned by CreateFile.
	There's no need to access the buffer directly,
	but HidP_GetCaps and other API functions require a pointer to the buffer.
	*/
	bResult=HidD_GetPreparsedData(hDeviceHandle,&PreparsedData);
	if( !bResult )
		return bResult;

	/*
	API function: HidP_GetCaps
	Learn the device's capabilities.
	For standard devices such as joysticks, you can find out the specific
	capabilities of the device.
	For a custom device, the software will probably know what the device is capable of,
	and the call only verifies the information.
	Requires: the pointer to the buffer returned by HidD_GetPreparsedData.
	Returns: a Capabilities structure containing the information.
	*/
	
	if( HidP_GetCaps(PreparsedData,pCapabilities)==HIDP_STATUS_SUCCESS )
		bResult=TRUE;
	else
		bResult=FALSE;

	//the capabilities data is .......
	//pCapabilities->UsagePage : Usage Page
	//pCapabilities->InputReportByteLength : Input Report Byte Length
	//pCapabilities->OutputReportByteLength : Output Report Byte Length
	//pCapabilities->FeatureReportByteLength : Feature Report Byte Length
	//pCapabilities->NumberLinkCollectionNodes : Number of Link Collection Nodes
	//pCapabilities->NumberInputButtonCaps : Number of Input Button Caps
	//pCapabilities->NumberInputValueCaps : Number of InputValue Caps
	//pCapabilities->NumberInputDataIndices : Number of InputData Indices
	//pCapabilities->NumberOutputButtonCaps : Number of Output Button Caps
	//pCapabilities->NumberOutputValueCaps : Number of Output Value Caps
	//pCapabilities->NumberOutputDataIndices : Number of Output Data Indices
	//pCapabilities->NumberFeatureButtonCaps : Number of Feature Button Caps
	//pCapabilities->NumberFeatureValueCaps : Number of Feature Value Caps
	//pCapabilities->NumberFeatureDataIndices : Number of Feature Data Indices
	
	//No need for PreparsedData any more, so free the memory it's using.
	if( !HidD_FreePreparsedData(PreparsedData) )
		bResult=FALSE;
	//
	return bResult;
}


VOID Cns_Hidusb::RemoveDevList()
{
	HIDINFO * pHidInfo = NULL;

	list<HIDINFO*>::iterator devitor = m_listHids.begin();

	/////////////////////
	//remove device list.
	for( devitor = m_listHids.begin(); devitor != m_listHids.end(); devitor++ ){

			pHidInfo = *devitor;

			if( pHidInfo )
				delete( pHidInfo );
	}//end for

	m_listHids.clear();
}

VOID Cns_Hidusb::RefreshDevList()
{
	RemoveDevList();

	Find( m_listHids );

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//override functions
//Get device list
//return list and the number of device.
size_t Cns_Hidusb::GetDevList( list<_tstring> & listDevPath )
{
	INT nDev = 0;	//Error
	_tstring sCurpath;
	HIDINFO *pHidInfo;

	//get current devie list.......
	RefreshDevList();

	listDevPath.clear();

	list<HIDINFO*>::iterator devitera=  m_listHids.begin();

	for( devitera =  m_listHids.begin(); devitera != m_listHids.end(); devitera++ ){

		pHidInfo = *devitera;

		if( pHidInfo ){

			_tstring sCurpath = pHidInfo->sPath;

			listDevPath.push_back( sCurpath );

			nDev++;
		}

	}//end for

	return nDev;
}

HANDLE Cns_Hidusb::Open( const _tstring & szDevPath, bool bOverlap/* = false*/ )
{
	HANDLE hDev = NULL;
	DWORD dwFlagsAndAttributes;

	if( bOverlap )
		dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED;
	else
		dwFlagsAndAttributes = 0;
	//
	
	hDev = CreateFile(
		szDevPath.c_str(), 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL, 
		OPEN_EXISTING, 
		dwFlagsAndAttributes, 
		NULL
		);

	if( hDev==INVALID_HANDLE_VALUE )
		return hDev;

	if( bOverlap ){

		mapTypeOverLap::iterator iter = m_mapHandleToOverlap.find( hDev );

		if( iter == m_mapHandleToOverlap.end() ){

			HANDLE hEvent = NULL;
			try{
				// new overlap structure.
				shared_ptr<OVERLAPPED> ov(new OVERLAPPED);
				memset( ov.get(), 0, sizeof OVERLAPPED );

				hEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
				if( hEvent == NULL )
					throw 0;
				ov.get()->hEvent = hEvent;
				m_mapHandleToOverlap.insert( ov_pair(hDev, ov) );
			}
			catch(...){
				if( hEvent )
					::CloseHandle( hEvent );
				//
				::CloseHandle( hDev ); 
				hDev = NULL;
				return hDev;
			}
		}
		else{
			ResetEvent( iter->second->hEvent );
		}
	}

	GetCapabilities( &m_Capabilities, hDev );

	return hDev;
}

bool Cns_Hidusb::Close( HANDLE hDev )
{
	if( hDev ){

		mapTypeOverLap::iterator iter = m_mapHandleToOverlap.find( hDev );

		if( iter != m_mapHandleToOverlap.end() ){

			if( iter->second->hEvent )
				::CloseHandle(  iter->second->hEvent  );

			m_mapHandleToOverlap.erase( iter );
		}

		if( ::CloseHandle( hDev ) )
			return true;
		else
			return false;
	}
	else
		return false;
}

size_t Cns_Hidusb::Write( HANDLE hDev, bufType &buffer, size_t nData, bool bOverlap/* = false*/  )
{
	size_t nResult = 0;
	LPOVERLAPPED pOv = NULL;

	if( bOverlap ){
		mapTypeOverLap::iterator iter = m_mapHandleToOverlap.find( hDev );

		if( iter == m_mapHandleToOverlap.end() ){
			return nResult;	//fail. Not found overlap object.......
		}
		else{
			pOv = iter->second.get();
			::ResetEvent( pOv->hEvent );
			pOv->Offset = pOv->OffsetHigh = 0;
			pOv->Internal = pOv->InternalHigh = 0;
		}
	}

	if( hDev ){

		try{
			bool bReWrite = true;
			bool bReOverlap = true;

			while( bReWrite ){
				bReWrite = false;

				if( !::WriteFile( hDev, &buffer[0], nData, (LPDWORD)&nResult, pOv ) ){

					if( bOverlap ){
						if( ::GetLastError() == ERROR_IO_PENDING ){

							while( bReOverlap ){
								bReOverlap = false;

								if( !GetOverlappedResult( hDev, pOv,(LPDWORD)&nResult, FALSE) ){

									if( ::GetLastError() == ERROR_IO_INCOMPLETE ){
										//waits. retry GetOverlappedResult.
										bReOverlap = true;
									}
									else//error
										throw 0;
								}
								else{//success
									::ResetEvent( pOv->hEvent );
									pOv->Offset += nResult;

									if( pOv->Offset < nData ){
										//retry WriteFile .......
										bReWrite = true;
									}
									else{
										//ok getting everything.......
									}
								}//<<
							}
						}
						else//error
							throw 0;
					}
					else//error
						throw 0;
				}
			}//end while
		}
		catch( ... ){
			nResult = 0;
		}
	}

	if( bOverlap ){
		::ResetEvent( pOv->hEvent );
	}
	return nResult;
}

size_t Cns_Hidusb::Read( HANDLE hDev, bufType &buffer, size_t nData, bool bOverlap/* = false*/  )
{
	size_t nResult = 0;
	LPOVERLAPPED pOv = NULL;

	if( bOverlap ){
		mapTypeOverLap::iterator iter = m_mapHandleToOverlap.find( hDev );

		if( iter == m_mapHandleToOverlap.end() ){
			return nResult;	//fail. Not found overlap object.......
		}
		else{
			pOv = iter->second.get();
			::ResetEvent( pOv->hEvent );
			pOv->Offset = pOv->OffsetHigh = 0;
			pOv->Internal = pOv->InternalHigh = 0;
		}
	}

	if( hDev ){

		try{
			bool bReRead = true;
			bool bReOverlap = true;

			while( bReRead ){
				bReRead = false;

				if( !::ReadFile( hDev, &buffer[0], nData, (LPDWORD)&nResult, pOv ) ){

					if( bOverlap ){
						if( ::GetLastError() == ERROR_IO_PENDING ){

							while( bReOverlap ){
								bReOverlap = false;

								if( !GetOverlappedResult( hDev, pOv,(LPDWORD)&nResult, FALSE) ){

									if( ::GetLastError() == ERROR_IO_INCOMPLETE ){
										//waits. retry GetOverlappedResult.
										bReOverlap = true;
									}
									else//error
										throw 0;
								}
								else{//success
									::ResetEvent( pOv->hEvent );
									pOv->Offset += nResult;

									if( pOv->Offset < nData ){
										//retry ReadFile .......
										bReRead = true;
									}
									else{
										//ok getting everything.......
									}
								}//<<
							}
						}
						else//error
							throw 0;
					}
					else//error
						throw 0;
				}
			}//end while
		}
		catch( ... ){
			nResult = 0;
		}
	}

	if( bOverlap ){
		::ResetEvent( pOv->hEvent );
	}
	return nResult;
}
