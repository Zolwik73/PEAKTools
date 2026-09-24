using Peak.Can.Basic;
using System;
using System.Text;
using System.Timers;

namespace Samples.TraceFiles
{
	// Type alias for a CAN-FD bitrate string
	using TPCANBitrateFD = System.String;
	// Type alias for a CAN-XL bitrate string
	using TPCANBitrateXL = System.String;
	// Type alias for a PCAN-Basic channel handle
	using TPCANHandle = System.UInt16;

	// Enum for connection modes
	enum CANProtocol
	{
		CAN,
		CAN_FD,
		CAN_XL
	}

	public class TraceFilesSample
	{
		#region Defines
		/// <summary>
		/// Sets the PCANHandle (Hardware Channel)
		/// </summary>
		const TPCANHandle PcanHandle = PCANBasic.PCAN_USBBUS1;
		/// <summary>
		/// Sets the desired connection protocol
		/// </summary>
		const CANProtocol Protocol = CANProtocol.CAN;
		/// <summary>
		/// Sets the bitrate for normal CAN devices
		/// </summary>
		const TPCANBaudrate Bitrate = TPCANBaudrate.PCAN_BAUD_500K;
		/// <summary>
		/// Sets the bitrate for CAN FD devices. 
		/// Example - Bitrate Nom: 1Mbit/s FD: 2Mbit/s:
		///   "f_clock_mhz=20, nom_brp=5, nom_tseg1=2, nom_tseg2=1, nom_sjw=1, data_brp=2, data_tseg1=3, data_tseg2=1, data_sjw=1"
		/// </summary>
		const TPCANBitrateFD BitrateFD = "f_clock_mhz=20, nom_brp=5, nom_tseg1=2, nom_tseg2=1, nom_sjw=1, data_brp=2, data_tseg1=3, data_tseg2=1, data_sjw=1";
		/// <summary>
		/// Sets the bitrate for CAN XL devices. 
		/// Example - Bitrate Nom: 500kbit/s FD: 2Mbit/s XL: 8Mbit/s: 
		///   "f_clock=160000000, brp=1, nom_tseg1=255, nom_tseg2=64, nom_sjw=64, fd_tseg1=63, fd_tseg2=16, fd_sjw=16, fd_ssp_offset=0, xl_tseg1=10, xl_tseg2=9, xl_sjw=9, xl_ssp_offset=10, xl_error_signaling=1, xl_transceiver_mode_switch=0"
		/// </summary>
		const TPCANBitrateXL BitrateXL = "f_clock=160000000, brp=1, nom_tseg1=255, nom_tseg2=64, nom_sjw=64, fd_tseg1=63, fd_tseg2=16, fd_sjw=16, fd_ssp_offset=0, xl_tseg1=10, xl_tseg2=9, xl_sjw=9, xl_ssp_offset=10, xl_error_signaling=1, xl_transceiver_mode_switch=0";
		/// <summary>
		/// Sets if trace continue after reaching maximum size for the first file
		/// </summary>
		const bool TraceFileSingle = true;
		/// <summary>
		/// Set if date will be add to filename 
		/// </summary>
		const bool TraceFileDate = true;
		/// <summary>
		/// Set if time will be add to filename
		/// </summary>
		const bool TraceFileTime = true;
		/// <summary>
		/// Set if existing tracefile overwrites when a new trace session is started
		/// </summary>
		const bool TraceFileOverwrite = false;
		/// <summary>
		/// Set if the column "Data Length" should be used instead of the column "Data Length Code"
		/// </summary>
		const bool TraceFileDataLength = false;
		/// <summary>
		/// Sets the size (megabyte) of an tracefile 
		/// Example - 100 = 100 megabyte
		/// Range between 1 and 100
		/// </summary>
		const uint TraceFileSize = 2;
		/// <summary>
		/// Sets a fully-qualified and valid path to an existing directory. In order to use the default path 
		/// (calling process path) an empty string must be set.
		/// </summary>
		const string TracePath = "";
		/// <summary>
		/// Timerinterval (ms) for reading 
		/// </summary>
		const int TimerInterval = 250;
		#endregion

