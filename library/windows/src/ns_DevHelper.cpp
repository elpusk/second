#include "StdAfx.h"
#include "ns_DevHelper.h"
#include "ns_tchar.h"
#include <stdio.h>
#include <Dbt.h>

#pragma comment(lib, "Hid.lib")

//this function is called on WM_DEVICECHANGE message handler
Cns_DevHelper::ENUM_DevEvent Cns_DevHelper::OnDeviceChangeMsgHandler(
	UINT nEventType,
	DWORD dwData,
	CONST GUID & ClassGuid,
	INT nVID,
	INT nPID/*=1*/,
	INT nRev/*=1*/,
	INT nInf/*=1*/
	)
{
	PDEV_BROADCAST_HDR broadcastHdr;
	PDEV_BROADCAST_DEVICEINTERFACE broadcastInterface;
	ENUM_DevEvent nReturn=DE_NO_NEED_EVENT;

	switch(nEventType){
		case DBT_DEVICEARRIVAL:
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;

			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ) {

				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;

				if( broadcastInterface->dbcc_classguid==ClassGuid ){

					if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),nVID,nPID,nInf,nRev ) ){
						//"Plugin pinpad device"
						nReturn=DE_PLUGIN_WANTED_DEV;
					}
					else//"USB device(except pinpad)Plugin"
						nReturn=DE_PLUGIN_THE_OTHER_DEV;
				}
				else{
					if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),nVID,nPID,nInf,nRev ) ){
						//"Plugin pinpad device"
						nReturn=DE_PLUGIN_WANTED_DEV;
					}
					else//"USB device(except pinpad)Plugin"
						nReturn=DE_PLUGIN_THE_OTHER_DEV;
				}
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:

			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;

			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ){

				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;

				if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),nVID,nPID,nInf,nRev ) ){
					//"Plugout pinpad device"
					nReturn=DE_PLUGOUT_WANTED_DEV;
				}
				else//"USB device(except pinpad)Plugout"
					nReturn=DE_PLUGOUT_THE_OTHER_DEV;
			}
			break;
		case DBT_DEVICEQUERYREMOVE:break;//Permission to remove a device is requested. Any application can deny this request and cancel the removal.
		case DBT_DEVICEQUERYREMOVEFAILED:break;//Request to remove a device has been canceled.
		case DBT_DEVICEREMOVEPENDING:break;//Device is about to be removed. Cannot be denied.
		case DBT_DEVICETYPESPECIFIC:break;//Device-specific event.
		case DBT_CONFIGCHANGED:break;//Current configuration has changed.
		case DBT_DEVNODES_CHANGED:break;//Device node has changed. 
		default:break;
	}//end switch

	return nReturn;
}



Cns_DevHelper::ENUM_DevEvent Cns_DevHelper::OnDeviceChangeMsgHandler( 
	UINT nEventType,
	DWORD dwData,
	INT nDev,
	CONST GUID & ClassGuid,
	CONST INT *pnVID,
	CONST INT *pnPID /*=NULL*/,
	CONST INT *pnRev /*=NULL*/,
	CONST INT *pnInf/*=NULL*/
	)
{
	PDEV_BROADCAST_HDR broadcastHdr;
	PDEV_BROADCAST_DEVICEINTERFACE broadcastInterface;
	ENUM_DevEvent nReturn=DE_NO_NEED_EVENT;
	INT nVID, nPID, nRev, nInf;
	INT i;

	if( pnVID==NULL )
		return nReturn;

	switch(nEventType){
		case DBT_DEVICEARRIVAL:
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;

			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ) {

				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;

				if( broadcastInterface->dbcc_classguid==ClassGuid ){


					nReturn=DE_PLUGIN_THE_OTHER_DEV;

					for( i=0; i<nDev; i++ ){

						nVID = pnVID[i];
						//
						if( pnPID )	nPID = pnPID[i];
						else		nPID = -1;
						//
						if( pnRev )	nRev = pnRev[i];
						else		nRev = -1;
						//
						if( pnInf )	nInf = pnInf[i];
						else		nInf = -1;

						//
						if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),nVID,nPID,nInf,nRev ) ){
							nReturn=DE_PLUGIN_WANTED_DEV;
							break;//exit for
						}

					}//end for
						
				}
				else{

					nReturn=DE_PLUGIN_THE_OTHER_DEV;

					for( i=0; i<nDev; i++ ){

						nVID = pnVID[i];
						//
						if( pnPID )	nPID = pnPID[i];
						else		nPID = -1;
						//
						if( pnRev )	nRev = pnRev[i];
						else		nRev = -1;
						//
						if( pnInf )	nInf = pnInf[i];
						else		nInf = -1;

						//
						if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),nVID,nPID,nInf,nRev ) ){
							nReturn=DE_PLUGIN_WANTED_DEV;
							break;//exit for
						}

					}//end for
				}
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:

			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;

			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ){

				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;


				nReturn=DE_PLUGOUT_THE_OTHER_DEV;

				for( i=0; i<nDev; i++ ){
					nVID = pnVID[i];
					//
					if( pnPID )	nPID = pnPID[i];
					else		nPID = -1;
					//
					if( pnRev )	nRev = pnRev[i];
					else		nRev = -1;
					//
					if( pnInf )	nInf = pnInf[i];
					else		nInf = -1;

					if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),nVID,nPID,nInf,nRev ) ){
						nReturn=DE_PLUGOUT_WANTED_DEV;
						break;//exit for
					}

				}//end for
					
			}
			break;
		case DBT_DEVICEQUERYREMOVE:break;//Permission to remove a device is requested. Any application can deny this request and cancel the removal.
		case DBT_DEVICEQUERYREMOVEFAILED:break;//Request to remove a device has been canceled.
		case DBT_DEVICEREMOVEPENDING:break;//Device is about to be removed. Cannot be denied.
		case DBT_DEVICETYPESPECIFIC:break;//Device-specific event.
		case DBT_CONFIGCHANGED:break;//Current configuration has changed.
		case DBT_DEVNODES_CHANGED:break;//Device node has changed. 
		default:break;
	}//end switch

	return nReturn;
}


