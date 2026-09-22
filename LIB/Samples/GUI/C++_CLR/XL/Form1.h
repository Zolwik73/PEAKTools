#pragma once

#include ".\PCANBasicCLR.h"
#include "EditDataForm.h"

namespace PCANBasicExample 
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Inclusion of PEAK PCAN-Basic namespace
	/// </summary>
	using namespace Peak::Can::Basic;

	/// <summary>
	/// Converts a CAN DLC value into the actual data length of the CAN-XL frame.
	/// </summary>
	/// <param name="dlc">A value between 0 and 2047 (CAN XL DLC range)</param>
	/// <param name="messageTypeXL">The message type of the message</param>
	/// <returns>The length represented by the DLC</returns>
	static int GetLengthFromDLC(int dlc, TPCANMessageTypeXL messageTypeXL)
	{
		if ((messageTypeXL & TPCANMessageTypeXL::PCAN_MESSAGE_STATUS) == TPCANMessageTypeXL::PCAN_MESSAGE_STATUS ||
			(messageTypeXL & TPCANMessageTypeXL::PCAN_MESSAGE_XL) != TPCANMessageTypeXL::PCAN_MESSAGE_XL) 
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
	/// Message Status structure used to show CAN Messages
	/// in a ListView
	///
	/// </summary>
	public value class MessageStatus 
	{
		private: 
			TPCANMsgXL m_Msg;
			TPCANTimestampFD m_TimeStamp;
			TPCANTimestampFD m_oldTimeStamp;
			int m_iIndex;
			int m_Count;
			bool m_bShowPeriod;
			bool m_bWasChanged;

			// Constructor
			//
		public: MessageStatus(TPCANMsgXL canMsg, TPCANTimestampXL canTimestamp, int listIndex)
			{
				m_Msg = canMsg;
				m_TimeStamp = canTimestamp;
				m_oldTimeStamp = canTimestamp;
				m_iIndex = listIndex;
				m_Count = 1;
				m_bShowPeriod = true;
				m_bWasChanged = false;
			}

		public: void Update(TPCANMsgXL ^canMsg, TPCANTimestampXL canTimestamp)
            {
                m_Msg = *canMsg;
                m_oldTimeStamp = m_TimeStamp;
                m_TimeStamp = canTimestamp;
                m_bWasChanged = true;
                m_Count += 1;
            }

		private: String^ GetTimeString()
            {
                double fTime;

                fTime = (m_TimeStamp / 1000.0);
                if (m_bShowPeriod)
                    fTime -= (m_oldTimeStamp / 1000.0);
                return fTime.ToString("F1");                
            }

		private: String^ GetDataString()
            {
                String^ strTemp;

                strTemp = "";

				if ((m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_RTR) == TPCANMessageTypeXL::PCAN_MESSAGE_RTR)
                    return "Remote Request";
                else
					for (int i = 0; i < GetLengthFromDLC(m_Msg.DLC, m_Msg.MSGTYPE); i++)
						strTemp += String::Format("{0:X2} ", m_Msg.DATA[i]);

                return strTemp;
            }

		private: String^ GetPidString()
        {
            // We format the PID of the message and show it
            //
			if ((m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_EXTENDED) == TPCANMessageTypeXL::PCAN_MESSAGE_EXTENDED)
				return String::Format("{0:X8}h", m_Msg.PID);
            else
				return String::Format("{0:X3}h", m_Msg.PID);
        }

		private: String^ GetSdtString()
		{
			return String::Format("{0:X2}h", m_Msg.SDT);
		}

		private: String^ GetVcidString()
		{
			return String::Format("{0:X2}h", m_Msg.VCID);
		}

		private: String^ GetAfString()
		{
			return String::Format("{0:X8}h", m_Msg.AF);
		}

		private: String^ GetMsgTypeString()
		{
			String^ strTemp = "";
			bool isEcho = (m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_ECHO) == TPCANMessageTypeXL::PCAN_MESSAGE_ECHO;

			if ((m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_STATUS) == TPCANMessageTypeXL::PCAN_MESSAGE_STATUS)
				return "STATUS";

			if ((m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_ERRFRAME) == TPCANMessageTypeXL::PCAN_MESSAGE_ERRFRAME)
				return "ERROR";

			if ((m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_XL) == TPCANMessageTypeXL::PCAN_MESSAGE_XL)
				strTemp = "XL";
			else if ((m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_EXTENDED) == TPCANMessageTypeXL::PCAN_MESSAGE_EXTENDED)
				strTemp = "EXT";
			else
				strTemp = "STD";

			if ((m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_RTR) == TPCANMessageTypeXL::PCAN_MESSAGE_RTR)
			{
				strTemp += isEcho ? "/RTR [ ECHO ]" : "/RTR";
			}
			else
			{
				if (isEcho)
					strTemp += " [ ECHO";
				else
					strTemp += " [ ";

				if ((int)m_Msg.MSGTYPE >= (int)TPCANMessageTypeXL::PCAN_MESSAGE_XL)
				{
					if (m_Msg.RRS)
						strTemp += " RRS";
					if (m_Msg.SEC)
						strTemp += " SEC";
				}
				else if ((int)m_Msg.MSGTYPE > (int)TPCANMessageTypeXL::PCAN_MESSAGE_EXTENDED)
				{
					if ((m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_FD) == TPCANMessageTypeXL::PCAN_MESSAGE_FD)
						strTemp += " FD";
					if ((m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_BRS) == TPCANMessageTypeXL::PCAN_MESSAGE_BRS)
						strTemp += " BRS";
					if ((m_Msg.MSGTYPE & TPCANMessageTypeXL::PCAN_MESSAGE_ESI) == TPCANMessageTypeXL::PCAN_MESSAGE_ESI)
						strTemp += " ESI";
				}
				strTemp += " ]";
			}

			return strTemp;
		}

		public: property TPCANMsgXL CANMsg
            {
                TPCANMsgXL get() { return m_Msg; }
            }

		public: property TPCANTimestampFD Timestamp
            {
                TPCANTimestampFD get() { return m_TimeStamp; }
            }

		public: property int Position
            {
                int get() { return m_iIndex; }
            }

		public: property String^ TypeString
            {
                String^ get() { return GetMsgTypeString(); }
            }

		public: property String^ PidString
            {
                String^ get() { return GetPidString(); }
            }

		public:	property String^ SdtString
			{
				String^ get() { return GetSdtString(); }
			}

		public: property String^ VcidString
			{
				String^ get() { return GetVcidString(); }
			}

		public: property String^ AfString
			{
				String^ get() { return GetAfString(); }
			}

		public: property String^ DataString
            {
                String^ get() { return GetDataString(); }
            }

		public: property int Count
            {
                int get() { return m_Count; }
            }

		public: property bool ShowingPeriod
            {
                bool get() { return m_bShowPeriod; }
                void set(bool value)
                {
                    if (m_bShowPeriod ^ value)
                    {
                        m_bShowPeriod = value;
                        m_bWasChanged = true;
                    }
                }
            }

		public: property bool MarkedAsUpdated
            {
                bool get() { return m_bWasChanged; }
                void set(bool value) { m_bWasChanged = value; }
            }

		public: property String^ TimeString
            {
                String^ get() { return GetTimeString(); }
            }
	};


	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
            // Initializes Form's component
            //
			InitializeComponent();
            // Initializes specific components
            //
			InitializeBasicComponents();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^ groupBox5;
	protected:
	private: System::Windows::Forms::CheckBox^ chbShowPeriod;
	private: System::Windows::Forms::RadioButton^ rdbManual;
	private: System::Windows::Forms::RadioButton^ rdbEvent;
	private: System::Windows::Forms::ListView^ lstMessages;
	private: System::Windows::Forms::ColumnHeader^ clhType;
	private: System::Windows::Forms::ColumnHeader^ clhPID;
	private: System::Windows::Forms::ColumnHeader^ clhDLC;
	private: System::Windows::Forms::ColumnHeader^ clhSDT;
	private: System::Windows::Forms::ColumnHeader^ clhVCID;
	private: System::Windows::Forms::ColumnHeader^ clhAF;
	private: System::Windows::Forms::ColumnHeader^ clhRcvTime;
	private: System::Windows::Forms::ColumnHeader^ clhCount;
	private: System::Windows::Forms::ColumnHeader^ clhData;
	private: System::Windows::Forms::Button^ btnMsgClear;
	private: System::Windows::Forms::RadioButton^ rdbTimer;
	private: System::Windows::Forms::Button^ btnRead;
	private: System::Windows::Forms::Timer^ tmrRead;
	private: System::Windows::Forms::GroupBox^ groupBox6;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::TextBox^ txtData;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::TextBox^ txtAF;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ txtVCID;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::TextBox^ txtSDT;
	private: System::Windows::Forms::CheckBox^ chbSEC;
	private: System::Windows::Forms::CheckBox^ chbRRS;
	private: System::Windows::Forms::Button^ btnWrite;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::TextBox^ txtPID;
	private: System::Windows::Forms::NumericUpDown^ nudDLC;
	private: System::Windows::Forms::Timer^ tmrDisplay;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ txtBitrate;
	private: System::Windows::Forms::ComboBox^ cbbChannel;
	private: System::Windows::Forms::Button^ btnInit;
	private: System::Windows::Forms::Button^ btnRelease;
	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::RadioButton^ rdbFilterOpen;
	private: System::Windows::Forms::RadioButton^ rdbFilterClose;
	private: System::Windows::Forms::Button^ btnFilterQuery;
	private: System::Windows::Forms::Button^ btnFilterApply;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::NumericUpDown^ nudDelay;
	private: System::Windows::Forms::Button^ btnParameterGet;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::NumericUpDown^ nudDeviceId;
	private: System::Windows::Forms::Label^ laDeviceOrDelay;
	private: System::Windows::Forms::ComboBox^ cbbParameter;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::RadioButton^ rdbParamActive;
	private: System::Windows::Forms::RadioButton^ rdbParamInactive;
	private: System::Windows::Forms::Button^ btnParameterSet;
	private: System::Windows::Forms::Button^ btnInfoClear;
	private: System::Windows::Forms::Button^ btnGetVersions;
	private: System::Windows::Forms::ListBox^ lbxInfo;
	private: System::Windows::Forms::Button^ btnStatus;
	private: System::Windows::Forms::GroupBox^ groupBox4;
	private: System::Windows::Forms::Button^ btnReset;
	private: System::Windows::Forms::Button^ btnEdit;
	private: System::Windows::Forms::Button^ btnGenerate;
	private: System::Windows::Forms::Label^ laLength;

	private: System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->chbShowPeriod = (gcnew System::Windows::Forms::CheckBox());
			this->rdbManual = (gcnew System::Windows::Forms::RadioButton());
			this->rdbEvent = (gcnew System::Windows::Forms::RadioButton());
			this->lstMessages = (gcnew System::Windows::Forms::ListView());
			this->clhType = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhPID = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhDLC = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhSDT = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhVCID = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhAF = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhRcvTime = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhCount = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhData = (gcnew System::Windows::Forms::ColumnHeader());
			this->btnMsgClear = (gcnew System::Windows::Forms::Button());
			this->rdbTimer = (gcnew System::Windows::Forms::RadioButton());
			this->btnRead = (gcnew System::Windows::Forms::Button());
			this->tmrRead = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->btnGenerate = (gcnew System::Windows::Forms::Button());
			this->btnEdit = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->txtData = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->txtAF = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->txtVCID = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->txtSDT = (gcnew System::Windows::Forms::TextBox());
			this->chbSEC = (gcnew System::Windows::Forms::CheckBox());
			this->chbRRS = (gcnew System::Windows::Forms::CheckBox());
			this->btnWrite = (gcnew System::Windows::Forms::Button());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->txtPID = (gcnew System::Windows::Forms::TextBox());
			this->nudDLC = (gcnew System::Windows::Forms::NumericUpDown());
			this->tmrDisplay = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtBitrate = (gcnew System::Windows::Forms::TextBox());
			this->cbbChannel = (gcnew System::Windows::Forms::ComboBox());
			this->btnInit = (gcnew System::Windows::Forms::Button());
			this->btnRelease = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->rdbFilterOpen = (gcnew System::Windows::Forms::RadioButton());
			this->rdbFilterClose = (gcnew System::Windows::Forms::RadioButton());
			this->btnFilterQuery = (gcnew System::Windows::Forms::Button());
			this->btnFilterApply = (gcnew System::Windows::Forms::Button());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->nudDelay = (gcnew System::Windows::Forms::NumericUpDown());
			this->btnParameterGet = (gcnew System::Windows::Forms::Button());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->nudDeviceId = (gcnew System::Windows::Forms::NumericUpDown());
			this->laDeviceOrDelay = (gcnew System::Windows::Forms::Label());
			this->cbbParameter = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->rdbParamActive = (gcnew System::Windows::Forms::RadioButton());
			this->rdbParamInactive = (gcnew System::Windows::Forms::RadioButton());
			this->btnParameterSet = (gcnew System::Windows::Forms::Button());
			this->btnInfoClear = (gcnew System::Windows::Forms::Button());
			this->btnGetVersions = (gcnew System::Windows::Forms::Button());
			this->lbxInfo = (gcnew System::Windows::Forms::ListBox());
			this->btnStatus = (gcnew System::Windows::Forms::Button());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->btnReset = (gcnew System::Windows::Forms::Button());
			this->laLength = (gcnew System::Windows::Forms::Label());
			this->groupBox5->SuspendLayout();
			this->groupBox6->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudDLC))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudDelay))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudDeviceId))->BeginInit();
			this->groupBox4->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox5
			// 
			this->groupBox5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox5->Controls->Add(this->chbShowPeriod);
			this->groupBox5->Controls->Add(this->rdbManual);
			this->groupBox5->Controls->Add(this->rdbEvent);
			this->groupBox5->Controls->Add(this->lstMessages);
			this->groupBox5->Controls->Add(this->btnMsgClear);
			this->groupBox5->Controls->Add(this->rdbTimer);
			this->groupBox5->Controls->Add(this->btnRead);
			this->groupBox5->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->groupBox5->Location = System::Drawing::Point(21, 212);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(714, 140);
			this->groupBox5->TabIndex = 56;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L" Messages Reading ";
			// 
			// chbShowPeriod
			// 
			this->chbShowPeriod->AutoSize = true;
			this->chbShowPeriod->Checked = true;
			this->chbShowPeriod->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chbShowPeriod->Location = System::Drawing::Point(374, 15);
			this->chbShowPeriod->Name = L"chbShowPeriod";
			this->chbShowPeriod->Size = System::Drawing::Size(123, 17);
			this->chbShowPeriod->TabIndex = 75;
			this->chbShowPeriod->Text = L"Timestamp as period";
			this->chbShowPeriod->UseVisualStyleBackColor = true;
			this->chbShowPeriod->CheckedChanged += gcnew System::EventHandler(this, &Form1::chbShowPeriod_CheckedChanged);
			// 
			// rdbManual
			// 
			this->rdbManual->AutoSize = true;
			this->rdbManual->Location = System::Drawing::Point(276, 14);
			this->rdbManual->Name = L"rdbManual";
			this->rdbManual->Size = System::Drawing::Size(89, 17);
			this->rdbManual->TabIndex = 74;
			this->rdbManual->Text = L"Manual Read";
			this->rdbManual->UseVisualStyleBackColor = true;
			this->rdbManual->CheckedChanged += gcnew System::EventHandler(this, &Form1::rdbTimer_CheckedChanged);
			// 
			// rdbEvent
			// 
			this->rdbEvent->AutoSize = true;
			this->rdbEvent->Location = System::Drawing::Point(131, 14);
			this->rdbEvent->Name = L"rdbEvent";
			this->rdbEvent->Size = System::Drawing::Size(139, 17);
			this->rdbEvent->TabIndex = 73;
			this->rdbEvent->Text = L"Reading using an Event";
			this->rdbEvent->UseVisualStyleBackColor = true;
			this->rdbEvent->CheckedChanged += gcnew System::EventHandler(this, &Form1::rdbTimer_CheckedChanged);
			// 
			// lstMessages
			// 
			this->lstMessages->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(9) {
				this->clhType, this->clhPID,
					this->clhDLC, this->clhSDT, this->clhVCID, this->clhAF, this->clhRcvTime, this->clhCount, this->clhData
			});
			this->lstMessages->FullRowSelect = true;
			this->lstMessages->HideSelection = false;
			this->lstMessages->Location = System::Drawing::Point(8, 37);
			this->lstMessages->MultiSelect = false;
			this->lstMessages->Name = L"lstMessages";
			this->lstMessages->Size = System::Drawing::Size(629, 96);
			this->lstMessages->TabIndex = 28;
			this->lstMessages->UseCompatibleStateImageBehavior = false;
			this->lstMessages->View = System::Windows::Forms::View::Details;
			this->lstMessages->DoubleClick += gcnew System::EventHandler(this, &Form1::lstMessages_DoubleClick);
			// 
			// clhType
			// 
			this->clhType->Text = L"Type";
			this->clhType->Width = 80;
			// 
			// clhPID
			// 
			this->clhPID->Text = L"PID";
			this->clhPID->Width = 70;
			// 
			// clhDLC
			// 
			this->clhDLC->Text = L"Length";
			this->clhDLC->Width = 90;
			// 
			// clhSDT
			// 
			this->clhSDT->Text = L"SDT";
			this->clhSDT->Width = 70;
			// 
			// clhVCID
			// 
			this->clhVCID->Text = L"VCID";
			this->clhVCID->Width = 70;
			// 
			// clhAF
			// 
			this->clhAF->Text = L"AF";
			this->clhAF->Width = 110;
			// 
			// clhRcvTime
			// 
			this->clhRcvTime->Text = L"Rcv Time";
			this->clhRcvTime->Width = 110;
			// 
			// clhCount
			// 
			this->clhCount->Text = L"Count";
			this->clhCount->Width = 90;
			// 
			// clhData
			// 
			this->clhData->Text = L"Data";
			this->clhData->Width = 500;
			// 
			// btnMsgClear
			// 
			this->btnMsgClear->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnMsgClear->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnMsgClear->Location = System::Drawing::Point(643, 66);
			this->btnMsgClear->Name = L"btnMsgClear";
			this->btnMsgClear->Size = System::Drawing::Size(65, 23);
			this->btnMsgClear->TabIndex = 50;
			this->btnMsgClear->Text = L"Clear";
			this->btnMsgClear->UseVisualStyleBackColor = true;
			this->btnMsgClear->Click += gcnew System::EventHandler(this, &Form1::btnMsgClear_Click);
			// 
			// rdbTimer
			// 
			this->rdbTimer->AutoSize = true;
			this->rdbTimer->Checked = true;
			this->rdbTimer->Location = System::Drawing::Point(8, 14);
			this->rdbTimer->Name = L"rdbTimer";
			this->rdbTimer->Size = System::Drawing::Size(117, 17);
			this->rdbTimer->TabIndex = 72;
			this->rdbTimer->TabStop = true;
			this->rdbTimer->Text = L"Read using a Timer";
			this->rdbTimer->UseVisualStyleBackColor = true;
			this->rdbTimer->CheckedChanged += gcnew System::EventHandler(this, &Form1::rdbTimer_CheckedChanged);
			// 
			// btnRead
			// 
			this->btnRead->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnRead->Enabled = false;
			this->btnRead->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnRead->Location = System::Drawing::Point(643, 37);
			this->btnRead->Name = L"btnRead";
			this->btnRead->Size = System::Drawing::Size(65, 23);
			this->btnRead->TabIndex = 49;
			this->btnRead->Text = L"Read";
			this->btnRead->UseVisualStyleBackColor = true;
			this->btnRead->Click += gcnew System::EventHandler(this, &Form1::btnRead_Click);
			// 
			// tmrRead
			// 
			this->tmrRead->Interval = 50;
			this->tmrRead->Tick += gcnew System::EventHandler(this, &Form1::tmrRead_Tick);
			// 
			// groupBox6
			// 
			this->groupBox6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox6->Controls->Add(this->laLength);
			this->groupBox6->Controls->Add(this->btnGenerate);
			this->groupBox6->Controls->Add(this->btnEdit);
			this->groupBox6->Controls->Add(this->label7);
			this->groupBox6->Controls->Add(this->txtData);
			this->groupBox6->Controls->Add(this->label5);
			this->groupBox6->Controls->Add(this->txtAF);
			this->groupBox6->Controls->Add(this->label3);
			this->groupBox6->Controls->Add(this->txtVCID);
			this->groupBox6->Controls->Add(this->label4);
			this->groupBox6->Controls->Add(this->txtSDT);
			this->groupBox6->Controls->Add(this->chbSEC);
			this->groupBox6->Controls->Add(this->chbRRS);
			this->groupBox6->Controls->Add(this->btnWrite);
			this->groupBox6->Controls->Add(this->label12);
			this->groupBox6->Controls->Add(this->label13);
			this->groupBox6->Controls->Add(this->txtPID);
			this->groupBox6->Controls->Add(this->nudDLC);
			this->groupBox6->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->groupBox6->Location = System::Drawing::Point(21, 358);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(714, 149);
			this->groupBox6->TabIndex = 57;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Write Messages";
			// 
			// btnGenerate
			// 
			this->btnGenerate->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnGenerate->Cursor = System::Windows::Forms::Cursors::Default;
			this->btnGenerate->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnGenerate->Location = System::Drawing::Point(643, 81);
			this->btnGenerate->Name = L"btnGenerate";
			this->btnGenerate->Size = System::Drawing::Size(65, 23);
			this->btnGenerate->TabIndex = 120;
			this->btnGenerate->Text = L"Generate";
			this->btnGenerate->Click += gcnew System::EventHandler(this, &Form1::btnGenerate_Click);
			// 
			// btnEdit
			// 
			this->btnEdit->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnEdit->Cursor = System::Windows::Forms::Cursors::Default;
			this->btnEdit->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnEdit->Location = System::Drawing::Point(643, 52);
			this->btnEdit->Name = L"btnEdit";
			this->btnEdit->Size = System::Drawing::Size(65, 23);
			this->btnEdit->TabIndex = 120;
			this->btnEdit->Text = L"Edit";
			this->btnEdit->Click += gcnew System::EventHandler(this, &Form1::btnEdit_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(242, 29);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(77, 13);
			this->label7->TabIndex = 119;
			this->label7->Text = L"Data (random):";
			// 
			// txtData
			// 
			this->txtData->Location = System::Drawing::Point(245, 51);
			this->txtData->Multiline = true;
			this->txtData->Name = L"txtData";
			this->txtData->ReadOnly = true;
			this->txtData->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->txtData->Size = System::Drawing::Size(392, 82);
			this->txtData->TabIndex = 118;
			this->txtData->TabStop = false;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(146, 91);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(51, 13);
			this->label5->TabIndex = 117;
			this->label5->Text = L"AF (Hex):";
			// 
			// txtAF
			// 
			this->txtAF->CharacterCasing = System::Windows::Forms::CharacterCasing::Upper;
			this->txtAF->Location = System::Drawing::Point(149, 113);
			this->txtAF->MaxLength = 8;
			this->txtAF->Name = L"txtAF";
			this->txtAF->Size = System::Drawing::Size(90, 20);
			this->txtAF->TabIndex = 6;
			this->txtAF->Text = L"00000000";
			this->txtAF->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::txtPID_KeyPress);
			this->txtAF->Leave += gcnew System::EventHandler(this, &Form1::txtAF_Leave);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(74, 91);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(63, 13);
			this->label3->TabIndex = 115;
			this->label3->Text = L"VCID (Hex):";
			// 
			// txtVCID
			// 
			this->txtVCID->CharacterCasing = System::Windows::Forms::CharacterCasing::Upper;
			this->txtVCID->Location = System::Drawing::Point(77, 113);
			this->txtVCID->MaxLength = 2;
			this->txtVCID->Name = L"txtVCID";
			this->txtVCID->Size = System::Drawing::Size(53, 20);
			this->txtVCID->TabIndex = 5;
			this->txtVCID->Text = L"00";
			this->txtVCID->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::txtPID_KeyPress);
			this->txtVCID->Leave += gcnew System::EventHandler(this, &Form1::txtVCID_Leave);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(7, 91);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(60, 13);
			this->label4->TabIndex = 113;
			this->label4->Text = L"SDT (Hex):";
			// 
			// txtSDT
			// 
			this->txtSDT->CharacterCasing = System::Windows::Forms::CharacterCasing::Upper;
			this->txtSDT->Location = System::Drawing::Point(10, 113);
			this->txtSDT->MaxLength = 2;
			this->txtSDT->Name = L"txtSDT";
			this->txtSDT->Size = System::Drawing::Size(53, 20);
			this->txtSDT->TabIndex = 4;
			this->txtSDT->Text = L"00";
			this->txtSDT->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::txtPID_KeyPress);
			this->txtSDT->Leave += gcnew System::EventHandler(this, &Form1::txtSDT_Leave);
			// 
			// chbSEC
			// 
			this->chbSEC->Cursor = System::Windows::Forms::Cursors::Default;
			this->chbSEC->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chbSEC->Location = System::Drawing::Point(199, 51);
			this->chbSEC->Name = L"chbSEC";
			this->chbSEC->Size = System::Drawing::Size(40, 24);
			this->chbSEC->TabIndex = 3;
			this->chbSEC->Text = L"SEC";
			// 
			// chbRRS
			// 
			this->chbRRS->Cursor = System::Windows::Forms::Cursors::Default;
			this->chbRRS->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chbRRS->Location = System::Drawing::Point(149, 51);
			this->chbRRS->Name = L"chbRRS";
			this->chbRRS->Size = System::Drawing::Size(44, 24);
			this->chbRRS->TabIndex = 2;
			this->chbRRS->Text = L"RRS";
			// 
			// btnWrite
			// 
			this->btnWrite->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnWrite->Cursor = System::Windows::Forms::Cursors::Default;
			this->btnWrite->Enabled = false;
			this->btnWrite->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnWrite->Location = System::Drawing::Point(643, 110);
			this->btnWrite->Name = L"btnWrite";
			this->btnWrite->Size = System::Drawing::Size(65, 23);
			this->btnWrite->TabIndex = 7;
			this->btnWrite->Text = L"Write";
			this->btnWrite->Click += gcnew System::EventHandler(this, &Form1::btnWrite_Click);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(74, 29);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(31, 13);
			this->label12->TabIndex = 31;
			this->label12->Text = L"DLC:";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(7, 29);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(56, 13);
			this->label13->TabIndex = 30;
			this->label13->Text = L"PID (Hex):";
			// 
			// txtPID
			// 
			this->txtPID->CharacterCasing = System::Windows::Forms::CharacterCasing::Upper;
			this->txtPID->Location = System::Drawing::Point(10, 51);
			this->txtPID->MaxLength = 3;
			this->txtPID->Name = L"txtPID";
			this->txtPID->Size = System::Drawing::Size(53, 20);
			this->txtPID->TabIndex = 0;
			this->txtPID->Text = L"000";
			this->txtPID->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::txtPID_KeyPress);
			this->txtPID->Leave += gcnew System::EventHandler(this, &Form1::txtPID_Leave);
			// 
			// nudDLC
			// 
			this->nudDLC->BackColor = System::Drawing::Color::White;
			this->nudDLC->Location = System::Drawing::Point(77, 51);
			this->nudDLC->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2047, 0, 0, 0 });
			this->nudDLC->Name = L"nudDLC";
			this->nudDLC->Size = System::Drawing::Size(52, 20);
			this->nudDLC->TabIndex = 1;
			this->nudDLC->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 8, 0, 0, 0 });
			this->nudDLC->ValueChanged += gcnew System::EventHandler(this, &Form1::nudDLC_ValueChanged);
			// 
			// tmrDisplay
			// 
			this->tmrDisplay->Tick += gcnew System::EventHandler(this, &Form1::tmrDisplay_Tick);
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->txtBitrate);
			this->groupBox1->Controls->Add(this->cbbChannel);
			this->groupBox1->Controls->Add(this->btnInit);
			this->groupBox1->Controls->Add(this->btnRelease);
			this->groupBox1->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->groupBox1->Location = System::Drawing::Point(21, 4);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(714, 70);
			this->groupBox1->TabIndex = 52;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L" Connection ";
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(187, 22);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(59, 15);
			this->label2->TabIndex = 50;
			this->label2->Text = L"Bit rate:";
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(14, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(59, 15);
			this->label1->TabIndex = 49;
			this->label1->Text = L"Channel:";
			// 
			// txtBitrate
			// 
			this->txtBitrate->Location = System::Drawing::Point(188, 43);
			this->txtBitrate->Name = L"txtBitrate";
			this->txtBitrate->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->txtBitrate->Size = System::Drawing::Size(449, 20);
			this->txtBitrate->TabIndex = 48;
			this->txtBitrate->Text = resources->GetString(L"txtBitrate.Text");
			// 
			// cbbChannel
			// 
			this->cbbChannel->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbbChannel->Font = (gcnew System::Drawing::Font(L"Consolas", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->cbbChannel->Location = System::Drawing::Point(15, 42);
			this->cbbChannel->Name = L"cbbChannel";
			this->cbbChannel->Size = System::Drawing::Size(163, 21);
			this->cbbChannel->TabIndex = 32;
			this->cbbChannel->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::cbbChannel_SelectedIndexChanged);
			// 
			// btnInit
			// 
			this->btnInit->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnInit->Cursor = System::Windows::Forms::Cursors::Default;
			this->btnInit->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnInit->Location = System::Drawing::Point(643, 15);
			this->btnInit->Name = L"btnInit";
			this->btnInit->Size = System::Drawing::Size(65, 23);
			this->btnInit->TabIndex = 34;
			this->btnInit->Text = L"Initialize";
			this->btnInit->Click += gcnew System::EventHandler(this, &Form1::btnInit_Click);
			// 
			// btnRelease
			// 
			this->btnRelease->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnRelease->Cursor = System::Windows::Forms::Cursors::Default;
			this->btnRelease->Enabled = false;
			this->btnRelease->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnRelease->Location = System::Drawing::Point(643, 40);
			this->btnRelease->Name = L"btnRelease";
			this->btnRelease->Size = System::Drawing::Size(65, 23);
			this->btnRelease->TabIndex = 35;
			this->btnRelease->Text = L"Release";
			this->btnRelease->Click += gcnew System::EventHandler(this, &Form1::btnRelease_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox3->Controls->Add(this->rdbFilterOpen);
			this->groupBox3->Controls->Add(this->rdbFilterClose);
			this->groupBox3->Controls->Add(this->btnFilterQuery);
			this->groupBox3->Controls->Add(this->btnFilterApply);
			this->groupBox3->Location = System::Drawing::Point(21, 80);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(714, 61);
			this->groupBox3->TabIndex = 53;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L" Message Filtering ";
			// 
			// rdbFilterOpen
			// 
			this->rdbFilterOpen->Checked = true;
			this->rdbFilterOpen->Location = System::Drawing::Point(20, 26);
			this->rdbFilterOpen->Name = L"rdbFilterOpen";
			this->rdbFilterOpen->Size = System::Drawing::Size(53, 17);
			this->rdbFilterOpen->TabIndex = 57;
			this->rdbFilterOpen->TabStop = true;
			this->rdbFilterOpen->Text = L"Open";
			this->rdbFilterOpen->UseVisualStyleBackColor = true;
			// 
			// rdbFilterClose
			// 
			this->rdbFilterClose->Location = System::Drawing::Point(77, 26);
			this->rdbFilterClose->Name = L"rdbFilterClose";
			this->rdbFilterClose->Size = System::Drawing::Size(58, 17);
			this->rdbFilterClose->TabIndex = 56;
			this->rdbFilterClose->Text = L"Close";
			this->rdbFilterClose->UseVisualStyleBackColor = true;
			// 
			// btnFilterQuery
			// 
			this->btnFilterQuery->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnFilterQuery->Enabled = false;
			this->btnFilterQuery->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnFilterQuery->Location = System::Drawing::Point(643, 26);
			this->btnFilterQuery->Name = L"btnFilterQuery";
			this->btnFilterQuery->Size = System::Drawing::Size(65, 23);
			this->btnFilterQuery->TabIndex = 55;
			this->btnFilterQuery->Text = L"Query";
			this->btnFilterQuery->UseVisualStyleBackColor = true;
			this->btnFilterQuery->Click += gcnew System::EventHandler(this, &Form1::btnFilterQuery_Click);
			// 
			// btnFilterApply
			// 
			this->btnFilterApply->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnFilterApply->Enabled = false;
			this->btnFilterApply->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnFilterApply->Location = System::Drawing::Point(572, 26);
			this->btnFilterApply->Name = L"btnFilterApply";
			this->btnFilterApply->Size = System::Drawing::Size(65, 23);
			this->btnFilterApply->TabIndex = 44;
			this->btnFilterApply->Text = L"Apply";
			this->btnFilterApply->UseVisualStyleBackColor = true;
			this->btnFilterApply->Click += gcnew System::EventHandler(this, &Form1::btnFilterApply_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox2->Controls->Add(this->nudDelay);
			this->groupBox2->Controls->Add(this->btnParameterGet);
			this->groupBox2->Controls->Add(this->label10);
			this->groupBox2->Controls->Add(this->nudDeviceId);
			this->groupBox2->Controls->Add(this->laDeviceOrDelay);
			this->groupBox2->Controls->Add(this->cbbParameter);
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Controls->Add(this->rdbParamActive);
			this->groupBox2->Controls->Add(this->rdbParamInactive);
			this->groupBox2->Controls->Add(this->btnParameterSet);
			this->groupBox2->Location = System::Drawing::Point(21, 147);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(714, 58);
			this->groupBox2->TabIndex = 54;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L" Configuration Parameters ";
			// 
			// nudDelay
			// 
			this->nudDelay->Location = System::Drawing::Point(408, 29);
			this->nudDelay->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { -1, 0, 0, 0 });
			this->nudDelay->Name = L"nudDelay";
			this->nudDelay->Size = System::Drawing::Size(99, 20);
			this->nudDelay->TabIndex = 55;
			// 
			// btnParameterGet
			// 
			this->btnParameterGet->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnParameterGet->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnParameterGet->Location = System::Drawing::Point(643, 26);
			this->btnParameterGet->Name = L"btnParameterGet";
			this->btnParameterGet->Size = System::Drawing::Size(65, 23);
			this->btnParameterGet->TabIndex = 54;
			this->btnParameterGet->Text = L"Get";
			this->btnParameterGet->UseVisualStyleBackColor = true;
			this->btnParameterGet->Click += gcnew System::EventHandler(this, &Form1::btnParameterGet_Click);
			// 
			// label10
			// 
			this->label10->Location = System::Drawing::Point(241, 11);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(59, 23);
			this->label10->TabIndex = 46;
			this->label10->Text = L"Activation:";
			// 
			// nudDeviceId
			// 
			this->nudDeviceId->Enabled = false;
			this->nudDeviceId->Hexadecimal = true;
			this->nudDeviceId->Location = System::Drawing::Point(408, 29);
			this->nudDeviceId->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { -1, 0, 0, 0 });
			this->nudDeviceId->Name = L"nudDeviceId";
			this->nudDeviceId->Size = System::Drawing::Size(99, 20);
			this->nudDeviceId->TabIndex = 6;
			// 
			// laDeviceOrDelay
			// 
			this->laDeviceOrDelay->Location = System::Drawing::Point(405, 12);
			this->laDeviceOrDelay->Name = L"laDeviceOrDelay";
			this->laDeviceOrDelay->Size = System::Drawing::Size(102, 23);
			this->laDeviceOrDelay->TabIndex = 45;
			this->laDeviceOrDelay->Text = L"Device ID (Hex):";
			// 
			// cbbParameter
			// 
			this->cbbParameter->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbbParameter->FormattingEnabled = true;
			this->cbbParameter->Items->AddRange(gcnew cli::array< System::Object^  >(27) {
				L"Device ID", L"5V Power", L"Auto-reset on BUS-OFF",
					L"CAN Listen-Only", L"Debug\'s Log", L"Receive Status", L"CAN Controller Number", L"Trace File", L"Channel Identification (USB)",
					L"Channel Capabilities", L"Bit rate Adaptation", L"Get Bit rate Nominal Information", L"Get Bit rate FD Information", L"Get Bit rate XL Information",
					L"Get CAN Nominal Speed Bit/s", L"Get CAN FD Speed Bit/s", L"Get CAN XL Speed Bit/s\t", L"Get IP Address", L"Get LAN Service Status",
					L"Reception of Status Frames", L"Reception of RTR Frames", L"Reception of Error Frames", L"Interframe Transmit Delay", L"Reception of Echo Frames",
					L"Hard Reset Status", L"Communication Direction", L"Global Unique Identifier (GUID)"
			});
			this->cbbParameter->Location = System::Drawing::Point(10, 31);
			this->cbbParameter->Name = L"cbbParameter";
			this->cbbParameter->Size = System::Drawing::Size(217, 21);
			this->cbbParameter->TabIndex = 44;
			this->cbbParameter->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::cbbParameter_SelectedIndexChanged);
			// 
			// label6
			// 
			this->label6->Location = System::Drawing::Point(7, 14);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(64, 23);
			this->label6->TabIndex = 43;
			this->label6->Text = L"Parameter:";
			// 
			// rdbParamActive
			// 
			this->rdbParamActive->Checked = true;
			this->rdbParamActive->Location = System::Drawing::Point(238, 32);
			this->rdbParamActive->Name = L"rdbParamActive";
			this->rdbParamActive->Size = System::Drawing::Size(56, 17);
			this->rdbParamActive->TabIndex = 2;
			this->rdbParamActive->TabStop = true;
			this->rdbParamActive->Text = L"Active";
			this->rdbParamActive->UseVisualStyleBackColor = true;
			// 
			// rdbParamInactive
			// 
			this->rdbParamInactive->Location = System::Drawing::Point(300, 32);
			this->rdbParamInactive->Name = L"rdbParamInactive";
			this->rdbParamInactive->Size = System::Drawing::Size(67, 17);
			this->rdbParamInactive->TabIndex = 0;
			this->rdbParamInactive->Text = L"Inactive";
			this->rdbParamInactive->UseVisualStyleBackColor = true;
			// 
			// btnParameterSet
			// 
			this->btnParameterSet->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnParameterSet->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnParameterSet->Location = System::Drawing::Point(572, 26);
			this->btnParameterSet->Name = L"btnParameterSet";
			this->btnParameterSet->Size = System::Drawing::Size(65, 23);
			this->btnParameterSet->TabIndex = 46;
			this->btnParameterSet->Text = L"Set";
			this->btnParameterSet->UseVisualStyleBackColor = true;
			this->btnParameterSet->Click += gcnew System::EventHandler(this, &Form1::btnParameterSet_Click);
			// 
			// btnInfoClear
			// 
			this->btnInfoClear->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnInfoClear->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnInfoClear->Location = System::Drawing::Point(643, 19);
			this->btnInfoClear->Name = L"btnInfoClear";
			this->btnInfoClear->Size = System::Drawing::Size(65, 23);
			this->btnInfoClear->TabIndex = 52;
			this->btnInfoClear->Text = L"Clear";
			this->btnInfoClear->UseVisualStyleBackColor = true;
			this->btnInfoClear->Click += gcnew System::EventHandler(this, &Form1::btnInfoClear_Click);
			// 
			// btnGetVersions
			// 
			this->btnGetVersions->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnGetVersions->Enabled = false;
			this->btnGetVersions->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnGetVersions->Location = System::Drawing::Point(574, 19);
			this->btnGetVersions->Name = L"btnGetVersions";
			this->btnGetVersions->Size = System::Drawing::Size(65, 23);
			this->btnGetVersions->TabIndex = 53;
			this->btnGetVersions->Text = L"Versions";
			this->btnGetVersions->UseVisualStyleBackColor = true;
			this->btnGetVersions->Click += gcnew System::EventHandler(this, &Form1::btnGetVersions_Click);
			// 
			// lbxInfo
			// 
			this->lbxInfo->FormattingEnabled = true;
			this->lbxInfo->Items->AddRange(gcnew cli::array< System::Object^  >(3) {
				L"Select a Hardware and a configuration for it. Then click \"Initialize\" button",
					L"When activated, the Debug-Log file will be found in the same directory as this ap"
					L"plication", L"When activated, the PCAN-Trace file will be found in the same directory as this a"
					L"pplication"
			});
			this->lbxInfo->Location = System::Drawing::Point(10, 19);
			this->lbxInfo->Name = L"lbxInfo";
			this->lbxInfo->ScrollAlwaysVisible = true;
			this->lbxInfo->Size = System::Drawing::Size(558, 56);
			this->lbxInfo->TabIndex = 56;
			this->lbxInfo->DoubleClick += gcnew System::EventHandler(this, &Form1::lbxInfo_DoubleClick);
			// 
			// btnStatus
			// 
			this->btnStatus->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnStatus->Enabled = false;
			this->btnStatus->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnStatus->Location = System::Drawing::Point(572, 48);
			this->btnStatus->Name = L"btnStatus";
			this->btnStatus->Size = System::Drawing::Size(65, 23);
			this->btnStatus->TabIndex = 57;
			this->btnStatus->Text = L"Status";
			this->btnStatus->UseVisualStyleBackColor = true;
			this->btnStatus->Click += gcnew System::EventHandler(this, &Form1::btnStatus_Click);
			// 
			// groupBox4
			// 
			this->groupBox4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox4->Controls->Add(this->btnReset);
			this->groupBox4->Controls->Add(this->btnStatus);
			this->groupBox4->Controls->Add(this->btnGetVersions);
			this->groupBox4->Controls->Add(this->lbxInfo);
			this->groupBox4->Controls->Add(this->btnInfoClear);
			this->groupBox4->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->groupBox4->Location = System::Drawing::Point(21, 513);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(714, 87);
			this->groupBox4->TabIndex = 55;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Information";
			// 
			// btnReset
			// 
			this->btnReset->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnReset->Enabled = false;
			this->btnReset->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnReset->Location = System::Drawing::Point(643, 48);
			this->btnReset->Name = L"btnReset";
			this->btnReset->Size = System::Drawing::Size(65, 23);
			this->btnReset->TabIndex = 58;
			this->btnReset->Text = L"Reset";
			this->btnReset->UseVisualStyleBackColor = true;
			this->btnReset->Click += gcnew System::EventHandler(this, &Form1::btnReset_Click);
			// 
			// laLength
			// 
			this->laLength->AutoSize = true;
			this->laLength->Location = System::Drawing::Point(146, 29);
			this->laLength->Name = L"laLength";
			this->laLength->Size = System::Drawing::Size(65, 13);
			this->laLength->TabIndex = 121;
			this->laLength->Text = L"Length: 9 B.";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(757, 605);
			this->Controls->Add(this->groupBox5);
			this->Controls->Add(this->groupBox6);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox4);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"PCAN-Basic XL Sample";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudDLC))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudDelay))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudDeviceId))->EndInit();
			this->groupBox4->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