		#region Members
		/// <summary>
		/// Shows if DLL was found
		/// </summary>
		private bool m_DLLFound;
		/// <summary>
		/// Used for reading
		/// </summary>
		private System.Timers.Timer m_Timer;
		#endregion

		/// <summary>
		/// Starts the PCANBasic Sample
		/// </summary>
		public TraceFilesSample()
		{
			ShowConfigurationHelp(); // Shows information about this sample 
			ShowCurrentConfiguration(); // Shows the current parameters configuration

			// Checks if PCANBasic.dll is available, if not, the program terminates
			m_DLLFound = CheckForLibrary();
			if (!m_DLLFound)
				return;

			TPCANStatus stsResult;
			// Initialization of the selected channel
			if (Protocol == CANProtocol.CAN_XL)
				stsResult = PCANBasic.InitializeXL(PcanHandle, BitrateXL);
			else if (Protocol == CANProtocol.CAN_FD)
				stsResult = PCANBasic.InitializeFD(PcanHandle, BitrateFD);
			else
				stsResult = PCANBasic.Initialize(PcanHandle, Bitrate);

			if (stsResult != TPCANStatus.PCAN_ERROR_OK)
			{
				Console.WriteLine("Can not initialize. Please check the defines in the code.");
				ShowStatus(stsResult);
				Console.WriteLine();
				Console.WriteLine("Press any key to close");
				Console.Read();
				return;
			}

			// Trace messages...
			Console.WriteLine("Successfully initialized.");
			Console.WriteLine("Press any key to start tracing...");
			Console.ReadKey();
			if (ConfigureTrace())
			{
				if (StartTrace())
				{
					SetTimer();
					Console.WriteLine("Messages are being traced.");
					Console.WriteLine("Press any key to stop trace and quit");
					Console.ReadKey();
					StopTrace();
					return;
				}
			}
			Console.WriteLine();
			Console.WriteLine("Press any key to close");
			Console.Read();
		}

		~TraceFilesSample()
		{
			if (m_DLLFound)
			{
				PCANBasic.Uninitialize(PCANBasic.PCAN_NONEBUS);
			}
		}

		#region Main-Functions
		/// <summary>
		/// Configures and activates the timer
		/// </summary>
		private void SetTimer()
		{
			m_Timer = new System.Timers.Timer(TimerInterval);
			m_Timer.Elapsed += OnTimedEvent; // Hook up the Elapsed event for the timer. 
			m_Timer.AutoReset = true;
			m_Timer.Enabled = true;
		}

		/// <summary>
		/// Handles the "time elapsed" event
		/// </summary>
		/// <param name="source">The source of the event.</param>
		/// <param name="e">An <seealso cref="ElapsedEventArgs"/> object that contains the event data</param>
		private void OnTimedEvent(Object source, ElapsedEventArgs e)
		{
			ReadMessages();
		}

		/// <summary>
		/// Reads PCAN-Basic messages
		/// </summary>
		private void ReadMessages()
		{
			// We read at least one time the queue looking for messages. If a message is found, we look again trying to 
			// find more. If the queue is empty or an error occur, we get out from the do-while statement.
			TPCANStatus stsResult;
			do
			{
				if (Protocol == CANProtocol.CAN_XL)
					stsResult = PCANBasic.ReadXL(PcanHandle, out TPCANMsgXL CANMsgXL);
				else if (Protocol == CANProtocol.CAN_FD)
					stsResult = PCANBasic.ReadFD(PcanHandle, out TPCANMsgFD CANMsgFD);
				else
					stsResult = PCANBasic.Read(PcanHandle, out TPCANMsg CANMsg);

				if (stsResult != TPCANStatus.PCAN_ERROR_OK && stsResult != TPCANStatus.PCAN_ERROR_QRCVEMPTY)
				{
					ShowStatus(stsResult);
					return;
				}
			} while ((!Convert.ToBoolean(stsResult & TPCANStatus.PCAN_ERROR_QRCVEMPTY)));
		}