Cns_DevHelper::ENUM_DevEvent Cns_DevHelper::OnDeviceChangeMsgHandler(
	UINT nEventType,
	DWORD dwData,
	std::vector<ng_DEVH_UsbId> & vectorUsbId,
	std::vector<HANDLE> & vectorhDev,
	INT & nIndexVectorhDev,
	_tstring & sInsertedDevName
	)
{
	PDEV_BROADCAST_HDR broadcastHdr;
	PDEV_BROADCAST_DEVICEINTERFACE broadcastInterface;
	PDEV_BROADCAST_HANDLE broadcastHandle;
	ENUM_DevEvent nReturn=DE_NO_NEED_EVENT;

	std::vector<ng_DEVH_UsbId>::iterator iterUsbId = vectorUsbId.begin();
	std::vector<HANDLE>::iterator iterhDev = vectorhDev.begin();

	switch(nEventType){
		case DBT_DEVICEARRIVAL:
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;

			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ) {

				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;

				nReturn=DE_PLUGIN_THE_OTHER_DEV;

				 int i =0;
				for( iterUsbId = vectorUsbId.begin(); iterUsbId != vectorUsbId.end(); iterUsbId++, i++ ){

					if( IsWantUSBDevice( 
						(LPTSTR)(broadcastInterface->dbcc_name),
						iterUsbId->wVid,iterUsbId->nPid,
						iterUsbId->nInf,	iterUsbId->nRev
						) ){
						nReturn=DE_PLUGIN_WANTED_DEV;
						nIndexVectorhDev = i;
						sInsertedDevName = broadcastInterface->dbcc_name;
						break;//exit for
					}

				}//end for
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			nIndexVectorhDev =-1;
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;

			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ){

				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;

				nReturn=DE_PLUGOUT_THE_OTHER_DEV;

				for( iterUsbId = vectorUsbId.begin(); iterUsbId != vectorUsbId.end(); iterUsbId++ ){

					if( IsWantUSBDevice( 
						(LPTSTR)(broadcastInterface->dbcc_name),
						iterUsbId->wVid,iterUsbId->nPid,
						iterUsbId->nInf,	iterUsbId->nRev
						) ){
						nReturn=DE_PLUGOUT_WANTED_DEV;
						break;//exit for
					}
				}//end for
			}
			else if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_HANDLE ){
				broadcastHandle = (PDEV_BROADCAST_HANDLE) dwData;

				nReturn=DE_PLUGOUT_THE_OTHER_DEV;
				 int i =0;
				for( iterhDev = vectorhDev.begin(); iterhDev != vectorhDev.end(); iterhDev++, i++ ){

					if( *iterhDev == broadcastHandle->dbch_handle ){
						nReturn=DE_PLUGOUT_WANTED_HANDLE;
						nIndexVectorhDev = i;
						break;
					}
				}//end for
			}
			break;
		case DBT_DEVICEQUERYREMOVE:break;//Permission to remove a device is requested. Any application can deny this request and cancel the removal.
		case DBT_DEVICEQUERYREMOVEFAILED:break;//Request to remove a device has been canceled.
		case DBT_DEVICEREMOVEPENDING:break;//Device is about to be removed. Cannot be denied.
		case DBT_DEVICETYPESPECIFIC:break;//Device-specific event.
		case DBT_CONFIGCHANGED:break;//Current configuration has changed.
		case DBT_DEVNODES_CHANGED:break;//Device node has changed. 
		default:break;
	}//end switch

	return nReturn;
}

