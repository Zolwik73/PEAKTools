use crate::PCANBasic::*;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use std::thread;
use std::time::Duration;

// Enum for connection modes
enum CANProtocol {
    CAN,
    CAN_FD,
    CAN_XL,
}

/// Sets the PCANHandle (Hardware Channel)
const PCAN_HANDLE: TPCANHandle = PCAN_USBBUS1;
/// Sets the desired connection protocol
const PROTOCOL: CANProtocol = CANProtocol::CAN;
/// Sets the bitrate for normal CAN devices
const BITRATE: TPCANBaudrate = PCAN_BAUD_500K;
/// Sets the bitrate for CAN FD devices.
const BITRATE_FD: &str = "f_clock_mhz=20, nom_brp=5, nom_tseg1=2, nom_tseg2=1, nom_sjw=1, data_brp=2, data_tseg1=3, data_tseg2=1, data_sjw=1";
/// Sets the bitrate for CAN XL devices.
const BITRATE_XL: &str = "f_clock=160000000, brp=1, nom_tseg1=255, nom_tseg2=64, nom_sjw=64, fd_tseg1=63, fd_tseg2=16, fd_sjw=16, fd_ssp_offset=0, xl_tseg1=10, xl_tseg2=9, xl_sjw=9, xl_ssp_offset=10, xl_error_signaling=1, xl_transceiver_mode_switch=0";
/// Sets if trace continue after reaching maximum size for the first file
const TRACE_FILE_SINGLE_MODE: bool = true;
/// Set if date will be add to filename
const TRACE_FILE_DATE_MODE: bool = true;
/// Set if time will be add to filename
const TRACE_FILE_TIME_MODE: bool = true;
/// Set if existing tracefile overwrites when a new trace session is started
const TRACE_FILE_OVERWRITE_MODE: bool = false;
/// Set if the column "Data Length" should be used instead of the column "Data Length Code"
const TRACE_FILE_DATA_LENGTH_MODE: bool = false;
/// Sets the size (megabyte) of an tracefile
const TRACE_FILE_SIZE: u32 = 2;
/// Sets a fully-qualified and valid path to an existing directory.
const TRACE_PATH: &str = "";
/// Timerinterval (ms) for reading
const TIMER_INTERVAL: u64 = 250;

/// Starts the PCANBasic Sample
pub fn run() {
    show_configuration_help();
    show_current_configuration();

    let init_result = match PROTOCOL {
        CANProtocol::CAN_XL => InitializeXL(PCAN_HANDLE, BITRATE_XL),
        CANProtocol::CAN_FD => InitializeFD(PCAN_HANDLE, BITRATE_FD),
        CANProtocol::CAN => Initialize(PCAN_HANDLE, BITRATE),
    };
    if let Err(status) = init_result {
        println!("Can not initialize. Please check the defines in the code.");
        show_status(status);
        return;
    }

    println!("Successfully initialized.");
    println!("Press Enter to start tracing...");
    let _ = std::io::stdin().read_line(&mut String::new());
    if configure_trace() && start_trace() {
        let running = Arc::new(AtomicBool::new(true));
        let running_thread = running.clone();
        let reader = thread::spawn(move || {
            while running_thread.load(Ordering::Relaxed) {
                read_messages();
                thread::sleep(Duration::from_millis(TIMER_INTERVAL));
            }
        });

        println!("Messages are being traced.");
        println!("Press Enter to stop trace and quit");
        let _ = std::io::stdin().read_line(&mut String::new());
        running.store(false, Ordering::Relaxed);
        let _ = reader.join();
        stop_trace();
    }

    let _ = Uninitialize(PCAN_NONEBUS);
}

/// Function for reading PCAN-Basic messages
fn read_messages() {
    // We read at least one time the queue looking for messages. If a message is found, we look again trying to
    // find more. If the queue is empty or an error occur, we get out from the do-while statement.
    loop {
        let result = match PROTOCOL {
            CANProtocol::CAN_XL => ReadXL(PCAN_HANDLE).map(|_| ()),
            CANProtocol::CAN_FD => ReadFD(PCAN_HANDLE).map(|_| ()),
            CANProtocol::CAN => Read(PCAN_HANDLE).map(|_| ()),
        };

        match result {
            Ok(_) => continue,
            Err(sts) if sts == PCAN_ERROR_QRCVEMPTY => break,
            Err(sts) => {
                show_status(sts);
                return;
            }
        }
    }
}

/// Stops tracing
fn stop_trace() {
    let status = PCAN_PARAMETER_OFF as u32;
    if let Err(sts) = SetValueInt32(PCAN_HANDLE, PCAN_TRACE_STATUS, &status) {
        show_status(sts);
    }
}

