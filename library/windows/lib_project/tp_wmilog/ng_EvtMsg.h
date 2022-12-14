 //ng_EvtMsg.mc
 //#pragma once
////////////////////////////////////////
// SUCCESS messages
//
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_SYSTEM                  0x0
#define FACILITY_STUBS                   0x3
#define FACILITY_RUNTIME                 0x2
#define FACILITY_IO_ERROR_CODE           0x4


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: IDS_EVTMSG_SUCCESS
//
// MessageText:
//
// SUCCESS : %1
//
#define IDS_EVTMSG_SUCCESS               ((DWORD)0x200203E8L)

////////////////////////////////////////
// ERROR messages
//
//
// MessageId: IDS_EVTMSG_ERROR
//
// MessageText:
//
// ERROR : %1
//
#define IDS_EVTMSG_ERROR                 ((DWORD)0xE0021F40L)

////////////////////////////////////////
// INFO messages
//
//
// MessageId: IDS_EVTMSG_INFO
//
// MessageText:
//
// INFO : %1
//
#define IDS_EVTMSG_INFO                  ((DWORD)0x60020FA0L)

////////////////////////////////////////
// WARNING messages
//
//
// MessageId: IDS_EVTMSG_WARNING
//
// MessageText:
//
// WARNING : %1
//
#define IDS_EVTMSG_WARNING               ((DWORD)0xA00207D0L)


// the end of file