//checks whether the given VID & PID exsit in the given device path 
// if nPID<0 : No consider PID number.
// if nInf<0 : No consider interface number.
// if nRev<0 : No consider revision number.
BOOL Cns_DevHelper::IsWantUSBDevice(
									 PTCHAR psDevicePath,
									 INT nVID,
									 INT nPID /*= -1*/,
									 INT nInf/*= -1*/,
									 INT nRev/*= -1*/
									 )


{
	TCHAR sVID[8];
	TCHAR sPID[8];
	TCHAR sInf[8];
	TCHAR sRev[8];
	PTCHAR psID;
	PTCHAR pssID[4] = { NULL, NULL, NULL, NULL };
	WORD wID[4] = { 0,0,0,0 };//wID[0] - PID, wID[1] - VID, wID[2] - REV or INF, wID[3] -  REV or INF.
	BYTE *pcID;
	BYTE cItem;
	INT32 i,j;
	TCHAR sDevPath[256];
	BOOL bResult = FALSE;
	enum WantUSBDeviceMode nMode = ModeVidPid;
	INT32 nLen;

	pssID[0] = sVID;	pssID[1] = sPID;

	_tcscpy( sDevPath,psDevicePath );

	//Get VID, PID from Path-string.
	if( nRev<0 ){
		if( nInf<0 ){
			bResult = ParsingUSBDevicePath( sDevPath,(PTCHAR)sVID,(PTCHAR)sPID,NULL,NULL );

			if( bResult ){
					//convert lower case
					_tcslwr( (PTCHAR)sVID );	_tcslwr( (PTCHAR)sPID );
					nMode = ModeVidPid;
			}
		}
		else{
			bResult = ParsingUSBDevicePath( sDevPath,(PTCHAR)sVID,(PTCHAR)sPID,(PTCHAR)sInf,NULL );
			if( bResult ){
					//convert lower case
					_tcslwr( (PTCHAR)sVID );	_tcslwr( (PTCHAR)sPID );	_tcslwr( (PTCHAR)sInf );
					nMode = ModeVidPidInf;
					pssID[2] = sInf;
			}
		}
	}
	else{
		if( nInf<0 ){
			bResult = ParsingUSBDevicePath( sDevPath,(PTCHAR)sVID,(PTCHAR)sPID, NULL, (PTCHAR)sRev );
			if( bResult ){
					//convert lower case
					_tcslwr( (PTCHAR)sVID );	_tcslwr( (PTCHAR)sPID );	_tcslwr( (PTCHAR)sRev );
					nMode = ModeVidPidRev;
					pssID[2] = sRev;
			}

		}
		else{
			bResult = ParsingUSBDevicePath( sDevPath,(PTCHAR)sVID,(PTCHAR)sPID,(PTCHAR)sInf, (PTCHAR)sRev );
			if( bResult ){
					//convert lower case
					_tcslwr( (PTCHAR)sVID );	_tcslwr( (PTCHAR)sPID );	_tcslwr( (PTCHAR)sInf );	_tcslwr( (PTCHAR)sRev );
					nMode = ModeVidPidInfRev;
					pssID[3] = sRev;	pssID[2] = sInf;
			}

		}
	}

	
	if( !bResult )
		return FALSE;

	
	//convert VID and PID string type to VID and PID word type.
	for( j=0; j<4; j++ ){

		pcID = (LPBYTE)(&(wID[j]));
		pcID++;
		psID = pssID[j];

		nLen = _tcsclen( psID );

		for( i=0; i<nLen; i++ ){
			//high nibble
			if( psID[i]>=_T('0') && psID[i]<=_T('9') ){
				cItem=(BYTE)( psID[i] - _T('0') );
				cItem=cItem<<4;
			}
			else if( psID[i]>=_T('a') && psID[i]<=_T('f') ){
				cItem=(BYTE)( psID[i] - _T('a') + 0x0A);
				cItem=cItem<<4;
			}
			else
				return FALSE;

			i++;
			//low nibble
			if( psID[i]>=_T('0') && psID[i]<=_T('9') ){
				cItem+=(BYTE)( psID[i] - _T('0') );
			}
			else if( psID[i]>=_T('a') && psID[i]<=_T('f') ){
				cItem+=(BYTE)( psID[i] - _T('a') + 0x0A );
			}
			else
				return FALSE;
			*pcID=cItem;
			pcID--;
		}//end for

		for( ; i<(sizeof(WORD)*2); i++ ){
			wID[j] = wID[j] >> 4;
		}//end for

		if( j == 1 ){

			if( nMode == ModeVidPid )
				break;	//exit for
		}
		else if( j == 2 ){
			if( nMode == ModeVidPidInf || nMode == ModeVidPidRev )
				break;//exit for

		}

	}//end for

	switch( nMode ){
		case ModeVidPid:

			if( nPID < 0 ){
				if( (WORD)nVID == wID[0] )
					return TRUE;	//matching VID.
			}
			else{
				if( (WORD)nVID == wID[0] &&  (WORD)nPID == wID[1] ){
					return TRUE;	//matching VID & PID.
				}
			}
			break;
		case ModeVidPidRev:
			if( (WORD)nVID == wID[0] &&  (WORD)nPID == wID[1] && (WORD)nRev == wID[2] ){
				return TRUE;	//matching VID & PID & Rev.
			}
			break;
		case ModeVidPidInf:
			if( (WORD)nVID == wID[0] &&  (WORD)nPID == wID[1] && (WORD)nInf == wID[2] ){
				return TRUE;	//matching VID & PID & interface.
			}
			break;
		case ModeVidPidInfRev:
			if( (WORD)nVID == wID[0] &&  (WORD)nPID == wID[1] && (WORD)nRev == wID[3] && (WORD)nInf == wID[2] ){
				return TRUE;	//matching VID & PID & Rev & interface.
			}
			break;

	}//end switch

	return FALSE;
}

