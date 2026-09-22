Imports System.Text

' Inclusion of PEAK PCAN-Basic namespace
'
Imports Peak.Can.Basic
Imports TPCANHandle = System.UInt16
Imports TPCANTimestampXL = System.UInt64

Public Class Form1

    Public Sub New()
        ' Initializes Form's component
        '
        InitializeComponent()
        ' Initializes specific components
        '
        InitializeBasicComponents()
    End Sub

#Region "Enums"
    <Flags()>
    Enum CANProtocol
        CAN = &H1
        CAN_FD = &H2
        CAN_XL = &H4
    End Enum
#End Region

#Region "Structures"
    ''' <summary>
    ''' Message Status structure used to show CAN Messages
    ''' in a ListView
    ''' </summary>
    Private Class MessageStatus
        Private m_MsgXL As TPCANMsgXL
        Private m_TimeStampXL As TPCANTimestampXL
        Private m_oldTimeStampXL As TPCANTimestampXL
        Private m_iIndex As Integer
        Private m_Count As Integer
        Private m_bShowPeriod As Boolean
        Private m_bWasChanged As Boolean

        Public Sub New(ByVal canMsg As TPCANMsgXL, ByVal canTimestamp As TPCANTimestampXL, ByVal listIndex As Integer)
            m_MsgXL = canMsg
            m_TimeStampXL = canTimestamp
            m_oldTimeStampXL = canTimestamp
            m_iIndex = listIndex
            m_Count = 1
            m_bShowPeriod = True
            m_bWasChanged = False
        End Sub

        Public Sub Update(ByVal canMsg As TPCANMsgXL, ByVal canTimestamp As TPCANTimestampXL)
            m_MsgXL = canMsg
            m_oldTimeStampXL = m_TimeStampXL
            m_TimeStampXL = canTimestamp
            m_bWasChanged = True
            m_Count += 1
        End Sub

        Private Function GetMsgTypeString() As String
            Dim strTemp As String
            Dim isEcho As Boolean = (m_MsgXL.MSGTYPE And TPCANMessageTypeXL.PCAN_MESSAGE_ECHO) = TPCANMessageTypeXL.PCAN_MESSAGE_ECHO

            If (m_MsgXL.MSGTYPE And TPCANMessageTypeXL.PCAN_MESSAGE_STATUS) = TPCANMessageTypeXL.PCAN_MESSAGE_STATUS Then
                Return "STATUS"
            End If

            If (m_MsgXL.MSGTYPE And TPCANMessageTypeXL.PCAN_MESSAGE_ERRFRAME) = TPCANMessageTypeXL.PCAN_MESSAGE_ERRFRAME Then
                Return "ERROR"
            End If

            If (m_MsgXL.MSGTYPE And TPCANMessageTypeXL.PCAN_MESSAGE_XL) = TPCANMessageTypeXL.PCAN_MESSAGE_XL Then
                strTemp = "XL"
            ElseIf (m_MsgXL.MSGTYPE And TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED) = TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED Then
                strTemp = "EXT"
            Else
                strTemp = "STD"
            End If

            If (m_MsgXL.MSGTYPE And TPCANMessageTypeXL.PCAN_MESSAGE_RTR) = TPCANMessageTypeXL.PCAN_MESSAGE_RTR Then
                If isEcho Then
                    strTemp &= "/RTR [ ECHO ]"
                Else
                    strTemp &= "/RTR"
                End If
            Else
                If isEcho Then
                    strTemp &= " [ ECHO"
                Else
                    strTemp &= " [ "
                End If

                If CInt(m_MsgXL.MSGTYPE) >= CUInt(TPCANMessageTypeXL.PCAN_MESSAGE_XL) Then
                    If m_MsgXL.RRS Then
                        strTemp &= " RRS"
                    End If
                    If m_MsgXL.SEC Then
                        strTemp &= " SEC"
                    End If
                ElseIf CInt(m_MsgXL.MSGTYPE) > CInt(TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED) Then
                    If (m_MsgXL.MSGTYPE And TPCANMessageTypeXL.PCAN_MESSAGE_FD) = TPCANMessageTypeXL.PCAN_MESSAGE_FD Then
                        strTemp &= " FD"
                    End If
                    If (m_MsgXL.MSGTYPE And TPCANMessageTypeXL.PCAN_MESSAGE_BRS) = TPCANMessageTypeXL.PCAN_MESSAGE_BRS Then
                        strTemp &= " BRS"
                    End If
                    If (m_MsgXL.MSGTYPE And TPCANMessageTypeXL.PCAN_MESSAGE_ESI) = TPCANMessageTypeXL.PCAN_MESSAGE_ESI Then
                        strTemp &= " ESI"
                    End If
                End If

                strTemp &= " ]"
            End If

            Return strTemp
        End Function

        Private Function GetPIDString() As String
            Return String.Format("{0:X3}h", m_MsgXL.PID)
        End Function

        Private Function GetSDTString() As String
            Return String.Format("{0:X2}h", m_MsgXL.SDT)
        End Function

        Private Function GetVCIDString() As String
            Return String.Format("{0:X2}h", m_MsgXL.VCID)
        End Function

        Private Function GetAFString() As String
            Return String.Format("{0:X8}h", m_MsgXL.AF)
        End Function

        Private Function GetDataString() As String
            Dim strTemp As String = ""
            Dim iDLC As Integer = Form1.GetLengthFromDLC(m_MsgXL.DLC, m_MsgXL.MSGTYPE)

            If (m_MsgXL.MSGTYPE And TPCANMessageTypeXL.PCAN_MESSAGE_RTR) = TPCANMessageTypeXL.PCAN_MESSAGE_RTR Then
                Return "Remote Request"
            Else
                For i As Integer = 0 To iDLC - 1
                    strTemp &= String.Format("{0:X2} ", m_MsgXL.DATA(i))
                Next
            End If

            Return strTemp
        End Function

        Private Function GetTimeString() As String
            Dim fTime As Double

            fTime = (m_TimeStampXL / 1000.0R)
            If m_bShowPeriod Then
                fTime -= (m_oldTimeStampXL / 1000.0R)
            End If

            Return fTime.ToString("F1")
        End Function

        Public ReadOnly Property CANMsg() As TPCANMsgXL
            Get
                Return m_MsgXL
            End Get
        End Property

        Public ReadOnly Property Timestamp() As TPCANTimestampXL
            Get
                Return m_TimeStampXL
            End Get
        End Property

        Public ReadOnly Property Position() As Integer
            Get
                Return m_iIndex
            End Get
        End Property

        Public ReadOnly Property TypeString() As String
            Get
                Return GetMsgTypeString()
            End Get
        End Property

        Public ReadOnly Property PIDString As String
            Get
                Return GetPIDString()
            End Get
        End Property

        Public ReadOnly Property SDTString As String
            Get
                Return GetSDTString()
            End Get
        End Property

        Public ReadOnly Property VCIDString As String
            Get
                Return GetVCIDString()
            End Get
        End Property

        Public ReadOnly Property AFString As String
            Get
                Return GetAFString()
            End Get
        End Property

        Public ReadOnly Property DataString() As String
            Get
                Return GetDataString()
            End Get
        End Property

        Public ReadOnly Property Count() As Integer
            Get
                Return m_Count
            End Get
        End Property

        Public Property ShowingPeriod() As Boolean
            Get
                Return m_bShowPeriod
            End Get
            Set(ByVal value As Boolean)
                If m_bShowPeriod Xor value Then
                    m_bShowPeriod = value
                    m_bWasChanged = True
                End If
            End Set
        End Property

        Public Property MarkedAsUpdated() As Boolean
            Get
                Return m_bWasChanged
            End Get
            Set(ByVal value As Boolean)
                m_bWasChanged = value
            End Set
        End Property

        Public ReadOnly Property TimeString() As String
            Get
                Return GetTimeString()
            End Get
        End Property
    End Class
