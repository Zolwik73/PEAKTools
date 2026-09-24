use crate::PCANBasic::*;
use std::ffi::CStr;

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

    println!("Successfully initialized.");
    println!("Press Enter to get/set parameter");
    let _ = std::io::stdin().read_line(&mut String::new());
    println!();

    run_selected_commands();

    println!();
    println!("Press Enter to close");
    let _ = std::io::stdin().read_line(&mut String::new());
    let _ = Uninitialize(PCAN_NONEBUS);
}

/// Runs all commands for get or set parameters
fn run_selected_commands() {
    // Fill commands here

    get_pcan_device_id();

    println!("Fill \"RunSelectedCommands\"-function with parameter functions from \"Parameter commands\"-Region in the code.");
}

/// Shows device identifier parameter
fn get_pcan_device_id() {
    match GetValueInt32(PCAN_HANDLE, PCAN_DEVICE_ID) {
        Ok(device_id) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_DEVICE_ID: {}", device_id);
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Sets device identifier parameter
fn set_pcan_device_id(device_id: u32) {
    match SetValueInt32(PCAN_HANDLE, PCAN_DEVICE_ID, &device_id) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_DEVICE_ID: {}", device_id);
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows all information about attached channels
fn get_pcan_attached_channels() {
    match GetValueInt32(PCAN_NONEBUS, PCAN_ATTACHED_CHANNELS_COUNT) {
        Ok(channels_count) => match GetValueVector(PCAN_NONEBUS, PCAN_ATTACHED_CHANNELS, channels_count as usize) {
            Ok(channels) => {
                println!("-----------------------------------------------------------------------------------------");
                println!("Get PCAN_ATTACHED_CHANNELS:");

                for ch in channels {
                    let name = unsafe {
                        CStr::from_ptr(ch.device_name.as_ptr())
                            .to_string_lossy()
                            .to_string()
                    };
                    println!("---------------------------");
                    println!("channel_handle:      {}", convert_to_channel_handle(ch.channel_handle as u32));
                    println!("device_type:         {}", ch.device_type);
                    println!("controller_number:   {}", ch.controller_number);
                    println!("device_features:     {}", convert_to_channel_features(ch.device_features));
                    println!("device_name:         {}", name);
                    println!("device_id:           {}", ch.device_id);
                    println!("channel_condition:   {}", convert_to_channel_condition(ch.channel_condition));
                }
                println!();
            }
            Err(sts) => show_status(sts),
        },
        Err(sts) => show_status(sts),
    }
}

/// Shows the status of selected PCAN-Channel
fn get_pcan_channel_condition() {
    match GetValueInt32(PCAN_HANDLE, PCAN_CHANNEL_CONDITION) {
        Ok(channel_condition) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_CHANNEL_CONDITION: {}", convert_to_channel_condition(channel_condition));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the status from the status LED of the USB devices
fn get_pcan_channel_identifying() {
    match GetValueInt32(PCAN_HANDLE, PCAN_CHANNEL_IDENTIFYING) {
        Ok(channel_identifying) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_CHANNEL_IDENTIFYING: {}", convert_to_parameter_on_off(channel_identifying));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// De/Activates the status LED of the USB devices
fn set_pcan_channel_identifying(value: bool) {
    let channel_identifying = if value {
        PCAN_PARAMETER_ON as u32
    } else {
        PCAN_PARAMETER_OFF as u32
    };

    match SetValueInt32(PCAN_HANDLE, PCAN_CHANNEL_IDENTIFYING, &channel_identifying) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_CHANNEL_IDENTIFYING: {}", convert_to_parameter_on_off(channel_identifying));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows information about features
fn get_pcan_channel_features() {
    match GetValueInt32(PCAN_HANDLE, PCAN_CHANNEL_FEATURES) {
        Ok(channel_features) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_CHANNEL_FEATURES: {}", convert_to_channel_features(channel_features));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the status from Bitrate-Adapting mode
fn get_pcan_bitrate_adapting() {
    match GetValueInt32(PCAN_HANDLE, PCAN_BITRATE_ADAPTING) {
        Ok(bitrate_adapting) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_BITRATE_ADAPTING: {}", convert_to_parameter_on_off(bitrate_adapting));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// De/Activates the Bitrate-Adapting mode
fn set_pcan_bitrate_adapting(value: bool) {
    // Note: SetPCAN_BITRATE_ADAPTING requires an uninitialized channel,
    let _ = Uninitialize(PCAN_NONEBUS);

    let bitrate_adapting = if value {
        PCAN_PARAMETER_ON as u32
    } else {
        PCAN_PARAMETER_OFF as u32
    };

    match SetValueInt32(PCAN_HANDLE, PCAN_BITRATE_ADAPTING, &bitrate_adapting) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_BITRATE_ADAPTING: {}", convert_to_parameter_on_off(bitrate_adapting));
            println!();
        }
        Err(sts) => show_status(sts),
    }

    // Channel will be connected again
    let sts_result = match PROTOCOL {
        CANProtocol::CAN_XL => InitializeXL(PCAN_HANDLE, BITRATE_XL),
        CANProtocol::CAN_FD => InitializeFD(PCAN_HANDLE, BITRATE_FD),
        CANProtocol::CAN => Initialize(PCAN_HANDLE, BITRATE),
    };

    if let Err(sts) = sts_result {
        println!("Error while re-initializing the channel.");
        show_status(sts);
    }
}

/// Shows the status from the reception of status frames
fn get_pcan_allow_status_frames() {
    match GetValueInt32(PCAN_HANDLE, PCAN_ALLOW_STATUS_FRAMES) {
        Ok(allow_status_frames) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_ALLOW_STATUS_FRAMES: {}", convert_to_parameter_on_off(allow_status_frames));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// De/Activates the reception of status frames
fn set_pcan_allow_status_frames(value: bool) {
    let allow_status_frames = if value {
        PCAN_PARAMETER_ON as u32
    } else {
        PCAN_PARAMETER_OFF as u32
    };

    match SetValueInt32(PCAN_HANDLE, PCAN_ALLOW_STATUS_FRAMES, &allow_status_frames) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_ALLOW_STATUS_FRAMES: {}", convert_to_parameter_on_off(allow_status_frames));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the status from the reception of RTR frames
fn get_pcan_allow_rtr_frames() {
    match GetValueInt32(PCAN_HANDLE, PCAN_ALLOW_RTR_FRAMES) {
        Ok(allow_rtr_frames) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_ALLOW_RTR_FRAMES: {}", convert_to_parameter_on_off(allow_rtr_frames));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// De/Activates the reception of RTR frames
fn set_pcan_allow_rtr_frames(value: bool) {
    let allow_rtr_frames = if value {
        PCAN_PARAMETER_ON as u32
    } else {
        PCAN_PARAMETER_OFF as u32
    };

    match SetValueInt32(PCAN_HANDLE, PCAN_ALLOW_RTR_FRAMES, &allow_rtr_frames) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_ALLOW_RTR_FRAMES: {}", convert_to_parameter_on_off(allow_rtr_frames));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the status from the reception of CAN error frames
fn get_pcan_allow_error_frames() {
    match GetValueInt32(PCAN_HANDLE, PCAN_ALLOW_ERROR_FRAMES) {
        Ok(allow_error_frames) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_ALLOW_ERROR_FRAMES: {}", convert_to_parameter_on_off(allow_error_frames));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// De/Activates the reception of CAN error frames
fn set_pcan_allow_error_frames(value: bool) {
    let allow_error_frames = if value {
        PCAN_PARAMETER_ON as u32
    } else {
        PCAN_PARAMETER_OFF as u32
    };

    match SetValueInt32(PCAN_HANDLE, PCAN_ALLOW_ERROR_FRAMES, &allow_error_frames) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_ALLOW_ERROR_FRAMES: {}", convert_to_parameter_on_off(allow_error_frames));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the status from the reception of Echo frames
fn get_pcan_echo_error_frames() {
    match GetValueInt32(PCAN_HANDLE, PCAN_ALLOW_ECHO_FRAMES) {
        Ok(allow_echo_frames) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_ALLOW_ECHO_FRAMES: {}", convert_to_parameter_on_off(allow_echo_frames));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// De/Activates the reception of Echo frames
fn set_pcan_allow_echo_frames(value: bool) {
    let allow_echo_frames = if value {
        PCAN_PARAMETER_ON as u32
    } else {
        PCAN_PARAMETER_OFF as u32
    };

    match SetValueInt32(PCAN_HANDLE, PCAN_ALLOW_ECHO_FRAMES, &allow_echo_frames) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_ALLOW_ECHO_FRAMES: {}", convert_to_parameter_on_off(allow_echo_frames));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the reception filter with a specific 11-bit acceptance code and mask
fn get_pcan_acceptance_filter_11bit() {
    match GetValueInt64(PCAN_HANDLE, PCAN_ACCEPTANCE_FILTER_11BIT) {
        Ok(acceptance_filter_11_bit) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_ACCEPTANCE_FILTER_11BIT: {:016X}h", acceptance_filter_11_bit as u64);
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Sets the reception filter with a specific 11-bit acceptance code and mask
fn set_pcan_acceptance_filter_11bit(acceptance_filter_11_bit: u64) {
    match SetValueInt64(PCAN_HANDLE, PCAN_ACCEPTANCE_FILTER_11BIT, &acceptance_filter_11_bit) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_ACCEPTANCE_FILTER_11BIT: {:016X}h", acceptance_filter_11_bit);
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the reception filter with a specific 29-bit acceptance code and mask
fn get_pcan_acceptance_filter_29bit() {
    match GetValueInt64(PCAN_HANDLE, PCAN_ACCEPTANCE_FILTER_29BIT) {
        Ok(acceptance_filter_29_bit) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_ACCEPTANCE_FILTER_29BIT: {:016X}h", acceptance_filter_29_bit as u64);
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Sets the reception filter with a specific 29-bit acceptance code and mask
fn set_pcan_acceptance_filter_29bit(acceptance_filter_29_bit: u64) {
    match SetValueInt64(PCAN_HANDLE, PCAN_ACCEPTANCE_FILTER_29BIT, &acceptance_filter_29_bit) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_ACCEPTANCE_FILTER_29BIT: {:016X}h", acceptance_filter_29_bit);
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the status of the reception filter
fn get_pcan_message_filter() {
    match GetValueInt32(PCAN_HANDLE, PCAN_MESSAGE_FILTER) {
        Ok(message_filter) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_MESSAGE_FILTER: {}", convert_to_filter_open_close_custom(message_filter));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// De/Activates the reception filter
fn set_pcan_message_filter(message_filter: u32) {
    match SetValueInt32(PCAN_HANDLE, PCAN_MESSAGE_FILTER, &message_filter) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_MESSAGE_FILTER: {}", convert_to_filter_open_close_custom(message_filter));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the status of the hard reset within the PCANBasic.Reset method
fn get_pcan_hard_reset_status() {
    match GetValueInt32(PCAN_HANDLE, PCAN_HARD_RESET_STATUS) {
        Ok(hard_reset_status) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_HARD_RESET_STATUS: {}", convert_to_parameter_on_off(hard_reset_status));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// De/Activates the hard reset within the PCANBasic.Reset method
fn set_pcan_hard_reset_status(value: bool) {
    let hard_reset_status = if value {
        PCAN_PARAMETER_ON as u32
    } else {
        PCAN_PARAMETER_OFF as u32
    };

    match SetValueInt32(PCAN_HANDLE, PCAN_HARD_RESET_STATUS, &hard_reset_status) {
        Ok(_) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Set PCAN_HARD_RESET_STATUS: {}", convert_to_parameter_on_off(hard_reset_status));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the communication direction of a PCAN-Channel representing a LAN interface
fn get_pcan_lan_channel_direction() {
    match GetValueInt32(PCAN_HANDLE, PCAN_LAN_CHANNEL_DIRECTION) {
        Ok(channel_direction) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_LAN_CHANNEL_DIRECTION: {}", convert_to_channel_direction(channel_direction));
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

/// Shows the global unique identifier associated to a PCAN device
fn get_pcan_device_guid() {
    match GetValueString(PCAN_HANDLE, PCAN_DEVICE_GUID) {
        Ok(buffer) => {
            println!("-----------------------------------------------------------------------------------------");
            println!("Get PCAN_DEVICE_GUID: {}", buffer);
            println!();
        }
        Err(sts) => show_status(sts),
    }
}

fn show_configuration_help() {
    println!("=========================================================================================");
    println!("|                           PCAN-Basic GetSetParameter Example                           |");
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
    println!("* PCANHandle: {}", format_channel_name(PCAN_HANDLE, &PROTOCOL));
    println!("* Protocol: {}", protocol_to_string(&PROTOCOL));
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

/// Convert uint value to readable string value
fn convert_to_channel_handle(value: u32) -> String {
    match value {
        v if v == PCAN_USBBUS1 as u32 => "PCAN_USBBUS1".to_string(),
        v if v == PCAN_USBBUS2 as u32 => "PCAN_USBBUS2".to_string(),
        v if v == PCAN_USBBUS3 as u32 => "PCAN_USBBUS3".to_string(),
        v if v == PCAN_USBBUS4 as u32 => "PCAN_USBBUS4".to_string(),
        v if v == PCAN_USBBUS5 as u32 => "PCAN_USBBUS5".to_string(),
        v if v == PCAN_USBBUS6 as u32 => "PCAN_USBBUS6".to_string(),
        v if v == PCAN_USBBUS7 as u32 => "PCAN_USBBUS7".to_string(),
        v if v == PCAN_USBBUS8 as u32 => "PCAN_USBBUS8".to_string(),
        v if v == PCAN_USBBUS9 as u32 => "PCAN_USBBUS9".to_string(),
        v if v == PCAN_USBBUS10 as u32 => "PCAN_USBBUS10".to_string(),
        v if v == PCAN_USBBUS11 as u32 => "PCAN_USBBUS11".to_string(),
        v if v == PCAN_USBBUS12 as u32 => "PCAN_USBBUS12".to_string(),
        v if v == PCAN_USBBUS13 as u32 => "PCAN_USBBUS13".to_string(),
        v if v == PCAN_USBBUS14 as u32 => "PCAN_USBBUS14".to_string(),
        v if v == PCAN_USBBUS15 as u32 => "PCAN_USBBUS15".to_string(),
        v if v == PCAN_USBBUS16 as u32 => "PCAN_USBBUS16".to_string(),

        v if v == PCAN_LANBUS1 as u32 => "PCAN_LANBUS1".to_string(),
        v if v == PCAN_LANBUS2 as u32 => "PCAN_LANBUS2".to_string(),
        v if v == PCAN_LANBUS3 as u32 => "PCAN_LANBUS3".to_string(),
        v if v == PCAN_LANBUS4 as u32 => "PCAN_LANBUS4".to_string(),
        v if v == PCAN_LANBUS5 as u32 => "PCAN_LANBUS5".to_string(),
        v if v == PCAN_LANBUS6 as u32 => "PCAN_LANBUS6".to_string(),
        v if v == PCAN_LANBUS7 as u32 => "PCAN_LANBUS7".to_string(),
        v if v == PCAN_LANBUS8 as u32 => "PCAN_LANBUS8".to_string(),
        v if v == PCAN_LANBUS9 as u32 => "PCAN_LANBUS9".to_string(),
        v if v == PCAN_LANBUS10 as u32 => "PCAN_LANBUS10".to_string(),
        v if v == PCAN_LANBUS11 as u32 => "PCAN_LANBUS11".to_string(),
        v if v == PCAN_LANBUS12 as u32 => "PCAN_LANBUS12".to_string(),
        v if v == PCAN_LANBUS13 as u32 => "PCAN_LANBUS13".to_string(),
        v if v == PCAN_LANBUS14 as u32 => "PCAN_LANBUS14".to_string(),
        v if v == PCAN_LANBUS15 as u32 => "PCAN_LANBUS15".to_string(),
        v if v == PCAN_LANBUS16 as u32 => "PCAN_LANBUS16".to_string(),

        v if v == PCAN_PCIBUS1 as u32 => "PCAN_PCIBUS1".to_string(),
        v if v == PCAN_PCIBUS2 as u32 => "PCAN_PCIBUS2".to_string(),
        v if v == PCAN_PCIBUS3 as u32 => "PCAN_PCIBUS3".to_string(),
        v if v == PCAN_PCIBUS4 as u32 => "PCAN_PCIBUS4".to_string(),
        v if v == PCAN_PCIBUS5 as u32 => "PCAN_PCIBUS5".to_string(),
        v if v == PCAN_PCIBUS6 as u32 => "PCAN_PCIBUS6".to_string(),
        v if v == PCAN_PCIBUS7 as u32 => "PCAN_PCIBUS7".to_string(),
        v if v == PCAN_PCIBUS8 as u32 => "PCAN_PCIBUS8".to_string(),
        v if v == PCAN_PCIBUS9 as u32 => "PCAN_PCIBUS9".to_string(),
        v if v == PCAN_PCIBUS10 as u32 => "PCAN_PCIBUS10".to_string(),
        v if v == PCAN_PCIBUS11 as u32 => "PCAN_PCIBUS11".to_string(),
        v if v == PCAN_PCIBUS12 as u32 => "PCAN_PCIBUS12".to_string(),
        v if v == PCAN_PCIBUS13 as u32 => "PCAN_PCIBUS13".to_string(),
        v if v == PCAN_PCIBUS14 as u32 => "PCAN_PCIBUS14".to_string(),
        v if v == PCAN_PCIBUS15 as u32 => "PCAN_PCIBUS15".to_string(),
        v if v == PCAN_PCIBUS16 as u32 => "PCAN_PCIBUS16".to_string(),
        _ => format!("Handle unknown: {}", value),
    }
}

/// Convert uint value to readable string value
fn convert_to_parameter_on_off(value: u32) -> String {
    match value {
        v if v == PCAN_PARAMETER_OFF as u32 => "PCAN_PARAMETER_OFF".to_string(),
        v if v == PCAN_PARAMETER_ON as u32 => "PCAN_PARAMETER_ON".to_string(),
        _ => format!("Status unknown: {}", value),
    }
}

/// Convert uint value to readable string value
fn convert_to_channel_direction(value: u32) -> String {
    match value {
        v if v == LAN_DIRECTION_READ as u32 => "incoming only".to_string(),
        v if v == LAN_DIRECTION_WRITE as u32 => "outgoing only".to_string(),
        v if v == LAN_DIRECTION_READ_WRITE as u32 => "bidirectional".to_string(),
        _ => format!("undefined (0x{:04X})", value),
    }
}

/// Convert uint value to readable string value
fn convert_to_channel_features(value: u32) -> String {
    let mut features: Vec<&str> = Vec::new();
    if (value & FEATURE_FD_CAPABLE as u32) == FEATURE_FD_CAPABLE as u32 {
        features.push("FEATURE_FD_CAPABLE");
    }
    if (value & FEATURE_XL_CAPABLE as u32) == FEATURE_XL_CAPABLE as u32 {
        features.push("FEATURE_XL_CAPABLE");
    }
    if (value & FEATURE_DELAY_CAPABLE as u32) == FEATURE_DELAY_CAPABLE as u32 {
        features.push("FEATURE_DELAY_CAPABLE");
    }
    if (value & FEATURE_IO_CAPABLE as u32) == FEATURE_IO_CAPABLE as u32 {
        features.push("FEATURE_IO_CAPABLE");
    }
    features.join(", ")
}

/// Convert uint value to readable string value
fn convert_to_channel_condition(value: u32) -> String {
    match value {
        v if v == PCAN_CHANNEL_UNAVAILABLE as u32 => "PCAN_CHANNEL_UNAVAILABLE".to_string(),
        v if v == PCAN_CHANNEL_AVAILABLE as u32 => "PCAN_CHANNEL_AVAILABLE".to_string(),
        v if v == PCAN_CHANNEL_OCCUPIED as u32 => "PCAN_CHANNEL_OCCUPIED".to_string(),
        v if v == PCAN_CHANNEL_PCANVIEW as u32 => "PCAN_CHANNEL_PCANVIEW".to_string(),
        _ => format!("Status unknow: {}", value),
    }
}

/// Convert uint value to readable string value
fn convert_to_filter_open_close_custom(value: u32) -> String {
    match value {
        v if v == PCAN_FILTER_CLOSE as u32 => "PCAN_FILTER_CLOSE".to_string(),
        v if v == PCAN_FILTER_OPEN as u32 => "PCAN_FILTER_OPEN".to_string(),
        v if v == PCAN_FILTER_CUSTOM as u32 => "PCAN_FILTER_CUSTOM".to_string(),
        _ => format!("Status unknown: {}", value),
    }
}

