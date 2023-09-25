// t0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <list>
#include <vector>

#include <ns_hidusb.h>
#include <ns_Etc.h>

CONST INT FIND_ALL_DEVICES = -1;
bool bNoNeedDevCap = false;

struct HIDINFO {
	USHORT VID;//vender ID
	USHORT PID;//product ID
	TCHAR sPath[_MAX_PATH];//device path
	HIDP_CAPS HidCaps;//HID capabilities
};
enum ENUM_FindMode {
	FM_ALL = -1,		//all find usb device
	FM_VID = 0,			//find matched VID device.
	FM_VID_PID,			//find matched VID & PID device.
	FM_VID_PID_INF,		//find matched VID & PID & interface device.
	FM_VID_PID_REV,		//find matched VID & PID & revision device.
	FM_VID_PID_INF_REV	//find matched VID & PID * interface & revision device.
};


size_t _GetDevList(list<_tstring>& listDevPath);
INT _Find(
	list<HIDINFO*>& listDev,
	INT nVID = 0, INT nPID = 0,
	INT nInterface = -1,
	INT nRev = -1
);

BOOL _GetCapabilities(PHIDP_CAPS pCapabilities, HANDLE hDeviceHandle);
INT _GetVidOnDevicePath(LPCTSTR psDevPath);
INT _GetPidOnDevicePath(LPCTSTR psDevPath);
INT _GetInterfaceNumberOnDevicePath(LPCTSTR psDevPath);

int main()
{
    std::wcout << L"Hello World!\n";

    do {
        std::vector<wchar_t> v_multi_path(2048, 0);
        std::list<std::wstring> list_dev;

        /*
        size_t n_path = Cns_Hidusb::GetPath(&v_multi_path[0], v_multi_path.size());
        std::wcout << L"size = " << n_path << L"\n";
        if (n_path == 0) {
            std::wcout << L"none device!\n";
            continue;
        }
        
        if (Cns_Etc::GetStringFromMultiString(list_dev, &v_multi_path[0]) <= 0) {
            std::wcout << L"none multi device!\n";
            continue;
        }
        */

		_GetDevList(list_dev);
        for (auto item : list_dev) {
            std::wcout << L" MAIN : " << item << std::endl;
        }

    } while (false);

	return 0;
}

size_t _GetDevList(list<_tstring>& listDevPath)
{
    INT nDev = 0;	//Error
    _tstring sCurpath;
    HIDINFO* pHidInfo;

    //get current devie list.......
	std::list<HIDINFO*> listHids;
	_Find(listHids);

    listDevPath.clear();

    list<HIDINFO*>::iterator devitera = listHids.begin();

    for (devitera = listHids.begin(); devitera != listHids.end(); devitera++) {

        pHidInfo = *devitera;

        if (pHidInfo) {

            _tstring sCurpath = pHidInfo->sPath;

            listDevPath.push_back(sCurpath);

            nDev++;
        }

    }//end for

    return nDev;
}