#pragma region Delegates
		/// <summary>
		/// Read-Delegate Handler
		/// </summary>
	private: 
		delegate void ReadDelegateHandler();
#pragma endregion

#pragma region Members
	private:

		enum class CANProtocol
		{
			CAN = 0x1,
			CAN_FD = 0x2,
			CAN_XL = 0x4,
		};

		/// <summary>
		/// Saves the handle of a PCAN hardware
		/// </summary>
		TPCANHandle ^m_PcanHandle = gcnew System::UInt16(0);
		/// <summary>
		/// Stores the status of received messages for its display
		/// </summary>
		System::Collections::ArrayList ^m_LastMsgsList;
		/// <summary>
		/// Read Delegate for calling the function "ReadMessages"
		/// </summary>
		ReadDelegateHandler ^m_ReadDelegate;
		/// <summary>
		/// Receive-Event
		/// </summary>
		System::Threading::AutoResetEvent ^m_ReceiveEvent;
		/// <summary>
		/// Thread for message reading (using events)
		/// </summary>
		System::Threading::Thread ^m_ReadThread;
		/// <summary>
		/// Data for message writing. Can be edited in <see cref="EditDataWindow"/>
		/// </summary>
		array<unsigned char>^ m_data;
		/// <summary>
		/// Indicates whether the message-reading thread is running or not
		/// </summary>
		bool m_readThreadRunning;
