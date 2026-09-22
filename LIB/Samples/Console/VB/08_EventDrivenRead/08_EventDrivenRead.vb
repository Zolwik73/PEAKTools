' Inclusion of PEAK PCAN-Basic namespace
'
Imports System.Text
Imports System.Threading
Imports EventDrivenRead.Peak.Can.Basic
Imports TPCANHandle = System.UInt16
Imports TPCANTimestampFD = System.UInt64
Imports TPCANTimestampXL = System.UInt64

''' <summary>
''' Enum for connection modes
''' </summary>
Enum CANProtocol
	CAN
	CAN_FD
	CAN_XL
End Enum

Class EventDrivenRead

#Region "Defines"
	''' <summary>
	''' Sets the PCANHandle (Hardware Channel)
	''' </summary>
	Private Const PcanHandle = PCANBasic.PCAN_USBBUS1
	''' <summary>
	''' Sets the desired connection protocol
	''' </summary>
	Private Const Protocol As CANProtocol = CANProtocol.CAN
	''' <summary>
	''' Sets the bitrate for normal CAN devices
	''' </summary>
	Private Const Bitrate = TPCANBaudrate.PCAN_BAUD_500K
	''' <summary>
	''' Sets the bitrate for CAN FD devices. 
	''' Example - Bitrate Nom: 1Mbit/s FD: 2Mbit/s:
	'''   "f_clock_mhz=20, nom_brp=5, nom_tseg1=2, nom_tseg2=1, nom_sjw=1, data_brp=2, data_tseg1=3, data_tseg2=1, data_sjw=1"
	''' </summary>
	Private Const BitrateFD = "f_clock_mhz=20, nom_brp=5, nom_tseg1=2, nom_tseg2=1, nom_sjw=1, data_brp=2, data_tseg1=3, data_tseg2=1, data_sjw=1"
	''' <summary>
	''' Sets the bitrate for CAN XL devices. 
	''' Example - Bitrate Nom: 500kbit/s FD: 2Mbit/s XL: 8Mbit/s: 
	'''   "f_clock=160000000, brp=1, nom_tseg1=255, nom_tseg2=64, nom_sjw=64, fd_tseg1=63, fd_tseg2=16, fd_sjw=16, fd_ssp_offset=0, xl_tseg1=10, xl_tseg2=9, xl_sjw=9, xl_ssp_offset=10, xl_error_signaling=1, xl_transceiver_mode_switch=0"
	''' </summary>
	Private Const BitrateXL = "f_clock=160000000, brp=1, nom_tseg1=255, nom_tseg2=64, nom_sjw=64, fd_tseg1=63, fd_tseg2=16, fd_sjw=16, fd_ssp_offset=0, xl_tseg1=10, xl_tseg2=9, xl_sjw=9, xl_ssp_offset=10, xl_error_signaling=1, xl_transceiver_mode_switch=0"
#End Region