		/// <summary>
		/// Deactivates the tracing process
		/// </summary>
		private void StopTrace()
		{
			uint iStatus = PCANBasic.PCAN_PARAMETER_OFF;

			TPCANStatus stsResult = PCANBasic.SetValue(PcanHandle, TPCANParameter.PCAN_TRACE_STATUS, ref iStatus, sizeof(int)); // We stop the tracing by setting the parameter.

			if (stsResult != TPCANStatus.PCAN_ERROR_OK)
				ShowStatus(stsResult);
		}

		/// <summary>
		/// Configures the way how trace files are formatted
		/// </summary>
		/// <returns>Returns true if no error occur</returns>
		private bool ConfigureTrace()
		{
			uint iSize = TraceFileSize;

			// Sets path to store files
			TPCANStatus stsResult = PCANBasic.SetValue(PcanHandle, TPCANParameter.PCAN_TRACE_LOCATION, TracePath, sizeof(int));

			if (stsResult == TPCANStatus.PCAN_ERROR_OK)
			{
				// Sets the maximum size of a tracefile 
				stsResult = PCANBasic.SetValue(PcanHandle, TPCANParameter.PCAN_TRACE_SIZE, ref iSize, sizeof(int));

				if (stsResult == TPCANStatus.PCAN_ERROR_OK)
				{
					uint config;
					if (TraceFileSingle)
						config = PCANBasic.TRACE_FILE_SINGLE; // Creates one file 
					else
						config = PCANBasic.TRACE_FILE_SEGMENTED; // Creates more files

					// Activate overwriting existing tracefile
					if (TraceFileOverwrite)
						config = config | PCANBasic.TRACE_FILE_OVERWRITE;

					// Uses Data Length instead of Data Length Code
					if (TraceFileDataLength)
						config = config | PCANBasic.TRACE_FILE_DATA_LENGTH;

					// Adds date to tracefilename
					if (TraceFileDate)
						config = config | PCANBasic.TRACE_FILE_DATE;

					// Adds time to tracefilename
					if (TraceFileTime)
						config = config | PCANBasic.TRACE_FILE_TIME;

					// Sets the config
					stsResult = PCANBasic.SetValue(PcanHandle, TPCANParameter.PCAN_TRACE_CONFIGURE, ref config, sizeof(int));

					if (stsResult == TPCANStatus.PCAN_ERROR_OK)
						return true;
				}
			}
			ShowStatus(stsResult);
			return false;
		}

