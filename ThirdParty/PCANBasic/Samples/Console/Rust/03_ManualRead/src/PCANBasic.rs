//  PCANBasic.rs
//
//  ~~~~~~~~~~~~
//
//  PCAN-Basic API
//
//  ~~~~~~~~~~~~
//
//  ------------------------------------------------------------------
//  Author : Keneth Wagner
//  Last change: 2026-05-20
//
//  Language: Rust
//  ------------------------------------------------------------------
//
//  Copyright (C) 1999-2026  PEAK-System Technik GmbH, Darmstadt
//  more Info at http://www.peak-system.com 
//

use std::ffi::c_char;
use std::ffi::c_void;
use std::ffi::CStr;
use std::ffi::CString;

// /////////////////////////////////////////////////////////
// Value definitions
// /////////////////////////////////////////////////////////

// Currently defined and supported PCAN channels
//
pub const PCAN_NONEBUS: u16 = 0x00;     // Undefined/default value for a PCAN bus

pub const PCAN_PCIBUS1: u16 = 0x41;     // PCAN-PCI interface, channel 1
pub const PCAN_PCIBUS2: u16 = 0x42;     // PCAN-PCI interface, channel 2
pub const PCAN_PCIBUS3: u16 = 0x43;     // PCAN-PCI interface, channel 3
pub const PCAN_PCIBUS4: u16 = 0x44;     // PCAN-PCI interface, channel 4
pub const PCAN_PCIBUS5: u16 = 0x45;     // PCAN-PCI interface, channel 5
pub const PCAN_PCIBUS6: u16 = 0x46;     // PCAN-PCI interface, channel 6
pub const PCAN_PCIBUS7: u16 = 0x47;     // PCAN-PCI interface, channel 7
pub const PCAN_PCIBUS8: u16 = 0x48;     // PCAN-PCI interface, channel 8
pub const PCAN_PCIBUS9: u16 = 0x409;    // PCAN-PCI interface, channel 9
pub const PCAN_PCIBUS10: u16 = 0x40A;   // PCAN-PCI interface, channel 10
pub const PCAN_PCIBUS11: u16 = 0x40B;   // PCAN-PCI interface, channel 11
pub const PCAN_PCIBUS12: u16 = 0x40C;   // PCAN-PCI interface, channel 12
pub const PCAN_PCIBUS13: u16 = 0x40D;   // PCAN-PCI interface, channel 13
pub const PCAN_PCIBUS14: u16 = 0x40E;   // PCAN-PCI interface, channel 14
pub const PCAN_PCIBUS15: u16 = 0x40F;   // PCAN-PCI interface, channel 15
pub const PCAN_PCIBUS16: u16 = 0x410;   // PCAN-PCI interface, channel 16

pub const PCAN_USBBUS1: u16 = 0x51;     // PCAN-USB interface, channel 1
pub const PCAN_USBBUS2: u16 = 0x52;     // PCAN-USB interface, channel 2
pub const PCAN_USBBUS3: u16 = 0x53;     // PCAN-USB interface, channel 3
pub const PCAN_USBBUS4: u16 = 0x54;     // PCAN-USB interface, channel 4
pub const PCAN_USBBUS5: u16 = 0x55;     // PCAN-USB interface, channel 5
pub const PCAN_USBBUS6: u16 = 0x56;     // PCAN-USB interface, channel 6
pub const PCAN_USBBUS7: u16 = 0x57;     // PCAN-USB interface, channel 7
pub const PCAN_USBBUS8: u16 = 0x58;     // PCAN-USB interface, channel 8
pub const PCAN_USBBUS9: u16 = 0x509;    // PCAN-USB interface, channel 9
pub const PCAN_USBBUS10: u16 = 0x50A;   // PCAN-USB interface, channel 10
pub const PCAN_USBBUS11: u16 = 0x50B;   // PCAN-USB interface, channel 11
pub const PCAN_USBBUS12: u16 = 0x50C;   // PCAN-USB interface, channel 12
pub const PCAN_USBBUS13: u16 = 0x50D;   // PCAN-USB interface, channel 13
pub const PCAN_USBBUS14: u16 = 0x50E;   // PCAN-USB interface, channel 14
pub const PCAN_USBBUS15: u16 = 0x50F;   // PCAN-USB interface, channel 15
pub const PCAN_USBBUS16: u16 = 0x510;   // PCAN-USB interface, channel 16

pub const PCAN_LANBUS1: u16 = 0x801;    // PCAN-LAN interface, channel 1
pub const PCAN_LANBUS2: u16 = 0x802;    // PCAN-LAN interface, channel 2
pub const PCAN_LANBUS3: u16 = 0x803;    // PCAN-LAN interface, channel 3
pub const PCAN_LANBUS4: u16 = 0x804;    // PCAN-LAN interface, channel 4
pub const PCAN_LANBUS5: u16 = 0x805;    // PCAN-LAN interface, channel 5
pub const PCAN_LANBUS6: u16 = 0x806;    // PCAN-LAN interface, channel 6
pub const PCAN_LANBUS7: u16 = 0x807;    // PCAN-LAN interface, channel 7
pub const PCAN_LANBUS8: u16 = 0x808;    // PCAN-LAN interface, channel 8
pub const PCAN_LANBUS9: u16 = 0x809;    // PCAN-LAN interface, channel 9
pub const PCAN_LANBUS10: u16 = 0x80A;   // PCAN-LAN interface, channel 10
pub const PCAN_LANBUS11: u16 = 0x80B;   // PCAN-LAN interface, channel 11
pub const PCAN_LANBUS12: u16 = 0x80C;   // PCAN-LAN interface, channel 12
pub const PCAN_LANBUS13: u16 = 0x80D;   // PCAN-LAN interface, channel 13
pub const PCAN_LANBUS14: u16 = 0x80E;   // PCAN-LAN interface, channel 14
pub const PCAN_LANBUS15: u16 = 0x80F;   // PCAN-LAN interface, channel 15
pub const PCAN_LANBUS16: u16 = 0x810;   // PCAN-LAN interface, channel 16

