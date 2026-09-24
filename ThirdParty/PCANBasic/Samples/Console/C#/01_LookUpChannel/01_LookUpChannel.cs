using Peak.Can.Basic;
using System;
using System.Text;

namespace Samples.LookUpChannel
{
	// Type alias for a PCAN-Basic channel handle
	using TPCANHandle = System.UInt16;

	// Enum for connection modes
	enum CANProtocol
	{
		CAN,
		CAN_FD,
		CAN_XL
	}

	public class LookUpChannelSample
	{
		#region Defines
		/// <summary>
		/// Sets a TPCANDevice value. The input can be numeric, in hexadecimal or decimal format, or as string denoting 
		/// a TPCANDevice value name.
		/// </summary>
		const string DeviceType = "PCAN_USB";
		/// <summary>
		/// Sets value in range of a double. The input can be hexadecimal or decimal format.
		/// </summary>
		const string DeviceID = "";
		/// <summary>
		/// Sets a zero-based index value in range of a double. The input can be hexadecimal or decimal format.
		/// </summary>
		const string ControllerNumber = "";
		/// <summary>
		/// Sets a valid Internet Protocol address 
		/// </summary>
		const string IPAddress = "";
		/// <summary>
		/// Sets a valid GUID for a PCAN device
		/// </summary>
		const string DeviceGUID = "";
		#endregion

		#region Members
		/// <summary>
		/// Shows if DLL was found
		/// </summary>
		private bool m_DLLFound;
		#endregion

		/// <summary>
		/// Starts the PCANBasic Sample
		/// </summary>
		public LookUpChannelSample()
		{
			ShowConfigurationHelp(); // Shows information about this sample
			ShowCurrentConfiguration(); // Shows the current parameters configuration

			// Checks if PCANBasic.dll is available, if not, the program terminates
			m_DLLFound = CheckForLibrary();
			if (!m_DLLFound)
				return;

			Console.WriteLine("Press any key to start searching");
			Console.ReadKey();
			Console.WriteLine();

			string sParameters = "";
			if (DeviceType != "")
				sParameters += PCANBasic.LOOKUP_DEVICE_TYPE + "=" + DeviceType;
			if (DeviceID != "")
			{
				sParameters += ", " + PCANBasic.LOOKUP_DEVICE_ID + "=" + DeviceID;
			}
			if (ControllerNumber != "")
			{
				sParameters += ", " + PCANBasic.LOOKUP_CONTROLLER_NUMBER + "=" + ControllerNumber;
			}
			if (IPAddress != "")
			{
				sParameters += ", " + PCANBasic.LOOKUP_IP_ADDRESS + "=" + IPAddress;
			}
			if (DeviceGUID != "")
			{
				sParameters += ", " + PCANBasic.LOOKUP_DEVICE_GUID + "=" + DeviceGUID;
			}

			TPCANStatus stsResult = PCANBasic.LookUpChannel(sParameters, out TPCANHandle handle);

			if (stsResult == TPCANStatus.PCAN_ERROR_OK)
			{

				if (handle != PCANBasic.PCAN_NONEBUS)
				{
					stsResult = PCANBasic.GetValue(handle, TPCANParameter.PCAN_CHANNEL_FEATURES, out uint iFeatures, sizeof(uint));

					if (stsResult == TPCANStatus.PCAN_ERROR_OK)
					{
						bool isFD = (iFeatures & PCANBasic.FEATURE_FD_CAPABLE) == PCANBasic.FEATURE_FD_CAPABLE;
						bool isXL = (iFeatures & PCANBasic.FEATURE_XL_CAPABLE) == PCANBasic.FEATURE_XL_CAPABLE;

						Console.WriteLine("The channel handle " + FormatChannelName(handle,
																					GetProtocol(isFD, isXL)) + " was found");
					}
					else
						Console.WriteLine("There was an issue retrieving supported channel features");
				}
				else
					Console.WriteLine("A handle for these lookup-criteria was not found");
			}

			if (stsResult != TPCANStatus.PCAN_ERROR_OK)
			{
				Console.WriteLine("There was an error looking up the device, are any hardware channels attached?");
				ShowStatus(stsResult);
			}

			Console.WriteLine();
			Console.WriteLine("Press any key to close");
			Console.ReadKey();
		}

		~LookUpChannelSample()
		{
			if (m_DLLFound)
				PCANBasic.Uninitialize(PCANBasic.PCAN_NONEBUS);
		}

		#region Help-Functions
		/// <summary>
		/// Checks for availability of the PCANBasic library
		/// </summary>
		/// <returns>If the library was found or not</returns>
		private bool CheckForLibrary()
		{
			// Check for dll file
			try
			{
				PCANBasic.Uninitialize(PCANBasic.PCAN_NONEBUS);
				return true;
			}
			catch (DllNotFoundException)
			{
				Console.WriteLine("Unable to find the library: PCANBasic.dll !");
				Console.WriteLine("Press any key to close");
				Console.ReadKey();
			}
			return false;
		}