		/// <summary>
		/// Activates the tracing process
		/// </summary>
		/// <returns>Returns true if no error occur</returns>
		private bool StartTrace()
		{
			uint iStatus = PCANBasic.PCAN_PARAMETER_ON;

			TPCANStatus stsResult = PCANBasic.SetValue(PcanHandle, TPCANParameter.PCAN_TRACE_STATUS, ref iStatus, sizeof(int)); // We activate the tracing by setting the parameter.

			if (stsResult != TPCANStatus.PCAN_ERROR_OK)
			{
				ShowStatus(stsResult);
				return false;
			}
			return true;
		}
		#endregion

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
			Console.WriteLine("|                           PCAN-Basic TraceFiles Example                                |");
			Console.WriteLine("=========================================================================================");
			Console.WriteLine("Following parameters are to be adjusted before launching, according to the hardware used |");
			Console.WriteLine("                                                                                         |");
			Console.WriteLine("* PcanHandle: Numeric value that represents the handle of the PCAN-Basic channel to use. |");
			Console.WriteLine("              See 'PCAN-Handle Definitions' within the documentation                     |");
			Console.WriteLine("* Protocol: Enum value that indicates the communication protocol, CAN, CAN-FD or CAN-XL. |");
			Console.WriteLine("* Bitrate: Numeric value that represents the BTR0/BR1 bitrate value to be used for CAN   |");
			Console.WriteLine("           communication                                                                 |");
			Console.WriteLine("* BitrateFD: String value that represents the nominal/FD bitrate value to be used for    |");
			Console.WriteLine("             CAN-FD communication                                                        |");
			Console.WriteLine("* BitrateXL: String value that represents the nominal/FD/XL bitrate value to be used for |");
			Console.WriteLine("             CAN-XL communication                                                        |");
			Console.WriteLine("* TraceFileSingle: Boolean value that indicates if tracing ends after one file (true) or |");
			Console.WriteLine("                   continues                                                             |");
			Console.WriteLine("* TraceFileDate: Boolean value that indicates if the date will be added to filename      |");
			Console.WriteLine("* TraceFileTime: Boolean value that indicates if the time will be added to filename      |");
			Console.WriteLine("* TraceFileOverwrite: Boolean value that indicates if existing tracefiles should be      |");
			Console.WriteLine("                      overwritten                                                        |");
			Console.WriteLine("* TraceFileDataLength: Boolean value that indicates if the column 'Data Length' is used  |");
			Console.WriteLine("                       instead of the column 'Data Length Code'                          |");
			Console.WriteLine("* TraceFileSize: Numeric value that represents the size of a tracefile in meagabytes     |");
			Console.WriteLine("* TracePath: string value that represents a valid path to an existing directory          |");
			Console.WriteLine("* TimerInterval: The time, in milliseconds, to wait before trying to write a message     |");
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
			Console.WriteLine("* PCANHandle: " + FormatChannelName(PcanHandle, Protocol));
			Console.WriteLine("* Protocol: " + Protocol.ToString().Replace('_', ' '));
			Console.WriteLine("* Bitrate: " + ConvertBitrateToString(Bitrate));
			Console.WriteLine("* BitrateFD: " + BitrateFD);
			Console.WriteLine("* BitrateXL: " + BitrateXL);
			Console.WriteLine("* TraceFileSingle: " + TraceFileSingle);
			Console.WriteLine("* TraceFileDate: " + TraceFileDate);
			Console.WriteLine("* TraceFileTime: " + TraceFileTime);
			Console.WriteLine("* TraceFileOverwrite: " + TraceFileOverwrite);
			Console.WriteLine("* TraceFileDataLength: " + TraceFileDataLength);
			Console.WriteLine("* TraceFileSize: " + TraceFileSize + " MB");
			if (TracePath == "")
				Console.WriteLine("* TracePath: (calling application path)");
			else
				Console.WriteLine("* TracePath: " + TracePath);
			Console.WriteLine("* TimerInterval: " + TimerInterval);
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

			// Gets the text using the GetErrorText API function. If the function success, the translated error is 
			// returned. If the function success, the translated error is returned. If it fails, a text describing 
			// the  current  error is returned.
			if (PCANBasic.GetErrorText(error, 0x09, strTemp) != TPCANStatus.PCAN_ERROR_OK)
				return string.Format("An error occurred. Error-code's text ({0:X}) couldn't be retrieved", error);

			return strTemp.ToString();
		}

		/// <summary>
		/// Convert bitrate c_short value to readable string
		/// </summary>
		/// <param name="bitrate">Bitrate to be converted</param>
		/// <returns>A text with the converted bitrate</returns>
		private string ConvertBitrateToString(TPCANBaudrate bitrate)
		{
			switch (bitrate)
			{
				case TPCANBaudrate.PCAN_BAUD_1M:
					return "1 MBit/sec";
				case TPCANBaudrate.PCAN_BAUD_800K:
					return "800 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_500K:
					return "500 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_250K:
					return "250 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_125K:
					return "125 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_100K:
					return "100 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_95K:
					return "95,238 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_83K:
					return "83,333 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_50K:
					return "50 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_47K:
					return "47,619 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_33K:
					return "33,333 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_20K:
					return "20 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_10K:
					return "10 kBit/sec";
				case TPCANBaudrate.PCAN_BAUD_5K:
					return "5 kBit/sec";
				default:
					return "Unknown Bitrate";
			}
		}
		#endregion
	}
}