/*
{
	TCHAR sVID[8];
	TCHAR sPID[8];
	TCHAR sInf[8];
	TCHAR sRev[8];
	PTCHAR psID;
	PTCHAR pssID[4] = { NULL, NULL, NULL, NULL };
	WORD wID[4] = { 0,0,0,0 };//wID[0] - PID, wID[1] - VID, wID[2] - REV or INF, wID[3] -  REV or INF.
	BYTE *pcID;
	BYTE cItem;
	int i,j;
	TCHAR sDevPath[256];
	BOOL bResult = FALSE;
	INT nMode = 1;

	pssID[0] = sVID;	pssID[1] = sPID;

	_tcscpy( sDevPath,psDevicePath );

	//Get VID, PID from Path-string.
	if( nRev<0 ){
		if( nInf<0 ){
			bResult = ParsingUSBDevicePath( sDevPath,(PTCHAR)sVID,(PTCHAR)sPID,NULL,NULL );

			if( bResult ){
					//convert lower case
					_tcslwr( (PTCHAR)sVID );	_tcslwr( (PTCHAR)sPID );
					nMode = 0;
			}
		}
		else{
			bResult = ParsingUSBDevicePath( sDevPath,(PTCHAR)sVID,(PTCHAR)sPID,(PTCHAR)sInf,NULL );
			if( bResult ){
					//convert lower case
					_tcslwr( (PTCHAR)sVID );	_tcslwr( (PTCHAR)sPID );	_tcslwr( (PTCHAR)sInf );
					nMode = 2;
					pssID[2] = sInf;
			}
		}
	}
	else{
		if( nInf<0 ){
			bResult = ParsingUSBDevicePath( sDevPath,(PTCHAR)sVID,(PTCHAR)sPID, NULL, (PTCHAR)sRev );
			if( bResult ){
					//convert lower case
					_tcslwr( (PTCHAR)sVID );	_tcslwr( (PTCHAR)sPID );	_tcslwr( (PTCHAR)sRev );
					nMode = 1;
					pssID[2] = sRev;
			}

		}
		else{
			bResult = ParsingUSBDevicePath( sDevPath,(PTCHAR)sVID,(PTCHAR)sPID,(PTCHAR)sInf, (PTCHAR)sRev );
			if( bResult ){
					//convert lower case
					_tcslwr( (PTCHAR)sVID );	_tcslwr( (PTCHAR)sPID );	_tcslwr( (PTCHAR)sInf );	_tcslwr( (PTCHAR)sRev );
					nMode = 3;
					pssID[2] = sRev;	pssID[3] = sInf;
			}

		}
	}

	
	if( !bResult )
		return FALSE;

	
	//convert VID and PID string type to VID and PID word type.
	for( j=0; j<4; j++ ){

		pcID = (LPBYTE)(&(wID[j]));
		pcID++;
		psID = pssID[j];

		for( i=0; i<4; i++ ){
			//high nibble
			if( psID[i]>=_T('0') && psID[i]<=_T('9') ){
				cItem=(BYTE)( psID[i] - _T('0') );
				cItem=cItem<<4;
			}
			else if( psID[i]>=_T('a') && psID[i]<=_T('f') ){
				cItem=(BYTE)( psID[i] - _T('a') + 0x0A);
				cItem=cItem<<4;
			}
			else
				return FALSE;

			i++;
			//low nibble
			if( psID[i]>=_T('0') && psID[i]<=_T('9') ){
				cItem+=(BYTE)( psID[i] - _T('0') );
			}
			else if( psID[i]>=_T('a') && psID[i]<=_T('f') ){
				cItem+=(BYTE)( psID[i] - _T('a') + 0x0A );
			}
			else
				return FALSE;
			*pcID=cItem;
			pcID--;
		}//end for

		if( j == 1 ){

			if( nMode == 0 )
				break;	//exit for
		}
		else if( j == 2 ){
			if( nMode == 1 || nMode == 2 )
				break;//exit for

		}

	}//end for

	switch( nMode ){
		case 0:

			if( nPID < 0 ){
				if( (WORD)nVID == wID[0] )
					return TRUE;	//matching VID.
			}
			else{
				if( (WORD)nVID == wID[0] &&  (WORD)nPID == wID[1] ){
					return TRUE;	//matching VID & PID.
				}
			}
			break;
		case 1:
			if( (WORD)nVID == wID[0] &&  (WORD)nPID == wID[1] && (WORD)nRev == wID[2] ){
				return TRUE;	//matching VID & PID & Rev.
			}
			break;
		case 2:
			if( (WORD)nVID == wID[0] &&  (WORD)nPID == wID[1] && (WORD)nInf == wID[2] ){
				return TRUE;	//matching VID & PID & interface.
			}
			break;
		case 3:
			if( (WORD)nVID == wID[0] &&  (WORD)nPID == wID[1] && (WORD)nRev == wID[2] && (WORD)nInf == wID[3] ){
				return TRUE;	//matching VID & PID & Rev & interface.
			}
			break;

	}//end switch

	return FALSE;
}

*/


