#include"stdafx.h"
#include "PCANBasic.h"

// Enum for connection modes
enum CANProtocol { CAN, CAN_FD, CAN_XL };

class LookUpChannel
{
private:
    /// <summary>
    /// Sets a TPCANDevice value. The input can be numeric, in hexadecimal or decimal format, or as string denoting 
    /// a TPCANDevice value name.
    /// </summary>
    LPCSTR DeviceType = "PCAN_USB";
    /// <summary>
    /// Sets value in range of a double. The input can be hexadecimal or decimal format.
    /// </summary>
    LPCSTR DeviceID = "";
    /// <summary>
    /// Sets a zero-based index value in range of a double. The input can be hexadecimal or decimal format.
    /// </summary>
    LPCSTR ControllerNumber = "";
    /// <summary>
    /// Sets a valid Internet Protocol address 
    /// </summary>
    LPCSTR IPAddress = "";
    /// <summary>
    /// Sets a valid GUID for a PCAN device
    /// </summary>
    LPCSTR DeviceGUID = "";

public:
    // LookUpChannel constructor
    //
    LookUpChannel();

    // LookUpChannel destructor
    //
    ~LookUpChannel();

private:
    /// <summary>
    /// Shows/prints the configurable parameters for this sample and information about them
    /// </summary>
    void ShowConfigurationHelp();

    /// <summary>
    /// Shows/prints the configured parameters
    /// </summary>
    void ShowCurrentConfiguration();

    /// <summary>
    /// Shows formatted status
    /// </summary>
    /// <param name="status">Will be formatted</param>
    void ShowStatus(TPCANStatus status);

    /// <summary>
    /// Gets the formatted text for a PCAN-Basic channel handle
    /// </summary>
    /// <param name="handle">PCAN-Basic Handle to format</param>
    /// <param name="protocol">Used CAN protocol</param>
    /// <returns>The formatted text for a channel</returns>
    void FormatChannelName(TPCANHandle handle, LPSTR buffer, CANProtocol protocol);

    /// <summary>
    /// Gets name of a TPCANHandle
    /// </summary>
    /// <param name="handle">TPCANHandle to get name</param>
    /// <param name="buffer">A string buffer for the name of the TPCANHandle (size MAX_PATH)</param>
    void GetTPCANHandleName(TPCANHandle handle, LPSTR buffer);

    /// <summary>
    /// Help Function used to get an error as text
    /// </summary>
    /// <param name="error">Error code to be translated</param>
    /// <param name="buffer">A string buffer for the translated error (size MAX_PATH)</param>
    void GetFormattedError(TPCANStatus error, LPSTR buffer);

    /// <summary>
    /// Convert bitrate c_short value to readable string
    /// </summary>
    /// <param name="bitrate">Bitrate to be converted</param>
    /// <param name="buffer">A string buffer for the converted bitrate (size MAX_PATH)</param>
    void ConvertBitrateToString(TPCANBaudrate bitrate, LPSTR buffer);

    /// <summary>
    /// Converts PCANBasic Define (wchar_t*) to char*
    /// </summary>
    /// <param name="define">Define to be converted</param>
    /// <returns>Define as char*</returns>
    char* ConvertDefinesToChar(wchar_t* define);

    /// <summary>
    /// Gets the required CAN protocol
    /// </summary>
    /// <param name="isFD">If device supports CAN FD</param>
    /// <param name="isXL">If device supports CAN XL</param>
    /// <returns>A CAN protocol</returns>
    CANProtocol GetProtocol(bool isFD, bool isXL);
};