#Region "Members"
	''' <summary>
	''' Shows if DLL was found
	''' </summary>
	Private m_DLLFound As Boolean
	''' <summary>
	''' Thread for reading messages
	''' </summary>
	Private m_ReadThread As Thread
	''' <summary>
	''' Shows if thread run
	''' </summary>
	Private m_ThreadRun As Boolean
#End Region

	Sub Main()
		ShowConfigurationHelp() ' Shows information about this sample
		ShowCurrentConfiguration() ' Shows the current parameters configuration

		' Checks if PCANBasic.dll is available, if not, the program terminates
		m_DLLFound = CheckForLibrary()
		If Not m_DLLFound Then
			Return
		End If

		Dim stsResult As TPCANStatus
		' Initialization of the selected channel
		If Protocol = CANProtocol.CAN_XL Then
			stsResult = PCANBasic.InitializeXL(PcanHandle, BitrateXL)
		ElseIf Protocol = CANProtocol.CAN_FD Then
			stsResult = PCANBasic.InitializeFD(PcanHandle, BitrateFD)
		Else
			stsResult = PCANBasic.Initialize(PcanHandle, Bitrate)
		End If

		If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
			Console.WriteLine("Can not initialize. Please check the defines in the code.")
			ShowStatus(stsResult)
			Console.WriteLine()
			Console.WriteLine("Press any key to close")
			Console.Read()
			Return
		End If

		' Reading messages...
		Console.WriteLine("Successfully initialized.")
		m_ReadThread = New Thread(New ThreadStart(AddressOf Me.ThreadExecute))
		m_ThreadRun = True
		m_ReadThread.Start()
		Console.WriteLine("Started reading messages...")
		Console.WriteLine("")
		Console.WriteLine("Press any key to close")
		Console.ReadKey()
		m_ThreadRun = False
		m_ReadThread.Join()
	End Sub

	Protected Overrides Sub Finalize()
		If m_DLLFound Then
			PCANBasic.Uninitialize(PCANBasic.PCAN_NONEBUS)
		End If
	End Sub

#Region "Main-Functions"
	Private Sub ThreadExecute()
		'' Sets the handle of the Receive-Event.
		Dim evtReceiveEvent As AutoResetEvent
		evtReceiveEvent = New AutoResetEvent(False)
		Dim iBuffer As UInt32
		iBuffer = Convert.ToUInt32(evtReceiveEvent.SafeWaitHandle.DangerousGetHandle().ToInt32())

		Dim stsResult As TPCANStatus
		stsResult = PCANBasic.SetValue(PcanHandle, TPCANParameter.PCAN_RECEIVE_EVENT, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))

		If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
			ShowStatus(stsResult)
			Return
		End If

		While m_ThreadRun
			'' Checks for messages when an event Is received
			If evtReceiveEvent.WaitOne(50) Then
				ReadMessages()
			End If
		End While
		'' Removes the Receive-Event again.
		iBuffer = 0
		stsResult = PCANBasic.SetValue(PcanHandle, TPCANParameter.PCAN_RECEIVE_EVENT, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))

		If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
			ShowStatus(stsResult)
		End If
		evtReceiveEvent.Dispose()
	End Sub
	''' <summary>
	''' Function for reading PCAN-Basic messages
	''' </summary>
	Private Sub ReadMessages()
		Dim stsResult As TPCANStatus

		' We read at least one time the queue looking for messages. If a message is found, we look again trying to 
		' find more. If the queue is empty or an error occur, we get out from the do-while statement.
		Do
			If Protocol = CANProtocol.CAN_XL Then
				stsResult = ReadMessageXL()
			ElseIf Protocol = CANProtocol.CAN_FD Then
				stsResult = ReadMessageFD()
			Else
				stsResult = ReadMessage()
			End If

			If stsResult <> TPCANStatus.PCAN_ERROR_OK And stsResult <> TPCANStatus.PCAN_ERROR_QRCVEMPTY Then
				ShowStatus(stsResult)
				Return
			End If
		Loop While Not Convert.ToBoolean(stsResult And TPCANStatus.PCAN_ERROR_QRCVEMPTY)
	End Sub

	''' <summary>
	''' Function for reading messages on CAN-XL devices
	''' </summary>
	''' <returns>A TPCANStatus error code</returns>
	Private Function ReadMessageXL() As TPCANStatus
		Dim CANMsg As TPCANMsgXL = Nothing
		Dim CANTimeStamp As TPCANTimestampXL
		Dim stsResult As TPCANStatus

		' We execute the "Read" function of the PCANBasic 
		'
		stsResult = PCANBasic.ReadXL(PcanHandle, CANMsg, CANTimeStamp)
		If (stsResult <> TPCANStatus.PCAN_ERROR_QRCVEMPTY) Then
			' We process the received message
			'
			ProcessMessageCanXL(CANMsg, CANTimeStamp)
		End If

		Return stsResult
	End Function

	''' <summary>
	''' Function for reading messages on CAN-FD devices
	''' </summary>
	''' <returns>A TPCANStatus error code</returns>
	Private Function ReadMessageFD() As TPCANStatus
		Dim CANMsg As TPCANMsgFD = Nothing
		Dim CANTimeStamp As TPCANTimestampFD
		Dim stsResult As TPCANStatus

		' We execute the "Read" function of the PCANBasic 
		'
		stsResult = PCANBasic.ReadFD(PcanHandle, CANMsg, CANTimeStamp)
		If (stsResult <> TPCANStatus.PCAN_ERROR_QRCVEMPTY) Then
			' We process the received message
			'
			ProcessMessageCanFD(CANMsg, CANTimeStamp)
		End If

		Return stsResult
	End Function

	''' <summary>
	''' Function for reading CAN messages on normal CAN devices
	''' </summary>
	''' <returns>A TPCANStatus error code</returns>
	Private Function ReadMessage() As TPCANStatus
		Dim CANMsg As TPCANMsg = Nothing
		Dim CANTimeStamp As TPCANTimestamp
		Dim stsResult As TPCANStatus

		' We execute the "Read" function of the PCANBasic 
		'
		stsResult = PCANBasic.Read(PcanHandle, CANMsg, CANTimeStamp)
		If (stsResult <> TPCANStatus.PCAN_ERROR_QRCVEMPTY) Then
			' We process the received message
			'
			ProcessMessageCan(CANMsg, CANTimeStamp)
		End If

		Return stsResult
	End Function

	''' <summary>
	''' Processes a received CAN message
	''' </summary>
	''' <param name="msg">The received PCAN-Basic CAN message</param>
	''' <param name="itsTimeStamp">Timestamp of the message as TPCANTimestamp structure</param>
	Private Sub ProcessMessageCan(ByVal msg As TPCANMsg, ByVal itsTimeStamp As TPCANTimestamp)
		Console.WriteLine("Type: " + GetMsgTypeString(msg.MSGTYPE))
		Console.WriteLine("ID: " + GetIdString(msg.ID, msg.MSGTYPE))
		Console.WriteLine("Length: " + msg.LEN.ToString())
		Console.WriteLine("Time: " + GetTimeString(itsTimeStamp.micros + (1000UL * itsTimeStamp.millis) + (&H100_000_000UL * 1000UL * itsTimeStamp.millis_overflow)))
		Console.WriteLine("Data: " + GetDataString(msg.DATA, msg.MSGTYPE, msg.LEN))
		Console.WriteLine("----------------------------------------------------------")
	End Sub

	''' <summary>
	''' Processes a received CAN-FD message
	''' </summary>
	''' <param name="msg">The received PCAN-Basic CAN-FD message</param>
	''' <param name="itsTimeStamp">Timestamp of the message as microseconds (ulong)</param>
	Private Sub ProcessMessageCanFD(ByVal msg As TPCANMsgFD, ByVal itsTimeStamp As TPCANTimestampFD)
		Console.WriteLine("Type: " + GetMsgTypeString(msg.MSGTYPE))
		Console.WriteLine("ID: " + GetIdString(msg.ID, msg.MSGTYPE))
		Console.WriteLine("Length: " + GetLengthFromDLC(msg.DLC).ToString())
		Console.WriteLine("Time: " + GetTimeString(itsTimeStamp))
		Console.WriteLine("Data: " + GetDataString(msg.DATA, msg.MSGTYPE, GetLengthFromDLC(msg.DLC)))
		Console.WriteLine("----------------------------------------------------------")
	End Sub

	''' <summary>
	''' Processes a received CAN-XL message
	''' </summary>
	''' <param name="msg">The received PCAN-Basic CAN-XL message</param>
	''' <param name="itsTimeStamp">Timestamp of the message as microseconds (ulong)</param>
	Private Sub ProcessMessageCanXL(ByVal msg As TPCANMsgXL, ByVal itsTimeStamp As TPCANTimestampXL)
		Console.WriteLine("Type: " + GetMsgTypeStringXL(msg.MSGTYPE, msg.RRS, msg.SEC))
		Console.WriteLine("PID: " + GetIdString(msg.PID, msg.MSGTYPE))
		Console.WriteLine("Length: " + GetLengthFromDLC(msg.DLC).ToString())
		Console.WriteLine("Time: " + GetTimeString(itsTimeStamp))
		Console.WriteLine("Data: " + GetDataString(msg.DATA, msg.MSGTYPE, GetLengthFromDLC(msg.DLC)))
		Console.WriteLine("----------------------------------------------------------")
	End Sub
#End Region

#Region "Help-Functions"
	''' <summary>
	''' Checks for availability of the PCANBasic library
	''' </summary>
	''' <returns>If the library was found or not</returns>
	Private Function CheckForLibrary() As Boolean
		' Check for dll file
		Try
			PCANBasic.Uninitialize(PCANBasic.PCAN_NONEBUS)
			Return True
		Catch ex As Exception
			Console.WriteLine("Unable to find the library: PCANBasic.dll !")
			Console.WriteLine("Press any key to close")
			Console.ReadKey()
		End Try
		Return False
	End Function

	''' <summary>
	''' Shows/prints the configurable parameters for this sample and information about them
	''' </summary>
	Private Sub ShowConfigurationHelp()
		Console.WriteLine("=========================================================================================")
		Console.WriteLine("|                        PCAN-Basic EventDrivenRead Example                              |")
		Console.WriteLine("=========================================================================================")
		Console.WriteLine("Following parameters are to be adjusted before launching, according to the hardware used |")
		Console.WriteLine("                                                                                         |")
		Console.WriteLine("* PcanHandle: Numeric value that represents the handle of the PCAN-Basic channel to use. |")
		Console.WriteLine("              See 'PCAN-Handle Definitions' within the documentation                     |")
		Console.WriteLine("* Protocol: Enum value that indicates the communication protocol, CAN, CAN-FD or CAN-XL. |")
		Console.WriteLine("* Bitrate: Numeric value that represents the BTR0/BR1 bitrate value to be used for CAN   |")
		Console.WriteLine("           communication                                                                 |")
		Console.WriteLine("* BitrateFD: String value that represents the nominal/FD bitrate value to be used for    |")
		Console.WriteLine("             CAN-FD communication                                                        |")
		Console.WriteLine("* BitrateXL: String value that represents the nominal/FD/XL bitrate value to be used for |")
		Console.WriteLine("             CAN-XL communication                                                        |")
		Console.WriteLine("=========================================================================================")
		Console.WriteLine("")
	End Sub

	''' <summary>
	''' Shows/prints the configured parameters
	''' </summary>
	Private Sub ShowCurrentConfiguration()
		Console.WriteLine("Parameter values used")
		Console.WriteLine("----------------------")
		Console.WriteLine("* PCANHandle: " + FormatChannelName(PcanHandle, Protocol))
		Console.WriteLine("* Protocol: " + Protocol.ToString.Replace("_", " "))
		Console.WriteLine("* Bitrate: " + ConvertBitrateToString(Bitrate))
		Console.WriteLine("* BitrateFD: " + BitrateFD)
		Console.WriteLine("* BitrateXL: " + BitrateXL)
		Console.WriteLine("")
	End Sub

	''' <summary>
	''' Shows formatted status
	''' </summary>
	''' <param name="status">Will be formatted</param>
	Private Sub ShowStatus(status As TPCANStatus)
		Console.WriteLine("=========================================================================================")
		Console.WriteLine(GetFormattedError(status))
		Console.WriteLine("=========================================================================================")
	End Sub

	''' <summary>
	''' Gets the formatted text for a PCAN-Basic channel handle
	''' </summary>
	''' <param name="handle">PCAN-Basic Handle to format</param>
	''' <param name="protocol">Used CAN protocol</param>
	''' <returns>The formatted text for a channel</returns>
	Private Function FormatChannelName(ByVal handle As TPCANHandle, ByVal protocol As CANProtocol) As String
		Dim devDevice As TPCANDevice
		Dim byChannel As Byte

		' Gets the owner device and channel for a PCAN-Basic handle
		'
		If handle < &H100 Then
			devDevice = DirectCast(CType(handle >> 4, Byte), TPCANDevice)
			byChannel = CByte((handle And &HF))
		Else
			devDevice = DirectCast(CType(handle >> 8, Byte), TPCANDevice)
			byChannel = CByte((handle And &HFF))
		End If

		' Constructs the PCAN-Basic Channel name and return it
		'
		If protocol = CANProtocol.CAN_XL Then
			Return String.Format("{0}:XL {1} ({2:X2}h)", devDevice, byChannel, handle)
		ElseIf protocol = CANProtocol.CAN_FD Then
			Return String.Format("{0}:FD {1} ({2:X2}h)", devDevice, byChannel, handle)
		Else
			Return String.Format("{0} {1} ({2:X2}h)", devDevice, byChannel, handle)
		End If
	End Function


	''' <summary>
	''' Help Function used to get an error as text
	''' </summary>
	''' <param name="error">Error code to be translated</param>
	''' <returns>A text with the translated error</returns>
	Private Function GetFormattedError(ByVal [error] As TPCANStatus) As String
		Dim strTemp As StringBuilder

		' Creates a buffer big enough for a error-text
		'
		strTemp = New StringBuilder(256)
		' Gets the text using the GetErrorText API function
		' If the function success, the translated error is returned. If it fails,
		' a text describing the current error is returned.
		'
		If PCANBasic.GetErrorText([error], &H9, strTemp) <> TPCANStatus.PCAN_ERROR_OK Then
			Return String.Format("An error occurred. Error-code's text ({0:X}) couldn't be retrieved", [error])
		Else
			Return strTemp.ToString()
		End If
	End Function

	''' <summary>
	''' Convert bitrate c_short value to readable string
	''' </summary>
	''' <param name="bitrate">Bitrate to be converted</param>
	''' <returns>A text with the converted bitrate</returns>
	Private Function ConvertBitrateToString(ByVal bitrate As TPCANBaudrate) As String
		Select Case bitrate
			Case TPCANBaudrate.PCAN_BAUD_1M
				Return "1 MBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_800K
				Return "800 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_500K
				Return "500 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_250K
				Return "250 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_125K
				Return "125 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_100K
				Return "100 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_95K
				Return "95,238 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_83K
				Return "83,333 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_50K
				Return "50 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_47K
				Return "47,619 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_33K
				Return "33,333 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_20K
				Return "20 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_10K
				Return "10 kBit/sec"
			Case TPCANBaudrate.PCAN_BAUD_5K
				Return "5 kBit/sec"
			Case Else
				Return "Unknown Bitrate"
		End Select
	End Function

	''' <summary>
	''' Gets the string representation of the type of a CAN message
	''' </summary>
	''' <param name="msgType">Type of a CAN message</param>
	''' <returns>The type of the CAN message as string</returns>
	Private Function GetMsgTypeString(ByVal msgType As TPCANMessageType) As String
		Dim strTemp As String

		If (msgType And TPCANMessageType.PCAN_MESSAGE_STATUS) = TPCANMessageType.PCAN_MESSAGE_STATUS Then
			Return "STATUS"
		End If

		If (msgType And TPCANMessageType.PCAN_MESSAGE_ERRFRAME) = TPCANMessageType.PCAN_MESSAGE_ERRFRAME Then
			Return "ERROR"
		End If

		If (msgType And TPCANMessageType.PCAN_MESSAGE_EXTENDED) = TPCANMessageType.PCAN_MESSAGE_EXTENDED Then
			strTemp = "EXT"
		Else
			strTemp = "STD"
		End If

		If (msgType And TPCANMessageType.PCAN_MESSAGE_RTR) = TPCANMessageType.PCAN_MESSAGE_RTR Then
			strTemp += "/RTR"
		Else
			If (msgType > TPCANMessageType.PCAN_MESSAGE_EXTENDED) Then
				strTemp += " [ "
				If ((msgType And TPCANMessageType.PCAN_MESSAGE_FD) = TPCANMessageType.PCAN_MESSAGE_FD) Then
					strTemp += " FD"
				End If
				If ((msgType And TPCANMessageType.PCAN_MESSAGE_BRS) = TPCANMessageType.PCAN_MESSAGE_BRS) Then
					strTemp += " BRS"
				End If
				If ((msgType And TPCANMessageType.PCAN_MESSAGE_ESI) = TPCANMessageType.PCAN_MESSAGE_ESI) Then
					strTemp += " ESI"
				End If
				strTemp += " ]"
			End If
		End If

		Return strTemp
	End Function

	''' <summary>
	''' Gets the string representation of the type of a CAN XL message
	''' </summary>
	''' <param name="msgType">Type of a CAN XL message</param>
	''' <param name="rrsFlag">The RRS flag of the XL message</param>
	''' <param name="secFlag">The SEC flag of the XL message</param>
	''' <returns>The type of the CAN XL message as string</returns>
	Private Function GetMsgTypeStringXL(ByVal msgType As TPCANMessageTypeXL, ByVal rrsFlag As Boolean, ByVal secFlag As Boolean) As String

		Dim strTemp As String
		Dim isEcho = (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_ECHO) = TPCANMessageTypeXL.PCAN_MESSAGE_ECHO

		If (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_STATUS) = TPCANMessageTypeXL.PCAN_MESSAGE_STATUS Then
			Return "STATUS"
		End If

		If (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_ERRFRAME) = TPCANMessageTypeXL.PCAN_MESSAGE_ERRFRAME Then
			Return "ERROR"
		End If

		If (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_XL) = TPCANMessageTypeXL.PCAN_MESSAGE_XL Then
			strTemp = "XL"
		ElseIf (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED) = TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED Then
			strTemp = "EXT"
		Else
			strTemp = "STD"
		End If

		If (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_RTR) = TPCANMessageTypeXL.PCAN_MESSAGE_RTR Then
			strTemp += If(isEcho, "/RTR [ ECHO ]", "/RTR")
		Else
			If (isEcho) Then
				strTemp += " [ ECHO"
			Else
				strTemp += " [ "
			End If

			If CInt(msgType) >= CUInt(TPCANMessageTypeXL.PCAN_MESSAGE_XL) Then
				If rrsFlag Then
					strTemp += " RRS"
				End If
				If secFlag Then
					strTemp += " SEC"
				End If

				strTemp += " ]"
			ElseIf CInt(msgType) > CInt(TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED) Then
				If isEcho Then
					strTemp += " [ ECHO"
				Else
					strTemp += " [ "
				End If

				If (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_FD) = TPCANMessageTypeXL.PCAN_MESSAGE_FD Then
					strTemp += " FD"
				End If
				If (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_BRS) = TPCANMessageTypeXL.PCAN_MESSAGE_BRS Then
					strTemp += " BRS"
				End If
				If (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_ESI) = TPCANMessageTypeXL.PCAN_MESSAGE_ESI Then
					strTemp += " ESI"
				End If

				strTemp += " ]"
			End If
		End If

		Return strTemp
	End Function

	''' <summary>
	''' Gets the string representation of the ID of a CAN message
	''' </summary>
	''' <param name="id">Id to be parsed</param>
	''' <param name="msgType">Type flags of the message the Id belong</param>
	''' <returns>Hexadecimal representation of the ID of a CAN message</returns>
	Private Function GetIdString(ByVal id As UInteger, ByVal msgType As TPCANMessageType) As String
		If (msgType And TPCANMessageType.PCAN_MESSAGE_EXTENDED) = TPCANMessageType.PCAN_MESSAGE_EXTENDED Then
			Return String.Format("{0:X8}h", id)
		Else
			Return String.Format("{0:X3}h", id)
		End If
	End Function

	''' <summary>
	''' Gets the string representation of the ID of a CAN XL message
	''' </summary>
	''' <param name="id">Id to be parsed</param>
	''' <param name="msgType">Type flags of the message the Id belong</param>
	''' <returns>Hexadecimal representation of the ID of a CAN XL message</returns>
	Private Function GetIdString(ByVal id As UInteger, ByVal msgType As TPCANMessageTypeXL) As String
		If (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED) = TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED Then
			Return String.Format("{0:X8}h", id)
		Else
			Return String.Format("{0:X3}h", id)
		End If
	End Function

	''' <summary>
	''' Gets the data length of a CAN message
	''' </summary>
	''' <param name="dlc">Data length code of a CAN message</param>
	''' <returns>Data length as integer represented by the given DLC code</returns>
	Private Function GetLengthFromDLC(ByVal dlc As Byte) As Integer
		Select Case dlc
			Case 9
				Return 12
			Case 10
				Return 16
			Case 11
				Return 20
			Case 12
				Return 24
			Case 13
				Return 32
			Case 14
				Return 48
			Case 15
				Return 64
			Case Else
				Return dlc
		End Select
	End Function

	''' <summary>
	''' Converts a CAN DLC value into the actual data length of the CAN-XL frame
	''' </summary>
	''' <param name="dlc">A value between 0 and 2047 (CAN XL DLC range)</param>
	''' <returns>The length represented by the DLC</returns>
	Public Shared Function GetLengthFromDLC(ByVal dlc As UShort) As Integer
		If dlc < 0 Then
			Return 1
		End If
		If dlc > 2047 Then
			Return 2048
		End If

		Return dlc + 1
	End Function

	''' <summary>
	''' Gets the string representation of the timestamp of a CAN message, in milliseconds
	''' </summary>
	''' <returns></returns>
	Private Function GetTimeString(ByVal time As TPCANTimestampFD) As String
		Dim fTime As Double

		fTime = (time / 1000.0R)
		Return fTime.ToString("F1")
	End Function

	''' <summary>
	''' Gets the data of a CAN message as a string
	''' </summary>
	''' <param name="data">Array of bytes containing the data to parse</param>
	''' <param name="msgType">Type flags of the message the data belong</param>
	''' <param name="dataLength">The amount of bytes to take into account within the given data</param>
	''' <returns>A string with hexadecimal formatted data bytes of a CAN message</returns>
	Private Function GetDataString(ByVal data As Byte(), ByVal msgType As TPCANMessageType, ByVal dataLength As Integer) As String
		If (msgType And TPCANMessageType.PCAN_MESSAGE_RTR) = TPCANMessageType.PCAN_MESSAGE_RTR Then
			Return "Remote Request"
		Else
			Return GetDataString(data, dataLength)
		End If
	End Function

	''' <summary>
	''' Gets the data of a CAN XL message as a string
	''' </summary>
	''' <param name="data">Array of bytes containing the data to parse</param>
	''' <param name="msgType">Type flags of the message the data belong</param>
	''' <param name="dataLength">The amount of bytes to take into account within the given data</param>
	''' <returns>A string with hexadecimal formatted data bytes of a CAN message</returns>
	Private Function GetDataString(ByVal data As Byte(), ByVal msgType As TPCANMessageTypeXL, ByVal dataLength As Integer) As String
		If (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_RTR) = TPCANMessageTypeXL.PCAN_MESSAGE_RTR Then
			Return "Remote Request"
		Else
			Return GetDataString(data, dataLength)
		End If
	End Function

	''' <summary>
	''' Gets the data of a CAN message as a string
	''' </summary>
	''' <param name="data">Array of bytes containing the data to parse</param>
	''' <param name="dataLength">The amount of bytes to take into account within the given data</param>
	''' <returns>A string with hexadecimal formatted data bytes of a CAN message</returns>
	Private Function GetDataString(ByVal data As Byte(), ByVal dataLength As Integer) As String
		Dim strTemp As String = ""

		For i As Integer = 0 To dataLength - 1
			strTemp += String.Format("{0:X2} ", data(i))
		Next

		Return strTemp
	End Function
#End Region
End Class

Module Start
	Sub Main()
		Dim p = New EventDrivenRead()
		p.Main()
	End Sub
End Module