#End Region

#Region "Delegates"
    ''' <summary>
    ''' Read-Delegate Handler
    ''' </summary>
    Private Delegate Sub ReadDelegateHandler()
#End Region

#Region "Members"
    ''' <summary>
    ''' Saves the handle of a PCAN hardware
    ''' </summary>
    Private m_PcanHandle As TPCANHandle
    ''' <summary>
    ''' Stores the status of received messages for its display
    ''' </summary>
    Private m_LastMsgsList As System.Collections.ArrayList
    ''' <summary>
    ''' Read Delegate for calling the function "ReadMessages"
    ''' </summary>
    Private m_ReadDelegate As ReadDelegateHandler
    ''' <summary>
    ''' Receive-Event
    ''' </summary>
    Private m_ReceiveEvent As System.Threading.AutoResetEvent
    ''' <summary>
    ''' Thread for message reading (using events)
    ''' </summary>
    Private m_ReadThread As System.Threading.Thread
    ''' <summary>
    ''' CAN Data for message writing
    ''' </summary>
    Private m_data() As Byte
    ''' <summary>
    ''' Indicates whether the message-reading thread is running or not
    ''' </summary>
    Private m_readThreadRunning As Boolean
#End Region

#Region "Methods"
#Region "UI Handler"
    Private Sub btnInit_Click(ByVal sender As Object, ByVal e As EventArgs) Handles Button1.Click, btnInit.Click
        Dim stsResult As TPCANStatus

        ' Connects a selected PCAN-Basic channel
        '
        stsResult = PCANBasic.InitializeXL(m_PcanHandle, txtBitrate.Text)

        If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
            If stsResult <> TPCANStatus.PCAN_ERROR_CAUTION Then
                MessageBox.Show(GetFormattedError(stsResult))
            Else
                IncludeTextMessage("******************************************************")
                IncludeTextMessage("The bitrate being used is different than the given one")
                IncludeTextMessage("******************************************************")
                stsResult = TPCANStatus.PCAN_ERROR_OK
            End If

        Else
            ' Prepares the PCAN-Basic's PCAN-Trace file
            '
            ConfigureTraceFile()
        End If

        ' Sets the connection status of the main-form
        '
        SetConnectionStatus(stsResult = TPCANStatus.PCAN_ERROR_OK)
    End Sub

    Private Sub cbbChannel_SelectedIndexChanged(ByVal sender As Object, ByVal e As EventArgs) Handles cbbChannel.SelectedIndexChanged
        Dim strTemp As String

        ' Get the handle from the text being shown
        '
        strTemp = cbbChannel.Text
        If (strTemp = String.Empty) Then
            Return
        ElseIf (strTemp.Contains("refresh")) Then
            DoRefresh()
        Else
            strTemp = strTemp.Substring(strTemp.IndexOf("(") + 1, 3)

            strTemp = strTemp.Replace("h", " ").Trim(" ")

            m_PcanHandle = Convert.ToUInt16(strTemp, 16)
        End If

    End Sub

    Private Sub btnRelease_Click(ByVal sender As Object, ByVal e As EventArgs) Handles btnRelease.Click
        ' Releases a current connected PCAN-Basic channel
        '
        PCANBasic.Uninitialize(m_PcanHandle)
        tmrRead.Enabled = False
        If m_ReadThread IsNot Nothing Then
            m_readThreadRunning = False
            m_ReadThread.Join()
            m_ReadThread = Nothing
        End If

        ' Sets the connection status of the main-form
        '
        SetConnectionStatus(False)

    End Sub

    Private Sub btnFilterApply_Click(ByVal sender As Object, ByVal e As EventArgs) Handles btnFilterApply.Click
        Dim iBuffer As UInteger
        Dim stsResult As TPCANStatus

        ' Gets the current status of the message filter
        '
        If Not GetFilterStatus(iBuffer) Then
            Return
        End If

        ' The filter will be full opened or complete closed
        '
        If rdbFilterClose.Checked Then
            iBuffer = PCANBasic.PCAN_FILTER_CLOSE
        Else
            iBuffer = PCANBasic.PCAN_FILTER_OPEN
        End If

        ' The filter is configured
        '
        stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_MESSAGE_FILTER, iBuffer, CType(Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))

        ' If success, an information message is written, if it is not, an error message is shown
        '
        If stsResult = TPCANStatus.PCAN_ERROR_OK Then
            IncludeTextMessage(String.Format("The filter was successfully {0}", If(rdbFilterClose.Checked, "closed.", "opened.")))
        Else
            MessageBox.Show(GetFormattedError(stsResult))
        End If
    End Sub

    Private Sub btnFilterQuery_Click(ByVal sender As Object, ByVal e As EventArgs) Handles btnFilterQuery.Click
        Dim iBuffer As UInteger

        ' Queries the current status of the message filter
        '
        If GetFilterStatus(iBuffer) Then
            Select Case iBuffer
                ' The filter is closed
                '
                Case PCANBasic.PCAN_FILTER_CLOSE
                    IncludeTextMessage("The Status of the filter is: closed.")
                    Exit Select
                    ' The filter is fully opened
                    '
                Case PCANBasic.PCAN_FILTER_OPEN
                    IncludeTextMessage("The Status of the filter is: full opened.")
                    Exit Select
                Case Else
                    ' The status of the filter is undefined. (Should never happen)
                    '
                    IncludeTextMessage("The Status of the filter is: Invalid.")
                    Exit Select
            End Select
        End If
    End Sub

    Private Sub cbbParameter_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cbbParameter.SelectedIndexChanged
        ' Activates/deactivates controls according with the selected 
        ' PCAN-Basic parameter 
        '
        rdbParamActive.Enabled = (cbbParameter.SelectedIndex <> 0) And (cbbParameter.SelectedIndex <> 22)
        rdbParamInactive.Enabled = rdbParamActive.Enabled
        nudDeviceId.Enabled = Not rdbParamActive.Enabled
        nudDelay.Enabled = Not rdbParamActive.Enabled
        laDeviceOrDelay.Text = If((cbbParameter.SelectedIndex = 22), "Delay (μs):", "Device ID (Hex):")
        nudDelay.Visible = (cbbParameter.SelectedIndex = 22)
        nudDeviceId.Visible = Not nudDelay.Visible
    End Sub

    Private Sub btnParameterSet_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnParameterSet.Click
        Dim stsResult As TPCANStatus
        Dim iBuffer As UInt32
        Dim bActivate As Boolean

        bActivate = rdbParamActive.Checked

        ' Sets a PCAN-Basic parameter value
        '
        Select Case cbbParameter.SelectedIndex
            ' The device identifier of a channel will be set
            '
            Case 0
                iBuffer = Convert.ToUInt32(nudDeviceId.Value)
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_DEVICE_ID, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage("The desired Device-ID was successfully configured")
                End If
                Exit Select
                ' The 5 Volt Power feature of a channel will be set
                '
            Case 1
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_5VOLTS_POWER, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The USB/PC-Card 5 power was successfully {0}", If(bActivate, "activated", "deactivated")))
                End If
                Exit Select
                ' The feature for automatic reset on BUS-OFF will be set
                '
            Case 2
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_BUSOFF_AUTORESET, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The automatic-reset on BUS-OFF was successfully {0}", If(bActivate, "activated", "deactivated")))
                End If
                Exit Select
                ' The CAN option "Listen Only" will be set
                '
            Case 3
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_LISTEN_ONLY, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The CAN option ""Listen Only"" was successfully {0}", If(bActivate, "activated", "deactivated")))
                End If
                Exit Select
                ' The feature for logging debug-information will be set
                '
            Case 4
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_LOG_STATUS, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The feature for logging debug information was successfully {0}", If(bActivate, "activated", "deactivated")))
                End If
                Exit Select
                ' The channel option "Receive Status" will be set
                '
            Case 5
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_RECEIVE_STATUS, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The channel option ""Receive Status"" was set to {0}", If(bActivate, "ON", "OFF")))
                End If
                Exit Select
                ' The feature for tracing will be set
                '
            Case 7
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_TRACE_STATUS, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The feature for tracing data was successfully {0}", If(bActivate, "activated", "deactivated")))
                End If
                Exit Select
                ' The feature for identifying an USB Channel will be set
                '
            Case 8
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_CHANNEL_IDENTIFYING, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The procedure for channel identification was successfully {0}", If(bActivate, "activated", "deactivated")))
                End If
                Exit Select
                ' The feature for using an already configured speed will be set
                '
            Case 10
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_BITRATE_ADAPTING, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The feature for bit rate adaptation was successfully {0}", If(bActivate, "activated", "deactivated")))
                End If
                Exit Select
                ' The option "Allow Status Frames" will be set
                '
            Case 19
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_STATUS_FRAMES, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The reception of Status frames was successfully {0}", If(bActivate, "enabled", "disabled")))
                End If
                Exit Select
                ' The option "Allow RTR Frames" will be set
                '
            Case 20
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_RTR_FRAMES, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The reception of RTR frames was successfully {0}", If(bActivate, "enabled", "disabled")))
                End If
                Exit Select
                ' The option "Allow Error Frames" will be set
                '
            Case 21
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_ERROR_FRAMES, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The reception of Error frames was successfully {0}", If(bActivate, "enabled", "disabled")))
                End If
                Exit Select

                ' The option "Interframes Delay" will be set
                '
            Case 22
                iBuffer = Convert.ToUInt32(nudDelay.Value)
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_INTERFRAME_DELAY, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage("The delay between transmitting frames was successfully set")
                End If
                Exit Select

                ' The option "Allow Echo Frames" will be set
                '
            Case 23
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_ECHO_FRAMES, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The reception of Echo frames was successfully {0}", If(bActivate, "enabled", "disabled")))
                End If

                ' The option 'Hard Reset Status' will be set
                '
            Case 24
                iBuffer = CUInt((If(bActivate, PCANBasic.PCAN_PARAMETER_ON, PCANBasic.PCAN_PARAMETER_OFF)))
                stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_HARD_RESET_STATUS, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The activation of a hard reset within the method PCANBasic.Reset was successfully {0}", If(bActivate, "enabled", "disabled")))
                End If
                Exit Select

            Case Else
                ' The current parameter is invalid
                '
                stsResult = TPCANStatus.PCAN_ERROR_UNKNOWN
                MessageBox.Show("Wrong parameter code.")
                Return
        End Select

        ' If the function fail, an error message is shown
        '
        If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
            MessageBox.Show(GetFormattedError(stsResult))
        End If

    End Sub

    Private Sub btnParameterGet_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnParameterGet.Click
        Dim stsResult As TPCANStatus
        Dim iBuffer As UInt32
        Dim strBuffer As StringBuilder

        strBuffer = New StringBuilder(256)

        ' Gets a PCAN-Basic parameter value
        '
        Select Case cbbParameter.SelectedIndex
            ' The device identifier of a channel will be retrieved
            '
            Case 0
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_DEVICE_ID, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The configured Device-ID is 0x{0:X}", iBuffer))
                End If
                Exit Select
                ' The activation status of the 5 Volt Power feature of a channel will be retrieved
                '
            Case 1
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_5VOLTS_POWER, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The 5-Volt Power of the USB/PC-Card is {0:X}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "ON", "OFF")))
                End If
                Exit Select
                ' The activation status of the feature for automatic reset on BUS-OFF will be retrieved
                '
            Case 2
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BUSOFF_AUTORESET, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The automatic-reset on BUS-OFF is {0:X}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "ON", "OFF")))
                End If
                Exit Select
                ' The activation status of the CAN option "Listen Only" will be retrieved
                '
            Case 3
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_LISTEN_ONLY, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The CAN option ""Listen Only"" is {0:X}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "ON", "OFF")))
                End If
                Exit Select
                ' The activation status for the feature for logging debug-information will be retrieved
            Case 4
                stsResult = PCANBasic.GetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_LOG_STATUS, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The feature for logging debug information is {0:X}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "ON", "OFF")))
                End If
                Exit Select
                ' The activation status of the channel option "Receive Status"  will be retrieved
                '
            Case 5
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_RECEIVE_STATUS, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The channel option ""Receive Status"" is {0}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "ON", "OFF")))
                End If
                Exit Select
                ' The Number of the CAN-Controller used by a PCAN-Channel
                '
            Case 6
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_CONTROLLER_NUMBER, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The CAN Controller number is {0}", iBuffer))
                End If
                Exit Select
                ' The activation status for the feature for tracing data will be retrieved
                '
            Case 7
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_TRACE_STATUS, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The feature for tracing data is {0}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "ON", "OFF")))
                End If
                Exit Select
                ' The activation status of the Channel Identifying procedure will be retrieved
                '
            Case 8
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_CHANNEL_IDENTIFYING, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The identification procedure of the selected channel is {0}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "ON", "OFF")))
                End If
                Exit Select
            Case 9
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_CHANNEL_FEATURES, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The channel {0} Flexible Data-Rate (CAN-FD)", If(((iBuffer And PCANBasic.FEATURE_FD_CAPABLE) = PCANBasic.FEATURE_FD_CAPABLE), "does support", "DOESN'T SUPPORT")))
                    IncludeTextMessage(String.Format("The channel {0} Extra Long (CAN-XL)", If(((iBuffer And PCANBasic.FEATURE_XL_CAPABLE) = PCANBasic.FEATURE_XL_CAPABLE), "does support", "DOESN'T SUPPORT")))
                    IncludeTextMessage(String.Format("The channel {0} an inter-frame delay for sending messages", If(((iBuffer And PCANBasic.FEATURE_DELAY_CAPABLE) = PCANBasic.FEATURE_DELAY_CAPABLE), "does support", "DOESN'T SUPPORT")))
                    IncludeTextMessage(String.Format("The channel {0} using I/O pins", If(((iBuffer And PCANBasic.FEATURE_IO_CAPABLE) = PCANBasic.FEATURE_IO_CAPABLE), "does allow", "DOESN'T ALLOW")))
                End If
                Exit Select
                ' The status of the speed adapting feature will be retrieved
                '
            Case 10
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BITRATE_ADAPTING, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The feature for bit rate adaptation is {0}", If(((iBuffer And PCANBasic.FEATURE_FD_CAPABLE) = PCANBasic.FEATURE_FD_CAPABLE), "ON", "OFF")))
                End If
                Exit Select
                ' The bitrate of the connected Nominal channel will be retrieved (String value)
                '
            Case 11
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BITRATE_INFO_CC, strBuffer, 255)
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage("The bit rate Nominal of the channel is represented by the following values:")
                    For Each strPart As String In strBuffer.ToString().Split(",")
                        IncludeTextMessage("   * " + strPart)
                    Next
                End If
                Exit Select
                ' The bitrate of the connected FD channel will be retrieved (String value)
                '
            Case 12
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BITRATE_INFO_FD, strBuffer, 255)
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage("The bit rate FD of the channel is represented by the following values:")
                    For Each strPart As String In strBuffer.ToString().Split(",")
                        IncludeTextMessage("   * " + strPart)
                    Next
                End If
                Exit Select
                ' The bitrate of the connected XL channel will be retrieved (String value)
                '
            Case 13
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BITRATE_INFO_XL, strBuffer, 255)
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage("The bit rate XL of the channel is represented by the following values:")
                    For Each strPart As String In strBuffer.ToString().Split(",")
                        IncludeTextMessage("   * " + strPart)
                    Next
                End If
                Exit Select
                ' The nominal speed configured on the CAN bus
                '
            Case 14
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BUSSPEED_NOMINAL, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The nominal speed of the channel is {0} bit/s", iBuffer))
                End If
                Exit Select
                ' The data FD configured on the CAN bus
                '
            Case 15
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BUSSPEED_FD, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The FD speed of the channel is {0} bit/s", iBuffer))
                End If
                Exit Select
                ' The data XL configured on the CAN bus
                '
            Case 16
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BUSSPEED_XL, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The XL speed of the channel is {0} bit/s", iBuffer))
                End If
                Exit Select
                ' The IP address of a LAN channel as string, in IPv4 format
                '
            Case 17
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_IP_ADDRESS, strBuffer, 255)
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The IP address of the channel is {0}", strBuffer.ToString()))
                End If
                Exit Select
                ' The running status of the LAN service
                '
            Case 18
                stsResult = PCANBasic.GetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_LAN_SERVICE_STATUS, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The LAN service is {0}", If((iBuffer = PCANBasic.SERVICE_STATUS_RUNNING), "running", "NOT running")))
                End If
                Exit Select
                ' The reception of Status frames
                '
            Case 19
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_STATUS_FRAMES, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The reception of Status frames is {0}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "enabled", "disabled")))
                End If
                Exit Select
                ' The reception of RTR frames
                '
            Case 20
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_RTR_FRAMES, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The reception of RTR frames is {0}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "enabled", "disabled")))
                End If
                Exit Select
                ' The reception of Error frames
                '
            Case 21
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_ERROR_FRAMES, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The reception of Error frames is {0}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "enabled", "disabled")))
                End If
                Exit Select
                ' The Interframe delay of an USB channel will be retrieved
                '
            Case 22
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_INTERFRAME_DELAY, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The configured interframe delay is {0} μs", iBuffer))
                End If
                Exit Select
                ' The reception of Error frames
                '
            Case 23
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_ECHO_FRAMES, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The reception of Echo frames is {0}", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "enabled", "disabled")))
                End If
                Exit Select
                ' The activation of Hard Reset
                '
            Case 24
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_HARD_RESET_STATUS, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The method PCANBasic.Reset is {0} a hardware reset", If((iBuffer = PCANBasic.PCAN_PARAMETER_ON), "performing", "NOT performing")))
                End If
                Exit Select

                ' The direction of the communication with a LAN channel
                '
            Case 25
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_LAN_CHANNEL_DIRECTION, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    Select Case iBuffer
                        Case PCANBasic.LAN_DIRECTION_READ
                            IncludeTextMessage("The communication flow is: incoming only")
                            Exit Select
                        Case PCANBasic.LAN_DIRECTION_WRITE
                            IncludeTextMessage("The communication flow is: outgoing only")
                            Exit Select
                        Case PCANBasic.LAN_DIRECTION_READ_WRITE
                            IncludeTextMessage("The communication flow is: bidirectional")
                            Exit Select
                        Case Else
                            IncludeTextMessage(String.Format("The communication flow is: undefined (0x{0:X4})", iBuffer))
                            Exit Select
                    End Select
                End If
                Exit Select

                ' The GUID of the device
                ' 
            Case 26
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_DEVICE_GUID, strBuffer, 255)
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    IncludeTextMessage(String.Format("The GUID of the device is {0}", strBuffer.ToString()))
                    Exit Select
                End If

            Case Else
                ' The current parameter is invalid
                '
                stsResult = TPCANStatus.PCAN_ERROR_UNKNOWN
                MessageBox.Show("Wrong parameter code.")
                Return
        End Select

        ' If the function fail, an error message is shown
        '
        If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
            MessageBox.Show(GetFormattedError(stsResult))
        End If
    End Sub

    Private Sub rdbTimer_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles rdbTimer.CheckedChanged, rdbManual.CheckedChanged, rdbEvent.CheckedChanged
        If Not btnRelease.Enabled Then
            Return
        End If

        ' According with the kind of reading, a timer, a thread or a button will be enabled
        '
        If rdbTimer.Checked Then
            ' Stops Read Thread if it exists
            '
            If m_ReadThread IsNot Nothing Then
                m_readThreadRunning = False
                m_ReadThread.Join()
                m_ReadThread = Nothing
            End If

            ' Enable Timer
            '
            tmrRead.Enabled = btnRelease.Enabled
        End If
        If rdbEvent.Checked Then
            ' Disable Timer
            '
            tmrRead.Enabled = False
            ' Create and start the tread to read CAN Message using SetRcvEvent()
            '
            Dim threadDelegate As New System.Threading.ThreadStart(AddressOf Me.CANReadThreadFunc)
            m_ReadThread = New System.Threading.Thread(threadDelegate)
            m_ReadThread.IsBackground = True
            m_readThreadRunning = True
            m_ReadThread.Start()
        End If
        If rdbManual.Checked Then
            ' Stops Read Thread if it exists
            '
            If m_ReadThread IsNot Nothing Then
                m_readThreadRunning = False
                m_ReadThread.Join()
                m_ReadThread = Nothing
            End If
            ' Disable Timer
            '
            tmrRead.Enabled = False
        End If
        btnRead.Enabled = btnRelease.Enabled AndAlso rdbManual.Checked
    End Sub

    Private Sub nudDLC_ValueChanged(sender As Object, e As EventArgs) Handles nudDLC.ValueChanged
        FillDataSets()

        laLength.Text = String.Format("Length: {0} B.", nudDLC.Value + 1)
    End Sub

    Private Sub chbShowPeriod_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles chbShowPeriod.CheckedChanged
        ' According with the check-value of this checkbox,
        ' the received time of a messages will be interpreted as 
        ' period (time between the two last messages) or as time-stamp
        ' (the elapsed time since windows was started)
        '
        SyncLock m_LastMsgsList.SyncRoot
            For Each msg As MessageStatus In m_LastMsgsList
                msg.ShowingPeriod = chbShowPeriod.Checked
            Next
        End SyncLock
    End Sub

    Private Sub btnRead_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnRead.Click
        Dim stsResult As TPCANStatus

        ' We execute the "Read" function of the PCANBasic                
        '
        stsResult = ReadMessageXL()
        If Not stsResult = TPCANStatus.PCAN_ERROR_OK Then
            ' If an error occurred, an information message is included
            '
            IncludeTextMessage(GetFormattedError(stsResult))
        End If
    End Sub

    Private Sub btnMsgClear_Click(ByVal sender As Object, ByVal e As EventArgs) Handles btnMsgClear.Click
        ' The information contained in the messages List-View
        ' is cleared
        '
        SyncLock m_LastMsgsList.SyncRoot
            lstMessages.Items.Clear()
            m_LastMsgsList.Clear()
        End SyncLock
    End Sub

    Private Sub txtPID_KeyPress(ByVal sender As Object, ByVal e As KeyPressEventArgs) Handles txtPID.KeyPress, txtAF.KeyPress, txtSDT.KeyPress, txtVCID.KeyPress
        Dim chCheck As Short

        ' We convert the Character to its Upper case equivalent
        '
        chCheck = Asc(e.KeyChar.ToString.ToUpper)

        ' The Key is the Delete (Backspace) Key
        '
        If chCheck = 8 Then
            Return
        End If
        ' The Key is a number between 0-9
        '
        If chCheck > 47 AndAlso chCheck < 58 Then
            Return
        End If
        ' The Key is a character between A-F
        '
        If chCheck > 64 AndAlso chCheck < 71 Then
            Return
        End If

        ' Is neither a number nor a character between A(a) and F(f)
        '
        e.Handled = True

    End Sub

    Private Sub txtPID_Leave(ByVal sender As Object, ByVal e As EventArgs) Handles txtPID.Leave
        Dim iTextLength = 3
        Dim uiMaxValue As UInteger = &H7FF


        ' The Textbox for the PID Is represented with 3 characters.
        ' Therefore if the Length of the text Is smaller than TextLength,  
        ' we add "0"
        '
        While Not txtPID.Text.Length = iTextLength
            txtPID.Text = ("0" & txtPID.Text)
        End While

        ' We check that the PID is not bigger than current maximum value
        '
        If Convert.ToUInt32(txtPID.Text, 16) > uiMaxValue Then
            txtPID.Text = String.Format("{0:X" & iTextLength.ToString & "}", uiMaxValue)
        End If
    End Sub

    Private Sub btnEdit_Click(sender As Object, e As EventArgs) Handles btnEdit.Click
        Dim editDataWindow As New EditDataForm(m_data, nudDLC.Value)
        Dim result As DialogResult = editDataWindow.ShowDialog()

        If result = DialogResult.OK Then
            m_data = editDataWindow.Result
            InitializeData()
        End If

    End Sub

    Private Sub btnGenerate_Click(sender As Object, e As EventArgs) Handles btnGenerate.Click
        RandomizeData()
        InitializeData()
    End Sub

    Private Sub btnWrite_Click(ByVal sender As Object, ByVal e As EventArgs) Handles btnWrite.Click
        Dim stsResult As TPCANStatus

        ' Send the message
        '
        stsResult = WriteFrameXL()

        ' The message was successfully sent
        '
        If stsResult = TPCANStatus.PCAN_ERROR_OK Then
            IncludeTextMessage("Message was successfully SENT")
        Else
            ' An error occurred.  We show the error.
            '
            MessageBox.Show(GetFormattedError(stsResult))
        End If
    End Sub

    Private Sub btnGetVersions_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnGetVersions.Click
        Dim stsResult As TPCANStatus
        Dim strTemp As StringBuilder
        Dim strArrayVersion() As String

        strTemp = New StringBuilder(256)

        ' We get the version of the PCAN-Basic API
        '
        stsResult = PCANBasic.GetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_API_VERSION, strTemp, 256)
        If stsResult = TPCANStatus.PCAN_ERROR_OK Then
            IncludeTextMessage("API Version: " & strTemp.ToString())

            ' We get the version of the firmware on the device
            '
            stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_FIRMWARE_VERSION, strTemp, 256)
            If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                IncludeTextMessage("Firmware Version: " + strTemp.ToString())
            End If

            ' We get the driver version of the channel being used
            '
            stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_CHANNEL_VERSION, strTemp, 256)
            If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                ' Because this information contains line control characters (several lines)
                ' we split this also in several entries in the Information List-Box
                '
                strArrayVersion = strTemp.ToString().Split(New Char() {ControlChars.Lf})
                IncludeTextMessage("Channel/Driver Version: ")
                For i As Integer = 0 To strArrayVersion.Length - 1
                    IncludeTextMessage("     * " & strArrayVersion(i))
                Next
            End If
        End If

        ' If an error occurred, a message is shown
        '
        If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
            MessageBox.Show(GetFormattedError(stsResult))
        End If
    End Sub

    Private Sub btnInfoClear_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnInfoClear.Click
        ' The information contained in the Information List-Box 
        ' is cleared
        lbxInfo.Items.Clear()
    End Sub

    Private Sub Form_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        ' Releases the used PCAN-Basic channel
        '
        If (btnRelease.Enabled) Then
            btnRelease_Click(Me, New EventArgs())
        End If
    End Sub

    Private Sub tmrRead_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles tmrRead.Tick
        '' Checks if in the receive-queue are currently messages for read
        ReadMessages()
    End Sub

    Private Sub tmrDisplay_Tick(ByVal sender As Object, ByVal e As EventArgs) Handles tmrDisplay.Tick
        DisplayMessages()
    End Sub

    Private Sub Form_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        nudDLC.Value = 8
        chbShowPeriod.Checked = True
        rdbParamActive.Checked = True
        rdbTimer.Checked = True
        rdbFilterOpen.Checked = True
    End Sub

    Private Sub lstMessages_DoubleClick(ByVal sender As Object, ByVal e As EventArgs) Handles lstMessages.DoubleClick
        ' Clears the content of the Message List-View
        '
        btnMsgClear_Click(Me, New EventArgs)
    End Sub

    Private Sub lbxInfo_DoubleClick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles lbxInfo.DoubleClick
        ' Clears the content of the Information List-Box
        '
        btnInfoClear_Click(Me, New EventArgs())
    End Sub

    Private Sub txtAF_Leave(sender As Object, e As EventArgs) Handles txtAF.Leave

        Dim iTextLength As Integer = 8

        ' The Textbox for the AF field Is represented with 8 characters
        ' (eight bytes in length). Therefore if the Length of the text Is smaller than TextLength,  
        ' we add "0"
        '
        While Not txtAF.Text.Length = iTextLength
            txtAF.Text = ("0" + txtAF.Text)
        End While
    End Sub

    Private Sub txtSDT_Leave(sender As Object, e As EventArgs) Handles txtSDT.Leave
        Dim iTextLength As Int32 = 2

        ' The Textbox for the SDT is represented with 2 characters
        ' (one byte in length). Therefore if the Length of the text Is smaller than TextLength,  
        ' we add "0"
        '
        While Not txtSDT.Text.Length = iTextLength
            txtSDT.Text = ("0" + txtSDT.Text)
        End While
    End Sub

    Private Sub txtVCID_Leave(sender As Object, e As EventArgs) Handles txtSDT.Leave, txtVCID.Leave
        Dim iTextLength As Int32 = 2

        ' The Textbox for the VCID is represented with 2 characters
        ' (one byte in length). Therefore if the Length of the text Is smaller than TextLength,  
        ' we add "0"
        '
        While Not txtVCID.Text.Length = iTextLength
            txtVCID.Text = ("0" + txtVCID.Text)
        End While
    End Sub

    Private Sub btnReset_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnReset.Click
        Dim stsResult As TPCANStatus

        ' Resets the receive and transmit queues of a PCAN Channel.
        '
        stsResult = PCANBasic.Reset(m_PcanHandle)

        ' If it fails, a error message is shown
        '
        If (stsResult <> TPCANStatus.PCAN_ERROR_OK) Then
            MessageBox.Show(GetFormattedError(stsResult))
        Else
            IncludeTextMessage("Receive and transmit queues successfully reset")
        End If

    End Sub

    Private Sub btnStatus_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnStatus.Click
        Dim stsResult As TPCANStatus
        Dim errorName As String

        ' Gets the current BUS status of a PCAN Channel.
        '
        stsResult = PCANBasic.GetStatus(m_PcanHandle)

        ' Switch On Error Name
        '
        Select Case stsResult
            Case TPCANStatus.PCAN_ERROR_INITIALIZE
                errorName = "PCAN_ERROR_INITIALIZE"
                Exit Select

            Case TPCANStatus.PCAN_ERROR_BUSLIGHT
                errorName = "PCAN_ERROR_BUSLIGHT"
                Exit Select

            Case TPCANStatus.PCAN_ERROR_BUSWARNING
                errorName = "PCAN_ERROR_BUSWARNING"
                Exit Select

            Case TPCANStatus.PCAN_ERROR_BUSPASSIVE
                errorName = "PCAN_ERROR_BUSPASSIVE"
                Exit Select

            Case TPCANStatus.PCAN_ERROR_BUSOFF
                errorName = "PCAN_ERROR_BUSOFF"
                Exit Select

            Case TPCANStatus.PCAN_ERROR_OK
                errorName = "PCAN_ERROR_OK"
                Exit Select
            Case Else
                errorName = "See Documentation"
                Exit Select
        End Select

        ' Display Message
        '
        IncludeTextMessage(String.Format("Status: {0} ({1:X}h)", errorName, stsResult))
    End Sub