// Represent the PCAN error and status codes 
//
pub const PCAN_ERROR_OK: u32 = 0x00000;                         // No error 
pub const PCAN_ERROR_XMTFULL: u32 = 0x00001;                    // Transmit buffer in CAN controller is full
pub const PCAN_ERROR_OVERRUN: u32 = 0x00002;                    // CAN controller was read too late
pub const PCAN_ERROR_BUSLIGHT: u32 = 0x00004;                   // Bus error: an error counter reached the 'light' limit
pub const PCAN_ERROR_BUSHEAVY: u32 = 0x00008;                   // Bus error: an error counter reached the 'heavy' limit
pub const PCAN_ERROR_BUSWARNING: u32 = PCAN_ERROR_BUSHEAVY;     // Bus error: an error counter reached the 'warning' limit
pub const PCAN_ERROR_BUSPASSIVE: u32 = 0x40000;                 // Bus error: the CAN controller is error passive
pub const PCAN_ERROR_BUSOFF: u32 = 0x00010;                     // Bus error: the CAN controller is in bus-off state
pub const PCAN_ERROR_ANYBUSERR: u32 = (PCAN_ERROR_BUSWARNING | PCAN_ERROR_BUSLIGHT | PCAN_ERROR_BUSHEAVY | PCAN_ERROR_BUSOFF | PCAN_ERROR_BUSPASSIVE);  // Mask for all bus errors
pub const PCAN_ERROR_QRCVEMPTY: u32 = 0x00020;                  // Receive queue is empty
pub const PCAN_ERROR_QOVERRUN: u32 = 0x00040;                   // Receive queue was read too late
pub const PCAN_ERROR_QXMTFULL: u32 = 0x00080;                   // Transmit queue is full
pub const PCAN_ERROR_REGTEST: u32 = 0x00100;                    // Test of the CAN controller hardware registers failed (no hardware found)
pub const PCAN_ERROR_NODRIVER: u32 = 0x00200;                   // Driver not loaded
pub const PCAN_ERROR_HWINUSE: u32 = 0x00400;                    // Hardware already in use by a Net
pub const PCAN_ERROR_NETINUSE: u32 = 0x00800;                   // A Client is already connected to the Net
pub const PCAN_ERROR_ILLHW: u32 = 0x01400;                      // Hardware handle is invalid
pub const PCAN_ERROR_ILLNET: u32 = 0x01800;                     // Net handle is invalid
pub const PCAN_ERROR_ILLCLIENT: u32 = 0x01C00;                  // Client handle is invalid
pub const PCAN_ERROR_ILLHANDLE: u32 = (PCAN_ERROR_ILLHW | PCAN_ERROR_ILLNET | PCAN_ERROR_ILLCLIENT);  // Mask for all handle errors
pub const PCAN_ERROR_RESOURCE: u32 = 0x02000;                   // Resource (FIFO, Client, timeout) cannot be created
pub const PCAN_ERROR_ILLPARAMTYPE: u32 = 0x04000;               // Invalid parameter
pub const PCAN_ERROR_ILLPARAMVAL: u32 = 0x08000;                // Invalid parameter value
pub const PCAN_ERROR_UNKNOWN: u32 = 0x10000;                    // Unknown error
pub const PCAN_ERROR_ILLDATA: u32 = 0x20000;                    // Invalid data, function, or action
pub const PCAN_ERROR_ILLMODE: u32 = 0x80000;                    // Driver object state is wrong for the attempted operation
pub const PCAN_ERROR_CAUTION: u32 = 0x2000000;                  // An operation was successfully carried out, however, irregularities were registered
pub const PCAN_ERROR_INITIALIZE: u32 = 0x4000000;               // Channel is not initialized [Value was changed from 0x40000 to 0x4000000]
pub const PCAN_ERROR_ILLOPERATION: u32 = 0x8000000;             // Invalid operation [Value was changed from 0x80000 to 0x8000000]

// PCAN devices
//
pub const PCAN_NONE: u8 = 0x00;     // Undefined, unknown or not selected PCAN device value
pub const PCAN_PCI: u8 = 0x04;      // PCAN-PCI, PCAN-cPCI, PCAN-miniPCI, and PCAN-PCI Express
pub const PCAN_USB: u8 = 0x05;      // PCAN-USB and PCAN-USB Pro
pub const PCAN_LAN: u8 = 0x08;      // PCAN Gateway devices

// PCAN parameters
//
pub const PCAN_DEVICE_ID: u8 = 0x01;                    // Device identifier parameter
pub const PCAN_5VOLTS_POWER: u8 = 0x02;                 // 5-Volt power parameter
pub const PCAN_RECEIVE_EVENT: u8 = 0x03;                // PCAN receive event handler parameter
pub const PCAN_MESSAGE_FILTER: u8 = 0x04;               // PCAN message filter parameter
pub const PCAN_API_VERSION: u8 = 0x05;                  // PCAN-Basic API version parameter
pub const PCAN_CHANNEL_VERSION: u8 = 0x06;              // PCAN device channel version parameter
pub const PCAN_BUSOFF_AUTORESET: u8 = 0x07;             // PCAN Reset-On-Busoff parameter
pub const PCAN_LISTEN_ONLY: u8 = 0x08;                  // PCAN Listen-Only parameter
pub const PCAN_LOG_LOCATION: u8 = 0x09;                 // Directory path for log files
pub const PCAN_LOG_STATUS: u8 = 0x0A;                   // Debug-Log activation status
pub const PCAN_LOG_CONFIGURE: u8 = 0x0B;                // Configuration of the debugged information (LOG_FUNCTION_***)
pub const PCAN_LOG_TEXT: u8 = 0x0C;                     // Custom insertion of text into the log file
pub const PCAN_CHANNEL_CONDITION: u8 = 0x0D;            // Availability status of a PCAN-Channel
pub const PCAN_HARDWARE_NAME: u8 = 0x0E;                // PCAN hardware name parameter
pub const PCAN_RECEIVE_STATUS: u8 = 0x0F;               // Message reception status of a PCAN-Channel
pub const PCAN_CONTROLLER_NUMBER: u8 = 0x10;            // CAN-Controller number of a PCAN-Channel 
pub const PCAN_TRACE_LOCATION: u8 = 0x11;               // Directory path for PCAN trace files
pub const PCAN_TRACE_STATUS: u8 = 0x12;                 // CAN tracing activation status
pub const PCAN_TRACE_SIZE: u8 = 0x13;                   // Configuration of the maximum file size of a CAN trace
pub const PCAN_TRACE_CONFIGURE: u8 = 0x14;              // Configuration of the trace file storing mode (TRACE_FILE_***)
pub const PCAN_CHANNEL_IDENTIFYING: u8 = 0x15;          // Physical identification of a USB based PCAN-Channel by blinking its associated LED
pub const PCAN_CHANNEL_FEATURES: u8 = 0x16;             // Capabilities of a PCAN device (FEATURE_***)
pub const PCAN_BITRATE_ADAPTING: u8 = 0x17;             // Using of an existing bit rate (PCAN-View connected to a channel)
pub const PCAN_BITRATE_INFO_BTR: u8 = 0x18;             // Configured bit rate as a Baud Rate Timing Register value
pub const PCAN_BITRATE_INFO_FD: u8 = 0x19;              // Configured bit rate as TPCANBitrateFD string
pub const PCAN_BUSSPEED_NOMINAL: u8 = 0x1A;             // Configured nominal CAN Bus speed as Bits per seconds
pub const PCAN_BUSSPEED_FD: u8 = 0x1B;                  // Configured CAN data speed as Bits per seconds
pub const PCAN_IP_ADDRESS: u8 = 0x1C;                   // Remote address of a LAN channel as string in IPv4 format
pub const PCAN_LAN_SERVICE_STATUS: u8 = 0x1D;           // Status of the Virtual PCAN-Gateway Service
pub const PCAN_ALLOW_STATUS_FRAMES: u8 = 0x1E;          // Status messages reception status within a PCAN-Channel
pub const PCAN_ALLOW_RTR_FRAMES: u8 = 0x1F;             // RTR messages reception status within a PCAN-Channel
pub const PCAN_ALLOW_ERROR_FRAMES: u8 = 0x20;           // Error messages reception status within a PCAN-Channel
pub const PCAN_INTERFRAME_DELAY: u8 = 0x21;             // Delay, in microseconds, between sending frames
pub const PCAN_ACCEPTANCE_FILTER_11BIT: u8 = 0x22;      // Filter over code and mask patterns for 11-Bit messages
pub const PCAN_ACCEPTANCE_FILTER_29BIT: u8 = 0x23;      // Filter over code and mask patterns for 29-Bit messages
pub const PCAN_IO_DIGITAL_CONFIGURATION: u8 = 0x24;     // Output mode of 32 digital I/O pin of a PCAN-USB Chip. 1: Output-Active 0 : Output Inactive
pub const PCAN_IO_DIGITAL_VALUE: u8 = 0x25;             // Value assigned to a 32 digital I/O pins of a PCAN-USB Chip
pub const PCAN_IO_DIGITAL_SET: u8 = 0x26;               // Value assigned to a 32 digital I/O pins of a PCAN-USB Chip - Multiple digital I/O pins to 1 = High
pub const PCAN_IO_DIGITAL_CLEAR: u8 = 0x27;             // Clear multiple digital I/O pins to 0
pub const PCAN_IO_ANALOG_VALUE: u8 = 0x28;              // Get value of a single analog input pin
pub const PCAN_FIRMWARE_VERSION: u8 = 0x29;             // Get the version of the firmware used by the device associated with a PCAN-Channel
pub const PCAN_ATTACHED_CHANNELS_COUNT: u8 = 0x2A;      // Get the amount of PCAN channels attached to a system
pub const PCAN_ATTACHED_CHANNELS: u8 = 0x2B;            // Get information about PCAN channels attached to a system
pub const PCAN_ALLOW_ECHO_FRAMES: u8 = 0x2C;            // Echo messages reception status within a PCAN-Channel
pub const PCAN_DEVICE_PART_NUMBER: u8 = 0x2D;           // Get the part number associated to a device
pub const PCAN_HARD_RESET_STATUS: u8 = 0x2E;            // Activation status of hard reset processing via CAN_Reset calls
pub const PCAN_LAN_CHANNEL_DIRECTION: u8 = 0x2F;        // Communication direction of a PCAN-Channel representing a PCAN-LAN interface
pub const PCAN_DEVICE_GUID: u8 = 0x30;                  // Get the global unique device identifier (GUID) associated to a device
pub const PCAN_BITRATE_INFO_CC: u8 = 0x31;              // Configured bit rate as TPCANBitrateCC value
pub const PCAN_BITRATE_INFO_XL: u8 = 0x32;              // Configured bit rate as TPCANBitrateXL string
pub const PCAN_BUSSPEED_XL: u8 = 0x33;                  // Configured CAN XL Bus speed as Bits per seconds