/// Configures trace parameters
fn configure_trace() -> bool {
    let size = TRACE_FILE_SIZE;

    // Sets path to store files
    if let Err(sts) = SetValueString(PCAN_HANDLE, PCAN_TRACE_LOCATION, TRACE_PATH) {
        show_status(sts);
        return false;
    }

    // Sets the maximum size of a tracefile
    if let Err(sts) = SetValueInt32(PCAN_HANDLE, PCAN_TRACE_SIZE, &size) {
        show_status(sts);
        return false;
    }

    let mut config: u32 = if TRACE_FILE_SINGLE_MODE { TRACE_FILE_SINGLE as u32 } else { TRACE_FILE_SEGMENTED as u32 };

    // Activate overwriting existing tracefile
    if TRACE_FILE_OVERWRITE_MODE { config |= TRACE_FILE_OVERWRITE as u32; }
    // Uses Data Length instead of Data Length Code
    if TRACE_FILE_DATA_LENGTH_MODE { config |= TRACE_FILE_DATA_LENGTH as u32; }
    // Adds date to tracefilename
    if TRACE_FILE_DATE_MODE { config |= TRACE_FILE_DATE as u32; }
    // Adds time to tracefilename
    if TRACE_FILE_TIME_MODE { config |= TRACE_FILE_TIME as u32; }

    if let Err(sts) = SetValueInt32(PCAN_HANDLE, PCAN_TRACE_CONFIGURE, &config) {
        show_status(sts);
        return false;
    }

    true
}

/// Starts tracing
fn start_trace() -> bool {
    let status = PCAN_PARAMETER_ON as u32;
    if let Err(sts) = SetValueInt32(PCAN_HANDLE, PCAN_TRACE_STATUS, &status) {
        show_status(sts);
        return false;
    }
    true
}

/// Shows/prints the configurable parameters for this sample and information about them
fn show_configuration_help() {
    println!("=========================================================================================");
    println!("|                           PCAN-Basic TraceFiles Example                                |");
    println!("=========================================================================================");
    println!("Following parameters are to be adjusted before launching, according to the hardware used |");
    println!("                                                                                         |");
    println!("* PcanHandle: Numeric value that represents the handle of the PCAN-Basic channel to use. |");
    println!("              See 'PCAN-Handle Definitions' within the documentation                     |");
    println!("* Protocol: Enum value that indicates the communication protocol, CAN, CAN-FD or CAN-XL. |");
    println!("* Bitrate: Numeric value that represents the BTR0/BR1 bitrate value to be used for CAN   |");
    println!("           communication                                                                 |");
    println!("* BitrateFD: String value that represents the nominal/FD bitrate value to be used for    |");
    println!("             CAN-FD communication                                                        |");
    println!("* BitrateXL: String value that represents the nominal/FD/XL bitrate value to be used for |");
    println!("             CAN-XL communication                                                        |");
    println!("* TraceFileSingle: Boolean value that indicates if tracing ends after one file (true) or |");
    println!("                   continues                                                             |");
    println!("* TraceFileDate: Boolean value that indicates if the date will be added to filename      |");
    println!("* TraceFileTime: Boolean value that indicates if the time will be added to filename      |");
    println!("* TraceFileOverwrite: Boolean value that indicates if existing tracefiles should be      |");
    println!("                      overwritten                                                        |");
    println!("* TraceFileDataLength: Boolean value that indicates if the column 'Data Length' is used  |");
    println!("                       instead of the column 'Data Length Code'                          |");
    println!("* TraceFileSize: Numeric value that represents the size of a tracefile in meagabytes     |");
    println!("* TracePath: string value that represents a valid path to an existing directory          |");
    println!("* TimerInterval: The time, in milliseconds, to wait before trying to write a message     |");
    println!("=========================================================================================");
    println!();
}

/// Shows/prints the configured parameters
fn show_current_configuration() {
    println!("Parameter values used");
    println!("----------------------");
    println!("* PCANHandle: {}", format_channel_name(PCAN_HANDLE, &PROTOCOL));
    println!("* Protocol: {}", protocol_to_string(&PROTOCOL));
    println!("* Bitrate: {}", convert_bitrate_to_string(BITRATE));
    println!("* BitrateFD: {}", BITRATE_FD);
    println!("* BitrateXL: {}", BITRATE_XL);
    println!("* TraceFileSingle: {}", TRACE_FILE_SINGLE_MODE);
    println!("* TraceFileDate: {}", TRACE_FILE_DATE_MODE);
    println!("* TraceFileTime: {}", TRACE_FILE_TIME_MODE);
    println!("* TraceFileOverwrite: {}", TRACE_FILE_OVERWRITE_MODE);
    println!("* TraceFileDataLength: {}", TRACE_FILE_DATA_LENGTH_MODE);
    println!("* TraceFileSize: {} MB", TRACE_FILE_SIZE);
    if TRACE_PATH.is_empty() {
        println!("* TracePath: (calling application path)");
    } else {
        println!("* TracePath: {}", TRACE_PATH);
    }
    println!("* TimerInterval: {}", TIMER_INTERVAL);
    println!();
}

