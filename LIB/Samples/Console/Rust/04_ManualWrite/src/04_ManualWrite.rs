use crate::PCANBasic::*;

// Enum for connection modes
enum CANProtocol {
    CAN,
    CAN_FD,
    CAN_XL,
}

/// Sets the PCANHandle (Hardware Channel)
const PCAN_HANDLE: TPCANHandle = PCAN_USBBUS1;
/// Sets the desired connection mode (CAN = false / CAN-FD = true)
const PROTOCOL: CANProtocol = CANProtocol::CAN;
/// Sets the bitrate for normal CAN devices
const BITRATE: TPCANBaudrate = PCAN_BAUD_500K;
/// Sets the bitrate for CAN FD devices.
const BITRATE_FD: &str = "f_clock_mhz=20, nom_brp=5, nom_tseg1=2, nom_tseg2=1, nom_sjw=1, data_brp=2, data_tseg1=3, data_tseg2=1, data_sjw=1";
/// Sets the bitrate for CAN XL devices.
const BITRATE_XL: &str = "f_clock=160000000, brp=1, nom_tseg1=255, nom_tseg2=64, nom_sjw=64, fd_tseg1=63, fd_tseg2=16, fd_sjw=16, fd_ssp_offset=0, xl_tseg1=10, xl_tseg2=9, xl_sjw=9, xl_ssp_offset=10, xl_error_signaling=1, xl_transceiver_mode_switch=0";

pub fn run() {
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

    // Writing messages...
    println!("Successfully initialized.");
    println!("Press Enter to write");
    let _ = std::io::stdin().read_line(&mut String::new());

    loop {
        write_messages();
        println!("Do you want to write again? yes[y] or any other key to close");
        let mut input = String::new();
        let _ = std::io::stdin().read_line(&mut input);
        if input.trim().to_lowercase() != "y" {
            break;
        }
    }

    let _ = Uninitialize(PCAN_NONEBUS);
}

/// Function for writing PCAN-Basic messages
fn write_messages() {
    let result = match PROTOCOL {
        CANProtocol::CAN_XL => write_message_xl(),
        CANProtocol::CAN_FD => write_message_fd(),
        CANProtocol::CAN => write_message(),
    };

    // Checks if the message was sent
    match result {
        Ok(_) => println!("Message was successfully SENT"),
        Err(status) => show_status(status),
    }
}

/// Function for writing messages on CAN devices
fn write_message() -> Result<(), TPCANStatus> {
    // Sends a CAN message with extended ID, and 8 data bytes
    let mut can_message = TPCANMsg {
        ID: 0x100,
        MSGTYPE: PCAN_MESSAGE_EXTENDED,
        LEN: 8,
        DATA: [0; 8],
    };

    for i in 0..8 {
        can_message.DATA[i] = i as u8;
    }

    Write(PCAN_HANDLE, &mut can_message)
}

/// Function for writing messages on CAN-FD devices
fn write_message_fd() -> Result<(), TPCANStatus> {
    // Sends a CAN-FD message with standard ID, 64 data bytes, and bitrate switch
    let mut can_message_fd = TPCANMsgFD {
        ID: 0x100,
        MSGTYPE: PCAN_MESSAGE_FD | PCAN_MESSAGE_BRS,
        DLC: 15,
        DATA: [0; 64],
    };

    for i in 0..64 {
        can_message_fd.DATA[i] = i as u8;
    }

    WriteFD(PCAN_HANDLE, &mut can_message_fd)
}

/// Function for writing messages on CAN-XL devices
fn write_message_xl() -> Result<(), TPCANStatus> {
    // Sends a CAN-XL message with standard ID and 2048 data bytes
    let mut can_message_xl = TPCANMsgXL {
        PID: 0x100,
        VCID: 0,
        AF: 0,
        RRS: 0,
        SEC: 0,
        SDT: 0,
        MSGTYPE: PCAN_MESSAGE_XL,
        DLC: 2047,
        DATA: [0; 2048],
    };

    for i in 0..2048 {
        can_message_xl.DATA[i] = (i % 256) as u8;
    }

    WriteXL(PCAN_HANDLE, &mut can_message_xl)
}

fn show_configuration_help() {
    println!("=========================================================================================");
    println!("|                           PCAN-Basic ManualWrite Example                               |");
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

fn show_current_configuration() {
    println!("Parameter values used");
    println!("----------------------");
    println!("* PCANHandle: {:X}", PCAN_HANDLE);
    println!("* Protocol: {}", match PROTOCOL { CANProtocol::CAN => "CAN", CANProtocol::CAN_FD => "CAN FD", CANProtocol::CAN_XL => "CAN XL"});
    println!("* Bitrate: {}", convert_bitrate_to_string(BITRATE));
    println!("* BitrateFD: {}", BITRATE_FD);
    println!("* BitrateXL: {}", BITRATE_XL);
    println!();
}

fn show_status(status: TPCANStatus) {
    println!("=========================================================================================");
    println!("{}", get_formatted_error(status));
    println!("=========================================================================================");
}

fn get_formatted_error(error: TPCANStatus) -> String {
    match GetErrorText(error, 0x09) {
        Ok(text) => text,
        Err(_) => format!("An error occurred. Error-code's text ({:X}) couldn't be retrieved", error),
    }
}

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