//pHidInfos[OUT] : return found device infomation
//return the number of detected devices.
//if nVID==0 && nPID==0 , find all HID deivce 
INT _Find(
	list<HIDINFO*>& listDev,
	INT nVID/*=0*/, INT nPID/*=0*/,
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
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetailData = NULL;
	HDEVINFO hDevInfo;
	DWORD dwLength = 0;
	DWORD dwRequiredSize;
	INT nDetected = 0;
	BOOL bDected = FALSE;
	INT i = 0;
	INT nCurInterface = 0;
	HIDINFO* pHidInfo = NULL;

	list<HIDINFO*>::iterator devitor = listDev.begin();

	ENUM_FindMode nMatchMode = FM_ALL;		//-1 : all devices
	//0 : matching VID.
	//1	: matching VID & PID.
	//2 : matching VID & PID & INTERFACE
	//3 : matching VID & PID & REV
	//4 : matching VID & PID & INTERFACE & REV

/////////////////////
//set matching mode.
	if (nVID <= (INT)FIND_ALL_DEVICES) {
		nMatchMode = FM_ALL;//-1 : all device
	}
	else if (nPID <= FIND_ALL_DEVICES) {
		nMatchMode = FM_VID;//0 : matching VID.
	}
	else {
		if (nInterface <= FIND_ALL_DEVICES) {

			if (nRev <= FIND_ALL_DEVICES) {
				nMatchMode = FM_VID_PID;//1	: matching VID & PID.
			}
			else {
				nMatchMode = FM_VID_PID_REV;//3 : matching VID & PID & REV
			}
		}
		else {

			if (nRev <= FIND_ALL_DEVICES) {
				nMatchMode = FM_VID_PID_INF;//2 : matching VID & PID & INTERFACE
			}
			else {
				nMatchMode = FM_VID_PID_INF_REV;//4 : matching VID & PID & INTERFACE & REV
			}
		}
	}

	//Get HID GUID
	HidD_GetHidGuid(&HidGuid);

	//Get device information set of installed HID-Devices
	hDevInfo = SetupDiGetClassDevs(&HidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

	devInfoData.cbSize = sizeof(devInfoData);

	//find wanted device.
	nMemberIndex = 0;

	for (nMemberIndex = 0; TRUE; nMemberIndex++) {
		// 1.
		lResult = SetupDiEnumDeviceInterfaces(
			hDevInfo, 0, &HidGuid, nMemberIndex, &devInfoData
		);

		if (lResult == FALSE)
			break;//exit for;//NO more device.

		// 2.
		lResult = SetupDiGetDeviceInterfaceDetail(
			hDevInfo, &devInfoData, NULL, 0, &dwLength, NULL
		);

		pDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(dwLength);
		pDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		lResult = SetupDiGetDeviceInterfaceDetail(
			hDevInfo, &devInfoData, pDetailData, dwLength, &dwRequiredSize, NULL
		);
		if (lResult == FALSE)
			break;//exit for

		// 3.
		hHid = CreateFile(
			pDetailData->DevicePath, 0,
			FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING, 0, NULL
		);
		if (hHid == INVALID_HANDLE_VALUE) {
			//Free the memory used by the detailData structure (no longer needed).
			free(pDetailData);
			continue;
		}

		// 4.
		Attributes.Size = sizeof(Attributes);

		std::wcout <<pDetailData->DevicePath << L"\n";

		if (bNoNeedDevCap) {
			Attributes.Size = sizeof(Attributes);//
			Attributes.VendorID = _GetVidOnDevicePath(pDetailData->DevicePath);
			Attributes.ProductID = _GetPidOnDevicePath(pDetailData->DevicePath);
			Attributes.VersionNumber = 0;//
		}
		else {
			lResult = HidD_GetAttributes(hHid, &Attributes);
			if (lResult == FALSE) {
				std::wcout << L"ERR : " << pDetailData->DevicePath << L"\n";
				//break;//exit for
				CloseHandle(hHid);

				//Free the memory used by the detailData structure (no longer needed).
				free(pDetailData);
				continue;//next item
			}
		}

		//5.
		bDected = FALSE;

		if (Attributes.VendorID != 0) {
			switch (nMatchMode) {
			case FM_ALL://-1 : all device
				bDected = TRUE;
				break;
			case FM_VID://0 : matching VID.
				if (Attributes.VendorID == (USHORT)nVID)
					bDected = TRUE;
				break;
			case FM_VID_PID://1	: matching VID & PID.
				if (Attributes.VendorID == (USHORT)nVID
					&& Attributes.ProductID == (USHORT)nPID
					)
					bDected = TRUE;
				break;
			case FM_VID_PID_INF://2 : matching VID & PID & INTERFACE
				nCurInterface = _GetInterfaceNumberOnDevicePath(pDetailData->DevicePath);

				if (Attributes.VendorID == (USHORT)nVID
					&& Attributes.ProductID == (USHORT)nPID
					&& nCurInterface == nInterface
					)
					bDected = TRUE;
				break;
			case FM_VID_PID_REV://3 : matching VID & PID & REV
				if (Attributes.VendorID == (USHORT)nVID
					&& Attributes.ProductID == (USHORT)nPID
					&& Attributes.VersionNumber == (USHORT)nRev
					)
					bDected = TRUE;
				break;
			case FM_VID_PID_INF_REV://4 : matching VID & PID & INTERFACE & REV
				nCurInterface = _GetInterfaceNumberOnDevicePath(pDetailData->DevicePath);

				if (Attributes.VendorID == (USHORT)nVID
					&& Attributes.ProductID == (USHORT)nPID
					&& Attributes.VersionNumber == (USHORT)nRev
					&& nCurInterface == nInterface
					)
					bDected = TRUE;
				break;

			}//end switch
		}

		if (bDected) {

			if (bNoNeedDevCap) {
				pHidInfo = new HIDINFO();

				if (pHidInfo) {//copy path
					_tcscpy(pHidInfo->sPath, pDetailData->DevicePath);
					::memset(&(pHidInfo->HidCaps), 0, sizeof(HIDP_CAPS));
					pHidInfo->VID = Attributes.VendorID;
					pHidInfo->PID = Attributes.ProductID;

					listDev.push_back(pHidInfo);
					nDetected++;
				}
			}
			else {
				// save detect device information.
				//Get the device's capablities.
				_GetCapabilities(&Capabilities, hHid);

				// Find out if the device is a system mouse or keyboard.
				DeviceUsage = (Capabilities.UsagePage * 256) + Capabilities.Usage;

				if ((DeviceUsage != 0x102) && (DeviceUsage != 0x106)) {

					pHidInfo = new HIDINFO();
					if (pHidInfo) {//copy path
						_tcscpy(pHidInfo->sPath, pDetailData->DevicePath);
						memcpy(&(pHidInfo->HidCaps), &Capabilities, sizeof(HIDP_CAPS));
						pHidInfo->VID = Attributes.VendorID;
						pHidInfo->PID = Attributes.ProductID;

						listDev.push_back(pHidInfo);
						nDetected++;
					}

				}
				else {
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
INT _GetInterfaceNumberOnDevicePath(LPCTSTR psDevPath)
{
	INT nInterface = 0;
	TCHAR* pdest = NULL;
	INT i;
	TCHAR sInterface[5];
	LPTSTR psSrc = NULL;


	if (psDevPath == NULL)
		return -1;	//Not found interface.

	psSrc = _tcslwr(_tcsdup(psDevPath));

	pdest = _tcsstr(psSrc, _T("mi_"));	//mi_XX ==> xx is interface number( interface number is started from zero-base )

	if (pdest == NULL) {
		if (psSrc)
			free(psSrc);

		return -1;	//Not found interface.
	}
	//

	memset(sInterface, 0, 5 * sizeof(TCHAR));//reset buffer.
	pdest = pdest + 3 * sizeof(TCHAR);	//increase pointer number to interface number position.

	//get interface number string
	for (i = 0; i < 5; i++) {
		if (pdest[i]<_T('0') || pdest[i]>_T('9'))
			break;//exit for
		//
		sInterface[i] = pdest[i];

	}//end for

	if (i == 0) {
		if (psSrc)
			free(psSrc);
		return -1;	//Not found interface.
	}

	//convert string to digit.
	nInterface = Cns_Etc::ConvertToDigit((LPCTSTR)sInterface, 16);

	if (psSrc)
		free(psSrc);

	return nInterface;
}

//Get PID on HID-device path
INT _GetPidOnDevicePath(LPCTSTR psDevPath)
{
	INT nInterface = 0;
	TCHAR* pdest = NULL;
	INT i;
	TCHAR sPid[8];
	LPTSTR psSrc = NULL;


	if (psDevPath == NULL)
		return -1;	//Not found interface.

	psSrc = _tcslwr(_tcsdup(psDevPath));

	pdest = _tcsstr(psSrc, _T("pid_"));	//pid_XXXX ==> xxxx hexcimal is product ID.

	if (pdest == NULL) {
		if (psSrc)
			free(psSrc);

		return -1;	//Not found interface.
	}
	//

	memset(sPid, 0, 8 * sizeof(TCHAR));//reset buffer.
	pdest = &pdest[4];	//increase pointer number to interface number position.

	//get interface number string
	for (i = 0; i < 4; i++) {
		sPid[i] = pdest[i];
	}//end for

	if (i == 0) {
		if (psSrc)
			free(psSrc);
		return -1;	//Not found interface.
	}

	//convert string to digit.
	nInterface = Cns_Etc::ConvertToDigit((LPCTSTR)sPid, 16);

	if (psSrc)
		free(psSrc);

	return nInterface;
}

//Get VID on HID-device path
INT _GetVidOnDevicePath(LPCTSTR psDevPath)
{
	INT nInterface = 0;
	TCHAR* pdest = NULL;
	INT i;
	TCHAR sVid[8];
	LPTSTR psSrc = NULL;


	if (psDevPath == NULL)
		return 0;	//Not found interface.

	psSrc = _tcslwr(_tcsdup(psDevPath));

	pdest = _tcsstr(psSrc, _T("vid_"));	//vid_XXXX ==> xxxx hexcimal is vender ID.

	if (pdest == NULL) {
		if (psSrc)
			free(psSrc);

		return 0;	//Not found interface.
	}
	//

	memset(sVid, 0, 8 * sizeof(TCHAR));//reset buffer.
	pdest = &pdest[4];	//increase pointer number to interface number position.

	//get interface number string
	for (i = 0; i < 4; i++) {
		sVid[i] = pdest[i];
	}//end for

	if (i == 0) {
		if (psSrc)
			free(psSrc);
		return 0;	//Not found interface.
	}

	//convert string to digit.
	nInterface = Cns_Etc::ConvertToDigit((LPCTSTR)sVid, 16);

	if (psSrc)
		free(psSrc);

	return nInterface;
}

BOOL _GetCapabilities(PHIDP_CAPS pCapabilities, HANDLE hDeviceHandle)
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
	bResult = HidD_GetPreparsedData(hDeviceHandle, &PreparsedData);
	if (!bResult)
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

	if (HidP_GetCaps(PreparsedData, pCapabilities) == HIDP_STATUS_SUCCESS)
		bResult = TRUE;
	else
		bResult = FALSE;

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
	if (!HidD_FreePreparsedData(PreparsedData))
		bResult = FALSE;
	//
	return bResult;
}