//psInf == NULL, psRev == NULL,  pass interface & revision.
BOOL Cns_DevHelper::ParsingUSBDevicePath(
	PTCHAR pDevicePath,
	PTCHAR psVID,
	PTCHAR psPID/*=NULL*/,
	PTCHAR psInf/*=NULL*/,
	PTCHAR psRev/*=NULL*/
	)
{
	TCHAR seps[]=_T("\\#&");
	TCHAR *token;
	int i=0;
	//
	if( pDevicePath==NULL || psVID==NULL )
		return FALSE;

	/* Establish string and get the first token: */
	token = _tcstok( pDevicePath, seps );

	while( token != NULL ){
		/* While there are tokens in "pDevicePath" */
		switch( i ){
			case 1://may be USB device is "USB"
				//Perform a lowercase comparison of strings
				if( _tcsicmp( (LPCTSTR)token,_T("USB") )!=0 )
					if( _tcsicmp( (LPCTSTR)token,_T("HID") )!=0 )//USB HID ?
						return FALSE;//this device is none-usb device.
				break;
			case 2://may be Vid string(Vid_????)
				if( token[0]!=_T('V') && token[0]!=_T('v') )
					return FALSE;
				if( token[1]!=_T('I') && token[1]!=_T('i') )
					return FALSE;
				if( token[2]!=_T('D') && token[2]!=_T('d') )
					return FALSE;
				if( token[3]!=_T('_') )
					return FALSE;
				//
				if( _tcslen( token )!=8 )
					return FALSE;
				//Copy vender ID
				_tcscpy( psVID,&(token[4]) );

				if( psPID )
					break;
				else
					return TRUE;
			case 3://may be Pid string(Pid_????)
				if( token[0]!=_T('P') && token[0]!=_T('p') )
					return FALSE;
				if( token[1]!=_T('I') && token[1]!=_T('i') )
					return FALSE;
				if( token[2]!=_T('D') && token[2]!=_T('d') )
					return FALSE;
				if( token[3]!=_T('_') )
					return FALSE;
				//
				if( _tcslen( token )!=8 )
					return FALSE;
				//Copy product ID
				_tcscpy( psPID,&(token[4]) );

				if( psRev == NULL && psInf == NULL )
					return TRUE;
				else
					break;
			case 4:
				if( psRev ){
					//may be Rev string(Rev_????)
					if( token[0]!=_T('R') && token[0]!=_T('r') )
						return FALSE;
					if( token[1]!=_T('E') && token[1]!=_T('e') )
						return FALSE;
					if( token[2]!=_T('V') && token[2]!=_T('v') )
						return FALSE;
					if( token[3]!=_T('_') )
						return FALSE;
					//
					if( _tcslen( token )!=8 )
						return FALSE;
					//Copy revision string
					_tcscpy( psRev,&(token[4]) );

					if( psInf )
						break;
					else
						return TRUE;
				}
				else{//none rev & interface. 
					//may be Interface string(Mi_????)
					if( token[0]!=_T('M') && token[0]!=_T('m') )
						return FALSE;
					if( token[1]!=_T('I') && token[1]!=_T('i') )
						return FALSE;
					if( token[2]!=_T('_') )
						return FALSE;
					//
					if( _tcslen( token )!=5 )
						return FALSE;
					//Copy interface string
					_tcscpy( psInf,&(token[3]) );

					return TRUE;
				}
				break;
			case 5:
				//may be Interface string(Mi_????)
				if( token[0]!=_T('M') && token[0]!=_T('m') )
					return FALSE;
				if( token[1]!=_T('I') && token[1]!=_T('i') )
					return FALSE;
				if( token[2]!=_T('_') )
					return FALSE;
				//
				if( _tcslen( token )!=5 )
					return FALSE;
				//Copy interface string
				_tcscpy( psInf,&(token[3]) );

				return TRUE;
			default:
				break;
		}//end switch
		//m_ListReport.AddString( (LPCTSTR)token );
		//m_ListReport.SetCurSel( m_ListReport.GetCount()-1 );
		/* Get next token: */
		token = _tcstok( NULL, seps );
		i++;
	}
	return TRUE;
}