// DEPRECATED parameters
//
pub const PCAN_BUSSPEED_DATA: u8 = PCAN_BUSSPEED_FD;  // Deprecated parameter. Use PCAN_BUSSPEED_FD instead 
pub const PCAN_BITRATE_INFO: u8 = PCAN_BITRATE_INFO_BTR;  // Deprecated parameter. Use PCAN_BITRATE_INFO_BTR instead 

// PCAN parameter values
//
pub const PCAN_PARAMETER_OFF: u8 = 0x00;            // The PCAN parameter is not set (inactive)
pub const PCAN_PARAMETER_ON: u8 = 0x01;             // The PCAN parameter is set (active)
pub const PCAN_FILTER_CLOSE: u8 = 0x00;             // The PCAN filter is closed. No messages will be received
pub const PCAN_FILTER_OPEN: u8 = 0x01;              // The PCAN filter is fully opened. All messages will be received
pub const PCAN_FILTER_CUSTOM: u8 = 0x02;            // The PCAN filter is custom configured. Only registered messages will be received
pub const PCAN_CHANNEL_UNAVAILABLE: u8 = 0x00;      // The PCAN-Channel handle is illegal, or its associated hardware is not available
pub const PCAN_CHANNEL_AVAILABLE: u8 = 0x01;        // The PCAN-Channel handle is available to be connected (PnP Hardware: it means furthermore that the hardware is plugged-in)
pub const PCAN_CHANNEL_OCCUPIED: u8 = 0x02;         // The PCAN-Channel handle is valid, and is already being used
pub const PCAN_CHANNEL_PCANVIEW: u8 = (PCAN_CHANNEL_AVAILABLE | PCAN_CHANNEL_OCCUPIED);  // The PCAN-Channel handle is already being used by a PCAN-View application, but is available to connect

pub const LOG_FUNCTION_DEFAULT: u8 = 0x00;          // Logs system exceptions / errors
pub const LOG_FUNCTION_ENTRY: u8 = 0x01;            // Logs the entries to the PCAN-Basic API functions 
pub const LOG_FUNCTION_PARAMETERS: u8 = 0x02;       // Logs the parameters passed to the PCAN-Basic API functions 
pub const LOG_FUNCTION_LEAVE: u8 = 0x04;            // Logs the exits from the PCAN-Basic API functions 
pub const LOG_FUNCTION_WRITE: u8 = 0x08;            // Logs the CAN messages passed to the CAN_Write function
pub const LOG_FUNCTION_READ: u8 = 0x10;             // Logs the CAN messages received within the CAN_Read function
pub const LOG_FUNCTION_ALL: u16 = 0xFFFF;           // Logs all possible information within the PCAN-Basic API functions

pub const TRACE_FILE_SINGLE: u8 = 0x00;             // A single file is written until it size reaches PAN_TRACE_SIZE
pub const TRACE_FILE_SEGMENTED: u8 = 0x01;          // Traced data is distributed in several files with size PAN_TRACE_SIZE
pub const TRACE_FILE_DATE: u8 = 0x02;               // Includes the date into the name of the trace file
pub const TRACE_FILE_TIME: u8 = 0x04;               // Includes the start time into the name of the trace file
pub const TRACE_FILE_OVERWRITE: u8 = 0x80;          // Causes the overwriting of available traces (same name)
pub const TRACE_FILE_DATA_LENGTH: u16 = 0x100;      // Causes using the data length column ('l') instead of the DLC column ('L') in the trace file

pub const FEATURE_FD_CAPABLE: u8 = 0x01;            // Device supports the subsequent development of the classic CAN bus (CAN FD)
pub const FEATURE_DELAY_CAPABLE: u8 = 0x02;         // Device supports a delay between sending frames (FPGA based USB devices)
pub const FEATURE_IO_CAPABLE: u8 = 0x04;            // Device supports I/O functionality for electronic circuits (USB-Chip devices)
pub const FEATURE_XL_CAPABLE: u8 = 0x08;            // Device supports the subsequent development of the classic CAN bus (CAN-XL)

pub const SERVICE_STATUS_STOPPED: u8 = 0x01;        // The service is not running
pub const SERVICE_STATUS_RUNNING: u8 = 0x04;        // The service is running

pub const LAN_DIRECTION_READ: u8 = 0x01;            // The PCAN-Channel is limited to incoming communication only
pub const LAN_DIRECTION_WRITE: u8 = 0x02;           // The PCAN-Channel is limited to outgoing communication only
pub const LAN_DIRECTION_READ_WRITE: u8 = (LAN_DIRECTION_READ | LAN_DIRECTION_WRITE);  // The PCAN-Channel communication is bidirectional 

// Other constants
//
pub const MAX_LENGTH_HARDWARE_NAME: usize = 33;         // Maximum length of the name of a device: 32 characters + terminator
pub const MAX_LENGTH_VERSION_STRING: usize = 256;       // Maximum length of a version string: 255 characters + terminator
pub const MAX_LENGTH_DATA_XL: usize = 2048;             // Maximum amount of data bytes of a CAN-XL message
pub const MAX_VALUE_STANDARD_ID: u32 = 0x7FF;           // Maximum value for a standard CAN ID of a CAN CC/FD message
pub const MAX_VALUE_EXTENDED_ID: u32 = 0x1FFFFFFF;      // Maximum value for an extended CAN ID of a CAN CC/FD message
pub const MAX_VALUE_PRIORITY_ID: u32 = 0x7FF;           // Maximum value for a Priority ID of a CAN XL message

// PCAN message types
//
pub const PCAN_MESSAGE_STANDARD: u8 = 0x00;     // The PCAN message is a CAN Standard Frame (11-bit identifier)
pub const PCAN_MESSAGE_RTR: u8 = 0x01;          // The PCAN message is a CAN Remote-Transfer-Request Frame
pub const PCAN_MESSAGE_EXTENDED: u8 = 0x02;     // The PCAN message is a CAN Extended Frame (29-bit identifier)
pub const PCAN_MESSAGE_FD: u8 = 0x04;           // The PCAN message represents a FD frame in terms of CiA Specs
pub const PCAN_MESSAGE_BRS: u8 = 0x08;          // The PCAN message represents a FD bit rate switch (CAN data at a higher bit rate)
pub const PCAN_MESSAGE_ESI: u8 = 0x10;          // The PCAN message represents a FD error state indicator(CAN FD transmitter was error active)
pub const PCAN_MESSAGE_ECHO: u8 = 0x20;         // The PCAN message represents an echo CAN Frame
pub const PCAN_MESSAGE_ERRFRAME: u8 = 0x40;     // The PCAN message represents an error frame
pub const PCAN_MESSAGE_STATUS: u8 = 0x80;       // The PCAN message represents a PCAN status message