/// Shows formatted status
fn show_status(status: TPCANStatus) {
    println!("=========================================================================================");
    println!("{}", get_formatted_error(status));
    println!("=========================================================================================");
}

/// Help Function used to get an error as text
fn get_formatted_error(error: TPCANStatus) -> String {
    GetErrorText(error, 0x09)
        .unwrap_or_else(|_| format!("An error occurred. Error-code's text ({:X}) couldn't be retrieved", error))
}

/// Convert bitrate c_short value to readable string
fn convert_bitrate_to_string(bitrate: TPCANBaudrate) -> &'static str {
    match bitrate {
        PCAN_BAUD_1M => "1 MBit/sec",
        PCAN_BAUD_800K => "800 kBit/sec",
        PCAN_BAUD_500K => "500 kBit/sec",
        PCAN_BAUD_250K => "250 kBit/sec",
        PCAN_BAUD_125K => "125 kBit/sec",
        PCAN_BAUD_100K => "100 kBit/sec",
        PCAN_BAUD_95K => "95,238 kBit/sec",
        PCAN_BAUD_83K => "83,333 kBit/sec",
        PCAN_BAUD_50K => "50 kBit/sec",
        PCAN_BAUD_47K => "47,619 kBit/sec",
        PCAN_BAUD_33K => "33,333 kBit/sec",
        PCAN_BAUD_20K => "20 kBit/sec",
        PCAN_BAUD_10K => "10 kBit/sec",
        PCAN_BAUD_5K => "5 kBit/sec",
        _ => "Unknown Bitrate",
    }
}

/// Gets the formatted text for a PCAN-Basic channel handle
fn format_channel_name(handle: TPCANHandle, protocol: &CANProtocol) -> String {
    let by_channel: u8 = if handle < 0x100 {
        (handle & 0xF) as u8
    } else {
        (handle & 0xFF) as u8
    };

    let handle_name = get_tpcan_handle_name(handle);
    match protocol {
        CANProtocol::CAN_XL => format!("{}:XL {} ({:X}h)", handle_name, by_channel, handle),
        CANProtocol::CAN_FD => format!("{}:FD {} ({:X}h)", handle_name, by_channel, handle),
        CANProtocol::CAN => format!("{} {} ({:X}h)", handle_name, by_channel, handle),
    }
}

fn get_tpcan_handle_name(handle: TPCANHandle) -> &'static str {
    match handle {
        PCAN_PCIBUS1 | PCAN_PCIBUS2 | PCAN_PCIBUS3 | PCAN_PCIBUS4 | PCAN_PCIBUS5 | PCAN_PCIBUS6 | PCAN_PCIBUS7
        | PCAN_PCIBUS8 | PCAN_PCIBUS9 | PCAN_PCIBUS10 | PCAN_PCIBUS11 | PCAN_PCIBUS12 | PCAN_PCIBUS13
        | PCAN_PCIBUS14 | PCAN_PCIBUS15 | PCAN_PCIBUS16 => "PCAN_PCI",
        PCAN_USBBUS1 | PCAN_USBBUS2 | PCAN_USBBUS3 | PCAN_USBBUS4 | PCAN_USBBUS5 | PCAN_USBBUS6 | PCAN_USBBUS7
        | PCAN_USBBUS8 | PCAN_USBBUS9 | PCAN_USBBUS10 | PCAN_USBBUS11 | PCAN_USBBUS12 | PCAN_USBBUS13
        | PCAN_USBBUS14 | PCAN_USBBUS15 | PCAN_USBBUS16 => "PCAN_USB",
        PCAN_LANBUS1 | PCAN_LANBUS2 | PCAN_LANBUS3 | PCAN_LANBUS4 | PCAN_LANBUS5 | PCAN_LANBUS6 | PCAN_LANBUS7
        | PCAN_LANBUS8 | PCAN_LANBUS9 | PCAN_LANBUS10 | PCAN_LANBUS11 | PCAN_LANBUS12 | PCAN_LANBUS13
        | PCAN_LANBUS14 | PCAN_LANBUS15 | PCAN_LANBUS16 => "PCAN_LAN",
        _ => "UNKNOWN",
    }
}

fn protocol_to_string(protocol: &CANProtocol) -> &'static str {
    match protocol {
        CANProtocol::CAN => "CAN",
        CANProtocol::CAN_FD => "CAN FD",
        CANProtocol::CAN_XL => "CAN XL",
    }
}