#pragma endregion

#pragma region Methods

#pragma region Help functions
	/// <summary>
	/// Updates the control showing the CAN XL data in the UI
	/// </summary>
	private: void InitializeData()
	{
		String^ strData = "";

		for (int i = 0; i < static_cast<int>(nudDLC->Value) + 1; i++)
			strData = strData + String::Format("{0:X2} ", m_data[i]);

		txtData->Text = strData;
	}

	/// <summary>
	/// Creates random data bytes for a CAN XL message to be sent
	/// </summary>
	private: void RandomizeData()
	{
		Random^ rand = gcnew Random();
		array<unsigned char>^ data = gcnew array<unsigned char>(static_cast<int>(nudDLC->Value) + 1);
		rand->NextBytes(data);
		Array::Copy(data, m_data, data->Length);
	}

	/// <summary>
	/// Resizes the whole <see cref="m_data"/> array to the size dictated by <see cref="nudDLC"/>
	/// </summary>
	private: void ResizeData() {
		Array::Clear(m_data, (static_cast<int>(nudDLC->Value) + 1), m_data->Length - (static_cast<int>(nudDLC->Value) + 1));
	}

	/// <summary>
	/// Initialization of PCAN-Basic components
	/// </summary>
	private: void InitializeBasicComponents()
	{
		// Creates the list for received messages
		//
		m_LastMsgsList = gcnew System::Collections::ArrayList();

		// Creates the delegate used for message reading
		//
		m_ReadDelegate = gcnew ReadDelegateHandler(this, &Form1::ReadMessages);

		// Creates the event used for signalize incoming messages 
		//
		m_ReceiveEvent = gcnew System::Threading::AutoResetEvent(false);

		// Create a buffer of 2048 bytes for storing random CAN data for message writing
		//
		m_data = gcnew array<unsigned char>(2048);

		// Show the CAN data to send
		//
		FillDataSets();

		// Prepares the PCAN-Basic's debug-Log file
		//
		FillComboBoxData();

		// Prepares the PCAN-Basic's debug-Log file
		//
		ConfigureLogFile();
	}

    /// <summary>
    /// Gets the formatted text for a CPAN-Basic channel handle
    /// </summary>
    /// <param name="handle">PCAN-Basic Handle to format</param>
    /// <param name="protocol">The protocol of the channel</param>
    /// <returns>The formatted text for a channel</returns>
	 private: String ^FormatChannelName(TPCANHandle ^handle, CANProtocol protocol)
	 {
		TPCANDevice ^devDevice;
		Byte byChannel;
		
		// Gets the owner device and channel for a 
		// PCAN-Basic handle
		//
		if(safe_cast<UInt16>(handle) < 0x100)
		{
			devDevice = safe_cast<TPCANDevice>(safe_cast<UInt16>(handle) >> 4);
			byChannel = safe_cast<Byte>(safe_cast<UInt16>(handle) & 0xF);
		}
		else
		{		
			devDevice = safe_cast<TPCANDevice>(safe_cast<UInt16>(handle) >> 8);
			byChannel = safe_cast<Byte>(safe_cast<UInt16>(handle) & 0xFF);
		}

		// Constructs the PCAN-Basic Channel name and return it
		//
		String^ stringFormat = "{0}:{1} {2} ({3:X2}h)";
		bool bIsFD = (protocol & CANProtocol::CAN_FD) == CANProtocol::CAN_FD;
		bool bIsXL = (protocol & CANProtocol::CAN_XL) == CANProtocol::CAN_XL;

		if (bIsXL && bIsFD)
			return String::Format(stringFormat, devDevice, "FD/XL", byChannel, handle);
		else if (bIsFD)
			return String::Format(stringFormat, devDevice, "FD", byChannel, handle);
		else if (bIsXL)
			return String::Format(stringFormat, devDevice, "XL", byChannel, handle);
		else
			return String::Format(stringFormat, devDevice, String::Empty, byChannel, handle);
	 }
	
	/// <summary>
	/// Configures the data of all ComboBox components of the main-form
	/// </summary>
	private: void FillComboBoxData()
	{
		// Channels will be checked
		//
		DoRefresh();

		// XL Bitrate: 
		//      Arbitration: 500 kbit/s 
		//      FD: 2 Mbit/s (FD data)
		//      XL: 8 Mbit/s (XL data)
		//
        txtBitrate->Text = "f_clock_mhz=160,brp=1,nom_tseg1=255,nom_tseg2=64,nom_sjw=64,fd_tseg1=63,fd_tseg2=16,fd_sjw=16,fd_ssp_offset=0,xl_tseg1=10,xl_tseg2=9,xl_sjw=9,xl_ssp_offset=10,xl_error_signaling=1,xl_transceiver_mode_switch=0";

		// Parameters for GetValue and SetValue function calls
		//
		cbbParameter->SelectedIndex = 0;
	}

	/// <summary>
	/// Help Function used to get an error as text
	/// </summary>
	/// <param name="error">Error code to be translated</param>
	/// <returns>A text with the translated error</returns>
	private:String ^GetFormattedError(TPCANStatus ^error)
	{
		StringBuilder ^strTemp;

		// Creates a buffer big enough for a error-text
		//
		strTemp = gcnew StringBuilder(256);
		// Gets the text using the GetErrorText API function
		// If the function success, the translated error is returned. If it fails,
		// a text describing the current error is returned.
		//
		if (PCANBasic::GetErrorText(*error, 0, strTemp) != TPCANStatus::PCAN_ERROR_OK)
			return String::Format("An error occurred. Error-code's text ({0:X}) couldn't be retrieved", error);
		else
			return strTemp->ToString();
	}

	/// <summary>
	/// Sets and shows the CAN data to send
	/// </summary>
	private: void FillDataSets()
	{
		ResizeData();
		InitializeData();
	}

	/// <summary>
	/// Configure LogFile the Debug-Log file of PCAN-Basic
	/// </summary>
	/// <param name="bConnected">Current status. True if connected, false otherwise</param>
	private: void ConfigureLogFile()
	{
		UInt32 iBuffer;

		// Sets the mask to catch all events
		//
		iBuffer = PCANBasic::LOG_FUNCTION_ALL;

		// Configures the log file. 
		// NOTE: The Log capability is to be used with the NONEBUS Handle. Other handle than this will 
		// cause the function fail.
		//
		PCANBasic::SetValue(PCANBasic::PCAN_NONEBUS, TPCANParameter::PCAN_LOG_CONFIGURE, iBuffer, sizeof(UInt32));
	}

	/// <summary>
	/// Configures the PCAN-Trace file for a PCAN-Basic Channel
	/// </summary>
	private: void ConfigureTraceFile()
	{
		UInt32 iBuffer;
		TPCANStatus ^stsResult;

		// Configure the maximum size of a trace file to 5 megabytes
        //
		iBuffer = 5;
		stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_TRACE_SIZE, iBuffer, sizeof(UInt32));
		if (*stsResult != TPCANStatus::PCAN_ERROR_OK)
			IncludeTextMessage(GetFormattedError(stsResult));

        // Configure the way how trace files are created: 
        // * Standard name is used
        // * Existing file is overwritten, 
        // * Only one file is created.
        // * Recording stops when the file size reaches 5 megabytes.
        //
		iBuffer = PCANBasic::TRACE_FILE_SINGLE | PCANBasic::TRACE_FILE_OVERWRITE;
		stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_TRACE_CONFIGURE, iBuffer, sizeof(UInt32));
		if (*stsResult != TPCANStatus::PCAN_ERROR_OK)
			IncludeTextMessage(GetFormattedError(stsResult));
	}

	/// <summary>
	/// Activates/deactivates the different controls of the main-form according
	/// with the current connection status
	/// </summary>
	/// <param name="bConnected">Current status. True if connected, false otherwise</param>
	private: void SetConnectionStatus(bool bConnected)
	{
		// Buttons
		//
		btnInit->Enabled = !bConnected;
		btnRead->Enabled = bConnected && rdbManual->Checked;
		btnWrite->Enabled = bConnected;
		btnRelease->Enabled = bConnected;
		btnFilterApply->Enabled = bConnected;
		btnFilterQuery->Enabled = bConnected;
		btnGetVersions->Enabled = bConnected;
		btnStatus->Enabled = bConnected;
		btnReset->Enabled = bConnected;

		// ComboBoxs
		//
		cbbChannel->Enabled = !bConnected;

		// Hardware configuration and read mode
		//
		if (!bConnected)
			cbbChannel_SelectedIndexChanged(this, gcnew EventArgs());
		else
			rdbTimer_CheckedChanged(this, gcnew EventArgs());

        // Display messages in grid
        //
        tmrDisplay->Enabled = bConnected;
	}

	/// <summary>
	/// Gets the current status of the PCAN-Basic message filter
	/// </summary>
	/// <param name="status">Buffer to retrieve the filter status</param>
	/// <returns>If calling the function was successful or not</returns>
	private: bool GetFilterStatus([System::Runtime::InteropServices::Out] UInt32 %status)
	 {
		 TPCANStatus ^stsResult;

		 // Tries to get the status of the filter for the current connected hardware
		 //
		 stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_MESSAGE_FILTER, status, sizeof(UInt32));

		 // If it fails, a error message is shown
		 //
		 if (*stsResult != TPCANStatus::PCAN_ERROR_OK)
		 {
			 MessageBox::Show(GetFormattedError(stsResult));
			 return false;
		 }
		 return true;
	 }

	/// <summary>
	/// Includes a new line of text into the information Listview
	/// </summary>
	/// <param name="strMsg">Text to be included</param>
	private: void IncludeTextMessage(String ^strMsg)
	 {
		 lbxInfo->Items->Add(strMsg);
		 lbxInfo->SelectedIndex = lbxInfo->Items->Count - 1;
	 }
