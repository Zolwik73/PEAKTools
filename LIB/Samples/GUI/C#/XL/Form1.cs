using PCANBasicExample.Windows;
/// <summary>
/// Inclusion of PEAK PCAN-Basic namespace
/// </summary>
using Peak.Can.Basic;
using System;
using System.Text;
using System.Windows.Forms;
using TPCANHandle = System.UInt16;
using TPCANTimestampXL = System.UInt64;

namespace ICDIBasic
{
    public partial class Form1 : Form
    {
        #region Enums

        [Flags]
        enum CANProtocol
        {
            CAN = 0x1,
            CAN_FD = 0x2,
            CAN_XL = 0x4
        }

        #endregion

        #region Structures
        /// <summary>
        /// Message Status structure used to show CAN Messages
        /// in a ListView
        /// </summary>
        private class MessageStatus
        {
            private TPCANMsgXL m_MsgXL;
            private TPCANTimestampXL m_timeStampXL;
            private TPCANTimestampXL m_oldTimeStampXL;
            private int m_iIndex;
            private int m_Count;
            private bool m_bShowPeriod;
            private bool m_bWasChanged;

            public MessageStatus(TPCANMsgXL canMsg, TPCANTimestampXL canTimestamp, int listIndex)
            {
                m_MsgXL = canMsg;
                m_timeStampXL = canTimestamp;
                m_oldTimeStampXL = canTimestamp;
                m_iIndex = listIndex;
                m_Count = 1;
                m_bShowPeriod = true;
                m_bWasChanged = false;
            }

            public void Update(TPCANMsgXL canMsg, TPCANTimestampXL canTimestamp)
            {
                m_MsgXL = canMsg;
                m_oldTimeStampXL = m_timeStampXL;
                m_timeStampXL = canTimestamp;
                m_bWasChanged = true;
                m_Count += 1;
            }

            public TPCANMsgXL CANMsg
            {
                get { return m_MsgXL; }
            }

            public TPCANTimestampXL Timestamp
            {
                get { return m_timeStampXL; }
            }

            public int Position
            {
                get { return m_iIndex; }
            }

            public string TypeString
            {
                get { return GetMsgTypeString(); }
            }

            public string PIDString
            {
                get { return GetPIDString(); }
            }

            public string SDTString
            {
                get { return GetSDTString(); }
            }

            public string VCIDString
            {
                get { return GetVCIDString(); }
            }

            public string AFString
            {
                get { return GetAFString(); }
            }

            public string DataString
            {
                get { return GetDataString(); }
            }

            public int Count
            {
                get { return m_Count; }
            }

            public bool ShowingPeriod
            {
                get { return m_bShowPeriod; }
                set
                {
                    if (m_bShowPeriod ^ value)
                    {
                        m_bShowPeriod = value;
                        m_bWasChanged = true;
                    }
                }
            }

            public bool MarkedAsUpdated
            {
                get { return m_bWasChanged; }
                set { m_bWasChanged = value; }
            }

            public string TimeString
            {
                get { return GetTimeString(); }
            }

            private string GetTimeString()
            {
                double fTime;

                fTime = (m_timeStampXL / 1000.0);
                if (m_bShowPeriod)
                    fTime -= (m_oldTimeStampXL / 1000.0);
                return fTime.ToString("F1");
            }

            private string GetDataString()
            {
                string strTemp = "";
                int length = Form1.GetLengthFromDLC(m_MsgXL.DLC, m_MsgXL.MSGTYPE);

                if ((m_MsgXL.MSGTYPE & TPCANMessageTypeXL.PCAN_MESSAGE_RTR) == TPCANMessageTypeXL.PCAN_MESSAGE_RTR)
                    return "Remote Request";
                else
                    for (int i = 0; i < length; i++)
                        strTemp += string.Format("{0:X2} ", m_MsgXL.DATA[i]);

                return strTemp;
            }

            private string GetPIDString()
            {
                return string.Format("{0:X3}h", m_MsgXL.PID);
            }

            private string GetSDTString()
            {
                return string.Format("{0:X2}h", m_MsgXL.SDT);
            }

            private string GetVCIDString()
            {
                return string.Format("{0:X2}h", m_MsgXL.VCID);
            }

            private string GetAFString()
            {
                return string.Format("{0:X8}h", m_MsgXL.AF);
            }

            private string GetMsgTypeString()
            {
                string strTemp;
                bool isEcho = (m_MsgXL.MSGTYPE & TPCANMessageTypeXL.PCAN_MESSAGE_ECHO) == TPCANMessageTypeXL.PCAN_MESSAGE_ECHO;

                if ((m_MsgXL.MSGTYPE & TPCANMessageTypeXL.PCAN_MESSAGE_STATUS) == TPCANMessageTypeXL.PCAN_MESSAGE_STATUS)
                    return "STATUS";

                if ((m_MsgXL.MSGTYPE & TPCANMessageTypeXL.PCAN_MESSAGE_ERRFRAME) == TPCANMessageTypeXL.PCAN_MESSAGE_ERRFRAME)
                    return "ERROR";

                if ((m_MsgXL.MSGTYPE & TPCANMessageTypeXL.PCAN_MESSAGE_XL) == TPCANMessageTypeXL.PCAN_MESSAGE_XL)
                    strTemp = "XL";
                else
                    if ((m_MsgXL.MSGTYPE & TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED) == TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED)
                    strTemp = "EXT";
                else
                    strTemp = "STD";

                if ((m_MsgXL.MSGTYPE & TPCANMessageTypeXL.PCAN_MESSAGE_RTR) == TPCANMessageTypeXL.PCAN_MESSAGE_RTR)
                    strTemp += isEcho ? "/RTR [ ECHO ]" : "/RTR";
                else
                {
                    if (isEcho)
                        strTemp += " [ ECHO";
                    else
                        strTemp += " [ ";

                    if ((int)m_MsgXL.MSGTYPE >= (uint)TPCANMessageTypeXL.PCAN_MESSAGE_XL)
                    {
                        if (m_MsgXL.RRS)
                            strTemp += " RRS";
                        if (m_MsgXL.SEC)
                            strTemp += " SEC";
                    }
                    else if ((int)m_MsgXL.MSGTYPE > (int)TPCANMessageTypeXL.PCAN_MESSAGE_EXTENDED)
                    {
                        if ((m_MsgXL.MSGTYPE & TPCANMessageTypeXL.PCAN_MESSAGE_FD) == TPCANMessageTypeXL.PCAN_MESSAGE_FD)
                            strTemp += " FD";
                        if ((m_MsgXL.MSGTYPE & TPCANMessageTypeXL.PCAN_MESSAGE_BRS) == TPCANMessageTypeXL.PCAN_MESSAGE_BRS)
                            strTemp += " BRS";
                        if ((m_MsgXL.MSGTYPE & TPCANMessageTypeXL.PCAN_MESSAGE_ESI) == TPCANMessageTypeXL.PCAN_MESSAGE_ESI)
                            strTemp += " ESI";
                    }

                    strTemp += " ]";
                }

                return strTemp;
            }
        }
        #endregion