		/// <summary>
		/// Shows/prints the configurable parameters for this sample and information about them
		/// </summary>
		private void ShowConfigurationHelp()
		{
			Console.WriteLine("=========================================================================================");
			Console.WriteLine("|                        PCAN-Basic LookUpChannel Example                                |");
			Console.WriteLine("=========================================================================================");
			Console.WriteLine("Following parameters are to be adjusted before launching, according to the hardware used |");
			Console.WriteLine("                                                                                         |");
			Console.WriteLine("* DeviceType: Numeric value that represents a TPCANDevice                                |");
			Console.WriteLine("* DeviceID: Numeric value that represents the device identifier                          |");
			Console.WriteLine("* ControllerNumber: Numeric value that represents controller number                      |");
			Console.WriteLine("* IPAddress: String value that represents a valid Internet Protocol address              |");
			Console.WriteLine("* DeviceGUID: String value that represents a valid GUID for a PCAN device                |");
			Console.WriteLine("                                                                                         |");
			Console.WriteLine("For more information see 'LookUp Parameter Definition' within the documentation          |");
			Console.WriteLine("=========================================================================================");
			Console.WriteLine("");
		}

		/// <summary>
		/// Shows/prints the configured parameters
		/// </summary>
		private void ShowCurrentConfiguration()
		{
			Console.WriteLine("Parameter values used");
			Console.WriteLine("----------------------");
			Console.WriteLine("* DeviceType: " + DeviceType);
			Console.WriteLine("* DeviceID: " + DeviceID);
			Console.WriteLine("* ControllerNumber: " + ControllerNumber);
			Console.WriteLine("* IPAddress: " + IPAddress);
			Console.WriteLine("* DeviceGUID: " + DeviceGUID);
			Console.WriteLine("");
		}

		/// <summary>
		/// Shows formatted status
		/// </summary>
		/// <param name="status">Will be formatted</param>
		private void ShowStatus(TPCANStatus status)
		{
			Console.WriteLine("=========================================================================================");
			Console.WriteLine(GetFormattedError(status));
			Console.WriteLine("=========================================================================================");
		}

		/// <summary>
		/// Gets the formatted text for a PCAN-Basic channel handle
		/// </summary>
		/// <param name="handle">PCAN-Basic Handle to format</param>
		/// <param name="protocol">Used CAN protocol</param>
		/// <returns>The formatted text for a channel</returns>
		private string FormatChannelName(TPCANHandle handle, CANProtocol protocol)
		{
			TPCANDevice devDevice;
			byte byChannel;

			// Gets the owner device and channel for a PCAN-Basic handle
			if (handle < 0x100)
			{
				devDevice = (TPCANDevice)(handle >> 4);
				byChannel = (byte)(handle & 0xF);
			}
			else
			{
				devDevice = (TPCANDevice)(handle >> 8);
				byChannel = (byte)(handle & 0xFF);
			}

			// Constructs the PCAN-Basic Channel name and return it
			if (protocol == CANProtocol.CAN_XL)
				return string.Format("{0}:XL {1} ({2:X2}h)", devDevice, byChannel, handle);

			if (protocol == CANProtocol.CAN_FD)
				return string.Format("{0}:FD {1} ({2:X2}h)", devDevice, byChannel, handle);

			return string.Format("{0} {1} ({2:X2}h)", devDevice, byChannel, handle);
		}

		/// <summary>
		/// Help Function used to get an error as text
		/// </summary>
		/// <param name="error">Error code to be translated</param>
		/// <returns>A text with the translated error</returns>
		private string GetFormattedError(TPCANStatus error)
		{
			// Creates a buffer big enough for a error-text
			var strTemp = new StringBuilder(256);

			// Gets the text using the GetErrorText API function
			// If the function success, the translated error is returned. If it fails, a text describing the current 
			// error is returned.
			if (PCANBasic.GetErrorText(error, 0x09, strTemp) != TPCANStatus.PCAN_ERROR_OK)
				return string.Format("An error occurred. Error-code's text ({0:X}) couldn't be retrieved", error);

			return strTemp.ToString();
		}

		/// <summary>
		/// Gets the required CAN protocol
		/// </summary>
		/// <param name="isFD">If device supports CAN FD</param>
		/// <param name="isXL">If device supports CAN XL</param>
		/// <returns>A CAN protocol</returns>
		private CANProtocol GetProtocol(bool isFD, bool isXL)
		{
			if (!isFD && !isXL)
				return CANProtocol.CAN;
			else if (isFD && !isXL)
				return CANProtocol.CAN_FD;
			else if (!isFD && isXL)
				return CANProtocol.CAN_XL;
			else if (isFD && isXL)
				return CANProtocol.CAN_XL;
			else
				return CANProtocol.CAN;
		}
		#endregion
	}
}
