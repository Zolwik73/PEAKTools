use crate::PCANBasic::*;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use tokio::time::{interval, Duration};

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
/// Timerinterval (ms) for reading
const TIMER_INTERVAL: u64 = 250;

/// Starts the PCANBasic Sample
pub async fn run() {
    show_configuration_help(); // Shows information about this sample
    show_current_configuration(); // Shows the current parameters configuration

    let init_result = match PROTOCOL {
        CANProtocol::CAN_XL => InitializeXL(PCAN_HANDLE, BITRATE_XL),
        CANProtocol::CAN_FD => InitializeFD(PCAN_HANDLE, BITRATE_FD),
        CANProtocol::CAN => Initialize(PCAN_HANDLE, BITRATE),
    };

    if let Err(status) = init_result {
        println!("Can not initialize. Please check the defines in the code.");
        show_status(status);
        println!();
        println!("Press Enter to close");
        let _ = std::io::stdin().read_line(&mut String::new());
        return;
    }

    // Reading messages...
    println!("Successfully initialized.");
    println!("Started reading messages...");
    println!();
    println!("Press Enter to close");

    // Spawn the timer task
    let timer_task = tokio::spawn(async move {
        let mut timer = interval(Duration::from_millis(TIMER_INTERVAL));
        loop {
            timer.tick().await;
            read_messages();
        }
    });

    // Wait for user input
    tokio::task::spawn_blocking(|| {
        let _ = std::io::stdin().read_line(&mut String::new());
    })
    .await
    .unwrap_or(());

    timer_task.abort();
    let _ = timer_task.await;
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
        Ok((can_msg, can_timestamp)) => {
            // We process the received message
            process_message_can_xl(can_msg, can_timestamp);
            Ok(())
        }
        Err(status) => Err(status),
    }
}

/// Function for reading messages on FD devices
fn read_message_fd() -> Result<(), TPCANStatus> {
    // We execute the "Read" function of the PCANBasic
    match ReadFD(PCAN_HANDLE) {
        Ok((can_msg, can_timestamp)) => {
            // We process the received message
            process_message_can_fd(can_msg, can_timestamp);
            Ok(())
        }
        Err(status) => Err(status),
    }
}

/// Function for reading CAN messages on normal CAN devices
fn read_message() -> Result<(), TPCANStatus> {
    // We execute the "Read" function of the PCANBasic
    match Read(PCAN_HANDLE) {
        Ok((can_msg, can_timestamp)) => {
            // We process the received message
            process_message_can(can_msg, can_timestamp);
            Ok(())
        }
        Err(status) => Err(status),
    }
}

/// Processes a received CAN message
fn process_message_can(msg: TPCANMsg, timestamp: TPCANTimestamp) {
    let micros_timestamp = (timestamp.micros as u64)
        + (1000_u64 * timestamp.millis as u64)
        + (0x100_000_000_u64 * 1000_u64 * timestamp.millis_overflow as u64);

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
    println!("|                          PCAN-Basic TimerRead Example                                  |");
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
    println!("  TimerInterval: The time, in milliseconds, to wait before trying to write a message     |");
    println!("=========================================================================================");
    println!();
}

/// Shows/prints the configured parameters
fn show_current_configuration() {
    println!("Parameter values used");
    println!("----------------------");
    println!("* PCANHandle: {:X}", PCAN_HANDLE);
    println!("* Protocol: {}", match PROTOCOL { CANProtocol::CAN => "CAN", CANProtocol::CAN_FD => "CAN FD", CANProtocol::CAN_XL => "CAN XL"});
    println!("* Bitrate: {}", convert_bitrate_to_string(BITRATE));
    println!("* BitrateFD: {}", BITRATE_FD);
    println!("* BitrateXL: {}", BITRATE_XL);
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
    match GetErrorText(error, 0x09) {
        Ok(text) => text,
        Err(_) => format!("An error occurred. Error-code's text ({:X}) couldn't be retrieved", error),
    }
}

/// Convert bitrate c_short value to readable string
fn convert_bitrate_to_string(bitrate: TPCANBaudrate) -> &'static str {
    match bitrate {
        PCAN_BAUD_1M => "1 MBit/sec", PCAN_BAUD_800K => "800 kBit/sec", PCAN_BAUD_500K => "500 kBit/sec",
        PCAN_BAUD_250K => "250 kBit/sec", PCAN_BAUD_125K => "125 kBit/sec", PCAN_BAUD_100K => "100 kBit/sec",
        PCAN_BAUD_95K => "95,238 kBit/sec", PCAN_BAUD_83K => "83,333 kBit/sec", PCAN_BAUD_50K => "50 kBit/sec",
        PCAN_BAUD_47K => "47,619 kBit/sec", PCAN_BAUD_33K => "33,333 kBit/sec", PCAN_BAUD_20K => "20 kBit/sec",
        PCAN_BAUD_10K => "10 kBit/sec", PCAN_BAUD_5K => "5 kBit/sec", _ => "Unknown Bitrate",
    }
}