        #region Delegates
        /// <summary>
        /// Read-Delegate Handler
        /// </summary>
        private delegate void ReadDelegateHandler();
        #endregion

        #region Members

        /// <summary>
        /// Saves the handle of a PCAN hardware
        /// </summary>
        private TPCANHandle m_PcanHandle;

        /// <summary>
        /// Stores the status of received messages for its display
        /// </summary>
        private System.Collections.ArrayList m_LastMsgsList;

        /// <summary>
        /// Read Delegate for calling the function "ReadMessages"
        /// </summary>
        private ReadDelegateHandler m_ReadDelegate;

        /// <summary>
        /// Receive-Event
        /// </summary>
        private System.Threading.AutoResetEvent m_ReceiveEvent;

        /// <summary>
        /// Thread for message reading (using events)
        /// </summary>
        private System.Threading.Thread m_ReadThread;

        /// <summary>
        /// Data for message writing. Can be edited in <see cref="EditDataForm"/>
        /// </summary>
        private byte[] m_data;

        /// <summary>
        /// Indicates whether the message-reading thread is running or not
        /// </summary>
        private bool m_ReadThreadRunning;
        #endregion

        #region Methods
        #region Help functions

        /// <summary>
        /// Converts a CAN DLC value into the actual data length of the CAN-XL frame.
        /// </summary>
        /// <param name="dlc">A value between 0 and 2047 (CAN XL DLC range)</param>
        /// <param name="messageTypeXL">The message type of the message</param>
        /// <returns>The length represented by the DLC</returns>        
        public static int GetLengthFromDLC(int dlc, TPCANMessageTypeXL messageTypeXL)
        {
            if ((messageTypeXL & TPCANMessageTypeXL.PCAN_MESSAGE_STATUS) == TPCANMessageTypeXL.PCAN_MESSAGE_STATUS ||
                (messageTypeXL & TPCANMessageTypeXL.PCAN_MESSAGE_XL) != TPCANMessageTypeXL.PCAN_MESSAGE_XL)
            {
                if (dlc <= 8)
                    return dlc;

                switch (dlc)
                {
                    case 9: return 12;
                    case 10: return 16;
                    case 11: return 20;
                    case 12: return 24;
                    case 13: return 32;
                    case 14: return 48;
                    case 15: return 64;
                    default: return dlc;
                }
            }

            if (dlc < 0)
                return 1;

            if (dlc > 2047)
                return 2048;

            return dlc + 1;
        }

        /// <summary>
        /// Updates the control showing the CAN XL data in the UI
        /// </summary>
        public void InitializeData()
        {
            string strData = "";

            for (int i = 0; i < nudDLC.Value + 1; i++)
            {
                strData += string.Format("{0:X2} ", m_data[i]);
            }

            txtData.Text = strData;
        }

        /// <summary>
        /// Generates random data
        /// </summary>
        private void RandomizeData()
        {
            Random rand = new Random();
            var buffer = new byte[(int)nudDLC.Value + 1];
            rand.NextBytes(buffer);
            Array.Copy(buffer, m_data, buffer.Length);
        }

        /// <summary>
        /// Resizes the whole <see cref="m_data"/> array to the size dictated by <see cref="nudDLC"/>
        /// </summary>
        private void ResizeData()
        {
            Array.Clear(m_data, (int)nudDLC.Value + 1, m_data.Length - ((int)nudDLC.Value + 1));
        }

        /// <summary>
        /// Initialization of PCAN-Basic components
        /// </summary>
        private void InitializeBasicComponents()
        {
            // Creates the list for received messages
            //
            m_LastMsgsList = new System.Collections.ArrayList();

            // Creates the delegate used for message reading
            //
            m_ReadDelegate = new ReadDelegateHandler(ReadMessages);
            // Creates the event used for signalize incoming messages 
            //
            m_ReceiveEvent = new System.Threading.AutoResetEvent(false);

            // Create a buffer of 2048 bytes for storing CAN data for message writing
            //
            m_data = new byte[2048];

            // Show the CAN data to send
            //
            FillDataSets();

            // Fills and configures the Data of several comboBox components
            //
            FillComboBoxData();

            // Prepares the PCAN-Basic's debug-Log file
            //
            ConfigureLogFile();
        }

        /// <summary>
        /// Sets and shows the CAN data to send
        /// </summary>
        private void FillDataSets()
        {
            ResizeData();
            InitializeData();
        }

        /// <summary>
        /// Configures the Debug-Log file of PCAN-Basic
        /// </summary>
        private void ConfigureLogFile()
        {
            UInt32 iBuffer;

            // Sets the mask to catch all events
            //
            iBuffer = PCANBasic.LOG_FUNCTION_ALL;

            // Configures the log file. 
            // NOTE: The Log capability is to be used with the NONEBUS Handle. Other handle than this will 
            // cause the function fail.
            //
            PCANBasic.SetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_LOG_CONFIGURE, ref iBuffer, sizeof(UInt32));
        }

        /// <summary>
        /// Configures the PCAN-Trace file for a PCAN-Basic Channel
        /// </summary>
        private void ConfigureTraceFile()
        {
            UInt32 iBuffer;
            TPCANStatus stsResult;

            // Configure the maximum size of a trace file to 5 megabytes
            //
            iBuffer = 5;
            stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_TRACE_SIZE, ref iBuffer, sizeof(UInt32));
            if (stsResult != TPCANStatus.PCAN_ERROR_OK)
                IncludeTextMessage(GetFormattedError(stsResult));