#pragma endregion

#pragma region Message-proccessing functions

	/// <summary>
	/// Thread-Function used for reading PCAN-Basic messages
	/// </summary>
	private:void CANReadThreadFunc()
	{
		UInt32 iBuffer;
		TPCANStatus ^stsResult;

		iBuffer = Convert::ToUInt32(m_ReceiveEvent->SafeWaitHandle->DangerousGetHandle().ToInt32());
		// Sets the handle of the Receive-Event.
		//
		stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_RECEIVE_EVENT,iBuffer, sizeof(UInt32));
		
		if(*stsResult != TPCANStatus::PCAN_ERROR_OK)
		{
			MessageBox::Show(GetFormattedError(stsResult),"Error!",MessageBoxButtons::OK,MessageBoxIcon::Error);
			return;
		}

		// While this mode is selected
		while (m_readThreadRunning)
		{
			// Waiting for Receive-Event
			// 
			if(m_ReceiveEvent->WaitOne(50))
				// Process Receive-Event using .NET BeginInvoke function
				// in order to interact with Winforms UI (calling the 
				// function ReadMessages)
				// Warning: Do not use Invoke when calling Join with the main thread, since this could lead to a deadlock.
				// 
				this->BeginInvoke(m_ReadDelegate);
		}

		// Resets the Event-handle configuration
		//
		iBuffer = 0;
		PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_RECEIVE_EVENT,iBuffer, sizeof(UInt32));
	}

	/// <summary>
	/// Function for reading CAN XL messages on CAN XL devices
	/// </summary>
	/// <returns>A TPCANStatus error code</returns>
	private: TPCANStatus ^ReadMessageXL()
	{
		TPCANMsgXL ^CANMsg;
		TPCANTimestampXL ^CANTimeStamp;
		TPCANStatus ^stsResult;

        // We execute the "Read" function of the PCANBasic                
        //
		CANMsg = gcnew TPCANMsgXL();
		CANTimeStamp = gcnew TPCANTimestampXL;
        stsResult = PCANBasic::ReadXL(*m_PcanHandle, *CANMsg, *CANTimeStamp);
		if (*stsResult != TPCANStatus::PCAN_ERROR_QRCVEMPTY)
            // We process the received message
            //
            ProcessMessage(CANMsg, CANTimeStamp);

        return stsResult;
	}

	/// <summary>
	/// Function for writing CAN XL messages on CAN XL devices
	/// </summary>
	/// <returns></returns>
	private: TPCANStatus ^WriteFrameXL()
	{
		TPCANMsgXL CANMsg;

		// We create a TPCANMsgFD message structure 
		//
		CANMsg = TPCANMsgXL();
		CANMsg.DATA = gcnew array<unsigned char>(2048);

		// We configurate the message.  
		//
		CANMsg.MSGTYPE = TPCANMessageTypeXL::PCAN_MESSAGE_XL;
		CANMsg.PID = Convert::ToUInt16(txtPID->Text, 16);
		CANMsg.DLC = Convert::ToUInt16(nudDLC->Value);
		CANMsg.SDT = Convert::ToByte(txtSDT->Text, 16);
		CANMsg.VCID = Convert::ToByte(txtVCID->Text, 16);
		CANMsg.AF = Convert::ToUInt32(txtAF->Text, 16);
		CANMsg.RRS = chbRRS->Checked;
		CANMsg.SEC = chbSEC->Checked;
		Array::Copy(m_data, CANMsg.DATA, static_cast<int>(nudDLC->Value) + 1);

		// The message is sent to the configured hardware
		//
		return PCANBasic::WriteXL(*m_PcanHandle, CANMsg);

	}

	/// <summary>
	/// Function for reading PCAN-Basic messages
	/// </summary>
	private: void ReadMessages()
	{
		TPCANStatus ^stsResult;

		// We read at least one time the queue looking for messages.
		// If a message is found, we look again trying to find more.
		// If the queue is empty or an error occur, we get out from
		// the do-while statement.
		//			
		do
		{
			// We execute the "Read" function of the PCANBasic                
			//
			stsResult = ReadMessageXL();
			if (*stsResult == TPCANStatus::PCAN_ERROR_ILLOPERATION)
				break;
		} while (btnRelease->Enabled && (!Convert::ToBoolean(*stsResult & TPCANStatus::PCAN_ERROR_QRCVEMPTY)));
	}

    /// <summary>
    /// Processes a received message, in order to show it in the Message-ListView
    /// </summary>
    /// <param name="theMsg">The received PCAN-Basic message</param>
    /// <returns>True if the message must be created, false if it must be modified</returns>
	private: void ProcessMessage(TPCANMsgXL ^theMsg, TPCANTimestampXL ^itsTimeStamp)
    {
		try
		{
	        // We search if a message is 
			// already received or if this is a new message
			//
			Monitor::Enter(m_LastMsgsList->SyncRoot);
            for each (MessageStatus^ msg in m_LastMsgsList)
            {
                if ((msg->CANMsg.PID == theMsg->PID) && (msg->CANMsg.MSGTYPE == theMsg->MSGTYPE) &&
					(msg->CANMsg.SDT == theMsg->SDT) && (msg->CANMsg.VCID == theMsg->VCID) &&
					(msg->CANMsg.AF == theMsg->AF) && (msg->CANMsg.RRS == theMsg->RRS) &&
					(msg->CANMsg.SEC == theMsg->SEC))
                {
                    // Modify the message and exit
                    //
                    msg->Update(theMsg, *itsTimeStamp);
                    return;
                }
            }
			// Message not found. It will created
			//
			InsertMsgEntry(theMsg, itsTimeStamp);
		}
		finally
		{
			Monitor::Exit(m_LastMsgsList->SyncRoot);
		}	
    }

	/// <summary>
	/// Inserts a new entry for a new message in the Message-ListView
	/// </summary>
	/// <param name="newMsg">The message to be inserted</param>
	/// <param name="timeStamp">The Timestamp of the new message</param>
	private: void InsertMsgEntry(TPCANMsgXL ^newMsg, TPCANTimestampFD ^timeStamp)
	{
		ListViewItem ^lviCurrentItem;
		MessageStatus ^msgStsCurrentMsg;

		try
		{
			Monitor::Enter(m_LastMsgsList->SyncRoot);

			// Create a new message status
			msgStsCurrentMsg = gcnew MessageStatus(*newMsg, *timeStamp, lstMessages->Items->Count);
			msgStsCurrentMsg->ShowingPeriod = chbShowPeriod->Checked;

			// Add to the last message list
			m_LastMsgsList->Add(msgStsCurrentMsg);

			// Add new ListViewItem with type
			lviCurrentItem = lstMessages->Items->Add(msgStsCurrentMsg->TypeString);

			// Add PID
			lviCurrentItem->SubItems->Add(msgStsCurrentMsg->PidString);
			// Length from DLC
			lviCurrentItem->SubItems->Add(GetLengthFromDLC(newMsg->DLC, newMsg->MSGTYPE).ToString());
			// SDT
			lviCurrentItem->SubItems->Add(msgStsCurrentMsg->SdtString);
			// VCID
			lviCurrentItem->SubItems->Add(msgStsCurrentMsg->VcidString);
			// AF
			lviCurrentItem->SubItems->Add(msgStsCurrentMsg->AfString);
			// Timestamp
			lviCurrentItem->SubItems->Add(msgStsCurrentMsg->TimeString);
			// Count
			lviCurrentItem->SubItems->Add(msgStsCurrentMsg->Count.ToString());
			// Data
			lviCurrentItem->SubItems->Add(msgStsCurrentMsg->DataString);
        }
		finally
		{
			Monitor::Exit(m_LastMsgsList->SyncRoot);
		}
	}


	/// <summary>
	/// Display CAN messages in the Message-ListView
	/// </summary>
	private: void DisplayMessages()
	{
		ListViewItem ^lviCurrentItem;

		try
		{
			Monitor::Enter(m_LastMsgsList->SyncRoot);
			
            for each (MessageStatus^ msgStatus in m_LastMsgsList)
            {
                // Get the data to actualize
                //
				if (msgStatus->MarkedAsUpdated)
				{
					msgStatus->MarkedAsUpdated = false;
					lviCurrentItem = lstMessages->Items[msgStatus->Position];

					lviCurrentItem->SubItems[2]->Text = GetLengthFromDLC(msgStatus->CANMsg.DLC, msgStatus->CANMsg.MSGTYPE).ToString();
					lviCurrentItem->SubItems[3]->Text = msgStatus->SdtString;
					lviCurrentItem->SubItems[4]->Text = msgStatus->VcidString;
					lviCurrentItem->SubItems[5]->Text = msgStatus->AfString;
					lviCurrentItem->SubItems[6]->Text = msgStatus->TimeString;
					lviCurrentItem->SubItems[7]->Text = msgStatus->Count.ToString();
					lviCurrentItem->SubItems[8]->Text = msgStatus->DataString;
				}
            }	
		}
		finally
		{
			Monitor::Exit(m_LastMsgsList->SyncRoot);
		}
	}