// PCAN message types XL
//
pub const PCAN_MESSAGE_XL: u16 = 0x100;                     // The PCAN message represents a XL frame in terms of CiA Specs
pub const PCAN_MESSAGE_PROTOCOL_EXCEPTION: u16 = 0x200;     // The PCAN message represents a protocol exception from CAN core
pub const PCAN_MESSAGE_ERROR_NOTIFICATION: u16 = 0x400;     // The PCAN message represents an error notification from CAN core

// LookUp Parameters
//
pub const LOOKUP_DEVICE_TYPE: &str = "devicetype";              // Lookup channel by Device type (see PCAN devices e.g. PCAN_USB)
pub const LOOKUP_DEVICE_ID: &str = "deviceid";                  // Lookup channel by device id
pub const LOOKUP_CONTROLLER_NUMBER: &str = "controllernumber";  // Lookup channel by CAN controller 0-based index
pub const LOOKUP_IP_ADDRESS: &str = "ipaddress";                // Lookup channel by IP address (LAN channels only)
pub const LOOKUP_DEVICE_GUID: &str = "deviceguid";              // Lookup channel by device unique identifier (USB channels only)

// Frame Type / Initialization Mode
//
pub const PCAN_MODE_STANDARD: u8 = PCAN_MESSAGE_STANDARD;  
pub const PCAN_MODE_EXTENDED: u8 = PCAN_MESSAGE_EXTENDED;  

// Baud rate codes = BTR0/BTR1 register values for the CAN controller.
//
pub const PCAN_BAUD_1M: u16 = 0x0014;       //   1 MBit/s
pub const PCAN_BAUD_800K: u16 = 0x0016;     // 800 kBit/s
pub const PCAN_BAUD_500K: u16 = 0x001C;     // 500 kBit/s
pub const PCAN_BAUD_250K: u16 = 0x011C;     // 250 kBit/s
pub const PCAN_BAUD_125K: u16 = 0x031C;     // 125 kBit/s
pub const PCAN_BAUD_100K: u16 = 0x432F;     // 100 kBit/s
pub const PCAN_BAUD_95K: u16 = 0xC34E;      //  95,238 kBit/s
pub const PCAN_BAUD_83K: u16 = 0x852B;      //  83,333 kBit/s
pub const PCAN_BAUD_50K: u16 = 0x472F;      //  50 kBit/s
pub const PCAN_BAUD_47K: u16 = 0x1414;      //  47,619 kBit/s
pub const PCAN_BAUD_33K: u16 = 0x8B2F;      //  33,333 kBit/s
pub const PCAN_BAUD_20K: u16 = 0x532F;      //  20 kBit/s
pub const PCAN_BAUD_10K: u16 = 0x672F;      //  10 kBit/s
pub const PCAN_BAUD_5K: u16 = 0x7F7F;       //   5 kBit/s

// Represents the configuration for a CAN bit rate
// Note: 
//    * Each parameter and its value must be separated with a '='.
//    * Each pair of parameter/value must be separated using ','. 
//
// Example:
//    f_clock=80000000,nom_brp=10,nom_tseg1=5,nom_tseg2=2,nom_sjw=1,data_brp=4,data_tseg1=7,data_tseg2=2,data_sjw=1
//
pub const PCAN_BR_CLOCK: &str = "f_clock";              // Clock frequency in Herz (160000000, 80000000, 60000000, 40000000, 30000000, 24000000, 20000000)
pub const PCAN_BR_CLOCK_MHZ: &str = "f_clock_mhz";      // Clock frequency in Megaherz (160, 80, 60, 40, 30, 24, 20)

// CAN-FD Bit rate specific values
//
pub const PCAN_BR_NOM_BRP: &str = "nom_brp";                    // Clock prescaler for nominal time quantum
pub const PCAN_BR_NOM_TSEG1: &str = "nom_tseg1";                // TSEG1 segment for nominal bit rate in time quanta
pub const PCAN_BR_NOM_TSEG2: &str = "nom_tseg2";                // TSEG2 segment for nominal bit rate in time quanta
pub const PCAN_BR_NOM_SJW: &str = "nom_sjw";                    // Synchronization Jump Width for nominal bit rate in time quanta
pub const PCAN_BR_NOM_SAMPLE: &str = "nom_sam";                 // Sample point for nominal bit rate
pub const PCAN_BR_DATA_BRP: &str = "data_brp";                  // Clock prescaler for highspeed data time quantum
pub const PCAN_BR_DATA_TSEG1: &str = "data_tseg1";              // TSEG1 segment for fast data bit rate in time quanta
pub const PCAN_BR_DATA_TSEG2: &str = "data_tseg2";              // TSEG2 segment for fast data bit rate in time quanta
pub const PCAN_BR_DATA_SJW: &str = "data_sjw";                  // Synchronization Jump Width for highspeed data bit rate in time quanta
pub const PCAN_BR_DATA_SAMPLE: &str = "data_ssp_offset";        // DEPRECATED: Secondary sample point delay for highspeed data bit rate in cycles
                                                                //             Use "PCAN_BR_DATA_SSP_OFFSET" instead
pub const PCAN_BR_DATA_SSP_OFFSET: &str = "data_ssp_offset";    // Secondary sample point delay for highspeed data bit rate in cycles

// CAN-XL Bit rate specific values
//
pub const PCAN_BR_BRP: &str = "brp";                        // Clock prescaler for nominal, CAN FD and CAN XL bit rates
pub const PCAN_BR_FD_TSEG1: &str = "fd_tseg1";              // Clock prescaler for fast data time quantum
pub const PCAN_BR_FD_TSEG2: &str = "fd_tseg2";              // Clock prescaler for fast data time quantum
pub const PCAN_BR_FD_SJW: &str = "fd_sjw";                  // Synchronization Jump Width for fast data bit rate in time quanta
pub const PCAN_BR_FD_SSP_OFFSET: &str = "fd_ssp_offset";    // Secondary sample point delay for fast data bit rate in cycles
pub const PCAN_BR_XL_TSEG1: &str = "xl_tseg1";              // Clock prescaler for XL time quantum
pub const PCAN_BR_XL_TSEG2: &str = "xl_tseg2";              // Clock prescaler for XL time quantum
pub const PCAN_BR_XL_SJW: &str = "xl_sjw";                  // Synchronization Jump Width for XL bit rate in time quanta
pub const PCAN_BR_XL_SSP_OFFSET: &str = "xl_ssp_offset";    // Secondary sample point delay for XL bit rate in cycles
pub const PCAN_BR_XL_PWM_OFFSET: &str = "xl_pwm_offset";    // CAN XL PWM Offset in mtq ticks == f_cancore cycles
pub const PCAN_BR_XL_PWM_SHORT: &str = "xl_pwm_short";      // CAN XL PWM Short phase in mtq ticks == f_cancore cycles
pub const PCAN_BR_XL_PWM_LONG: &str = "xl_pwm_long";        // CAN XL PWM Long phase in mtq ticks == f_cancore cycles
pub const PCAN_BR_XL_TRANSCEIVER_MODE_SWITCH: &str = "xl_transceiver_mode_switch";  // 1 = CAN XL Data Phase uses 'fast TX' or 'fast RX' with PWM encoding
                                                                                    // 0 = CAN XL Data Phase uses no PWM encoding (recessive/dominant only, like CAN FD)
pub const PCAN_BR_XL_ERROR_SIGNALING: &str = "xl_error_signaling";  // 1 = Error Signaling with Error Frame in case of bus errors
                                                                    // 0 = No Error Signaling

// /////////////////////////////////////////////////////////
// Type definitions
// /////////////////////////////////////////////////////////