            // Configure the way how trace files are created: 
            // * Standard name is used
            // * Existing file is overwritten, 
            // * Only one file is created.
            // * Recording stops when the file size reaches 5 megabytes.
            //
            iBuffer = PCANBasic.TRACE_FILE_SINGLE | PCANBasic.TRACE_FILE_OVERWRITE;
            stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_TRACE_CONFIGURE, ref iBuffer, sizeof(UInt32));
            if (stsResult != TPCANStatus.PCAN_ERROR_OK)
                IncludeTextMessage(GetFormattedError(stsResult));
        }

        /// <summary>
        /// Help Function used to get an error as text
        /// </summary>
        /// <param name="error">Error code to be translated</param>
        /// <returns>A text with the translated error</returns>
        private string GetFormattedError(TPCANStatus error)
        {
            StringBuilder strTemp;

            // Creates a buffer big enough for a error-text
            //
            strTemp = new StringBuilder(256);
            // Gets the text using the GetErrorText API function
            // If the function success, the translated error is returned. If it fails,
            // a text describing the current error is returned.
            //
            if (PCANBasic.GetErrorText(error, 0, strTemp) != TPCANStatus.PCAN_ERROR_OK)
                return string.Format("An error occurred. Error-code's text (0x{0:X}) couldn't be retrieved", error);
            else
                return strTemp.ToString();
        }

        /// <summary>
        /// Includes a new line of text into the information Listview
        /// </summary>
        /// <param name="strMsg">Text to be included</param>
        private void IncludeTextMessage(string strMsg)
        {
            lbxInfo.Items.Add(strMsg);
            lbxInfo.SelectedIndex = lbxInfo.Items.Count - 1;
        }

        /// <summary>
        /// Gets the current status of the PCAN-Basic message filter
        /// </summary>
        /// <param name="status">Buffer to retrieve the filter status</param>
        /// <returns>If calling the function was successful or not</returns>
        private bool GetFilterStatus(out uint status)
        {
            TPCANStatus stsResult;

            // Tries to get the status of the filter for the current connected hardware
            //
            stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_MESSAGE_FILTER, out status, sizeof(UInt32));

            // If it fails, a error message is shown
            //
            if (stsResult != TPCANStatus.PCAN_ERROR_OK)
            {
                MessageBox.Show(GetFormattedError(stsResult));
                return false;
            }
            return true;
        }

        /// <summary>
        /// Configures the data of all ComboBox components of the main-form
        /// </summary>
        private void FillComboBoxData()
        {
            // Channels will be check
            //
            DoRefresh();

            // XL Bitrate: 
            //      Arbitration: 500 kbit/s 
            //      FD: 2 Mbit/s (FD data)
            //      XL: 8 Mbit/s (XL data)
            //
            txtBitrate.Text = "f_clock_mhz=160,brp=1,nom_tseg1=255,nom_tseg2=64,nom_sjw=64,fd_tseg1=63,fd_tseg2=16,fd_sjw=16,fd_ssp_offset=0,xl_tseg1=10,xl_tseg2=9,xl_sjw=9,xl_ssp_offset=10,xl_error_signaling=1,xl_transceiver_mode_switch=0";

            // Parameters for GetValue and SetValue function calls
            //
            cbbParameter.SelectedIndex = 0;
        }

        /// <summary>
        /// Activates/deactivates the different controls of the main-form according
        /// with the current connection status
        /// </summary>
        /// <param name="bConnected">Current status. True if connected, false otherwise</param>
        private void SetConnectionStatus(bool bConnected)
        {
            // Buttons
            //
            btnInit.Enabled = !bConnected;
            btnRead.Enabled = bConnected && rdbManual.Checked;
            btnWrite.Enabled = bConnected;
            btnRelease.Enabled = bConnected;
            btnFilterApply.Enabled = bConnected;
            btnFilterQuery.Enabled = bConnected;
            btnGetVersions.Enabled = bConnected;
            btnStatus.Enabled = bConnected;
            btnReset.Enabled = bConnected;

            // ComboBoxes
            //
            cbbChannel.Enabled = !bConnected;

            // Hardware configuration and read mode
            //
            if (!bConnected)
                cbbChannel_SelectedIndexChanged(this, new EventArgs());
            else
                rdbTimer_CheckedChanged(this, new EventArgs());

            // Display messages in grid
            //
            tmrDisplay.Enabled = bConnected;
        }

        /// <summary>
        /// Gets the formatted text for a PCAN-Basic channel handle
        /// </summary>
        /// <param name="handle">PCAN-Basic Handle to format</param>
        /// <param name="protocol">The CAN-Protocol supported by the current device</param>
        /// <returns>The formatted text for a channel</returns>
        private string FormatChannelName(TPCANHandle handle, CANProtocol protocol)
        {
            TPCANDevice devDevice;
            byte byChannel;

            // Gets the owner device and channel for a 
            // PCAN-Basic handle
            //
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
            //
            string stringFormat = "{0}:{1} {2} ({3:X2}h)";
            bool bIsXL = (protocol & CANProtocol.CAN_XL) == CANProtocol.CAN_XL;
            bool bIsFD = (protocol & CANProtocol.CAN_FD) == CANProtocol.CAN_FD;

            if (bIsXL && bIsFD)
                return string.Format(stringFormat, devDevice, "FD/XL", byChannel, handle);
            else if (bIsXL)
                return string.Format(stringFormat, devDevice, "XL", byChannel, handle);
            else if (bIsFD)
                return string.Format(stringFormat, devDevice, "FD", byChannel, handle);
            else
                return string.Format(stringFormat, devDevice, string.Empty, byChannel, handle);
        }

        private void DoRefresh()
        {
            TPCANStatus stsResult;
            uint iChannelsCount;

            // Clears the Channel comboBox and fill it again with 
            // the detected hardware
            //
            cbbChannel.Items.Clear();
            try
            {
                // Checks for available channels
                //
                stsResult = PCANBasic.GetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_ATTACHED_CHANNELS_COUNT, out iChannelsCount, sizeof(uint));
                if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                {
                    TPCANChannelInformation[] info = new TPCANChannelInformation[iChannelsCount];

                    stsResult = PCANBasic.GetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_ATTACHED_CHANNELS, info);
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        // Include only connectable channels
                        //
                        foreach (TPCANChannelInformation channel in info)
                        {
                            // Set the correct protocols according to the device features
                            CANProtocol protocol = CANProtocol.CAN;
                            if ((channel.device_features & PCANBasic.FEATURE_XL_CAPABLE) == PCANBasic.FEATURE_XL_CAPABLE)
                                protocol |= CANProtocol.CAN_XL;
                            if ((channel.device_features & PCANBasic.FEATURE_FD_CAPABLE) == PCANBasic.FEATURE_FD_CAPABLE)
                                protocol |= CANProtocol.CAN_FD;

                            // Add channel to the combobox if available and compatible with either CAN XL or CAN FD
                            if ((channel.channel_condition & PCANBasic.PCAN_CHANNEL_AVAILABLE) == PCANBasic.PCAN_CHANNEL_AVAILABLE &&
                                (protocol & CANProtocol.CAN_XL) == CANProtocol.CAN_XL)
                            {
                                cbbChannel.Items.Add(FormatChannelName(channel.channel_handle, protocol));
                            }
                        }
                }

                cbbChannel.Items.Add("... refresh ...");
                if (cbbChannel.Items.Count > 1)
                    cbbChannel.SelectedIndex = 0;
                btnInit.Enabled = cbbChannel.Items.Count > 0;

                if (stsResult != TPCANStatus.PCAN_ERROR_OK)
                    MessageBox.Show(GetFormattedError(stsResult));
            }
            catch (DllNotFoundException)
            {
                MessageBox.Show("Unable to find the library: PCANBasic.dll !", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Environment.Exit(-1);
            }
        }
        #endregion

        #region Message-proccessing functions
        /// <summary>
        /// Display CAN messages in the Message-ListView
        /// </summary>
        private void DisplayMessages()
        {
            ListViewItem lviCurrentItem;

            lock (m_LastMsgsList.SyncRoot)
            {
                foreach (MessageStatus msgStatus in m_LastMsgsList)
                {
                    // Get the data to actualize
                    //
                    if (msgStatus.MarkedAsUpdated)
                    {
                        msgStatus.MarkedAsUpdated = false;
                        lviCurrentItem = lstMessages.Items[msgStatus.Position];

                        lviCurrentItem.SubItems[2].Text = GetLengthFromDLC(msgStatus.CANMsg.DLC, msgStatus.CANMsg.MSGTYPE).ToString();
                        lviCurrentItem.SubItems[3].Text = msgStatus.SDTString;
                        lviCurrentItem.SubItems[4].Text = msgStatus.VCIDString;
                        lviCurrentItem.SubItems[5].Text = msgStatus.AFString;
                        lviCurrentItem.SubItems[6].Text = msgStatus.TimeString;
                        lviCurrentItem.SubItems[7].Text = msgStatus.Count.ToString();
                        lviCurrentItem.SubItems[8].Text = msgStatus.DataString;
                    }
                }
            }
        }

        /// <summary>
        /// Inserts a new entry for a new message in the Message-ListView
        /// </summary>
        /// <param name="newMsg">The message to be inserted</param>
        /// <param name="timeStamp">The Timestamp of the new message</param>
        private void InsertMsgEntry(TPCANMsgXL newMsg, TPCANTimestampXL timeStamp)
        {
            MessageStatus msgStsCurrentMsg;
            ListViewItem lviCurrentItem;

            lock (m_LastMsgsList.SyncRoot)
            {
                // We add this status in the last message list
                //
                msgStsCurrentMsg = new MessageStatus(newMsg, timeStamp, lstMessages.Items.Count);
                msgStsCurrentMsg.ShowingPeriod = chbShowPeriod.Checked;
                m_LastMsgsList.Add(msgStsCurrentMsg);

                // Add the new ListView Item with the Type of the message
                //	
                lviCurrentItem = lstMessages.Items.Add(msgStsCurrentMsg.TypeString);

                // Add the new ListView Item with the PID of the message
                //	
                lviCurrentItem.SubItems.Add(msgStsCurrentMsg.PIDString);
                // We set the length of the Message
                //
                lviCurrentItem.SubItems.Add(GetLengthFromDLC(newMsg.DLC, newMsg.MSGTYPE).ToString());
                // We set the SDT information of the message
                //
                lviCurrentItem.SubItems.Add(msgStsCurrentMsg.SDTString);
                // We set the VCID information of the message
                //
                lviCurrentItem.SubItems.Add(msgStsCurrentMsg.VCIDString);
                // We set the AF information of the message
                //
                lviCurrentItem.SubItems.Add(msgStsCurrentMsg.AFString);
                // Add time stamp information if needed
                //
                lviCurrentItem.SubItems.Add(msgStsCurrentMsg.TimeString);
                // we set the message count message (this is the First, so count is 1)            
                //
                lviCurrentItem.SubItems.Add(msgStsCurrentMsg.Count.ToString());
                // We set the data of the message. 	
                //
                lviCurrentItem.SubItems.Add(msgStsCurrentMsg.DataString);
            }
        }

        /// <summary>
        /// Processes a received message, in order to show it in the Message-ListView
        /// </summary>
        /// <param name="theMsg">The received PCAN-Basic message</param>
        /// <param name="timeStamp">The Timestamp of the new message</param>
        private void ProcessMessage(TPCANMsgXL theMsg, TPCANTimestampXL itsTimeStamp)
        {
            // We search if a message is 
            // already received or if this is a new message
            //
            lock (m_LastMsgsList.SyncRoot)
            {
                foreach (MessageStatus msg in m_LastMsgsList)
                {
                    if ((msg.CANMsg.PID == theMsg.PID) && (msg.CANMsg.MSGTYPE == theMsg.MSGTYPE) &&
                        (msg.CANMsg.VCID == theMsg.VCID) && (msg.CANMsg.SDT == theMsg.SDT) &&
                        (msg.CANMsg.AF == theMsg.AF) && (msg.CANMsg.RRS == theMsg.RRS) &&
                        (msg.CANMsg.SEC == theMsg.SEC))
                    {
                        // Modify the message and exit
                        //
                        msg.Update(theMsg, itsTimeStamp);
                        return;
                    }
                }
                // Message not found. It will created
                //
                InsertMsgEntry(theMsg, itsTimeStamp);
            }
        }

        /// <summary>
        /// Thread-Function used for reading PCAN-Basic messages
        /// </summary>
        private void CANReadThreadFunc()
        {
            UInt32 iBuffer;
            TPCANStatus stsResult;

            iBuffer = Convert.ToUInt32(m_ReceiveEvent.SafeWaitHandle.DangerousGetHandle().ToInt32());
            // Sets the handle of the Receive-Event.
            //
            stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_RECEIVE_EVENT, ref iBuffer, sizeof(UInt32));

            if (stsResult != TPCANStatus.PCAN_ERROR_OK)
            {
                MessageBox.Show(GetFormattedError(stsResult), "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // While this mode is selected
            while (m_ReadThreadRunning)
            {
                // Waiting for Receive-Event
                // 
                if (m_ReceiveEvent.WaitOne(50))
                    // Process Receive-Event using .NET BeginInvoke function
                    // in order to interact with Winforms UI (calling the 
                    // function ReadMessages)
                    // Warning: Do not use Invoke when calling Join with the main thread, since this could lead to a deadlock.
                    // 
                    this.BeginInvoke(m_ReadDelegate);
            }
        }

        /// <summary>
        /// Function for reading messages on XL devices
        /// </summary>
        /// <returns>A TPCANStatus error code</returns>
        private TPCANStatus ReadMessageXL()
        {
            TPCANMsgXL CANMsg;
            TPCANTimestampXL CANTimeStamp;
            TPCANStatus stsResult;

            // We execute the "Read" function of the PCANBasic                
            //
            stsResult = PCANBasic.ReadXL(m_PcanHandle, out CANMsg, out CANTimeStamp);
            if ((stsResult != TPCANStatus.PCAN_ERROR_QRCVEMPTY) && (stsResult != TPCANStatus.PCAN_ERROR_ILLDATA))
                // We process the received message
                //
                ProcessMessage(CANMsg, CANTimeStamp);

            return stsResult;
        }

        /// <summary>
        /// Function for reading PCAN-Basic messages
        /// </summary>
        private void ReadMessages()
        {
            TPCANStatus stsResult;

            // We read at least one time the queue looking for messages.
            // If a message is found, we look again trying to find more.
            // If the queue is empty or an error occur, we get out from
            // the do-while statement.
            //			
            do
            {
                stsResult = ReadMessageXL();
                if ((stsResult == TPCANStatus.PCAN_ERROR_ILLOPERATION) || (stsResult == TPCANStatus.PCAN_ERROR_ILLDATA))
                    break;
            } while (btnRelease.Enabled && (!Convert.ToBoolean(stsResult & TPCANStatus.PCAN_ERROR_QRCVEMPTY)));
        }
        #endregion

        #region Event Handlers
        #region Form event-handlers
        /// <summary>
        /// Constructor
        /// </summary>
        public Form1()
        {
            // Initializes form's component
            //
            InitializeComponent();
            // Initializes specific components
            //
            InitializeBasicComponents();
        }

        /// <summary>
        /// Form-Closing function / Finish function
        /// </summary>
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Releases the used PCAN-Basic channel
            //
            if (btnRelease.Enabled)
                btnRelease_Click(this, new EventArgs());
        }
        #endregion

        #region ComboBox event-handlers
        private void cbbChannel_SelectedIndexChanged(object sender, EventArgs e)
        {
            string strTemp;

            // Get the handle from the text being shown
            //
            strTemp = cbbChannel.Text;
            if (strTemp == string.Empty)
                return;
            else if (strTemp.Contains("refresh"))
                DoRefresh();
            else
            {
                strTemp = strTemp.Substring(strTemp.IndexOf('(') + 1, 3);

                strTemp = strTemp.Replace('h', ' ').Trim(' ');

                m_PcanHandle = Convert.ToUInt16(strTemp, 16);
            }
        }

        private void cbbParameter_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Activates/deactivates controls according with the selected 
            // PCAN-Basic parameter 
            //
            rdbParamActive.Enabled = (cbbParameter.SelectedIndex != 0) && (cbbParameter.SelectedIndex != 22);
            rdbParamInactive.Enabled = rdbParamActive.Enabled;
            nudDeviceId.Enabled = !rdbParamActive.Enabled;
            nudDelay.Enabled = !rdbParamActive.Enabled;
            laDeviceOrDelay.Text = (cbbParameter.SelectedIndex == 22) ? "Delay (μs):" : "Device ID (Hex):";
            nudDelay.Visible = cbbParameter.SelectedIndex == 22;
            nudDeviceId.Visible = !nudDelay.Visible;
        }
        #endregion

        #region Button event-handlers
        private void btnInit_Click(object sender, EventArgs e)
        {
            TPCANStatus stsResult;

            // Connects a selected PCAN-Basic channel
            //
            stsResult = PCANBasic.InitializeXL(
                m_PcanHandle,
                txtBitrate.Text);

            if (stsResult != TPCANStatus.PCAN_ERROR_OK)
                if (stsResult != TPCANStatus.PCAN_ERROR_CAUTION)
                    MessageBox.Show(GetFormattedError(stsResult));
                else
                {
                    IncludeTextMessage("******************************************************");
                    IncludeTextMessage("The bitrate being used is different than the given one");
                    IncludeTextMessage("******************************************************");
                    stsResult = TPCANStatus.PCAN_ERROR_OK;
                }
            else
                // Prepares the PCAN-Basic's PCAN-Trace file
                //
                ConfigureTraceFile();

            // Sets the connection status of the main-form
            //
            SetConnectionStatus(stsResult == TPCANStatus.PCAN_ERROR_OK);
        }

        private void btnRelease_Click(object sender, EventArgs e)
        {
            // Releases a current connected PCAN-Basic channel
            //
            PCANBasic.Uninitialize(m_PcanHandle);
            tmrRead.Enabled = false;
            if (m_ReadThread != null)
            {
                m_ReadThreadRunning = false;
                m_ReadThread.Join();
                m_ReadThread = null;
            }

            // Sets the connection status of the main-form
            //
            SetConnectionStatus(false);
        }

        private void btnFilterApply_Click(object sender, EventArgs e)
        {
            UInt32 iBuffer;
            TPCANStatus stsResult;

            // Gets the current status of the message filter
            //
            if (!GetFilterStatus(out iBuffer))
                return;

            // The filter will be full opened or complete closed
            //
            if (rdbFilterClose.Checked)
                iBuffer = PCANBasic.PCAN_FILTER_CLOSE;
            else
                iBuffer = PCANBasic.PCAN_FILTER_OPEN;

            // The filter is configured
            //
            stsResult = PCANBasic.SetValue(
                m_PcanHandle,
                TPCANParameter.PCAN_MESSAGE_FILTER,
                ref iBuffer,
                sizeof(UInt32));

            // If success, an information message is written, if it is not, an error message is shown
            //
            if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                IncludeTextMessage(string.Format("The filter was successfully {0}", rdbFilterClose.Checked ? "closed." : "opened."));
            else
                MessageBox.Show(GetFormattedError(stsResult));
        }

        private void btnFilterQuery_Click(object sender, EventArgs e)
        {
            UInt32 iBuffer;

            // Queries the current status of the message filter
            //
            if (GetFilterStatus(out iBuffer))
            {
                switch (iBuffer)
                {
                    // The filter is closed
                    //
                    case PCANBasic.PCAN_FILTER_CLOSE:
                        IncludeTextMessage("The Status of the filter is: closed.");
                        break;
                    // The filter is fully opened
                    //
                    case PCANBasic.PCAN_FILTER_OPEN:
                        IncludeTextMessage("The Status of the filter is: full opened.");
                        break;
                    // The status of the filter is undefined. (Should never happen)
                    //
                    default:
                        IncludeTextMessage("The Status of the filter is: Invalid.");
                        break;
                }
            }
        }

        private void btnParameterSet_Click(object sender, EventArgs e)
        {
            TPCANStatus stsResult;
            UInt32 iBuffer;
            bool bActivate;

            bActivate = rdbParamActive.Checked;

            // Sets a PCAN-Basic parameter value
            //
            switch (cbbParameter.SelectedIndex)
            {
                // The device identifier of a channel will be set
                //
                case 0:
                    iBuffer = Convert.ToUInt32(nudDeviceId.Value);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_DEVICE_ID, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage("The desired Device-ID was successfully configured");
                    break;
                // The 5 Volt Power feature of a channel will be set
                //
                case 1:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_5VOLTS_POWER, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The USB/PC-Card 5 power was successfully {0}", bActivate ? "activated" : "deactivated"));
                    break;
                // The feature for automatic reset on BUS-OFF will be set
                //
                case 2:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_BUSOFF_AUTORESET, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The automatic-reset on BUS-OFF was successfully {0}", bActivate ? "activated" : "deactivated"));
                    break;
                // The CAN option "Listen Only" will be set
                //
                case 3:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_LISTEN_ONLY, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The CAN option \"Listen Only\" was successfully {0}", bActivate ? "activated" : "deactivated"));
                    break;
                // The feature for logging debug-information will be set
                //
                case 4:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_LOG_STATUS, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The feature for logging debug information was successfully {0}", bActivate ? "activated" : "deactivated"));
                    break;
                // The channel option "Receive Status" will be set
                //
                case 5:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_RECEIVE_STATUS, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The channel option \"Receive Status\" was set to {0}", bActivate ? "ON" : "OFF"));
                    break;
                // The feature for tracing will be set
                //
                case 7:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_TRACE_STATUS, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The feature for tracing data was successfully {0}", bActivate ? "activated" : "deactivated"));
                    break;

                // The feature for identifying an USB Channel will be set
                //
                case 8:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_CHANNEL_IDENTIFYING, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The procedure for channel identification was successfully {0}", bActivate ? "activated" : "deactivated"));
                    break;

                // The feature for using an already configured speed will be set
                //
                case 10:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_BITRATE_ADAPTING, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The feature for bit rate adaptation was successfully {0}", bActivate ? "activated" : "deactivated"));
                    break;

                // The option "Allow Status Frames" will be set
                //
                case 19:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_STATUS_FRAMES, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The reception of Status frames was successfully {0}", bActivate ? "enabled" : "disabled"));
                    break;

                // The option "Allow RTR Frames" will be set
                //
                case 20:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_RTR_FRAMES, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The reception of RTR frames was successfully {0}", bActivate ? "enabled" : "disabled"));
                    break;

                // The option "Allow Error Frames" will be set
                //
                case 21:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_ERROR_FRAMES, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The reception of Error frames was successfully {0}", bActivate ? "enabled" : "disabled"));
                    break;

                // The option "Interframes Delay" will be set
                //
                case 22:
                    iBuffer = Convert.ToUInt32(nudDelay.Value);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_INTERFRAME_DELAY, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage("The delay between transmitting frames was successfully set");
                    break;

                // The option "Allow Echo Frames" will be set
                //
                case 23:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_ECHO_FRAMES, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The reception of Echo frames was successfully {0}", bActivate ? "enabled" : "disabled"));
                    break;

                // The option "Hard Reset Status" will be set
                //
                case 24:
                    iBuffer = (uint)(bActivate ? PCANBasic.PCAN_PARAMETER_ON : PCANBasic.PCAN_PARAMETER_OFF);
                    stsResult = PCANBasic.SetValue(m_PcanHandle, TPCANParameter.PCAN_HARD_RESET_STATUS, ref iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The activation of a hard reset within the method PCANBasic.Reset was successfully {0}", bActivate ? "enabled" : "disabled"));
                    break;

                // The current parameter is invalid
                //
                default:
                    stsResult = TPCANStatus.PCAN_ERROR_UNKNOWN;
                    MessageBox.Show("Wrong parameter code.");
                    return;
            }

            // If the function fail, an error message is shown
            //
            if (stsResult != TPCANStatus.PCAN_ERROR_OK)
                MessageBox.Show(GetFormattedError(stsResult));
        }

        private void btnParameterGet_Click(object sender, EventArgs e)
        {
            TPCANStatus stsResult;
            UInt32 iBuffer;
            StringBuilder strBuffer;

            strBuffer = new StringBuilder(255);

            // Gets a PCAN-Basic parameter value
            //
            switch (cbbParameter.SelectedIndex)
            {
                // The device identifier of a channel will be retrieved
                //
                case 0:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_DEVICE_ID, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The configured Device-ID is 0x{0:X}", iBuffer));
                    break;
                // The activation status of the 5 Volt Power feature of a channel will be retrieved
                //
                case 1:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_5VOLTS_POWER, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The 5-Volt Power of the USB/PC-Card is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "ON" : "OFF"));
                    break;
                // The activation status of the feature for automatic reset on BUS-OFF will be retrieved
                //
                case 2:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BUSOFF_AUTORESET, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The automatic-reset on BUS-OFF is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "ON" : "OFF"));
                    break;
                // The activation status of the CAN option "Listen Only" will be retrieved
                //
                case 3:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_LISTEN_ONLY, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The CAN option \"Listen Only\" is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "ON" : "OFF"));
                    break;
                // The activation status for the feature for logging debug-information will be retrieved
                case 4:
                    stsResult = PCANBasic.GetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_LOG_STATUS, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The feature for logging debug information is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "ON" : "OFF"));
                    break;
                // The activation status of the channel option "Receive Status"  will be retrieved
                //
                case 5:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_RECEIVE_STATUS, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The channel option \"Receive Status\" is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "ON" : "OFF"));
                    break;
                // The Number of the CAN-Controller used by a PCAN-Channel
                // 
                case 6:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_CONTROLLER_NUMBER, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The CAN Controller number is {0}", iBuffer));
                    break;
                // The activation status for the feature for tracing data will be retrieved
                //
                case 7:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_TRACE_STATUS, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The feature for tracing data is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "ON" : "OFF"));
                    break;
                // The activation status of the Channel Identifying procedure will be retrieved
                //
                case 8:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_CHANNEL_IDENTIFYING, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The identification procedure of the selected channel is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "ON" : "OFF"));
                    break;
                // The extra capabilities of a hardware will asked
                //
                case 9:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_CHANNEL_FEATURES, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                    {
                        IncludeTextMessage(string.Format("The channel {0} Flexible Data-Rate (CAN-FD)", ((iBuffer & PCANBasic.FEATURE_FD_CAPABLE) == PCANBasic.FEATURE_FD_CAPABLE) ? "does support" : "DOESN'T SUPPORT"));
                        IncludeTextMessage(string.Format("The channel {0} Extra Long (CAN-XL)", ((iBuffer & PCANBasic.FEATURE_XL_CAPABLE) == PCANBasic.FEATURE_XL_CAPABLE) ? "does support" : "DOESN'T SUPPORT"));
                        IncludeTextMessage(string.Format("The channel {0} an inter-frame delay for sending messages", ((iBuffer & PCANBasic.FEATURE_DELAY_CAPABLE) == PCANBasic.FEATURE_DELAY_CAPABLE) ? "does support" : "DOESN'T SUPPORT"));
                        IncludeTextMessage(string.Format("The channel {0} using I/O pins", ((iBuffer & PCANBasic.FEATURE_IO_CAPABLE) == PCANBasic.FEATURE_IO_CAPABLE) ? "does allow" : "DOESN'T ALLOW"));
                    }
                    break;
                // The status of the speed adapting feature will be retrieved
                //
                case 10:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BITRATE_ADAPTING, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The feature for bit rate adaptation is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "ON" : "OFF"));
                    break;

                // The bitrate of the connected Nominal channel will be retrieved (String value)
                //
                case 11:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BITRATE_INFO_CC, strBuffer, 255);
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                    {
                        IncludeTextMessage("The bit rate Nominal of the channel is represented by the following values:");
                        foreach (string strPart in strBuffer.ToString().Split(','))
                            IncludeTextMessage("   * " + strPart);
                    }
                    break;
                // The bitrate of the connected FD channel will be retrieved (String value)
                //
                case 12:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BITRATE_INFO_FD, strBuffer, 255);
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                    {
                        IncludeTextMessage("The bit rate FD of the channel is represented by the following values:");
                        foreach (string strPart in strBuffer.ToString().Split(','))
                            IncludeTextMessage("   * " + strPart);
                    }
                    break;
                // The bitrate of the connected XL channel will be retrieved (String value)
                //
                case 13:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BITRATE_INFO_XL, strBuffer, 255);
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                    {
                        IncludeTextMessage("The bit rate XL of the channel is represented by the following values:");
                        foreach (string strPart in strBuffer.ToString().Split(','))
                            IncludeTextMessage("   * " + strPart);
                    }
                    break;
                // The nominal speed configured on the CAN bus
                //
                case 14:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BUSSPEED_NOMINAL, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The nominal speed of the channel is {0} bit/s", iBuffer));
                    break;
                // The FD speed configured on the CAN bus
                //
                case 15:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BUSSPEED_FD, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The FD speed of the channel is {0} bit/s", iBuffer));
                    break;
                // The XL speed configured on the CAN bus
                //
                case 16:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_BUSSPEED_XL, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The XL speed of the channel is {0} bit/s", iBuffer));
                    break;
                // The IP address of a LAN channel as string, in IPv4 format
                //
                case 17:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_IP_ADDRESS, strBuffer, 255);
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The IP address of the channel is {0}", strBuffer.ToString()));
                    break;
                // The running status of the LAN Service
                //
                case 18:
                    stsResult = PCANBasic.GetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_LAN_SERVICE_STATUS, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The LAN service is {0}", (iBuffer == PCANBasic.SERVICE_STATUS_RUNNING) ? "running" : "NOT running"));
                    break;
                // The reception of Status frames
                //
                case 19:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_STATUS_FRAMES, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The reception of Status frames is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "enabled" : "disabled"));
                    break;
                // The reception of RTR frames
                //
                case 20:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_RTR_FRAMES, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The reception of RTR frames is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "enabled" : "disabled"));
                    break;
                // The reception of Error frames
                //
                case 21:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_ERROR_FRAMES, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The reception of Error frames is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "enabled" : "disabled"));
                    break;
                // The Interframe delay of an USB channel will be retrieved
                //
                case 22:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_INTERFRAME_DELAY, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The configured interframe delay is {0} μs", iBuffer));
                    break;
                // The reception of Echo frames
                //
                case 23:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_ALLOW_ECHO_FRAMES, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The reception of Echo frames is {0}", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "enabled" : "disabled"));
                    break;
                // The activation of Hard Reset
                case 24:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_HARD_RESET_STATUS, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The method PCANBasic.Reset is {0} a hardware reset", (iBuffer == PCANBasic.PCAN_PARAMETER_ON) ? "performing" : "NOT performing"));
                    break;

                // The direction of the communication with a LAN channel
                //
                case 25:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_LAN_CHANNEL_DIRECTION, out iBuffer, sizeof(UInt32));
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                    {
                        switch (iBuffer)
                        {
                            case PCANBasic.LAN_DIRECTION_READ:
                                IncludeTextMessage("The communication flow is: incoming only");
                                break;
                            case PCANBasic.LAN_DIRECTION_WRITE:
                                IncludeTextMessage("The communication flow is: outgoing only");
                                break;
                            case PCANBasic.LAN_DIRECTION_READ_WRITE:
                                IncludeTextMessage("The communication flow is: bidirectional");
                                break;
                            default:
                                IncludeTextMessage(string.Format("The communication flow is: undefined (0x{0:X4})", iBuffer));
                                break;
                        }
                    }
                    break;

                // The GUID of the device
                // 
                case 26:
                    stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_DEVICE_GUID, strBuffer, 255);
                    if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                        IncludeTextMessage(string.Format("The GUID of the device is {0}", strBuffer.ToString()));
                    break;

                // The current parameter is invalid
                //
                default:
                    stsResult = TPCANStatus.PCAN_ERROR_UNKNOWN;
                    MessageBox.Show("Wrong parameter code.");
                    return;
            }

            // If the function fail, an error message is shown
            //
            if (stsResult != TPCANStatus.PCAN_ERROR_OK)
                MessageBox.Show(GetFormattedError(stsResult));
        }

        private void btnRead_Click(object sender, EventArgs e)
        {
            TPCANStatus stsResult;

            // We execute the "Read" function of the PCANBasic                
            //
            stsResult = ReadMessageXL();
            if (stsResult != TPCANStatus.PCAN_ERROR_OK)
                // If an error occurred, an information message is included
                //
                IncludeTextMessage(GetFormattedError(stsResult));
        }

        private void btnGetVersions_Click(object sender, EventArgs e)
        {
            TPCANStatus stsResult;
            StringBuilder strTemp;
            string[] strArrayVersion;

            strTemp = new StringBuilder(256);

            // We get the version of the PCAN-Basic API
            //
            stsResult = PCANBasic.GetValue(PCANBasic.PCAN_NONEBUS, TPCANParameter.PCAN_API_VERSION, strTemp, 256);
            if (stsResult == TPCANStatus.PCAN_ERROR_OK)
            {
                IncludeTextMessage("API Version: " + strTemp.ToString());

                // We get the version of the firmware on the device
                //
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_FIRMWARE_VERSION, strTemp, 256);
                if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                    IncludeTextMessage("Firmware Version: " + strTemp.ToString());

                // We get the driver version of the channel being used
                //
                stsResult = PCANBasic.GetValue(m_PcanHandle, TPCANParameter.PCAN_CHANNEL_VERSION, strTemp, 256);
                if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                {
                    // Because this information contains line control characters (several lines)
                    // we split this also in several entries in the Information List-Box
                    //
                    strArrayVersion = strTemp.ToString().Split(new char[] { '\n' });
                    IncludeTextMessage("Channel/Driver Version: ");
                    for (int i = 0; i < strArrayVersion.Length; i++)
                        IncludeTextMessage("     * " + strArrayVersion[i]);
                }
            }

            // If an error occurred, a message is shown
            //
            if (stsResult != TPCANStatus.PCAN_ERROR_OK)
                MessageBox.Show(GetFormattedError(stsResult));
        }

        private void btnMsgClear_Click(object sender, EventArgs e)
        {
            // The information contained in the messages List-View
            // is cleared
            //
            lock (m_LastMsgsList.SyncRoot)
            {
                m_LastMsgsList.Clear();
                lstMessages.Items.Clear();
            }
        }

        private void btnInfoClear_Click(object sender, EventArgs e)
        {
            // The information contained in the Information List-Box 
            // is cleared
            //
            lbxInfo.Items.Clear();
        }

        private TPCANStatus WriteFrameXL()
        {
            TPCANMsgXL CANMsg;

            // We create a TPCANMsgXL message structure 
            //
            CANMsg = new TPCANMsgXL();
            CANMsg.DATA = new byte[2048];

            // We configurate the Message.
            //
            CANMsg.MSGTYPE = TPCANMessageTypeXL.PCAN_MESSAGE_XL;
            CANMsg.PID = Convert.ToUInt16(txtPID.Text, 16);
            CANMsg.DLC = Convert.ToUInt16(nudDLC.Value);
            CANMsg.SDT = Convert.ToByte(txtSDT.Text, 16);
            CANMsg.VCID = Convert.ToByte(txtVCID.Text, 16);
            CANMsg.AF = Convert.ToUInt32(txtAF.Text, 16);
            CANMsg.RRS = chbRRS.Checked;
            CANMsg.SEC = chbSEC.Checked;
            Array.Copy(m_data, CANMsg.DATA, (int)nudDLC.Value + 1);

            // The message is sent to the configured hardware
            //
            return PCANBasic.WriteXL(m_PcanHandle, ref CANMsg);
        }

        private void btnEdit_Click(object sender, EventArgs e)
        {
            EditDataForm editDataWindow = new EditDataForm(m_data, nudDLC.Value)
            {
                Owner = this
            };
            DialogResult result = editDataWindow.ShowDialog();

            if (result == DialogResult.OK)
            {
                m_data = editDataWindow.Result;
                InitializeData();
            }
        }

        private void btnGenerate_Click(object sender, EventArgs e)
        {
            RandomizeData();
            InitializeData();
        }

        private void btnWrite_Click(object sender, EventArgs e)
        {
            TPCANStatus stsResult;

            // Send the message
            //
            stsResult = WriteFrameXL();

            // The message was successfully sent
            //
            if (stsResult == TPCANStatus.PCAN_ERROR_OK)
                IncludeTextMessage("Message was successfully SENT");
            // An error occurred.  We show the error.
            //			
            else
                MessageBox.Show(GetFormattedError(stsResult));
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            TPCANStatus stsResult;

            // Resets the receive and transmit queues of a PCAN Channel.
            //
            stsResult = PCANBasic.Reset(m_PcanHandle);

            // If it fails, a error message is shown
            //
            if (stsResult != TPCANStatus.PCAN_ERROR_OK)
                MessageBox.Show(GetFormattedError(stsResult));
            else
                IncludeTextMessage("Receive and transmit queues successfully reset");
        }

        private void btnStatus_Click(object sender, EventArgs e)
        {
            TPCANStatus stsResult;
            String errorName;

            // Gets the current BUS status of a PCAN Channel.
            //
            stsResult = PCANBasic.GetStatus(m_PcanHandle);

            // Switch On Error Name
            //
            switch (stsResult)
            {
                case TPCANStatus.PCAN_ERROR_INITIALIZE:
                    errorName = "PCAN_ERROR_INITIALIZE";
                    break;

                case TPCANStatus.PCAN_ERROR_BUSLIGHT:
                    errorName = "PCAN_ERROR_BUSLIGHT";
                    break;

                case TPCANStatus.PCAN_ERROR_BUSWARNING:
                    errorName = "PCAN_ERROR_BUSWARNING";
                    break;

                case TPCANStatus.PCAN_ERROR_BUSPASSIVE:
                    errorName = "PCAN_ERROR_BUSPASSIVE";
                    break;

                case TPCANStatus.PCAN_ERROR_BUSOFF:
                    errorName = "PCAN_ERROR_BUSOFF";
                    break;

                case TPCANStatus.PCAN_ERROR_OK:
                    errorName = "PCAN_ERROR_OK";
                    break;

                default:
                    errorName = "See Documentation";
                    break;
            }

            // Display Message
            //
            IncludeTextMessage(String.Format("Status: {0} (0x{1:X}h)", errorName, stsResult));
        }
        #endregion

        #region Timer event-handler
        private void tmrRead_Tick(object sender, EventArgs e)
        {
            // Checks if in the receive-queue are currently messages for read
            // 
            ReadMessages();
        }

        private void tmrDisplay_Tick(object sender, EventArgs e)
        {
            DisplayMessages();
        }
        #endregion

        #region Message List-View event-handler
        private void lstMessages_DoubleClick(object sender, EventArgs e)
        {
            // Clears the content of the Message List-View
            //
            btnMsgClear_Click(this, new EventArgs());
        }
        #endregion

        #region Information List-Box event-handler
        private void lbxInfo_DoubleClick(object sender, EventArgs e)
        {
            // Clears the content of the Information List-Box
            //
            btnInfoClear_Click(this, new EventArgs());
        }
        #endregion

        #region Textbox event handlers
        private void txtAF_Leave(object sender, EventArgs e)
        {
            int iTextLength = 8;

            // The Textbox for the AF field is represented with 8 characters
            // (eight bytes in length). Therefore if the Length of the text is smaller than TextLength,  
            // we add "0"
            //
            while (txtAF.Text.Length != iTextLength)
                txtAF.Text = ("0" + txtAF.Text);
        }

        private void txtSDT_Leave(object sender, EventArgs e)
        {
            int iTextLength = 2;

            // The Textbox for the SDT is represented with 2 characters
            // (one byte in length). Therefore if the Length of the text is smaller than TextLength,  
            // we add "0"
            //
            while (txtSDT.Text.Length != iTextLength)
                txtSDT.Text = ("0" + txtSDT.Text);
        }

        private void txtVCID_Leave(object sender, EventArgs e)
        {
            int iTextLength = 2;

            // The Textbox for the VCID are represented with 2 characters
            // (one byte in length). Therefore if the Length of the text is smaller than TextLength,  
            // we add "0"
            //
            while (txtVCID.Text.Length != iTextLength)
                txtVCID.Text = ("0" + txtVCID.Text);
        }

        private void txtPID_Leave(object sender, EventArgs e)
        {
            int iTextLength = 3;
            uint uiMaxValue = (uint)0x7FF;

            // The Textbox for the PID is represented with 3 characters.
            // Therefore if the Length of the text is smaller than TextLength,  
            // we add "0"
            //
            while (txtPID.Text.Length != iTextLength)
                txtPID.Text = ("0" + txtPID.Text);

            // We check that the PID is not bigger than current maximum value
            //
            if (Convert.ToUInt32(txtPID.Text, 16) > uiMaxValue)
                txtPID.Text = string.Format("{0:X" + iTextLength.ToString() + "}", uiMaxValue);
        }

        private void txtPID_KeyPress(object sender, KeyPressEventArgs e)
        {
            char chCheck;

            // We convert the Character to its Upper case equivalent
            //
            chCheck = char.ToUpper(e.KeyChar);

            // The Key is the Delete (Backspace) Key
            //
            if (chCheck == 8)
                return;
            // The Key is a number between 0-9
            //
            if ((chCheck > 47) && (chCheck < 58))
                return;
            // The Key is a character between A-F
            //
            if ((chCheck > 64) && (chCheck < 71))
                return;

            // Is neither a number nor a character between A(a) and F(f)
            //
            e.Handled = true;
        }
        #endregion

        #region Radio- and Check- Buttons event-handlers
        private void chbShowPeriod_CheckedChanged(object sender, EventArgs e)
        {
            // According with the check-value of this checkbox,
            // the received time of a messages will be interpreted as 
            // period (time between the two last messages) or as time-stamp
            // (the elapsed time since windows was started)
            //
            lock (m_LastMsgsList.SyncRoot)
            {
                foreach (MessageStatus msg in m_LastMsgsList)
                    msg.ShowingPeriod = chbShowPeriod.Checked;
            }
        }

        private void rdbTimer_CheckedChanged(object sender, EventArgs e)
        {
            if (!btnRelease.Enabled)
                return;

            // According with the kind of reading, a timer, a thread or a button will be enabled
            //
            if (rdbTimer.Checked)
            {
                // Stops Read Thread if it exists
                //
                if (m_ReadThread != null)
                {
                    m_ReadThreadRunning = false;
                    m_ReadThread.Join();
                    m_ReadThread = null;
                }

                // Enable Timer
                //
                tmrRead.Enabled = btnRelease.Enabled;
            }
            if (rdbEvent.Checked)
            {
                // Disable Timer
                //
                tmrRead.Enabled = false;
                // Create and start the tread to read CAN Message using SetRcvEvent()
                //
                System.Threading.ThreadStart threadDelegate = new System.Threading.ThreadStart(this.CANReadThreadFunc);
                m_ReadThread = new System.Threading.Thread(threadDelegate)
                {
                    IsBackground = true
                };
                m_ReadThreadRunning = true;
                m_ReadThread.Start();
            }
            if (rdbManual.Checked)
            {
                // Stops Read Thread if it exists
                //
                if (m_ReadThread != null)
                {
                    m_ReadThreadRunning = false;
                    m_ReadThread.Join();
                    m_ReadThread = null;

                }
                // Disable Timer
                //
                tmrRead.Enabled = false;
            }
            btnRead.Enabled = btnRelease.Enabled && rdbManual.Checked;
        }

        private void nudDLC_ValueChanged(object sender, EventArgs e)
        {
            FillDataSets();
            laLength.Text = $"Length: {nudDLC.Value + 1} B.";
        }
        #endregion

        #endregion

        #endregion
    }
}