/// Gets the string representation of the type of a CAN message
fn get_msg_type_string(msg_type: TPCANMessageType) -> String {
    if (msg_type & PCAN_MESSAGE_STATUS) == PCAN_MESSAGE_STATUS { return "STATUS".into(); }
    if (msg_type & PCAN_MESSAGE_ERRFRAME) == PCAN_MESSAGE_ERRFRAME { return "ERROR".into(); }
    let mut s = if (msg_type & PCAN_MESSAGE_EXTENDED) == PCAN_MESSAGE_EXTENDED { "EXT".to_string() } else { "STD".to_string() };
    if (msg_type & PCAN_MESSAGE_RTR) == PCAN_MESSAGE_RTR { s.push_str("/RTR"); }
    else if msg_type > PCAN_MESSAGE_EXTENDED {
        s.push_str(" [ ");
        if (msg_type & PCAN_MESSAGE_FD) == PCAN_MESSAGE_FD { s.push_str(" FD"); }
        if (msg_type & PCAN_MESSAGE_BRS) == PCAN_MESSAGE_BRS { s.push_str(" BRS"); }
        if (msg_type & PCAN_MESSAGE_ESI) == PCAN_MESSAGE_ESI { s.push_str(" ESI"); }
        s.push_str(" ]");
    }
    s
}

/// Gets the string representation of the type of a CAN XL message
fn get_msg_type_string_xl(msg_type: TPCANMessageTypeXL, rrs_flag: bool, sec_flag: bool) -> String {
    let is_echo = (msg_type & PCAN_MESSAGE_ECHO as u16) == PCAN_MESSAGE_ECHO as u16;
    if (msg_type & PCAN_MESSAGE_STATUS as u16) == PCAN_MESSAGE_STATUS as u16 { return "STATUS".into(); }
    if (msg_type & PCAN_MESSAGE_ERRFRAME as u16) == PCAN_MESSAGE_ERRFRAME as u16 { return "ERROR".into(); }
    let mut s = if (msg_type & PCAN_MESSAGE_XL) == PCAN_MESSAGE_XL { "XL".to_string() }
        else if (msg_type & PCAN_MESSAGE_EXTENDED as u16) == PCAN_MESSAGE_EXTENDED as u16 { "EXT".to_string() }
        else { "STD".to_string() };
    if (msg_type & PCAN_MESSAGE_RTR as u16) == PCAN_MESSAGE_RTR as u16 { s.push_str(if is_echo { "/RTR [ ECHO ]" } else { "/RTR" }); }
    else {
        if is_echo { s.push_str(" [ ECHO"); } else { s.push_str(" [ "); }
        if msg_type >= PCAN_MESSAGE_XL { if rrs_flag { s.push_str(" RRS"); } if sec_flag { s.push_str(" SEC"); } s.push_str(" ]"); }
        else if msg_type > PCAN_MESSAGE_EXTENDED as u16 {
            if (msg_type & PCAN_MESSAGE_FD as u16) == PCAN_MESSAGE_FD as u16 { s.push_str(" FD"); }
            if (msg_type & PCAN_MESSAGE_BRS as u16) == PCAN_MESSAGE_BRS as u16 { s.push_str(" BRS"); }
            if (msg_type & PCAN_MESSAGE_ESI as u16) == PCAN_MESSAGE_ESI as u16 { s.push_str(" ESI"); }
            s.push_str(" ]");
        }
    }
    s
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
fn get_data_string(data: &[u8], msg_type: TPCANMessageType, data_length: usize) -> String {
    if (msg_type & PCAN_MESSAGE_RTR) == PCAN_MESSAGE_RTR {
        "Remote Request".into()
    } else {
        get_data_string_raw(data, data_length)
    }
}

/// Gets the data of a CAN XL message as a string
fn get_data_string_xl(data: &[u8], msg_type: TPCANMessageTypeXL, data_length: usize) -> String {
    if (msg_type & PCAN_MESSAGE_RTR as u16) == PCAN_MESSAGE_RTR as u16 {
        "Remote Request".into()
    } else {
        get_data_string_raw(data, data_length)
    }
}

/// Gets the data of a CAN message as a string
fn get_data_string_raw(data: &[u8], data_length: usize) -> String {
    data.iter()
        .take(data_length)
        .map(|b| format!("{:02X} ", b))
        .collect::<String>()
}