pub type TPCANHandle = u16;                     // Represents a PCAN hardware channel handle
pub type TPCANStatus = u32;                     // Represents a PCAN status/error code
pub type TPCANParameter = u8;                   // Represents a PCAN parameter to be read or set
pub type TPCANDevice = u8;                      // Represents a PCAN device
pub type TPCANMessageType = u8;                 // Represents the type of a PCAN message
pub type TPCANMessageTypeXL = u16;              // Represents the type of a PCAN XL message
pub type TPCANMode = u8;                        // Represents a PCAN filter mode
pub type TPCANBaudrate = u16;                   // Represents a PCAN Baud Rate Timing Register value
pub type TPCANBitrateCC = str;                  // Represents a PCAN classic bit rate string
pub type TPCANBitrateFD = str;                  // Represents a PCAN FD bit rate string
pub type TPCANBitrateXL = str;                  // Represents a PCAN XL bit rate string
pub type TPCANTimestampFD = u64;                // Represents a timestamp of a received PCAN FD message
pub type TPCANTimestampXL = TPCANTimestampFD;   // Represents a timestamp of a received PCAN XL message

// /////////////////////////////////////////////////////////
// Structure definitions
// /////////////////////////////////////////////////////////

// Represents a PCAN message
//
#[repr(C)]
#[derive(Clone, Copy)]
pub struct TPCANMsg {
    pub ID: u32,                        // 11/29-bit message identifier
    pub MSGTYPE: TPCANMessageType,      // Type of the message
    pub LEN: u8,                        // Data Length Code of the message (0..8)
    pub DATA: [u8; 8],                  // Data of the message (DATA[0]..DATA[7])
}

// Represents a timestamp of a received PCAN message
// Total Microseconds = micros + (1000ULL * millis) + (0x100000000ULL * 1000ULL * millis_overflow)
//
#[repr(C)]
#[derive(Clone, Copy)]
pub struct TPCANTimestamp {
    pub millis: u32,             // Base-value: milliseconds: 0.. 2^32-1
    pub millis_overflow: u16,    // Roll-arounds of millis
    pub micros: u16,             // Microseconds: 0..999
}

// Represents a PCAN message from a FD capable hardware
//
#[repr(C)]
#[derive(Clone, Copy)]
pub struct TPCANMsgFD {
    pub ID: u32,                        // 11/29-bit message identifier
    pub MSGTYPE: TPCANMessageType,      // Type of the message
    pub DLC: u8,                        // Data Length Code of the message (0..15)
    pub DATA: [u8; 64],                 // Data of the message (DATA[0]..DATA[63])
}

// Represents a PCAN message from a XL capable hardware
// REMARKS:
// CAN-CC/CAN-FD: only the fields PID, MSGTYPE, DLC,and DATA are used. 
// Additonal CAN-XL related fields are ignored
//
#[repr(C)]
#[derive(Clone, Copy)]
pub struct TPCANMsgXL {
    pub PID: u32,                       // CAN-XL: Priority ID (physical layer) (0..0x7FF)
                                        // CAN-CC/CAN-FD: 11/29-bit message identifier
    pub VCID: u8,                       // Virtual CAN network ID
    pub MSGTYPE: TPCANMessageTypeXL,    // Type of the message
    pub DLC: u16,                       // Data Length Code of the message (0..2047)  
    pub SDT: u8,                        // Service Data unit(SDU) protocol Type
    pub AF: u32,                        // Acceptance Field, SDU - specific high - layer ID
    pub RRS: u8,                        // Remote Request Substitution flag (0..1)
    pub SEC: u8,                        // Simple Extended Content flag (0..1)
    pub DATA: [u8; 2048],               // Data of the message (DATA[0]..DATA[2047])
}

// Describes an available PCAN channel
//
#[repr(C)]
#[derive(Clone, Copy)]
pub struct TPCANChannelInformation {
    pub channel_handle: TPCANHandle,                        // PCAN channel handle   
    pub device_type: TPCANDevice,                           // Kind of PCAN device
    pub controller_number: u8,                              // CAN-Controller number
    pub device_features: u32,                               // Device capabilities flag (see FEATURE_*)
    pub device_name: [c_char; MAX_LENGTH_HARDWARE_NAME],    // Device name
    pub device_id: u32,                                     // Device number   
    pub channel_condition: u32,                             // Availability status of a PCAN-Channel          
}

// /////////////////////////////////////////////////////////
// PCAN-Basic API function declarations
// /////////////////////////////////////////////////////////

