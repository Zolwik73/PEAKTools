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
    let running = Arc::new(AtomicBool::new(true));
    let running_thread = running.clone();
    let read_thread = thread::spawn(move || {
        while running_thread.load(Ordering::Relaxed) {
            // Thread.Sleep(1); //Use Sleep to reduce the CPU load
            read_messages();
            thread::sleep(Duration::from_millis(1));
        }
    });

    println!("Started reading messages...");
    println!();
    println!("Press Enter to close");
    let _ = std::io::stdin().read_line(&mut String::new());
    running.store(false, Ordering::Relaxed);
    let _ = read_thread.join();
    let _ = Uninitialize(PCAN_NONEBUS);
}

/// Function for reading PCAN-Basic messages
fn read_messages() {
    // We read at least one time the queue looking for messages. If a message is found, we look again trying to
    // find more. If the queue is empty or an error occur, we get out from the do-while statement.
    loop {
        let status = match PROTOCOL {
            CANProtocol::CAN_XL => read_message_xl(),
            CANProtocol::CAN_FD => read_message_fd(),
            CANProtocol::CAN => read_message(),
        };
        match status {
            Ok(_) => continue,
            Err(sts) if sts == PCAN_ERROR_QRCVEMPTY => break,
            Err(sts) => {
                show_status(sts);
                return;
            }
        }
    }
}

/// Function for reading messages on CAN-XL devices
fn read_message_xl() -> Result<(), TPCANStatus> {
    // We execute the "Read" function of the PCANBasic
    match ReadXL(PCAN_HANDLE) {
        Ok((m, t)) => {
            // We process the received message
            process_message_can_xl(m, t);
            Ok(())
        }
        Err(s) => Err(s),
    }
}

/// Function for reading messages on FD devices
fn read_message_fd() -> Result<(), TPCANStatus> {
    // We execute the "Read" function of the PCANBasic
    match ReadFD(PCAN_HANDLE) {
        Ok((m, t)) => {
            // We process the received message
            process_message_can_fd(m, t);
            Ok(())
        }
        Err(s) => Err(s),
    }
}

/// Function for reading CAN messages on normal CAN devices
fn read_message() -> Result<(), TPCANStatus> {
    // We execute the "Read" function of the PCANBasic
    match Read(PCAN_HANDLE) {
        Ok((m, t)) => {
            // We process the received message
            process_message_can(m, t);
            Ok(())
        }
        Err(s) => Err(s),
    }
}

/// Processes a received CAN message
fn process_message_can(msg: TPCANMsg, timestamp: TPCANTimestamp) {
    let micros_timestamp = (timestamp.micros as u64) + (1000_u64 * timestamp.millis as u64) + (0x100_000_000_u64 * 1000_u64 * timestamp.millis_overflow as u64);
    println!("Type: {}", get_msg_type_string(msg.MSGTYPE));
    println!("ID: {}", get_id_string(msg.ID, msg.MSGTYPE));
    println!("Length: {}", msg.LEN);
    println!("Time: {}", get_time_string(micros_timestamp));
    println!("Data: {}", get_data_string(&msg.DATA, msg.MSGTYPE, msg.LEN as usize));
    println!("----------------------------------------------------------");
}
/// Processes a received CAN-FD message
fn process_message_can_fd(msg: TPCANMsgFD, timestamp: TPCANTimestampFD) {
    let length = get_length_from_dlc(msg.DLC);
    println!("Type: {}", get_msg_type_string(msg.MSGTYPE));
    println!("ID: {}", get_id_string(msg.ID, msg.MSGTYPE));
    println!("Length: {}", length);
    println!("Time: {}", get_time_string(timestamp));
    println!("Data: {}", get_data_string(&msg.DATA, msg.MSGTYPE, length));
    println!("----------------------------------------------------------");
}
/// Processes a received CAN-XL message
fn process_message_can_xl(msg: TPCANMsgXL, timestamp: TPCANTimestampXL) {
    let length = get_length_from_dlc_xl(msg.DLC);
    println!("Type: {}", get_msg_type_string_xl(msg.MSGTYPE, msg.RRS != 0, msg.SEC != 0));
    println!("PID: {}", get_id_string_xl(msg.PID, msg.MSGTYPE));
    println!("Length: {}", length);
    println!("Time: {}", get_time_string(timestamp));
    println!("Data: {}", get_data_string_xl(&msg.DATA, msg.MSGTYPE, length));
    println!("----------------------------------------------------------");
}

/// Shows/prints the configurable parameters for this sample and information about them
fn show_configuration_help() {
    println!("=========================================================================================");
    println!("|                           PCAN-Basic ThreadRead Example                                |");
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
/// Gets the string representation of the type of a CAN message
fn get_msg_type_string(msg_type: TPCANMessageType) -> String {
    if (msg_type & PCAN_MESSAGE_STATUS) == PCAN_MESSAGE_STATUS {
        "STATUS".into()
    } else if (msg_type & PCAN_MESSAGE_ERRFRAME) == PCAN_MESSAGE_ERRFRAME {
        "ERROR".into()
    } else if (msg_type & PCAN_MESSAGE_EXTENDED) == PCAN_MESSAGE_EXTENDED {
        "EXT".into()
    } else {
        "STD".into()
    }
}

/// Gets the string representation of the type of a CAN XL message
fn get_msg_type_string_xl(msg_type: TPCANMessageTypeXL, _rrs_flag: bool, _sec_flag: bool) -> String {
    if (msg_type & PCAN_MESSAGE_XL) == PCAN_MESSAGE_XL {
        "XL".into()
    } else {
        "STD".into()
    }
}

/// Gets the string representation of the ID of a CAN message
fn get_id_string(id: u32, msg_type: TPCANMessageType) -> String {
    if (msg_type & PCAN_MESSAGE_EXTENDED) == PCAN_MESSAGE_EXTENDED {
        format!("{:08X}h", id)
    } else {
        format!("{:03X}h", id)
    }
}

/// Gets the string representation of the ID of a CAN XL message
fn get_id_string_xl(id: u32, msg_type: TPCANMessageTypeXL) -> String {
    if (msg_type & PCAN_MESSAGE_EXTENDED as u16) == PCAN_MESSAGE_EXTENDED as u16 {
        format!("{:08X}h", id)
    } else {
        format!("{:03X}h", id)
    }
}

/// Gets the data length of a CAN message
fn get_length_from_dlc(dlc: u8) -> usize {
    match dlc {
        9 => 12,
        10 => 16,
        11 => 20,
        12 => 24,
        13 => 32,
        14 => 48,
        15 => 64,
        _ => dlc as usize,
    }
}

/// Converts a CAN DLC value into the actual data length of the CAN-XL frame
fn get_length_from_dlc_xl(dlc: u16) -> usize {
    if dlc > 2047 {
        2048
    } else {
        (dlc + 1) as usize
    }
}

/// Gets the string representation of the timestamp of a CAN message, in milliseconds
fn get_time_string(time: u64) -> String {
    format!("{:.1}", time as f64 / 1000.0)
}

/// Gets the data of a CAN message as a string
fn get_data_string(data: &[u8], _msg_type: TPCANMessageType, data_length: usize) -> String {
    data.iter()
        .take(data_length)
        .map(|b| format!("{:02X} ", b))
        .collect::<String>()
}

/// Gets the data of a CAN XL message as a string
fn get_data_string_xl(data: &[u8], _msg_type: TPCANMessageTypeXL, data_length: usize) -> String {
    data.iter()
        .take(data_length)
        .map(|b| format!("{:02X} ", b))
        .collect::<String>()
}