//register user device for device-change-notification
HDEVNOTIFY Cns_DevHelper::RegisterEventDevice( HANDLE hTargetWnd,CONST GUID & InterfaceClassGuid )
{
	////////////////////////////////////
	// interface class.
	HDEVNOTIFY diNotifyHandle;
	DEV_BROADCAST_DEVICEINTERFACE broadcastInterface;

	ZeroMemory(&broadcastInterface, sizeof(broadcastInterface));

	broadcastInterface.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	broadcastInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	broadcastInterface.dbcc_classguid = InterfaceClassGuid;

	
	diNotifyHandle = RegisterDeviceNotification(
		hTargetWnd,
		&broadcastInterface,
		DEVICE_NOTIFY_WINDOW_HANDLE
		);

	return diNotifyHandle;
}

//register user device-handle for device-change-notification
HDEVNOTIFY Cns_DevHelper::RegisterEventDevice( HANDLE hTargetWnd,HANDLE hDev )
{
	////////////////////////////////////
	// interface class.
	HDEVNOTIFY diNotifyHandle;
	DEV_BROADCAST_HANDLE filter;

	ZeroMemory(&filter, sizeof(DEV_BROADCAST_HANDLE));

	filter.dbch_size = sizeof(DEV_BROADCAST_HANDLE);
	filter.dbch_devicetype = DBT_DEVTYP_HANDLE;
	filter.dbch_handle = hDev;
	//filter.dbch_hdevnotify = hDevnotify;

	diNotifyHandle = RegisterDeviceNotification(
		hTargetWnd,
		&filter,
		DEVICE_NOTIFY_WINDOW_HANDLE
		);

	return diNotifyHandle;
}

//unregister device of RegisterEventDevice()
BOOL Cns_DevHelper::UnRegisterEventDevice( HDEVNOTIFY hNotify )
{
	return UnregisterDeviceNotification( hNotify );

}