#End Region

#Region "Help functions"
    ''' <summary>
    ''' Converts a CAN DLC value into the actual data length of the CAN-XL frame.
    ''' </summary>
    ''' <param name="dlc">A value between 0 and 2047 (CAN XL DLC range)</param>
    ''' <param name="msgType">The message type of the message</param>
    ''' <returns>The length represented by the DLC</returns>
    Public Shared Function GetLengthFromDLC(ByVal dlc As Integer, ByVal msgType As TPCANMessageTypeXL)
        If (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_STATUS) = TPCANMessageTypeXL.PCAN_MESSAGE_STATUS Or
           (msgType And TPCANMessageTypeXL.PCAN_MESSAGE_XL) <> TPCANMessageTypeXL.PCAN_MESSAGE_XL Then

            If dlc <= 8 Then
                Return dlc
            End If

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
        End If

        If dlc < 0 Then
            Return 1
        End If

        If dlc > 2047 Then
            Return 2048
        End If

        Return dlc + 1
    End Function

    Public Sub InitializeData()
        Dim strData = String.Empty

        For index = 0 To nudDLC.Value
            strData &= String.Format("{0:X2} ", m_data(index))
        Next

        txtData.Text = strData
    End Sub

    ''' <summary>
    ''' Initialization of PCAN-Basic components
    ''' </summary>
    Private Sub InitializeBasicComponents()
        ' Creates the list for received messages
        '
        m_LastMsgsList = New System.Collections.ArrayList()
        ' Creates the delegate used for message reading
        '
        m_ReadDelegate = New ReadDelegateHandler(AddressOf ReadMessages)

        ' Creates the event used for signalize incoming messages 
        '
        m_ReceiveEvent = New System.Threading.AutoResetEvent(False)

        ' Create a buffer of 2048 bytes for storing random CAN data for message writing
        '
        m_data = New Byte(2047) {}

        ' Show the CAN data to send
        '
        FillDataSets()

        ' Fills and configures the Data of several comboBox components
        '
        FillComboBoxData()

        ' Prepares the PCAN-Basic's debug-Log file
        '
        ConfigureLogFile()
    End Sub

    ''' <summary>
    ''' Generates random data
    ''' </summary>
    Private Sub RandomizeData()
        Dim rand As Random = New Random()
        Dim data As Byte() = New Byte(CType(nudDLC.Value, Integer)) {}
        rand.NextBytes(data)
        Array.Copy(data, m_data, data.Length)
    End Sub

    '''<summary>
    ''' Resizes the whole <see cref="m_data"/> array to the size dictated by <see cref="nudDLC"/>
    ''' </summary>
    Private Sub ResizeData()
        Array.Clear(m_data, Int(nudDLC.Value + 1), m_data.Length - Int(nudDLC.Value + 1))
    End Sub

    ''' <summary>
    ''' Sets And shows the CAN data to send
    ''' </summary>
    Private Sub FillDataSets()
        ResizeData()
        InitializeData()
    End Sub

    ''' <summary>
    ''' Configures the Debug-Log file of PCAN-Basic
    ''' </summary>
    Private Sub ConfigureLogFile()
        Dim iBuffer As UInt32

        ' Sets the mask to catch all events
        '
        iBuffer = PCANBasic.LOG_FUNCTION_ALL

        ' Configures the log file. 
        ' NOTE: The Log capability is to be used with the NONEBUS Handle. Other handle than this will 
        ' cause the function fail.
        '
        PCANBasic.SetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_LOG_CONFIGURE, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
    End Sub

    ''' <summary>
    ''' Configures the Debug-Log file of PCAN-Basic
    ''' </summary>
    Private Sub ConfigureTraceFile()
        Dim iBuffer As UInt32
        Dim stsResult As TPCANStatus

        ' Configure the maximum size of a trace file to 5 megabytes
        '
        iBuffer = 5
        stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_TRACE_SIZE, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
        If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
            IncludeTextMessage(GetFormattedError(stsResult))
        End If

        ' Configure the way how trace files are created: 
        ' * Standard name is used
        ' * Existing file is overwritten,
        ' * Only one file is created.
        ' * Recording stops when the file size reaches 5 megabytes.
        '
        iBuffer = PCANBasic.TRACE_FILE_SINGLE Or PCANBasic.TRACE_FILE_OVERWRITE
        stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_TRACE_CONFIGURE, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))
        If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
            IncludeTextMessage(GetFormattedError(stsResult))
        End If
    End Sub

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
        If PCANBasic.GetErrorText([error], 0, strTemp) <> TPCANStatus.PCAN_ERROR_OK Then
            Return String.Format("An error occurred. Error-code's text ({0:X}) couldn't be retrieved", [error])
        Else
            Return strTemp.ToString()
        End If
    End Function

    ''' <summary>
    ''' Includes a new line of text into the information Listview
    ''' </summary>
    ''' <param name="strMsg">Text to be included</param>
    Private Sub IncludeTextMessage(ByVal strMsg As String)
        lbxInfo.Items.Add(strMsg)
        lbxInfo.SelectedIndex = lbxInfo.Items.Count - 1
    End Sub

    ''' <summary>
    ''' Gets the current status of the PCAN-Basic message filter
    ''' </summary>
    ''' <param name="status">Buffer to retrieve the filter status</param>
    ''' <returns>If calling the function was successful or not</returns>
    Private Function GetFilterStatus(ByRef status As UInteger) As Boolean
        Dim stsResult As TPCANStatus

        ' Tries to get the status of the filter for the current connected hardware
        '
        stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_MESSAGE_FILTER, status, CType(System.Runtime.InteropServices.Marshal.SizeOf(status), UInteger))

        ' If it fails, a error message is shown
        '
        If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
            MessageBox.Show(GetFormattedError(stsResult))
            Return False
        End If
        Return True
    End Function

    ''' <summary>
    ''' Configures the data of all ComboBox components of the main-form
    ''' </summary>
    Private Sub FillComboBoxData()
        ' Channels will be check
        '
        DoRefresh()

        'XL Bitrate 
        '   Arbitration 500 kbit/s
        '   FD: 2 Mbit/s (FD data)
        '   XL: 8 Mbit/s (XL data)

        txtBitrate.Text = "f_clock_mhz=160,brp=1,nom_tseg1=255,nom_tseg2=64,nom_sjw=64,fd_tseg1=63,fd_tseg2=16,fd_sjw=16,fd_ssp_offset=0,xl_tseg1=10,xl_tseg2=9,xl_sjw=9,xl_ssp_offset=10,xl_error_signaling=1,xl_transceiver_mode_switch=0"

        ' Parameters for GetValue and SetValue function calls
        '
        cbbParameter.SelectedIndex = 0
    End Sub

    ''' <summary>
    ''' Activates/deactivates the different controls of the main-form according
    ''' with the current connection status
    ''' </summary>
    ''' <param name="bConnected">Current status. True if connected, false otherwise</param>
    Private Sub SetConnectionStatus(ByVal bConnected As Boolean)
        ' Buttons
        '
        btnInit.Enabled = Not bConnected
        btnRead.Enabled = bConnected AndAlso rdbManual.Checked
        btnWrite.Enabled = bConnected
        btnRelease.Enabled = bConnected
        btnFilterApply.Enabled = bConnected
        btnFilterQuery.Enabled = bConnected
        btnGetVersions.Enabled = bConnected
        btnStatus.Enabled = bConnected
        btnReset.Enabled = bConnected

        ' ComboBoxes
        '
        cbbChannel.Enabled = Not bConnected

        ' Hardware configuration and read mode
        '
        If Not bConnected Then
            cbbChannel_SelectedIndexChanged(Me, New EventArgs())
        Else
            rdbTimer_CheckedChanged(Me, New EventArgs())
        End If

        ' Display messages in grid
        '
        tmrDisplay.Enabled = bConnected
    End Sub

    ''' <summary>
    ''' Gets the formatted text for a PCAN-Basic channel handle
    ''' </summary>
    ''' <param name="handle">PCAN-Basic Handle to format</param>
    ''' <param name="protocol">The protocol of the channel</param>
    ''' <returns>The formatted text for a channel</returns>
    Private Function FormatChannelName(ByVal handle As TPCANHandle, ByVal protocol As CANProtocol) As String
        Dim devDevice As TPCANDevice
        Dim byChannel As Byte

        ' Gets the owner device and channel for a 
        ' PCAN-Basic handle
        '
        If CType(handle, UShort) < &H100 Then
            devDevice = DirectCast(CType(handle >> 4, Byte), TPCANDevice)
            byChannel = CByte((handle And &HF))
        Else
            devDevice = DirectCast(CType(handle >> 8, Byte), TPCANDevice)
            byChannel = CByte((handle And &HFF))
        End If

        ' Constructs the PCAN-Basic Channel name and return it
        '
        Dim StringFormat = "{0}:{1} {2} ({3:X2}h)"
        Dim bIsXL = (protocol And CANProtocol.CAN_XL) = CANProtocol.CAN_XL
        Dim bIsFD = (protocol And CANProtocol.CAN_FD) = CANProtocol.CAN_FD

        If (bIsXL And bIsFD) Then
            Return String.Format(StringFormat, devDevice, "FD/XL", byChannel, handle)
        ElseIf (bIsXL) Then
            Return String.Format(StringFormat, devDevice, "XL", byChannel, handle)
        ElseIf (bIsFD) Then
            Return String.Format(StringFormat, devDevice, "FD", byChannel, handle)
        Else
            Return String.Format(StringFormat, devDevice, String.Empty, byChannel, handle)
        End If
    End Function

    Private Sub DoRefresh()
        Dim stsResult As TPCANStatus
        Dim iChannelsCount As UInteger

        ' Clears the Channel comboBox and fill it again with 
        ' the detected hardware
        '
        cbbChannel.Items.Clear()

        Try
            ' Checks for available channels
            '
            stsResult = PCANBasic.GetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_ATTACHED_CHANNELS_COUNT, iChannelsCount, CType(Runtime.InteropServices.Marshal.SizeOf(iChannelsCount), UInteger))
            If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                Dim info(iChannelsCount - 1) As TPCANChannelInformation

                stsResult = PCANBasic.GetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_ATTACHED_CHANNELS, info)
                If stsResult = TPCANStatus.PCAN_ERROR_OK Then
                    ' Include only connectable channels
                    '
                    For Each channel In info

                        Dim protocol As CANProtocol = CANProtocol.CAN
                        If ((channel.device_features And PCANBasic.FEATURE_XL_CAPABLE) = PCANBasic.FEATURE_XL_CAPABLE) Then
                            protocol = protocol Or CANProtocol.CAN_XL
                        End If
                        If ((channel.device_features And PCANBasic.FEATURE_FD_CAPABLE) = PCANBasic.FEATURE_FD_CAPABLE) Then
                            protocol = protocol Or CANProtocol.CAN_FD
                        End If

                        If ((channel.channel_condition And PCANBasic.PCAN_CHANNEL_AVAILABLE) = PCANBasic.PCAN_CHANNEL_AVAILABLE And
                            (protocol And CANProtocol.CAN_XL) = CANProtocol.CAN_XL) Then

                            cbbChannel.Items.Add(FormatChannelName(channel.channel_handle, protocol))
                        End If

                    Next
                End If
            End If

            cbbChannel.Items.Add("... refresh ...")
            If (cbbChannel.Items.Count > 1) Then
                cbbChannel.SelectedIndex = 0
            End If

            btnInit.Enabled = cbbChannel.Items.Count > 0

            If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
                MessageBox.Show(GetFormattedError(stsResult))
            End If

        Catch ex As DllNotFoundException
            MessageBox.Show("Unable to find the library: PCANBasic.dll !", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error)
            Environment.Exit(-1)
        End Try

    End Sub
#End Region

#Region "Message-proccessing functions"
    ''' <summary>
    ''' Display CAN messages in the Message-ListView
    ''' </summary>
    Private Sub DisplayMessages()
        Dim lviCurrentItem As ListViewItem

        SyncLock m_LastMsgsList.SyncRoot
            For Each msgStatus As MessageStatus In m_LastMsgsList
                ' Get the data to update
                If msgStatus.MarkedAsUpdated Then
                    msgStatus.MarkedAsUpdated = False
                    lviCurrentItem = lstMessages.Items(msgStatus.Position)

                    lviCurrentItem.SubItems(2).Text = GetLengthFromDLC(msgStatus.CANMsg.DLC, msgStatus.CANMsg.MSGTYPE).ToString()
                    lviCurrentItem.SubItems(3).Text = msgStatus.SDTString
                    lviCurrentItem.SubItems(4).Text = msgStatus.VCIDString
                    lviCurrentItem.SubItems(5).Text = msgStatus.AFString
                    lviCurrentItem.SubItems(6).Text = msgStatus.TimeString
                    lviCurrentItem.SubItems(7).Text = msgStatus.Count.ToString()
                    lviCurrentItem.SubItems(8).Text = msgStatus.DataString
                End If
            Next
        End SyncLock
    End Sub

    ''' <summary>
    ''' Inserts a new entry for a new message in the Message-ListView
    ''' </summary>
    ''' <param name="newMsg">The message to be inserted</param>
    ''' <param name="timeStamp">The Timestamp of the new message</param>
    Private Sub InsertMsgEntry(ByVal newMsg As TPCANMsgXL, ByVal timeStamp As TPCANTimestampXL)
        Dim msgStsCurrentMsg As MessageStatus
        Dim lviCurrentItem As ListViewItem

        SyncLock m_LastMsgsList.SyncRoot
            ' We add this status in the last message list
            '
            msgStsCurrentMsg = New MessageStatus(newMsg, timeStamp, lstMessages.Items.Count)
            msgStsCurrentMsg.ShowingPeriod = chbShowPeriod.Checked
            m_LastMsgsList.Add(msgStsCurrentMsg)

            ' Add the new ListView Item with the Type of the message
            '
            lviCurrentItem = lstMessages.Items.Add(msgStsCurrentMsg.TypeString)

            ' Add the new ListView Item with the PID of the message
            '
            lviCurrentItem.SubItems.Add(msgStsCurrentMsg.PIDString)
            ' We set the length of the Message
            '
            lviCurrentItem.SubItems.Add(GetLengthFromDLC(newMsg.DLC, newMsg.MSGTYPE).ToString())
            ' We set the SDT information of the message
            '
            lviCurrentItem.SubItems.Add(msgStsCurrentMsg.SDTString)
            ' We set the VCID information of the message
            '
            lviCurrentItem.SubItems.Add(msgStsCurrentMsg.VCIDString)
            ' We set the AF information of the message
            '
            lviCurrentItem.SubItems.Add(msgStsCurrentMsg.AFString)
            ' Add time stamp information if needed
            '
            lviCurrentItem.SubItems.Add(msgStsCurrentMsg.TimeString)
            ' We set the message count message (this is the first, so count is 1)            
            '
            lviCurrentItem.SubItems.Add(msgStsCurrentMsg.Count.ToString())
            ' We set the data of the message. 	
            '
            lviCurrentItem.SubItems.Add(msgStsCurrentMsg.DataString)
        End SyncLock

    End Sub

    ''' <summary>
    ''' Processes a received message, in order to show it in the Message-ListView
    ''' </summary>
    ''' <param name="theMsg">The received PCAN-Basic message</param>
    ''' <param name="itsTimeStamp">The Timestamp of the received message</param>
    Private Sub ProcessMessage(ByVal theMsg As TPCANMsgXL, ByVal itsTimeStamp As TPCANTimestampXL)
        ' We search if a message is 
        ' already received or if this is a new message
        '
        SyncLock m_LastMsgsList.SyncRoot
            For Each msg As MessageStatus In m_LastMsgsList

                If ((msg.CANMsg.PID = theMsg.PID) And (msg.CANMsg.MSGTYPE = theMsg.MSGTYPE) And
                        (msg.CANMsg.VCID = theMsg.VCID) And (msg.CANMsg.SDT = theMsg.SDT) And
                        (msg.CANMsg.AF = theMsg.AF) And (msg.CANMsg.RRS = theMsg.RRS) And
                        (msg.CANMsg.SEC = theMsg.SEC)) Then
                    ' Messages of this kind are already received; we do an update
                    '
                    msg.Update(theMsg, itsTimeStamp)
                    Exit Sub
                End If
            Next

            ' Message not found. It will created
            '
            InsertMsgEntry(theMsg, itsTimeStamp)
        End SyncLock
    End Sub

    ''' <summary>
    ''' Thread-Function used for reading PCAN-Basic messages
    ''' </summary>
    Private Sub CANReadThreadFunc()
        Dim iBuffer As UInt32
        Dim stsResult As TPCANStatus

        iBuffer = Convert.ToUInt32(m_ReceiveEvent.SafeWaitHandle.DangerousGetHandle().ToInt32())
        ' Sets the handle of the Receive-Event.
        '
        stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_RECEIVE_EVENT, iBuffer, CType(System.Runtime.InteropServices.Marshal.SizeOf(iBuffer), UInteger))

        If stsResult <> TPCANStatus.PCAN_ERROR_OK Then
            MessageBox.Show(GetFormattedError(stsResult), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error)
            Return
        End If

        ' While this mode is selected
        While m_readThreadRunning
            ' Waiting for Receive-Event
            ' 
            If m_ReceiveEvent.WaitOne(50) Then
                ' Process Receive-Event using .NET BeginInvoke function
                ' in order to interact with Winforms UI (calling the 
                ' function ReadMessages)
                ' Warning: Do Not use Invoke when calling Join with the main thread, since this could lead to a deadlock.
                ' 
                Me.BeginInvoke(m_ReadDelegate)
            End If
        End While
    End Sub

    ''' <summary>
    ''' Function for reading messages on XL devices
    ''' </summary>
    ''' <returns>A TPCANStatus error code</returns>
    Private Function ReadMessageXL() As TPCANStatus
        Dim CANMsg As TPCANMsgXL = Nothing
        Dim CANTimeStamp As TPCANTimestampXL
        Dim stsResult As TPCANStatus

        ' We execute the "Read" function of the PCANBasic 
        '
        stsResult = PCANBasic.ReadXL(m_PcanHandle, CANMsg, CANTimeStamp)
        If (stsResult <> TPCANStatus.PCAN_ERROR_QRCVEMPTY And
            stsResult <> TPCANStatus.PCAN_ERROR_ILLDATA) Then
            ' We process the received message
            '
            ProcessMessage(CANMsg, CANTimeStamp)
        End If

        Return stsResult
    End Function

    ''' <summary>
    ''' Function for writing messages on XL devices
    ''' </summary>
    ''' <returns></returns>
    Private Function WriteFrameXL()
        Dim CANMsg As TPCANMsgXL

        ' We create a TPCANMsg message structure 
        '
        CANMsg = New TPCANMsgXL()
        CANMsg.DATA = New Byte(2047) {}

        ' We configurate the message.
        '
        CANMsg.PID = Convert.ToInt32(txtPID.Text, 16)
        CANMsg.DLC = Convert.ToUInt16(nudDLC.Value)
        CANMsg.SDT = Convert.ToByte(txtSDT.Text, 16)
        CANMsg.VCID = Convert.ToByte(txtVCID.Text, 16)
        CANMsg.AF = Convert.ToByte(txtAF.Text, 16)
        CANMsg.RRS = chbRRS.Checked
        CANMsg.SEC = chbSEC.Checked
        CANMsg.MSGTYPE = TPCANMessageTypeXL.PCAN_MESSAGE_XL
        Array.Copy(m_data, CANMsg.DATA, CType(nudDLC.Value + 1, Integer))

        ' The message is sent to the configured hardware
        '
        Return PCANBasic.WriteXL(m_PcanHandle, CANMsg)
    End Function

    ''' <summary>
    ''' Function for reading PCAN-Basic messages
    ''' </summary>
    Private Sub ReadMessages()
        Dim stsResult As TPCANStatus

        ' We read at least one time the queue looking for messages.
        ' If a message is found, we look again trying to find more.
        ' If the queue is empty or an error occur, we get out from
        ' the do-while statement.
        '			
        Do
            ' We execute the "ReadXL" function of the PCANBasic
            '
            stsResult = ReadMessageXL()
            If (stsResult = TPCANStatus.PCAN_ERROR_ILLOPERATION) Then
                Exit Do
            End If
        Loop While btnRelease.Enabled AndAlso (Not Convert.ToBoolean(stsResult And TPCANStatus.PCAN_ERROR_QRCVEMPTY))
    End Sub

#End Region
#End Region
End Class
