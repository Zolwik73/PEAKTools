use crate::PCANBasic::*;

// Enum for connection modes
enum CANProtocol {
    CAN,
    CAN_FD,
    CAN_XL,
}

/// Sets a TPCANDevice value. The input can be numeric, in hexadecimal or decimal format, or as string denoting
/// a TPCANDevice value name.
const DEVICE_TYPE: &str = "PCAN_USB";
/// Sets value in range of a double. The input can be hexadecimal or decimal format.
const DEVICE_ID: &str = "";
/// Sets a zero-based index value in range of a double. The input can be hexadecimal or decimal format.
const CONTROLLER_NUMBER: &str = "";
/// Sets a valid Internet Protocol address
const IP_ADDRESS: &str = "";
/// Sets a valid GUID for a PCAN device
const DEVICE_GUID: &str = "";

pub fn run() {
    show_configuration_help(); // Shows information about this sample
    show_current_configuration(); // Shows the current parameters configuration

    println!("Press Enter to start searching");
    let _ = std::io::stdin().read_line(&mut String::new());
    println!();

    let mut parameters = String::new();
    if !DEVICE_TYPE.is_empty() {
        parameters.push_str(&format!("{}={}", LOOKUP_DEVICE_TYPE, DEVICE_TYPE));
    }
    if !DEVICE_ID.is_empty() {
        parameters.push_str(&format!(", {}={}", LOOKUP_DEVICE_ID, DEVICE_ID));
    }
    if !CONTROLLER_NUMBER.is_empty() {
        parameters.push_str(&format!(", {}={}", LOOKUP_CONTROLLER_NUMBER, CONTROLLER_NUMBER));
    }
    if !IP_ADDRESS.is_empty() {
        parameters.push_str(&format!(", {}={}", LOOKUP_IP_ADDRESS, IP_ADDRESS));
    }
    if !DEVICE_GUID.is_empty() {
        parameters.push_str(&format!(", {}={}", LOOKUP_DEVICE_GUID, DEVICE_GUID));
    }

    match LookUpChannel(&parameters) {
        Ok(handle) => {
            if handle != PCAN_NONEBUS {
                match GetValueInt32(handle, PCAN_CHANNEL_FEATURES) {
                    Ok(features) => {
                        let is_fd = (features & FEATURE_FD_CAPABLE as u32) == FEATURE_FD_CAPABLE as u32;
                        let is_xl = (features & FEATURE_XL_CAPABLE as u32) == FEATURE_XL_CAPABLE as u32;
                        println!(
                            "The channel handle {} was found",
                            format_channel_name(handle, get_protocol(is_fd, is_xl))
                        );
                    }
                    Err(_) => println!("There was an issue retrieving supported channel features"),
                }
            } else {
                println!("A handle for these lookup-criteria was not found");
            }
        }
        Err(status) => {
            println!("There was an error looking up the device, are any hardware channels attached?");
            show_status(status);
        }
    }

    println!();
    println!("Press Enter to close");
    let _ = std::io::stdin().read_line(&mut String::new());
    let _ = Uninitialize(PCAN_NONEBUS);
}

/// Shows/prints the configurable parameters for this sample and information about them
fn show_configuration_help() {
    println!("=========================================================================================");
    println!("|                        PCAN-Basic LookUpChannel Example                                |");
    println!("=========================================================================================");
    println!("Following parameters are to be adjusted before launching, according to the hardware used |");
    println!("                                                                                         |");
    println!("* DeviceType: Numeric value that represents a TPCANDevice                                |");
    println!("* DeviceID: Numeric value that represents the device identifier                          |");
    println!("* ControllerNumber: Numeric value that represents controller number                      |");
    println!("* IPAddress: String value that represents a valid Internet Protocol address              |");
    println!("* DeviceGUID: String value that represents a valid GUID for a PCAN device                |");
    println!("                                                                                         |");
    println!("For more information see 'LookUp Parameter Definition' within the documentation          |");
    println!("=========================================================================================");
    println!();
}

/// Shows/prints the configured parameters
fn show_current_configuration() {
    println!("Parameter values used");
    println!("----------------------");
    println!("* DeviceType: {}", DEVICE_TYPE);
    println!("* DeviceID: {}", DEVICE_ID);
    println!("* ControllerNumber: {}", CONTROLLER_NUMBER);
    println!("* IPAddress: {}", IP_ADDRESS);
    println!("* DeviceGUID: {}", DEVICE_GUID);
    println!();
}

/// Shows formatted status
fn show_status(status: TPCANStatus) {
    println!("=========================================================================================");
    println!("{}", get_formatted_error(status));
    println!("=========================================================================================");
}

/// Gets the formatted text for a PCAN-Basic channel handle
fn format_channel_name(handle: TPCANHandle, protocol: CANProtocol) -> String {
    let (device, channel) = if handle < 0x100 {
        (((handle >> 4) as u8), (handle & 0xF) as u8)
    } else {
        (((handle >> 8) as u8), (handle & 0xFF) as u8)
    };

    let dev_name = match device {
        PCAN_PCI => "PCAN_PCI",
        PCAN_USB => "PCAN_USB",
        PCAN_LAN => "PCAN_LAN",
        _ => "PCAN_NONE",
    };

    match protocol {
        CANProtocol::CAN_XL => format!("{}:XL {} ({:02X}h)", dev_name, channel, handle),
        CANProtocol::CAN_FD => format!("{}:FD {} ({:02X}h)", dev_name, channel, handle),
        CANProtocol::CAN => format!("{} {} ({:02X}h)", dev_name, channel, handle),
    }
}

/// Help Function used to get an error as text
fn get_formatted_error(error: TPCANStatus) -> String {
    // Gets the text using the GetErrorText API function
    // If the function success, the translated error is returned. If it fails, a text describing the current
    // error is returned.
    match GetErrorText(error, 0x09) {
        Ok(text) => text,
        Err(_) => format!("An error occurred. Error-code's text ({:X}) couldn't be retrieved", error),
    }
}

/// Gets the required CAN protocol
fn get_protocol(is_fd: bool, is_xl: bool) -> CANProtocol {
    if !is_fd && !is_xl {
        CANProtocol::CAN
    } else if is_fd && !is_xl {
        CANProtocol::CAN_FD
    } else {
        CANProtocol::CAN_XL
    }
}