unsafe extern "system" {
    /// Initializes a PCAN Channel 
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a PCAN Channel
    /// * `Btr0Btr1` - The speed for the communication (BTR0BTR1 code)
    /// * `deprecated1` - Deprecated. Parameter is ignored
    /// * `deprecated2` - Deprecated. Parameter is ignored
    /// * `deprecated3` - Deprecated. Parameter is ignored
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_Initialize(
        Channel: TPCANHandle, 
        Btr0Btr1: TPCANBaudrate, 
        deprecated1: u8,
        deprecated2: u32,
        deprecated3: u16) -> TPCANStatus;

    /// Initializes a FD capable PCAN Channel  
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a FD capable PCAN Channel
    /// * `BitrateFD` - The speed for the communication (FD bit rate string)
    ///
    /// # Remarks
    ///
    /// See PCAN_BR_* values
    /// * Parameter and values must be separated by '='
    /// * Couples of Parameter/value must be separated by ','
    /// * Following Parameter must be filled out: f_clock, data_brp, data_sjw, data_tseg1, data_tseg2,
    /// nom_brp, nom_sjw, nom_tseg1, nom_tseg2.
    /// * Following Parameters are optional (not used yet): data_ssp_offset, nom_sam
    ///
    /// # Examples
    ///
    /// f_clock=80000000,nom_brp=10,nom_tseg1=5,nom_tseg2=2,nom_sjw=1,data_brp=4,data_tseg1=7,data_tseg2=2,data_sjw=1
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_InitializeFD(
        Channel: TPCANHandle,
        BitrateFD: *const c_char) -> TPCANStatus;

    

    /// Initializes a XL capable PCAN Channel  
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a XL capable PCAN Channel
    /// * `BitrateXL` - The speed for the communication (XL bit rate string)
    ///
    /// # Remarks
    ///
    /// See PCAN_BR_* values
    /// * Parameter and values must be separated by '='
    /// * Couples of Parameter/value must be separated by ','
    /// * Following Parameter must be filled out: f_clock, brp, nom_brp, nom_sjw, nom_tseg1, nom_tseg2. 
    /// If xl_transceiver_mode_switch is active, also the parameters xl_sjw, xl_tseg1, and xl_tseg2, must be present.
    /// If error_signaling is active, also the parameters fd_sjw, fd_tseg1, and fd_tseg2, must be present. 
    /// * Following Parameters are optional: fd_ssp_offset, xl_ssp_offset, xl_transceiver_mode_switch, error_signaling,
    /// xl_pwm_offset, xl_pwm_short, and xl_pwm_long
    ///
    /// # Examples
    ///
    /// f_clock=160000000,brp=1,nom_tseg1=255,nom_tseg2=64,nom_sjw=64,fd_tseg1=63,fd_tseg2=16,fd_sjw=16,fd_ssp_offset=0,xl_tseg1=10,
    /// xl_tseg2=9,xl_sjw=9,xl_ssp_offset=10,xl_error_signaling=1,xl_transceiver_mode_switch=0
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_InitializeXL(
        Channel: TPCANHandle,
        BitrateXL: *const c_char) -> TPCANStatus;

    /// Uninitializes one or all PCAN Channels initialized by CAN_Initialize
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a PCAN Channel
    ///
    /// # Remarks
    ///
    /// Giving the TPCANHandle value "PCAN_NONEBUS", 
    /// uninitialize all initialized channels
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_Uninitialize(Channel: TPCANHandle) -> TPCANStatus;

    /// Resets the receive and transmit queues of the PCAN Channel  
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a PCAN Channel
    ///
    /// # Remarks
    ///
    /// A reset of the CAN controller is not performed.
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_Reset(Channel: TPCANHandle) -> TPCANStatus;

    /// Gets the current status of a PCAN Channel 
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a PCAN Channel
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_GetStatus(Channel: TPCANHandle) -> TPCANStatus;

    /// Reads a CAN message from the receive queue of a PCAN Channel 
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a PCAN Channel
    /// * `MessageBuffer` - A TPCANMsg structure buffer to store the CAN message
    /// * `TimestampBuffer` - A TPCANTimestamp structure buffer to get 
    ///   the reception time of the message. If this value is not desired, this parameter
    ///   should be passed as NULL
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_Read(
        Channel: TPCANHandle, 
        MessageBuffer: *mut TPCANMsg, 
        TimestampBuffer: *mut TPCANTimestamp) -> TPCANStatus;

    /// Reads a CAN message from the receive queue of a FD capable PCAN Channel 
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a FD capable PCAN Channel
    /// * `MessageBuffer` - A TPCANMsgFD structure buffer to store the CAN message
    /// * `TimestampBuffer` - A TPCANTimestampFD buffer to get 
    ///   the reception time of the message. If this value is not desired, this parameter
    ///   should be passed as NULL
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_ReadFD(
        Channel: TPCANHandle,
        MessageBuffer: *mut TPCANMsgFD, 
        TimestampBuffer: *mut TPCANTimestampFD) -> TPCANStatus;

    /// Reads a CAN message from the receive queue of a XL capable PCAN Channel 
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a XL capable PCAN Channel
    /// * `MessageBuffer` - A TPCANMsgXL structure buffer to store the CAN message
    /// * `TimestampBuffer` - A TPCANTimestampXL buffer to get 
    ///   the reception time of the message. If this value is not desired, this parameter
    ///   should be passed as NULL
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_ReadXL(
        Channel: TPCANHandle,
        MessageBuffer: *mut TPCANMsgXL,
        TimestampBuffer: *mut TPCANTimestampXL) -> TPCANStatus;

    /// Transmits a CAN message 
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a PCAN Channel
    /// * `MessageBuffer` - A TPCANMsg buffer with the message to be sent
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_Write(
        Channel: TPCANHandle, 
        MessageBuffer: *mut TPCANMsg) -> TPCANStatus;

    /// Transmits a CAN message over a FD capable PCAN Channel
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a FD capable PCAN Channel
    /// * `MessageBuffer` - A TPCANMsgFD buffer with the message to be sent
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_WriteFD(
        Channel: TPCANHandle,
        MessageBuffer: *mut TPCANMsgFD) -> TPCANStatus;

    /// Transmits a CAN message over a XL capable PCAN Channel
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a XL capable PCAN Channel
    /// * `MessageBuffer` - A TPCANMsgXL buffer with the message to be sent
    fn CAN_WriteXL(
        Channel: TPCANHandle,
        MessageBuffer: *mut TPCANMsgXL) -> TPCANStatus;

    /// Configures the reception filter. 
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a PCAN Channel
    /// * `FromID` - The lowest CAN ID to be received
    /// * `ToID` - The highest CAN ID to be received
    /// * `Mode` - Message type, Standard (11-bit identifier) or 
    ///   Extended (29-bit identifier)
    ///
    /// # Remarks
    ///
    /// The message filter will be expanded with every call to 
    /// this function. If it is desired to reset the filter, please use 
    /// the CAN_SetValue function
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_FilterMessages(
        Channel: TPCANHandle, 
        FromID: u32, 
        ToID: u32, 
        Mode: TPCANMode) -> TPCANStatus;

    /// Retrieves a PCAN Channel value
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a PCAN Channel
    /// * `Parameter` - The TPCANParameter parameter to get
    /// * `Buffer` - Buffer for the parameter value
    /// * `BufferLength` - Size in bytes of the buffer
    ///
    /// # Remarks
    ///
    /// Parameters can be present or not according with the kind 
    /// of Hardware (PCAN Channel) being used. If a parameter is not available,
    /// a PCAN_ERROR_ILLPARAMTYPE error will be returned
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_GetValue(
        Channel: TPCANHandle, 
        Parameter: TPCANParameter,  
        Buffer: *mut c_void, 
        BufferLength: u32) -> TPCANStatus;

    /// Configures or sets a PCAN Channel value 
    ///
    /// # Arguments
    ///
    /// * `Channel` - The handle of a PCAN Channel
    /// * `Parameter` - The TPCANParameter parameter to set
    /// * `Buffer` - Buffer with the value to be set
    /// * `BufferLength` - Size in bytes of the buffer
    ///
    /// # Remarks
    ///
    /// Parameters can be present or not according with the kind 
    /// of Hardware (PCAN Channel) being used. If a parameter is not available,
    /// a PCAN_ERROR_ILLPARAMTYPE error will be returned
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_SetValue(
        Channel: TPCANHandle,
        Parameter: TPCANParameter,
        Buffer: *mut c_void,
        BufferLength: u32) -> TPCANStatus;

    /// Returns a descriptive text of a given TPCANStatus error 
    /// code, in any desired language
    ///
    /// # Arguments
    ///
    /// * `Error` - A TPCANStatus error code
    /// * `Language` - Indicates a 'Primary language ID'
    /// * `Buffer` - Buffer for a null terminated char array
    ///
    /// # Remarks
    ///
    /// The current languages available for translation are: 
    /// Neutral (0x00), German (0x07), English (0x09), Spanish (0x0A),
    /// Italian (0x10) and French (0x0C)
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_GetErrorText(
        Error: TPCANStatus, 
        Language: u16, 
        Buffer: *mut c_char) -> TPCANStatus;

    /// Finds a PCAN-Basic channel that matches with the given parameters
    ///
    /// # Arguments
    ///
    /// * `Parameters` - A comma separated string contained pairs of 
    ///   parameter-name/value to be matched within a PCAN-Basic channel
    /// * `FoundChannel` - Buffer for returning the PCAN-Basic channel, 
    ///   when found
    ///
    /// # Returns
    ///
    /// A TPCANStatus error code
    fn CAN_LookUpChannel(
        Parameters: *const c_char, 
        FoundChannel: *mut TPCANHandle) -> TPCANStatus;
}