#pragma endregion

#pragma region Event Handlers
	private: System::Void cbbChannel_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		String ^strTemp;

		// Get the handle from the text being shown
		//
		strTemp = cbbChannel->Text;
		if (strTemp == String::Empty)
			return;
		if (strTemp->Contains("refresh"))
			DoRefresh();
		else
		{
			strTemp = strTemp->Substring(strTemp->IndexOf('(') + 1, 3);

			strTemp = strTemp->Replace('h', ' ')->Trim(' ');

			m_PcanHandle = Convert::ToUInt16(strTemp, 16);
		}
	}

	private: void DoRefresh()
	{
		UInt32 iChannelsCount;
		TPCANStatus ^stsResult;

		// Clears the channel comboBox and fill it again with 
		// the detected hardware
		//
		cbbChannel->Items->Clear();
		
		try
		{
            // Checks for available channels
            //
            stsResult = PCANBasic::GetValue(PCANBasic::PCAN_NONEBUS, TPCANParameter::PCAN_ATTACHED_CHANNELS_COUNT, iChannelsCount, sizeof(UInt32));
			if ((*stsResult) == TPCANStatus::PCAN_ERROR_OK) 
			{
				array<TPCANChannelInformation> ^info = gcnew array<TPCANChannelInformation>(iChannelsCount);

				stsResult = PCANBasic::GetValue(PCANBasic::PCAN_NONEBUS, TPCANParameter::PCAN_ATTACHED_CHANNELS, info);
				if ((*stsResult) == TPCANStatus::PCAN_ERROR_OK) 
					for each (TPCANChannelInformation ^channel in info)
						if ((channel->channel_condition & PCANBasic::PCAN_CHANNEL_AVAILABLE) == PCANBasic::PCAN_CHANNEL_AVAILABLE)
						{
							CANProtocol protocol = CANProtocol::CAN;
							if ((channel->device_features & PCANBasic::FEATURE_XL_CAPABLE) == PCANBasic::FEATURE_XL_CAPABLE)
								protocol = protocol | CANProtocol::CAN_XL;
							if ((channel->device_features & PCANBasic::FEATURE_FD_CAPABLE) == PCANBasic::FEATURE_FD_CAPABLE)
								protocol = protocol | CANProtocol::CAN_FD;

							// Checks whether the current channel is available and if it supports the XL protocol
							if ((channel->channel_condition & PCANBasic::PCAN_CHANNEL_AVAILABLE) == PCANBasic::PCAN_CHANNEL_AVAILABLE &&
								(protocol & CANProtocol::CAN_XL) == CANProtocol::CAN_XL)
							{
								cbbChannel->Items->Add(FormatChannelName(channel->channel_handle, protocol));
							}
						}			
			}

			cbbChannel->Items->Add("... refresh ...");
			if (cbbChannel->Items->Count > 1)
				cbbChannel->SelectedIndex = 0;
			btnInit->Enabled = cbbChannel->Items->Count > 0;

			if ((*stsResult) != TPCANStatus::PCAN_ERROR_OK) 
				MessageBox::Show(GetFormattedError(stsResult));
		}
		catch(DllNotFoundException^)
		{
			MessageBox::Show("Unable to find the library: PCANBasic.dll !", "Error!",MessageBoxButtons::OK,MessageBoxIcon::Error);
			Environment::Exit(-1);
		}
	}

	private:System::Void btnInit_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		TPCANStatus ^stsResult;

		// Connects a selected PCAN-Basic channel
		//
		stsResult = PCANBasic::InitializeXL(*m_PcanHandle, txtBitrate->Text);

		if (*stsResult != TPCANStatus::PCAN_ERROR_OK)
			if(*stsResult != TPCANStatus::PCAN_ERROR_CAUTION)
				MessageBox::Show(GetFormattedError(stsResult));
			else
			{
				IncludeTextMessage("******************************************************");
                IncludeTextMessage("The bitrate being used is different than the given one");
                IncludeTextMessage("******************************************************");
				stsResult = TPCANStatus::PCAN_ERROR_OK;
			}
		else
            // Prepares the PCAN-Basic's PCAN-Trace file
            //
			ConfigureTraceFile();

		// Sets the connection status of the main-form
		//
		SetConnectionStatus(*stsResult == TPCANStatus::PCAN_ERROR_OK);

	}

	private:System::Void rdbTimer_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{				
		// CheckedChanged event is fired for each Radiobutton of one group
		// So, we must only take in account the selected button
		RadioButton^ radioButton = dynamic_cast<RadioButton ^>(sender);
		if(radioButton && !radioButton->Checked)
			return;
				
		if (!btnRelease->Enabled)
			return;

		// According with the kind of reading, a timer, a thread or a button will be enabled
		//
		if (rdbTimer->Checked)
		{
			// Stops Read Thread if it exists
			//
			if (m_ReadThread != nullptr)
			{
				m_readThreadRunning = false;
				m_ReadThread->Join();
				m_ReadThread = nullptr;
			}

			// Enable Timer
			//
			tmrRead->Enabled = btnRelease->Enabled;
		}
		
		if (rdbEvent->Checked)
		{
			// Disable Timer
			//
			tmrRead->Enabled = false;
			// Create and start the tread to read CAN Message using SetRcvEvent()
			//
			System::Threading::ThreadStart ^threadDelegate = gcnew System::Threading::ThreadStart(this, &Form1::CANReadThreadFunc);
			m_ReadThread = gcnew System::Threading::Thread(threadDelegate);
			m_ReadThread->IsBackground = true;
			m_readThreadRunning = true;
			m_ReadThread->Start();
		}
		
		if (rdbManual->Checked)
		{
			// Stops Read Thread if it exists
			//
			if (m_ReadThread != nullptr)
			{
				m_readThreadRunning = false;
				m_ReadThread->Join();
				m_ReadThread = nullptr;
			}
			// Disable Timer
			//
			tmrRead->Enabled = false;
		}
		btnRead->Enabled = btnRelease->Enabled && rdbManual->Checked;
	}

	private: void nudDLC_ValueChanged(System::Object^ sender, System::EventArgs^ e)
	{
		FillDataSets();
		laLength->Text = String::Format("Length: {0} B.", static_cast<int>(nudDLC->Value) + 1);
	}

	private: System::Void btnRelease_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		//Stop Timer
		tmrRead->Enabled = false;

		//Stop Thread
		if (m_ReadThread != nullptr)
		{
			m_readThreadRunning = false;
			m_ReadThread->Join();
			m_ReadThread = nullptr;
		}

		// Releases a current connected PCAN-Basic channel
		//
		PCANBasic::Uninitialize(*m_PcanHandle);

		// Sets the connection status of the main-form
		//
		SetConnectionStatus(false);
	}

	private: System::Void btnFilterApply_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		UInt32 iBuffer;
		TPCANStatus ^stsResult;

		// Gets the current status of the message filter
		//
		if (!GetFilterStatus(iBuffer))
			return;

		 // The filter will be full opened or complete closed
		 //
		 if (rdbFilterClose->Checked)
			 iBuffer = PCANBasic::PCAN_FILTER_CLOSE;
		 else
			 iBuffer = PCANBasic::PCAN_FILTER_OPEN;

		 // The filter is configured
		 //
		 stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_MESSAGE_FILTER, iBuffer, sizeof(UInt32));

		 // If success, an information message is written, if it is not, an error message is shown
		 //
		 if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
			 IncludeTextMessage(String::Format("The filter was successfully {0}", rdbFilterClose->Checked ? "closed." : "opened."));
		 else
			 MessageBox::Show(GetFormattedError(stsResult));
	}

	private: System::Void btnRead_Click(System::Object^  sender, System::EventArgs^  e) 
	{
        TPCANStatus ^stsResult;

        // We execute the "Read" function of the PCANBasic                
        //
        stsResult = ReadMessageXL();
		if (*stsResult != TPCANStatus::PCAN_ERROR_OK)
            // If an error occurred, an information message is included
            //
            IncludeTextMessage(GetFormattedError(stsResult));
	}

	private: System::Void btnMsgClear_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		// The information contained in the messages List-View
		// is cleared
		//
		Monitor::Enter(m_LastMsgsList->SyncRoot);
		try
		{
			lstMessages->Items->Clear();
			m_LastMsgsList->Clear();
		}
		finally
		{
			Monitor::Exit(m_LastMsgsList->SyncRoot);
		}
	}

	private: System::Void txtAF_Leave(System::Object^ sender, System::EventArgs^ e)
	{
		int iTextLength = 8;

		// The Textbox for the AF field is represented with 8 characters
		// (eight bytes in length). Therefore if the Length of the text is smaller than TextLength,  
		// we add "0"
		//
		while (txtAF->Text->Length != iTextLength)
			txtAF->Text = ("0" + txtAF->Text);
	}

	private: System::Void txtSDT_Leave(System::Object^ sender, System::EventArgs^ e)
	{
		int iTextLength = 2;

		// The Textbox for the SDT are represented with 2 characters
		// (one byte in length). Therefore if the Length of the text is smaller than TextLength,
		// we add "0"
		//
		while (txtSDT->Text->Length != iTextLength)
			txtSDT->Text = ("0" + txtSDT->Text);
	}

	private: System::Void txtVCID_Leave(System::Object^ sender, System::EventArgs^ e) {
		int iTextLength = 2;

		// The Textbox for the VCID are represented with 2 characters
		// (one byte in length). Therefore if the Length of the text is smaller than TextLength,  
		// we add "0"
		//
		while (txtVCID->Text->Length != iTextLength)
			txtVCID->Text = ("0" + txtVCID->Text);
	}

	private: System::Void txtPID_Leave(System::Object^ sender, System::EventArgs^ e)
	{
		int iTextLength = 3;
		UInt32 uiMaxValue = safe_cast<UInt32>(0x7FF);

		// The Textbox for the PID is represented with 3 characters.
		// Therefore if the Length of the text is smaller than TextLength,  
		// we add "0"
		//
		while (txtPID->Text->Length != iTextLength)
			txtPID->Text = ("0" + txtPID->Text);

		// We check that the PID is not bigger than current maximum value
		//
		if (Convert::ToUInt32(txtPID->Text, 16) > uiMaxValue)
			txtPID->Text = String::Format("{0:X" + iTextLength.ToString() + "}", uiMaxValue);
	}

	private: System::Void txtPID_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
	{
		Char chCheck;

		// We convert the Character to its Upper case equivalent
		//
		chCheck = Char::ToUpper(e->KeyChar);

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
		e->Handled = true;
	}

	private: System::Void btnEdit_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		EditDataForm^ editDataForm = gcnew EditDataForm(m_data, nudDLC->Value);
		editDataForm->Owner = this;
		
		System::Windows::Forms::DialogResult result = editDataForm->ShowDialog();

		if (result == System::Windows::Forms::DialogResult::OK)
		{
			m_data = editDataForm->Result;
			InitializeData();
		}
	}

	private: System::Void btnGenerate_Click(System::Object^ sender, System::EventArgs^ e) {
		RandomizeData();
		InitializeData();
	}

	private: System::Void btnWrite_Click(System::Object^  sender, System::EventArgs^  e) 
	{    
        TPCANStatus ^stsResult;

        // Send the message
        //
        stsResult = WriteFrameXL();

        // The message was successfully sent
        //
		if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
            IncludeTextMessage("Message was successfully SENT");
        // An error occurred.  We show the error.
        //			
        else
			MessageBox::Show(GetFormattedError(stsResult));
	}

	private: System::Void btnGetVersions_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		TPCANStatus ^stsResult;
		StringBuilder ^strTemp;
		array<String^> ^strArrayVersion;

		strTemp = gcnew StringBuilder(256);

		// We get the version of the PCAN-Basic API
		//
		stsResult = PCANBasic::GetValue(PCANBasic::PCAN_NONEBUS, TPCANParameter::PCAN_API_VERSION, strTemp, 256);
		if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
		{
			IncludeTextMessage("API Version: " + strTemp->ToString());

			// We get the version of the firmware on the device
			//
			stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_FIRMWARE_VERSION, strTemp, 256);
			if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
				IncludeTextMessage("Firmware Version: " + strTemp->ToString());

			// We get the driver version of the channel being used
			//
			stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_CHANNEL_VERSION, strTemp, 256);
			if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
			{
				// Because this information contains line control characters (several lines)
				// we split this also in several entries in the information List-Box
				//
				strArrayVersion = strTemp->ToString()->Split(gcnew array<Char> { '\n' });
				IncludeTextMessage("Channel/Driver Version: ");
				for(int i =0; i < strArrayVersion->Length; i++)
					IncludeTextMessage("     * " + strArrayVersion[i]);
			}
		}

		// If an error occurred, a message is shown
		//
		if (*stsResult != TPCANStatus::PCAN_ERROR_OK)
			MessageBox::Show(GetFormattedError(stsResult));
	}

	private: System::Void btnInfoClear_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		// The information contained in the information List-Box 
		// is cleared
		//
		lbxInfo->Items->Clear();
	}

	private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
	{
		// Releases the used PCAN-Basic channel
		//
		if(btnRelease->Enabled)
			btnRelease_Click(nullptr , nullptr);
	}

	private: System::Void tmrRead_Tick(System::Object^  sender, System::EventArgs^  e) 
	{
		// Checks if in the receive-queue are currently messages for read
		// 
		ReadMessages();
	}

	private: System::Void tmrDisplay_Tick(System::Object^  sender, System::EventArgs^  e) 
	{
		DisplayMessages();
	}

	private: System::Void chbShowPeriod_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		// According with the check-value of this checkbox,
		// the received time of a messages will be interpreted as 
		// period (time between the two last messages) or as time-stamp
		// (the elapsed time since windows was started)
		//
		Monitor::Enter(m_LastMsgsList->SyncRoot);
		try
		{
			for each (MessageStatus^ msg in m_LastMsgsList)
				msg->ShowingPeriod = chbShowPeriod->Checked;
		}
		finally
		{
			Monitor::Exit(m_LastMsgsList->SyncRoot);
		}
	}

	private: System::Void btnFilterQuery_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		UInt32 iBuffer;

		// Queries the current status of the message filter
		//
		if (GetFilterStatus(iBuffer))
		{
			switch(iBuffer)
			{
				// The filter is closed
				//
				case PCANBasic::PCAN_FILTER_CLOSE:
					IncludeTextMessage("The Status of the filter is: closed.");
					break;
				// The filter is fully opened
				//
				case PCANBasic::PCAN_FILTER_OPEN:
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

	private: System::Void cbbParameter_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		// Activates/deactivates controls according with the selected 
		// PCAN-Basic parameter 
		//
		rdbParamActive->Enabled = ((dynamic_cast<ComboBox^>(sender))->SelectedIndex != 0) && ((dynamic_cast<ComboBox^>(sender))->SelectedIndex != 22);
		rdbParamInactive->Enabled = rdbParamActive->Enabled;
		nudDeviceId->Enabled = !rdbParamActive->Enabled;
		nudDelay->Enabled = !rdbParamActive->Enabled;
		laDeviceOrDelay->Text = (cbbParameter->SelectedIndex == 22) ? "Delay (\u00B5s):" : "Device ID (Hex):";
		nudDelay->Visible = cbbParameter->SelectedIndex == 22;
		nudDeviceId->Visible = !nudDelay->Visible;
	}

	private: System::Void btnParameterSet_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		TPCANStatus ^stsResult;
		UInt32 iBuffer;
		bool bActivate;

		bActivate = rdbParamActive->Checked;

		// Sets a PCAN-Basic parameter value
		//
		switch (cbbParameter->SelectedIndex)
		{
			// The device identifier of a channel will be set
			//
			case 0:
				iBuffer = Convert::ToUInt32(nudDeviceId->Value);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_DEVICE_ID, iBuffer, sizeof(UInt32));
				if(*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage("The desired Device-ID was successfully configured");
				break;
			// The 5 Volt Power feature of a channel will be set
			//
			case 1:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_5VOLTS_POWER, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The USB/PC-Card 5 power was successfully {0}", bActivate ? "activated" : "deactivated"));
				break;
			// The feature for automatic reset on BUS-OFF will be set
			//
			case 2:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_BUSOFF_AUTORESET, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The automatic-reset on BUS-OFF was successfully {0}", bActivate ? "activated" : "deactivated"));
				break;
			// The CAN option "Listen Only" will be set
			//
			case 3:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_LISTEN_ONLY, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The CAN option \"Listen Only\" was successfully {0}", bActivate ? "activated" : "deactivated"));
				break;
			// The feature for logging debug-information will be set
			//
			case 4:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(PCANBasic::PCAN_NONEBUS, TPCANParameter::PCAN_LOG_STATUS, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The feature for logging debug information was successfully {0}", bActivate ? "activated" : "deactivated"));
				break;
            // The channel option "Receive Status" will be set
            //			
			case 5:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_RECEIVE_STATUS, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The channel option \"Receive Status\" was set to {0}", bActivate ? "ON" : "OFF"));
				break;
            // The feature for tracing will be set
            //			
			case 7:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_TRACE_STATUS, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The feature for tracing data was successfully {0}", bActivate ? "activated" : "deactivated"));
				break;
            // The feature for identifying an USB Channel will be set
            //			
			case 8:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_CHANNEL_IDENTIFYING, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The procedure for channel identification was successfully {0}", bActivate ? "activated" : "deactivated"));
				break;
            // The feature for using an already configured speed will be set
            //
            case 10:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_BITRATE_ADAPTING, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The feature for bit rate adaptation was successfully {0}", bActivate ? "activated" : "deactivated"));
                break;
			// The option "Allow Status Frames" will be set
            //
			case 19:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_ALLOW_STATUS_FRAMES, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The reception of Status frames was successfully {0}", bActivate ? "enabled" : "disabled"));
				break;
			// The option "Allow RTR Frames" will be set
            //
			case 20:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_ALLOW_RTR_FRAMES, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The reception of RTR frames was successfully {0}", bActivate ? "enabled" : "disabled"));
				break;
			// The option "Allow Error Frames" will be set
            //
			case 21:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_ALLOW_ERROR_FRAMES, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The reception of Error frames was successfully {0}", bActivate ? "enabled" : "disabled"));
				break;
			// The option "Interframes Delay" will be set
			//
			case 22:
				iBuffer = Convert::ToUInt32(nudDelay->Value);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_INTERFRAME_DELAY, iBuffer, sizeof(UInt32));
				if(*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage("The delay between transmitting frames was successfully set");
				break;
                // The option "Allow Echo Frames" will be set
                //
            case 23:
                iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
                stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_ALLOW_ECHO_FRAMES, iBuffer, sizeof(UInt32));
                if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
                    IncludeTextMessage(String::Format("The reception of Echo frames was successfully {0}", bActivate ? "enabled" : "disabled"));
                break;
				// The option "Hard Reset Status" will be set
				//
			case 24:
				iBuffer = safe_cast<UInt32>(bActivate ? PCANBasic::PCAN_PARAMETER_ON : PCANBasic::PCAN_PARAMETER_OFF);
				stsResult = PCANBasic::SetValue(*m_PcanHandle, TPCANParameter::PCAN_HARD_RESET_STATUS, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The activation of a hard reset within the method PCANBasic.Reset was successfully {0}", bActivate ? "enabled" : "disabled"));
				break;
			// The current parameter is invalid
			//
			default:
				*stsResult = TPCANStatus::PCAN_ERROR_UNKNOWN;
				MessageBox::Show("Wrong parameter code.");
				return;
		}

		// If the function fail, an error message is shown
		//
		if(*stsResult != TPCANStatus::PCAN_ERROR_OK)
			MessageBox::Show(GetFormattedError(stsResult));
	}

	private: System::Void btnParameterGet_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		TPCANStatus ^stsResult;
		UInt32 iBuffer;
		StringBuilder ^strBuffer;

		strBuffer = gcnew StringBuilder(255);

		// Gets a PCAN-Basic parameter value
		//
		switch (cbbParameter->SelectedIndex)
		{
			// The device identifier of a channel will be retrieved
			//
			case 0:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_DEVICE_ID, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The configured Device-ID is 0x{0:X}",iBuffer));
				break;
				// The activation status of the 5 Volt Power feature of a channel will be retrieved
				//
			case 1:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_5VOLTS_POWER, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The 5-Volt Power of the USB/PC-Card is {0:X}", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "ON" : "OFF"));
				break;
			// The activation status of the feature for automatic reset on BUS-OFF will be retrieved
			//
			case 2:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_BUSOFF_AUTORESET, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The automatic-reset on BUS-OFF is {0:X}", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "ON" : "OFF"));
					break;
			// The activation status of the CAN option "Listen Only" will be retrieved
			//
			case 3:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_LISTEN_ONLY, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The CAN option \"Listen Only\" is {0:X}", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "ON" : "OFF"));
				break;
			// The activation status for the feature for logging debug-information will be retrieved
			//
			case 4:
				stsResult = PCANBasic::GetValue(PCANBasic::PCAN_NONEBUS, TPCANParameter::PCAN_LOG_STATUS, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The feature for logging debug information is {0:X}", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "ON" : "OFF"));
				break;
            // The activation status of the channel option "Receive Status"  will be retrieved
            //			
			case 5:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_RECEIVE_STATUS, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The channel option \"Receive Status\" is {0}", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "ON" : "OFF"));
				break;
			// The Number of the CAN-Controller used by a PCAN-Channel
			//
			case 6:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_CONTROLLER_NUMBER, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The CAN Controller number is {0}",iBuffer));
				break;
			// The activation status for the feature for tracing data will be retrieved
			//
			case 7:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_TRACE_STATUS, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The feature for tracing data is {0}",(iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "ON" : "OFF"));
				break;
			// The activation status of the channel identifying procedure will be retrieved
			//
			case 8:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_CHANNEL_IDENTIFYING, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The identification procedure of the selected channel is {0}",(iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "ON" : "OFF"));
				break;
			// The activation status of the channel identifying procedure will be retrieved
			//
			case 9:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_CHANNEL_FEATURES, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
				{
					IncludeTextMessage(String::Format("The channel {0} Flexible Data-Rate (CAN-FD)",((iBuffer & PCANBasic::FEATURE_FD_CAPABLE) == PCANBasic::FEATURE_FD_CAPABLE) ? "does support " : "DOESN'T SUPPORT"));
					IncludeTextMessage(String::Format("The channel {0} Extra Long (CAN-XL)", ((iBuffer & PCANBasic::FEATURE_XL_CAPABLE) == PCANBasic::FEATURE_XL_CAPABLE) ? "does support " : "DOESN'T SUPPORT"));
					IncludeTextMessage(String::Format("The channel {0} an inter-frame delay for sending messages",((iBuffer & PCANBasic::FEATURE_DELAY_CAPABLE) == PCANBasic::FEATURE_DELAY_CAPABLE) ? "does support " : "DOESN'T SUPPORT"));
					IncludeTextMessage(String::Format("The channel {0} using I/O pins",((iBuffer & PCANBasic::FEATURE_IO_CAPABLE) == PCANBasic::FEATURE_IO_CAPABLE) ? "does allow " : "DOESN'T ALLOW"));
				}
				break;
            // The status of the speed adapting feature will be retrieved
            //
            case 10:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_BITRATE_ADAPTING, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The feature for bit rate adaptation is {0}", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "ON" : "OFF"));
                break;
			// The bitrate of the connected Nominal channel will be retrieved (String value)
			//
			case 11:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_BITRATE_INFO_CC, strBuffer, 255);
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
				{
					IncludeTextMessage("The bit rate Nominal of the channel is represented by the following values:");
					for each (String ^ strPart in strBuffer->ToString()->Split(','))
						IncludeTextMessage("   * " + strPart);
				}
				break;
            // The bitrate of the connected FD channel will be retrieved (String value)
            //
            case 12:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_BITRATE_INFO_FD, strBuffer, 255);
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
				{
					IncludeTextMessage("The bit rate FD of the channel is represented by the following values:");
					for each (String^ strPart in strBuffer->ToString()->Split(','))
						IncludeTextMessage("   * " + strPart);
				}
                break;
			// The bitrate of the connected XL channel will be retrieved (String value)
			//
			case 13:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_BITRATE_INFO_XL, strBuffer, 255);
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
				{
					IncludeTextMessage("The bit rate XL of the channel is represented by the following values:");
					for each (String ^ strPart in strBuffer->ToString()->Split(','))
						IncludeTextMessage("   * " + strPart);
				}
				break;
			// The nominal speed configured on the CAN bus
			//
			case 14:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_BUSSPEED_NOMINAL, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The nominal speed of the channel is {0} bit/s", iBuffer));
				break;
			// The FD speed configured on the CAN bus
			//
			case 15:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_BUSSPEED_FD, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The FD speed of the channel is {0} bit/s", iBuffer));
				break;
			// The XL speed configured on the CAN bus
			//
			case 16:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_BUSSPEED_XL, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The XL speed of the channel is {0} bit/s", iBuffer));
				break;
			// The IP address of a LAN channel as string, in IPv4 format
			//
			case 17:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_IP_ADDRESS, strBuffer, 255);
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The IP address of the channel is {0}", strBuffer->ToString()));
				break;
			// The running status of the LAN Service
			//
			case 18:
				stsResult = PCANBasic::GetValue(PCANBasic::PCAN_NONEBUS, TPCANParameter::PCAN_LAN_SERVICE_STATUS, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The LAN service is {0}", (iBuffer == PCANBasic::SERVICE_STATUS_RUNNING) ? "running" : "NOT running"));
				break;
			// The reception of Status frames
            //
			case 19:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_ALLOW_STATUS_FRAMES, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The reception of Status frames is {0}", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "enabled" : "disabled"));
				break;
			// The reception of RTR frames
            //
			case 20:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_ALLOW_RTR_FRAMES, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The reception of RTR frames is {0}", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "enabled" : "disabled"));
				break;
			// The reception of Error frames
            //
			case 21:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_ALLOW_ERROR_FRAMES, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The reception of Error frames is {0}", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "enabled" : "disabled"));
				break;
			// The Interframe delay of an USB channel will be retrieved
			//
			case 22:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_INTERFRAME_DELAY, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The configured interframe delay is {0} \u00B5s",iBuffer));
				break;
                // The reception of Echo frames
                //
            case 23:
                stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_ALLOW_ECHO_FRAMES, iBuffer, sizeof(UInt32));
                if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
                    IncludeTextMessage(String::Format("The reception of Echo frames is {0}", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "enabled" : "disabled"));
                break;
				// The activation of Hard Reset
				//
			case 24:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_HARD_RESET_STATUS, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The method PCANBasic.Reset is {0} a hardware reset", (iBuffer == PCANBasic::PCAN_PARAMETER_ON) ? "performing" : "NOT performing"));
				break;
				// The direction of the communication with a LAN channel
				//
			case 25:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_LAN_CHANNEL_DIRECTION, iBuffer, sizeof(UInt32));
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
				{
					switch (iBuffer)
					{
						case PCANBasic::LAN_DIRECTION_READ:
							IncludeTextMessage("The communication flow is: incoming only");
							break;
						case PCANBasic::LAN_DIRECTION_WRITE:
							IncludeTextMessage("The communication flow is: outgoing only");
							break;
						default:
							if(iBuffer == PCANBasic::LAN_DIRECTION_READ_WRITE)
								IncludeTextMessage("The communication flow is: bidirectional");
							else
								IncludeTextMessage(String::Format("The communication flow is: undefined (0x{0:X4})", iBuffer));
							break;
					}
				}
				break;

				// The GUID of the device
				// 
			case 26:
				stsResult = PCANBasic::GetValue(*m_PcanHandle, TPCANParameter::PCAN_DEVICE_GUID, strBuffer, 255);
				if (*stsResult == TPCANStatus::PCAN_ERROR_OK)
					IncludeTextMessage(String::Format("The GUID of the device is {0}", strBuffer->ToString()));
				break;

			// The current parameter is invalid
			//
			default:
				*stsResult = TPCANStatus::PCAN_ERROR_UNKNOWN;
				MessageBox::Show("Wrong parameter code.");
				return;
		}

		// If the function fail, an error message is shown
		//
		if (*stsResult != TPCANStatus::PCAN_ERROR_OK)
			MessageBox::Show(GetFormattedError(stsResult));
	}

	private: System::Void btnReset_Click(System::Object^  sender, System::EventArgs^  e)
	{
		TPCANStatus ^stsResult;

		// Resets the receive and transmit queues of a PCAN Channel.
		//
		stsResult = PCANBasic::Reset(*m_PcanHandle);

		// If it fails, a error message is shown
		//
		if (*stsResult != TPCANStatus::PCAN_ERROR_OK)
			MessageBox::Show(GetFormattedError(stsResult));
		else
			IncludeTextMessage("Receive and transmit queues successfully reset");
	}

	private: System::Void btnStatus_Click(System::Object^ sender, System::EventArgs^ e)
	{
		TPCANStatus stsResult;
		String^ errorName;

		// Gets the current BUS status of a PCAN Channel.
		//
		stsResult = PCANBasic::GetStatus(*m_PcanHandle);

		// Determine the error name from the result
		//
		switch (stsResult)
		{
		case TPCANStatus::PCAN_ERROR_INITIALIZE:
			errorName = "PCAN_ERROR_INITIALIZE";
			break;

		case TPCANStatus::PCAN_ERROR_BUSLIGHT:
			errorName = "PCAN_ERROR_BUSLIGHT";
			break;

		case TPCANStatus::PCAN_ERROR_BUSWARNING:
			errorName = "PCAN_ERROR_BUSWARNING";
			break;

		case TPCANStatus::PCAN_ERROR_BUSPASSIVE:
			errorName = "PCAN_ERROR_BUSPASSIVE";
			break;

		case TPCANStatus::PCAN_ERROR_BUSOFF:
			errorName = "PCAN_ERROR_BUSOFF";
			break;

		case TPCANStatus::PCAN_ERROR_OK:
			errorName = "PCAN_ERROR_OK";
			break;

		default:
			errorName = "See Documentation";
			break;
		}

		// Display the status message
		//
		IncludeTextMessage(String::Format("Status: {0} (0x{1:X}h)", errorName, stsResult));
	}

	private: System::Void lstMessages_DoubleClick(System::Object^  sender, System::EventArgs^  e) 
	{
	    // Clears the content of the Message List-View
        //
        btnMsgClear_Click(this, gcnew EventArgs());
	}

	private: System::Void lbxInfo_DoubleClick(System::Object^ sender, System::EventArgs^ e) {
		// Clears the content of the information List-Box
		//
		btnInfoClear_Click(this, gcnew EventArgs());
	}
#pragma endregion

#pragma endregion
};

}