/// Initializes a PCAN Channel 
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
/// * `Btr0Btr1` - The speed for the communication (BTR0BTR1 code)
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn Initialize(Channel: TPCANHandle, Btr0Btr1: TPCANBaudrate) -> Result<(), TPCANStatus> {
    let status = unsafe { CAN_Initialize(Channel, Btr0Btr1, 0, 0, 0) };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Initializes a FD capable PCAN Channel  
///
/// # Arguments
///
/// * `Channel` - The handle of a FD capable PCAN Channel
/// * `BitrateFD` - The speed for the communication (FD bit rate string)
///
/// # Remarks
///
/// See PCAN_BR_* values
/// * Parameter and values must be separated by '='
/// * Couples of Parameter/value must be separated by ','
/// * Following Parameter must be filled out: f_clock, data_brp, data_sjw, data_tseg1, data_tseg2,
/// nom_brp, nom_sjw, nom_tseg1, nom_tseg2.
/// * Following Parameters are optional (not used yet): data_ssp_offset, nom_sam
///
/// # Examples
///
/// f_clock=80000000,nom_brp=10,nom_tseg1=5,nom_tseg2=2,nom_sjw=1,data_brp=4,data_tseg1=7,data_tseg2=2,data_sjw=1
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn InitializeFD(Channel: TPCANHandle, BitrateFD: &TPCANBitrateFD) -> Result<(), TPCANStatus> {
    let c_string = match CString::new(BitrateFD) {
        Ok(s) => s,
        Err(_) => return Err(PCAN_ERROR_UNKNOWN),
    };

    let status = unsafe { CAN_InitializeFD(Channel, c_string.as_ptr()) };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Initializes a XL capable PCAN Channel  
///
/// # Arguments
///
/// * `Channel` - The handle of a XL capable PCAN Channel
/// * `BitrateXL` - The speed for the communication (XL bit rate string)
///
/// # Remarks
///
/// See PCAN_BR_* values
/// * Parameter and values must be separated by '='
/// * Couples of Parameter/value must be separated by ','
/// * Following Parameter must be filled out: f_clock, brp, nom_brp, nom_sjw, nom_tseg1, nom_tseg2. 
/// If xl_transceiver_mode_switch is active, also the parameters xl_sjw, xl_tseg1, and xl_tseg2, must be present.
/// If error_signaling is active, also the parameters fd_sjw, fd_tseg1, and fd_tseg2, must be present. 
/// * Following Parameters are optional: fd_ssp_offset, xl_ssp_offset, xl_transceiver_mode_switch, error_signaling,
/// xl_pwm_offset, xl_pwm_short, and xl_pwm_long
///
/// # Examples
///
/// f_clock=160000000,brp=1,nom_tseg1=255,nom_tseg2=64,nom_sjw=64,fd_tseg1=63,fd_tseg2=16,fd_sjw=16,fd_ssp_offset=0,xl_tseg1=10,
/// xl_tseg2=9,xl_sjw=9,xl_ssp_offset=10,xl_error_signaling=1,xl_transceiver_mode_switch=0
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn InitializeXL(Channel: TPCANHandle, BitrateXL: &TPCANBitrateXL) -> Result<(), TPCANStatus> {
    let c_string = match CString::new(BitrateXL) {
        Ok(s) => s,
        Err(_) => return Err(PCAN_ERROR_UNKNOWN),
    };

    let status = unsafe { CAN_InitializeXL(Channel, c_string.as_ptr()) };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Uninitializes one or all PCAN Channels initialized by CAN_Initialize
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
///
/// # Remarks
///
/// Giving the TPCANHandle value "PCAN_NONEBUS", 
/// uninitialize all initialized channels
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn Uninitialize(Channel: TPCANHandle) -> Result<(), TPCANStatus> {
    let status = unsafe { CAN_Uninitialize(Channel) };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Resets the receive and transmit queues of the PCAN Channel  
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
///
/// # Remarks
///
/// A reset of the CAN controller is not performed.
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn Reset(Channel: TPCANHandle) -> Result<(), TPCANStatus> {
    let status = unsafe { CAN_Reset(Channel) };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Gets the current status of a PCAN Channel 
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
///
/// # Returns
///
/// Ok with the current status on success, or a TPCANStatus error code
pub fn GetStatus(Channel: TPCANHandle) -> Result<TPCANStatus, TPCANStatus> {
    let status = unsafe { CAN_GetStatus(Channel) };

    if status == PCAN_ERROR_OK { Ok(status) } else { Err(status) }
}

/// Reads a CAN message from the receive queue of a PCAN Channel 
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
///
/// # Returns
///
/// Ok with a CAN message and timestamp on success, or a TPCANStatus error code
pub fn Read(Channel: TPCANHandle) -> Result<(TPCANMsg, TPCANTimestamp), TPCANStatus> {
    let mut message_buffer: TPCANMsg = TPCANMsg {
        ID: 0,
        MSGTYPE: 0,
        LEN: 0,
        DATA: [0; 8],
    };

    let mut timestamp: TPCANTimestamp = TPCANTimestamp {
        millis: 0,
        millis_overflow: 0,
        micros: 0,
    };

    let status = unsafe { CAN_Read(Channel, &mut message_buffer as *mut TPCANMsg, &mut timestamp as *mut TPCANTimestamp) };

    if status == PCAN_ERROR_OK { Ok((message_buffer, timestamp)) } else { Err(status) }
}

/// Reads a CAN message from the receive queue of a FD capable PCAN Channel 
///
/// # Arguments
///
/// * `Channel` - The handle of a FD capable PCAN Channel
///
/// # Returns
///
/// Ok with a CAN message and timestamp on success, or a TPCANStatus error code
pub fn ReadFD(Channel: TPCANHandle) -> Result<(TPCANMsgFD, TPCANTimestampFD), TPCANStatus> {
    let mut message_buffer: TPCANMsgFD = TPCANMsgFD {
        ID: 0,
        MSGTYPE: 0,
        DLC: 0,
        DATA: [0; 64],
    };

    let mut timestamp: TPCANTimestampFD = 0;
    let status = unsafe { CAN_ReadFD(Channel, &mut message_buffer as *mut TPCANMsgFD, &mut timestamp as *mut TPCANTimestampFD) };

    if status == PCAN_ERROR_OK { Ok((message_buffer, timestamp)) } else { Err(status) }
}

/// Reads a CAN message from the receive queue of a XL capable PCAN Channel 
///
/// # Arguments
///
/// * `Channel` - The handle of a XL capable PCAN Channel
///
/// # Returns
///
/// Ok with a CAN message and timestamp on success, or a TPCANStatus error code
pub fn ReadXL(Channel: TPCANHandle) -> Result<(TPCANMsgXL, TPCANTimestampXL), TPCANStatus> {
    let mut message_buffer: TPCANMsgXL = TPCANMsgXL {
        PID: 0,
        VCID: 0,
        AF : 0,
        RRS: 0,
        SEC: 0,
        SDT: 0,
        MSGTYPE: 0,
        DLC: 0,
        DATA: [0; 2048],
    };

    let mut timestamp: TPCANTimestampXL = 0;
    let status = unsafe { CAN_ReadXL(Channel, &mut message_buffer as *mut TPCANMsgXL, &mut timestamp as *mut TPCANTimestampXL) };

    if status == PCAN_ERROR_OK { Ok((message_buffer, timestamp)) } else { Err(status) }
}

/// Transmits a CAN message 
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
/// * `MessageBuffer` - A TPCANMsg buffer with the message to be sent
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn Write(Channel: TPCANHandle, MessageBuffer: &mut TPCANMsg) -> Result<(), TPCANStatus> {
    let status = unsafe { CAN_Write(Channel, MessageBuffer as *mut TPCANMsg) };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Transmits a CAN message over a FD capable PCAN Channel
///
/// # Arguments
///
/// * `Channel` - The handle of a FD capable PCAN Channel
/// * `MessageBuffer` - A TPCANMsgFD buffer with the message to be sent
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn WriteFD(Channel: TPCANHandle, MessageBuffer: &mut TPCANMsgFD) -> Result<(), TPCANStatus> {
    let status = unsafe { CAN_WriteFD(Channel, MessageBuffer as *mut TPCANMsgFD) };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Transmits a CAN message over a XL capable PCAN Channel
///
/// # Arguments
///
/// * `Channel` - The handle of a XL capable PCAN Channel
/// * `MessageBuffer` - A TPCANMsgXL buffer with the message to be sent
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn WriteXL(Channel: TPCANHandle, MessageBuffer: &mut TPCANMsgXL) -> Result<(), TPCANStatus> {
    let status = unsafe { CAN_WriteXL(Channel, MessageBuffer as *mut TPCANMsgXL) };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Configures the reception filter. 
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
/// * `FromID` - The lowest CAN ID to be received
/// * `ToID` - The highest CAN ID to be received
/// * `Mode` - Message type, Standard (11-bit identifier) or 
///   Extended (29-bit identifier)
///
/// # Remarks
///
/// The message filter will be expanded with every call to 
/// this function. If it is desired to reset the filter, please use 
/// the SetValue function
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn FilterMessages(Channel: TPCANHandle, FromID: u32, ToID: u32, Mode: TPCANMode) -> Result<(), TPCANStatus> {
    let status = unsafe { CAN_FilterMessages(Channel, FromID, ToID, Mode) };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Retrieves a PCAN Channel value
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
/// * `Parameter` - The TPCANParameter parameter to get
///
/// # Remarks
///
/// Parameters can be present or not according with the kind 
/// of Hardware (PCAN Channel) being used. If a parameter is not available,
/// a PCAN_ERROR_ILLPARAMTYPE error will be returned
///
/// # Returns
///
/// Ok with the parameter value on success, or a TPCANStatus error code
pub fn GetValueInt32(Channel: TPCANHandle, Parameter: TPCANParameter) -> Result<u32, TPCANStatus> {
    let mut buffer: u32 = 0;

    let status = unsafe {
        CAN_GetValue(
            Channel,
            Parameter,
            &mut buffer as *mut u32 as *mut c_void,
            std::mem::size_of::<u32>() as u32)
    };

    if status == PCAN_ERROR_OK { Ok(buffer) } else { Err(status) }
}

/// Retrieves a PCAN Channel value
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
/// * `Parameter` - The TPCANParameter parameter to get
///
/// # Remarks
///
/// Parameters can be present or not according with the kind 
/// of Hardware (PCAN Channel) being used. If a parameter is not available,
/// a PCAN_ERROR_ILLPARAMTYPE error will be returned
///
/// # Returns
///
/// Ok with the parameter value on success, or a TPCANStatus error code
pub fn GetValueInt64(Channel: TPCANHandle, Parameter: TPCANParameter) -> Result<u64, TPCANStatus> {
    let mut buffer: u64 = 0;

    let status = unsafe {
        CAN_GetValue(
            Channel,
            Parameter,
            &mut buffer as *mut u64 as *mut c_void,
            std::mem::size_of::<u64>() as u32)
    };

    if status == PCAN_ERROR_OK { Ok(buffer) } else { Err(status) }
}

/// Retrieves a PCAN Channel value
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
/// * `Parameter` - The TPCANParameter parameter to get
///
/// # Remarks
///
/// Parameters can be present or not according with the kind 
/// of Hardware (PCAN Channel) being used. If a parameter is not available,
/// a PCAN_ERROR_ILLPARAMTYPE error will be returned
///
/// # Returns
///
/// Ok with the parameter value on success, or a TPCANStatus error code
pub fn GetValueString(Channel: TPCANHandle, Parameter: TPCANParameter) -> Result<String, TPCANStatus> {
    let mut buffer: [c_char; 256] = [0; 256];

    let status = unsafe {
        CAN_GetValue(
            Channel,
            Parameter,
            buffer.as_mut_ptr() as *mut c_void,
            buffer.len() as u32)
    };

    if status == PCAN_ERROR_OK {
        let c_str = unsafe { CStr::from_ptr(buffer.as_ptr()) };
        match c_str.to_str() {
            Ok(s) => Ok(s.to_string()),
            Err(_) => Err(PCAN_ERROR_UNKNOWN),
        }
    } else {
        Err(status)
    }
}

/// Retrieves a PCAN Channel value
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
/// * `Parameter` - The TPCANParameter parameter to get
/// * `Size` - The number of TPCANChannelInformation entries to retrieve
///
/// # Remarks
///
/// Parameters can be present or not according with the kind 
/// of Hardware (PCAN Channel) being used. If a parameter is not available,
/// a PCAN_ERROR_ILLPARAMTYPE error will be returned
///
/// # Returns
///
/// Ok with a vector of channel information on success, or a TPCANStatus error code
pub fn GetValueVector(Channel: TPCANHandle, Parameter: TPCANParameter, Size: usize) -> Result<Vec<TPCANChannelInformation>, TPCANStatus> {
    let mut buffer: Vec<TPCANChannelInformation> = Vec::with_capacity(Size);

    let status = unsafe {
        CAN_GetValue(
            Channel,
            Parameter,
            buffer.as_mut_ptr() as *mut c_void,
            (std::mem::size_of::<TPCANChannelInformation>() * Size) as u32,
        )
    };

    if status == PCAN_ERROR_OK {
        unsafe { buffer.set_len(Size) };
        Ok(buffer)
    } else {
        Err(status)
    }
}

/// Configures or sets a PCAN Channel value 
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
/// * `Parameter` - The TPCANParameter parameter to set
/// * `Buffer` - Buffer with the value to be set
///
/// # Remarks
///
/// Parameters can be present or not according with the kind 
/// of Hardware (PCAN Channel) being used. If a parameter is not available,
/// a PCAN_ERROR_ILLPARAMTYPE error will be returned
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn SetValueInt32(Channel: TPCANHandle, Parameter: TPCANParameter, Buffer: &u32) -> Result<(), TPCANStatus> {
    let status = unsafe {
        CAN_SetValue(
            Channel,
            Parameter,
            Buffer as *const u32 as *mut c_void,
            std::mem::size_of::<u32>() as u32)
    };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Configures or sets a PCAN Channel value 
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
/// * `Parameter` - The TPCANParameter parameter to set
/// * `Buffer` - Buffer with the value to be set
///
/// # Remarks
///
/// Parameters can be present or not according with the kind 
/// of Hardware (PCAN Channel) being used. If a parameter is not available,
/// a PCAN_ERROR_ILLPARAMTYPE error will be returned
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn SetValueInt64(Channel: TPCANHandle, Parameter: TPCANParameter, Buffer: &u64) -> Result<(), TPCANStatus> {
    let status = unsafe {
        CAN_SetValue(
            Channel,
            Parameter,
            Buffer as *const u64 as *mut c_void,
            std::mem::size_of::<u64>() as u32)
    };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Configures or sets a PCAN Channel value 
///
/// # Arguments
///
/// * `Channel` - The handle of a PCAN Channel
/// * `Parameter` - The TPCANParameter parameter to set
/// * `Buffer` - Buffer with the value to be set
///
/// # Remarks
///
/// Parameters can be present or not according with the kind 
/// of Hardware (PCAN Channel) being used. If a parameter is not available,
/// a PCAN_ERROR_ILLPARAMTYPE error will be returned
///
/// # Returns
///
/// Ok on success, or a TPCANStatus error code
pub fn SetValueString(Channel: TPCANHandle, Parameter: TPCANParameter, Buffer: &str) -> Result<(), TPCANStatus> {
    let c_string = match CString::new(Buffer) {
        Ok(s) => s,
        Err(_) => return Err(PCAN_ERROR_UNKNOWN),
    };

    let status = unsafe {
        CAN_SetValue(
            Channel,
            Parameter,
            c_string.as_ptr() as *mut c_void,
            (c_string.as_bytes_with_nul().len()) as u32)
    };

    if status == PCAN_ERROR_OK { Ok(()) } else { Err(status) }
}

/// Returns a descriptive text of a given TPCANStatus error 
/// code, in any desired language
///
/// # Arguments
///
/// * `Error` - A TPCANStatus error code
/// * `Language` - Indicates a 'Primary language ID'
///
/// # Remarks
///
/// The current languages available for translation are: 
/// Neutral (0x00), German (0x07), English (0x09), Spanish (0x0A),
/// Italian (0x10) and French (0x0C)
///
/// # Returns
///
/// Ok with the error description on success, or a TPCANStatus error code
pub fn GetErrorText(Error: TPCANStatus, Language: u16) -> Result<String, TPCANStatus> {
    let mut buffer: [c_char; 256] = [0; 256];

    let status = unsafe { CAN_GetErrorText(Error, Language, buffer.as_mut_ptr()) };

    if status == PCAN_ERROR_OK {
        let c_str = unsafe { CStr::from_ptr(buffer.as_ptr()) };
        match c_str.to_str() {
            Ok(s) => Ok(s.to_string()),
            Err(_) => Err(PCAN_ERROR_UNKNOWN),
        }
    } else {
        Err(status)
    }
}

/// Finds a PCAN-Basic channel that matches with the given parameters
///
/// # Arguments
///
/// * `Parameters` - A comma separated string contained pairs of 
///   parameter-name/value to be matched within a PCAN-Basic channel
///
/// # Returns
///
/// Ok with the found channel on success, or a TPCANStatus error code
pub fn LookUpChannel(Parameters: &str) -> Result<TPCANHandle, TPCANStatus> {
    let c_string = match CString::new(Parameters) {
        Ok(s) => s,
        Err(_) => return Err(PCAN_ERROR_UNKNOWN),
    };

    let mut found_channel: TPCANHandle = 0;
    let status = unsafe { CAN_LookUpChannel(c_string.as_ptr(), &mut found_channel as *mut TPCANHandle) };

    if status == PCAN_ERROR_OK { Ok(found_channel) } else { Err(status) }
}
