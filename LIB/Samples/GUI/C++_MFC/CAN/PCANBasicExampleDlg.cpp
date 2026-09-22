// PCANBasicExampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PCANBasicExample.h"
#include "PCANBasicExampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// CriticalSection class
//
#pragma region Critical Section Class
clsCritical::clsCritical(CRITICAL_SECTION *cs, bool createUnlocked, bool lockRecursively)
{
    ASSERT(cs != NULL);

    m_objpCS = cs;
    m_dwLocked = -1;
    m_bDoRecursive = lockRecursively;
    m_dwOwnerThread = GetCurrentThreadId();

    if(!createUnlocked)
        Enter();
}

clsCritical::~clsCritical()
{
    int iFail = (int)0x80000000;

    while(m_dwLocked >= 0)
        if(Leave() == iFail)
            break;
}

int clsCritical::Enter()
{
    if(m_dwOwnerThread != GetCurrentThreadId())
        throw "class clsCritical: Thread cross-over error. ";

    try
    {
        if(m_bDoRecursive || (m_dwLocked == -1))
        {
            EnterCriticalSection(m_objpCS);
            InterlockedIncrement(&m_dwLocked);
        }
        return m_dwLocked;
    }
    catch(...)
    {
        return 0x80000000;
    }
}

int clsCritical::Leave()
{
    if(m_dwOwnerThread != GetCurrentThreadId())
        throw "class clsCritical: Thread cross-over error. ";

    try
    {
        if(m_dwLocked >= 0)
        {
            LeaveCriticalSection(m_objpCS);
            InterlockedDecrement(&m_dwLocked);
            return m_dwLocked;
        }
        return -1;
    }
    catch(...)
    {
        return 0x80000000;
    }
}

bool clsCritical::IsLocked()
{
    return (m_dwLocked > -1);
}

int clsCritical::GetRecursionCount()
{
    return m_dwLocked;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////
// MessageStatus class
//
#pragma region Message Status class
MessageStatus::MessageStatus(TPCANMsg canMsg, TPCANTimestampULong canTimestamp, int listIndex)
{
    m_Msg = canMsg;
    m_TimeStamp = canTimestamp;
    m_oldTimeStamp = canTimestamp;
    m_iIndex = listIndex;
    m_Count = 1;
    m_bShowPeriod = true;
    m_bWasChanged = false;
}

void MessageStatus::Update(TPCANMsg canMsg, TPCANTimestampULong canTimestamp)
{
    m_Msg = canMsg;
    m_oldTimeStamp = m_TimeStamp;
    m_TimeStamp = canTimestamp;
    m_bWasChanged = true;
    m_Count += 1;
}

TPCANMsg MessageStatus::GetCANMsg()
{
    return m_Msg;
}

TPCANTimestampULong MessageStatus::GetTimestamp()
{
    return m_TimeStamp;
}

int MessageStatus::GetPosition()
{
    return m_iIndex;
}

CString MessageStatus::GetTypeString()
{
    CString strTemp;
    bool isEcho = (m_Msg.MSGTYPE & PCAN_MESSAGE_ECHO) == PCAN_MESSAGE_ECHO;

    // Add the new ListView Item with the type of the message
    //
    if ((m_Msg.MSGTYPE & PCAN_MESSAGE_STATUS) != 0)
        return "STATUS";

    if ((m_Msg.MSGTYPE & PCAN_MESSAGE_ERRFRAME) != 0)
        return "ERROR";

    if((m_Msg.MSGTYPE & PCAN_MESSAGE_EXTENDED) != 0)
        strTemp = "EXT";
    else
        strTemp = "STD";

    if((m_Msg.MSGTYPE & PCAN_MESSAGE_RTR) == PCAN_MESSAGE_RTR)        
       strTemp += isEcho ? "/RTR [ ECHO ]" : "/RTR";
    else
        if(m_Msg.MSGTYPE > PCAN_MESSAGE_EXTENDED)
        {
            if (isEcho)
                strTemp.Append(" [ ECHO ]");
        }

    return strTemp;
}

CString MessageStatus::GetIdString()
{
    CString strTemp;

    // We format the ID of the message and show it
    //
    if((m_Msg.MSGTYPE & PCAN_MESSAGE_EXTENDED) != 0)
        strTemp.Format("%08Xh",m_Msg.ID);
    else
        strTemp.Format("%03Xh",m_Msg.ID);

    return strTemp;
}

CString MessageStatus::GetDataString()
{
    CString strTemp, strTemp2;

    strTemp = "";
    strTemp2 = "";

    if((m_Msg.MSGTYPE & PCAN_MESSAGE_RTR) == PCAN_MESSAGE_RTR)
        return "Remote Request";
    else
        for(int i=0; i < m_Msg.LEN; i++)
        {
            strTemp.Format("%s %02X", strTemp2, m_Msg.DATA[i]);
            strTemp2 = strTemp;
        }

    return strTemp2;
}

CString MessageStatus::GetTimeString()
{
    double fTime;
    CString str;

    fTime = (m_TimeStamp / 1000.0);
    if (m_bShowPeriod)
        fTime -= (m_oldTimeStamp / 1000.0);
    str.Format("%.1f", fTime);

    return str;
}

int MessageStatus::GetCount()
{
    return m_Count;
}

bool MessageStatus::GetShowingPeriod()
{
    return m_bShowPeriod;
}

bool MessageStatus::GetMarkedAsUpdated()
{
    return m_bWasChanged;
}

void MessageStatus::SetShowingPeriod(bool value)
{
    if (m_bShowPeriod ^ value)
    {
        m_bShowPeriod = value;
        m_bWasChanged = true;
    }
}

void MessageStatus::SetMarkedAsUpdated(bool value)
{
    m_bWasChanged = value;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////
// PCANBasicExampleDlg dialog
//
CPCANBasicExampleDlg::CPCANBasicExampleDlg(CWnd* pParent)
    : CDialog(CPCANBasicExampleDlg::IDD, pParent)
    , txtID(_T(""))
, txtLength(_T(""))
, txtData0(_T(""))
, txtData1(_T(""))
, txtData2(_T(""))
, txtData3(_T(""))
, txtData4(_T(""))
, txtData5(_T(""))
, txtData6(_T(""))
, txtData7(_T(""))
, chbExtended(FALSE)
, chbRemote(FALSE)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPCANBasicExampleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_CBBCHANNEL, cbbChannel);
    DDX_Control(pDX, IDC_BUTTON_HWREFRESH, btnRefresh);
    DDX_Control(pDX, IDC_CBBBAUDRATES, cbbBaudrates);
    DDX_Control(pDX, IDC_BTNINIT, btnInit);
    DDX_Control(pDX, IDC_BTNRELEASE, btnRelease);

    DDX_Control(pDX, IDC_CHBFILTEREXTENDED, chbFilterExtended);
    DDX_Control(pDX, IDC_RADIOFILTEROPEN, rdbFilterOpen);
    DDX_Control(pDX, IDC_RADIOFILTERCLOSE, rdbFilterClose);
    DDX_Control(pDX, IDC_RADIOFILTERCUSTOM, rdbFilterCustom);
    DDX_Control(pDX, IDC_NUDFILTERFROM, nudFilterFrom);
    DDX_Text(pDX, IDC_TXTFILTERFROM, txtFilterFrom);
    DDX_Control(pDX, IDC_NUDFILTERTO, nudFilterTo);
    DDX_Text(pDX, IDC_TXTFILTERTO, txtFilterTo);
    DDX_Control(pDX, IDC_BUTTONFILTERAPPLY, btnFilterApply);
    DDX_Control(pDX, IDC_BUTTONFILTERQUERY, btnFilterQuery);

    DDX_Control(pDX, IDC_COMBOPARAMETER, cbbParameter);
    DDX_Control(pDX, IDC_RADIOPARAMACTIVE, rdbParameterActive);
    DDX_Control(pDX, IDC_RADIOPARAMINACTIVE, rdbParameterInactive);
    DDX_Control(pDX, IDC_TXTPARAMDEVNUMBER, editParameterDevNumberOrDelay);
    DDX_Text(pDX, IDC_TXTPARAMDEVNUMBER, txtParameterDevNumber);
    DDX_Control(pDX, IDC_NUDPARAMDEVNUMBER, nudParameterDevNumberOrDelay);
    DDX_Control(pDX, IDC_BUTTONPARAMSET, btnParameterSet);
    DDX_Control(pDX, IDC_BUTTONPARAMGET, btnParameterGet);

    DDX_Control(pDX, IDC_RDBTIMER, rdbReadingTimer);
    DDX_Control(pDX, IDC_RADIO_BY_EVENT, rdbReadingEvent);
    DDX_Control(pDX, IDC_RDBMANUAL, rdbReadingManual);
    DDX_Control(pDX, IDC_CHBTIMESTAMP, chbReadingTimeStamp);
    DDX_Control(pDX, IDC_BUTTONREAD, btnRead);
    DDX_Control(pDX, IDC_BUTTONREADINGCLEAR, btnReadingClear);

    DDX_Control(pDX, IDC_LISTINFO, listBoxInfo);
    DDX_Control(pDX, IDC_BUTTONSTATUS, btnStatus);
    DDX_Control(pDX, IDC_BUTTONRESET, btnReset);

    DDX_Text(pDX, IDC_TXTLENGTH, txtLength);
    DDX_Control(pDX, IDC_NUDLENGTH, nudLength);
    DDX_Text(pDX, IDC_TXTID, txtID);
    DDV_MaxChars(pDX, txtID, 8);
    DDX_Text(pDX, IDC_TXTDATA0, txtData0);
    DDV_MaxChars(pDX, txtData0, 2);
    DDX_Text(pDX, IDC_TXTDATA1, txtData1);
    DDV_MaxChars(pDX, txtData1, 2);
    DDX_Text(pDX, IDC_TXTDATA2, txtData2);
    DDV_MaxChars(pDX, txtData2, 2);
    DDX_Text(pDX, IDC_TXTDATA3, txtData3);
    DDV_MaxChars(pDX, txtData3, 2);
    DDX_Text(pDX, IDC_TXTDATA4, txtData4);
    DDV_MaxChars(pDX, txtData4, 2);
    DDX_Text(pDX, IDC_TXTDATA5, txtData5);
    DDV_MaxChars(pDX, txtData5, 2);
    DDX_Text(pDX, IDC_TXTDATA6, txtData6);
    DDV_MaxChars(pDX, txtData6, 2);
    DDX_Text(pDX, IDC_TXTDATA7, txtData7);
    DDV_MaxChars(pDX, txtData7, 2);
    DDX_Check(pDX, IDC_CHBEXTENDED, chbExtended);
    DDX_Check(pDX, IDC_CHBREMOTE, chbRemote);
    DDX_Control(pDX, IDC_LSTMESSAGES, lstMessages);
    DDX_Control(pDX, IDC_BTNWRITE, btnWrite);
    DDX_Control(pDX, IDC_BUTTONVERSION, btnVersions);
    DDX_Control(pDX, IDC_LA_DEVICEORDELAY, labelDeviceOrDelay);
}

BEGIN_MESSAGE_MAP(CPCANBasicExampleDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_CBN_SELCHANGE(IDC_CBBCHANNEL, OnCbnSelchangecbbChannel)
    ON_CBN_SELCHANGE(IDC_CBBBAUDRATES, OnCbnSelchangeCbbbaudrates)
    ON_EN_KILLFOCUS(IDC_TXTID, OnEnKillfocusTxtid)
    ON_EN_KILLFOCUS(IDC_TXTLENGTH, &CPCANBasicExampleDlg::OnEnKillfocusTxtlength)
    ON_EN_KILLFOCUS(IDC_TXTDATA0, OnEnKillfocusTxtdata0)
    ON_EN_KILLFOCUS(IDC_TXTDATA1, OnEnKillfocusTxtdata1)
    ON_EN_KILLFOCUS(IDC_TXTDATA2, OnEnKillfocusTxtdata2)
    ON_EN_KILLFOCUS(IDC_TXTDATA3, OnEnKillfocusTxtdata3)
    ON_EN_KILLFOCUS(IDC_TXTDATA4, OnEnKillfocusTxtdata4)
    ON_EN_KILLFOCUS(IDC_TXTDATA5, OnEnKillfocusTxtdata5)
    ON_EN_KILLFOCUS(IDC_TXTDATA6, OnEnKillfocusTxtdata6)
    ON_EN_KILLFOCUS(IDC_TXTDATA7, OnEnKillfocusTxtdata7)
    ON_NOTIFY(UDN_DELTAPOS, IDC_NUDLENGTH, OnDeltaposNudlength)
    ON_BN_CLICKED(IDC_CHBEXTENDED, OnBnClickedChbextended)
    ON_BN_CLICKED(IDC_CHBREMOTE, OnBnClickedChbremote)
    ON_NOTIFY(NM_DBLCLK, IDC_LSTMESSAGES, OnNMDblclkLstmessages)
    ON_BN_CLICKED(IDC_BTNINIT, OnBnClickedBtninit)
    ON_BN_CLICKED(IDC_BTNRELEASE, OnBnClickedBtnrelease)
    ON_BN_CLICKED(IDC_BTNWRITE, OnBnClickedBtnwrite)
    ON_WM_SHOWWINDOW()
    ON_BN_CLICKED(IDC_RDBTIMER, &CPCANBasicExampleDlg::OnBnClickedRdbtimer)
    ON_BN_CLICKED(IDC_RDBEVENT, &CPCANBasicExampleDlg::OnBnClickedRdbevent)
    ON_BN_CLICKED(IDC_CHBTIMESTAMP, &CPCANBasicExampleDlg::OnBnClickedChbtimestamp)
    ON_BN_CLICKED(IDC_BUTTON_HWREFRESH, &CPCANBasicExampleDlg::OnBnClickedButtonHwrefresh)
    ON_BN_CLICKED(IDC_CHBFILTEREXTENDED, &CPCANBasicExampleDlg::OnBnClickedChbfilterextended)
    ON_NOTIFY(UDN_DELTAPOS, IDC_NUDFILTERFROM, &CPCANBasicExampleDlg::OnDeltaposNudfilterfrom)
    ON_EN_KILLFOCUS(IDC_TXTFILTERFROM, &CPCANBasicExampleDlg::OnEnKillfocusTxtfilterfrom)
    ON_NOTIFY(UDN_DELTAPOS, IDC_NUDFILTERTO, &CPCANBasicExampleDlg::OnDeltaposNudfilterto)
    ON_EN_KILLFOCUS(IDC_TXTFILTERTO, &CPCANBasicExampleDlg::OnEnKillfocusTxtfilterto)
    ON_BN_CLICKED(IDC_BUTTONFILTERAPPLY, &CPCANBasicExampleDlg::OnBnClickedButtonfilterapply)
    ON_BN_CLICKED(IDC_BUTTONFILTERQUERY, &CPCANBasicExampleDlg::OnBnClickedButtonfilterquery)
    ON_BN_CLICKED(IDC_RDBMANUAL, &CPCANBasicExampleDlg::OnBnClickedRdbmanual)
    ON_BN_CLICKED(IDC_BUTTONREAD, &CPCANBasicExampleDlg::OnBnClickedButtonread)
    ON_BN_CLICKED(IDC_BUTTONREADINGCLEAR, &CPCANBasicExampleDlg::OnBnClickedButtonreadingclear)
    ON_CBN_SELCHANGE(IDC_COMBOPARAMETER, &CPCANBasicExampleDlg::OnCbnSelchangeComboparameter)
    ON_NOTIFY(UDN_DELTAPOS, IDC_NUDPARAMDEVNUMBER, &CPCANBasicExampleDlg::OnDeltaposNudparamdevnumber)
    ON_EN_KILLFOCUS(IDC_TXTPARAMDEVNUMBER, &CPCANBasicExampleDlg::OnEnKillfocusTxtparamdevnumber)
    ON_BN_CLICKED(IDC_BUTTONPARAMSET, &CPCANBasicExampleDlg::OnBnClickedButtonparamset)
    ON_BN_CLICKED(IDC_BUTTONPARAMGET, &CPCANBasicExampleDlg::OnBnClickedButtonparamget)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTONVERSION, &CPCANBasicExampleDlg::OnBnClickedButtonversion)
    ON_BN_CLICKED(IDC_BUTTONINFOCLEAR, &CPCANBasicExampleDlg::OnBnClickedButtoninfoclear)
    ON_BN_CLICKED(IDC_BUTTONSTATUS, &CPCANBasicExampleDlg::OnBnClickedButtonstatus)
    ON_BN_CLICKED(IDC_BUTTONRESET, &CPCANBasicExampleDlg::OnBnClickedButtonreset)
    ON_LBN_DBLCLK(IDC_LISTINFO, &CPCANBasicExampleDlg::OnLbnDblclkListinfo)
END_MESSAGE_MAP()

// PCANBasicExampleDlg message handlers
//
BOOL CPCANBasicExampleDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // Extra initialization here
    InitializeControls();

    IncludeTextMessage("Select a Hardware and a configuration for it. Then click \"Initialize\" button");
    IncludeTextMessage("When activated, the Debug-Log file will be found in the same directory as this application");
    IncludeTextMessage("When activated, the PCAN-Trace file will be found in the same directory as this application");

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPCANBasicExampleDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPCANBasicExampleDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CPCANBasicExampleDlg::InitializeControls(void)
{
    // Initialize the Critical Section
    //
    InitializeProtection();

    // List Control
    //
    lstMessages.InsertColumn(MSG_TYPE,"Type",LVCFMT_LEFT,110,1);
    lstMessages.InsertColumn(MSG_ID,"ID",LVCFMT_LEFT,90,2);
    lstMessages.InsertColumn(MSG_LENGTH,"Length",LVCFMT_LEFT,50,3);
    lstMessages.InsertColumn(MSG_COUNT,"Count",LVCFMT_LEFT,49,5);
    lstMessages.InsertColumn(MSG_TIME,"Rcv Time",LVCFMT_LEFT,70,5);
    lstMessages.InsertColumn(MSG_DATA,"Data",LVCFMT_LEFT,170,4);
    lstMessages.SetExtendedStyle(lstMessages.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

    // Initializes Edit Controls
    //
    txtID = "0";
    txtLength = "8";
    txtData0 = "00";
    txtData1 = "00";
    txtData2 = "00";
    txtData3 = "00";
    txtData4 = "00";
    txtData5 = "00";
    txtData6 = "00";
    txtData7 = "00";


    // We set the variable for the current
    // PCAN Basic Class instance to use it
    //
    m_objPCANBasic = new PCANBasicClass();

    // We set the variable to know which reading mode is
    // currently selected (Timer by default)
    //
    m_ActiveReadingMode = 0;

    // Create a list to store the displayed messages
    //
    m_LastMsgsList = new CPtrList();

    // Create Event to use Received-event
    //
    m_hEvent = CreateEvent(NULL, FALSE, FALSE, "");

    // Prepares the PCAN-Basic's debug-Log file
    //
    FillComboBoxData();

    // UpDown Length
    //
    nudLength.SetRange(0,8);
    nudLength.SetPos(8);

    // UpDown Filter From
    //
    nudFilterFrom.SetRange32(0,0x1FFFFFFF);
    nudFilterFrom.SetPos32(0);
    txtFilterFrom = "0";

    // UpDown Filter To
    //
    nudFilterTo.SetRange32(0,0x7FF);
    nudFilterTo.SetPos32(0x7FF);
    txtFilterTo = "7FF";

    // UpDown Device Number
    //
    nudParameterDevNumberOrDelay.SetRange32(0,INT32_MAX);
    nudParameterDevNumberOrDelay.SetPos32(0);
    txtParameterDevNumber = "0";

    // Init CheckBox
    rdbFilterOpen.SetCheck(1);
    rdbReadingTimer.SetCheck(1);
    rdbParameterActive.SetCheck(1);
    chbReadingTimeStamp.SetCheck(1);

    // Set default connection status
    SetConnectionStatus(false);

    // Init log parameters
    ConfigureLogFile();

    // Update UI
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::InitializeProtection()
{
    m_objpCS = new CRITICAL_SECTION();
    InitializeCriticalSection(m_objpCS);
}

void CPCANBasicExampleDlg::FinalizeProtection()
{
    try
    {
        DeleteCriticalSection(m_objpCS);
        delete m_objpCS;
        m_objpCS = NULL;
    }
    catch(...)
    {
        throw;
    }
}

void CPCANBasicExampleDlg::OnTimer(UINT_PTR nIDEvent)
{
    if(nIDEvent == 1)
        // Read Pending Messages
        //
        ReadMessages();
    if(nIDEvent == 2)
        // Display messages
        //
        DisplayMessages();

    CDialog::OnTimer(nIDEvent);
}

void CPCANBasicExampleDlg::OnCbnSelchangecbbChannel()
{
    CString strTemp;

    // Get the handle from the text being shown
    //
    strTemp = GetComboBoxSelectedLabel(&cbbChannel);
    strTemp = strTemp.Mid(strTemp.Find('(') + 1, 3);

    strTemp.Replace('h', ' ');
    strTemp.Trim(' ');

    m_PcanHandle = HexTextToInt(strTemp);
}

void CPCANBasicExampleDlg::OnCbnSelchangeCbbbaudrates()
{
    // We save the corresponding Baudrate enumeration
    // type value for every selected Baudrate from the
    // list.
    //
    switch(cbbBaudrates.GetCurSel())
    {
    case 0:
        m_Baudrate = PCAN_BAUD_1M;
        break;
    case 1:
        m_Baudrate = PCAN_BAUD_800K;
        break;
    case 2:
        m_Baudrate = PCAN_BAUD_500K;
        break;
    case 3:
        m_Baudrate = PCAN_BAUD_250K;
        break;
    case 4:
        m_Baudrate = PCAN_BAUD_125K;
        break;
    case 5:
        m_Baudrate = PCAN_BAUD_100K;
        break;
    case 6:
        m_Baudrate = PCAN_BAUD_95K;
        break;
    case 7:
        m_Baudrate = PCAN_BAUD_83K;
        break;
    case 8:
        m_Baudrate = PCAN_BAUD_50K;
        break;
    case 9:
        m_Baudrate = PCAN_BAUD_47K;
        break;
    case 10:
        m_Baudrate = PCAN_BAUD_33K;
        break;
    case 11:
        m_Baudrate = PCAN_BAUD_20K;
        break;
    case 12:
        m_Baudrate = PCAN_BAUD_10K;
        break;
    case 13:
        m_Baudrate = PCAN_BAUD_5K;
        break;
    default:
        m_Baudrate = (TPCANBaudrate)0;
        break;
    }
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtid()
{
    int iTest;

    // Apply Pending Updates
    //
    UpdateData(TRUE);

    // Format to Upper
    //
    txtID.MakeUpper();

    // Convert string value
    //
    iTest = HexTextToInt(txtID);

    // The Textbox for the ID is represented with 3 characters for
    // Standard and 8 characters for extended messages.
    // Therefore if the Length of the text is smaller than TextLength,
    // we add "0"
    //
    if(chbExtended)
    {
        if(iTest > 0)
            txtID = IntToHex(iTest,8);
        else
            txtID = "00000000";
    }
    else
    {
        if(iTest > 0x7FF)
            txtID = "7FF";
        else
        {
            // We test if the given ID is a valid hexadecimal number.
            //
            iTest = HexTextToInt(txtID);
            if(iTest > 0)
                txtID = IntToHex(iTest,3);
            else
                txtID = "000";
        }
    }

    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtlength()
{
    UpdateData(TRUE);

    // Convert the text length to a whole number
    //
    int txtLengthInt = atoi(txtLength);

    // Set constraints, so the text length cannot exceed the standard CAN range from 0 to 8
    //
    if (txtLengthInt < 0)
    {
        txtLengthInt = 0;
    }
    else if (txtLengthInt > 8)
    {
        txtLengthInt = 8;
    }

    // Display the text length on the label
    CString txtLengthString;
    txtLengthString.Format("%d", txtLengthInt);
    txtLength = txtLengthString;

    // Update the dlc label
    GetDlgItem(IDC_LALENGTH)->SetWindowText(IntToStr(txtLengthInt) + __T(" B."));

    // Enable data fields accordingly
    //
    EnableDisableDataFields(txtLengthInt);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtdata0()
{
    UpdateData(TRUE);
    // Check textBox content
    //
    CheckHexEditBox(&txtData0);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtdata1()
{
    UpdateData(TRUE);
    // Check textBox content
    //
    CheckHexEditBox(&txtData1);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtdata2()
{
    UpdateData(TRUE);
    // Check textBox content
    //
    CheckHexEditBox(&txtData2);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtdata3()
{
    UpdateData(TRUE);
    // Check textBox content
    //
    CheckHexEditBox(&txtData3);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtdata4()
{
    UpdateData(TRUE);
    // Check textBox content
    //
    CheckHexEditBox(&txtData4);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtdata5()
{
    UpdateData(TRUE);
    // Check textBox content
    //
    CheckHexEditBox(&txtData5);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtdata6()
{
    UpdateData(TRUE);
    // Check textBox content
    //
    CheckHexEditBox(&txtData6);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtdata7()
{
    UpdateData(TRUE);
    // Check textBox content
    //
    CheckHexEditBox(&txtData7);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnDeltaposNudlength(NMHDR *pNMHDR, LRESULT *pResult)
{
    int iNewVal;
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    // Compute new selected value
    //
    iNewVal =  pNMUpDown->iPos + ((pNMUpDown->iDelta > 0) ? 1 : -1);
    if(iNewVal > 8){
        iNewVal = 8;
        pNMUpDown->iPos -= 1;
    }
    if(iNewVal < 0)
        iNewVal = 0;

    GetDlgItem(IDC_LALENGTH)->SetWindowText(IntToStr(iNewVal) + __T(" B."));

    *pResult = 0;
    EnableDisableDataFields(chbRemote ? 0 : iNewVal);
}

void CPCANBasicExampleDlg::EnableDisableDataFields(int length)
{
    // Enable or disable control according selected message length
    //
    ((CEdit*)GetDlgItem(IDC_TXTDATA0))->EnableWindow(0 < length);
    ((CEdit*)GetDlgItem(IDC_TXTDATA1))->EnableWindow(1 < length);
    ((CEdit*)GetDlgItem(IDC_TXTDATA2))->EnableWindow(2 < length);
    ((CEdit*)GetDlgItem(IDC_TXTDATA3))->EnableWindow(3 < length);
    ((CEdit*)GetDlgItem(IDC_TXTDATA4))->EnableWindow(4 < length);
    ((CEdit*)GetDlgItem(IDC_TXTDATA5))->EnableWindow(5 < length);
    ((CEdit*)GetDlgItem(IDC_TXTDATA6))->EnableWindow(6 < length);
    ((CEdit*)GetDlgItem(IDC_TXTDATA7))->EnableWindow(7 < length);
}

void CPCANBasicExampleDlg::OnBnClickedChbextended()
{
    // Check Message ID
    //
    OnEnKillfocusTxtid();
}

void CPCANBasicExampleDlg::OnBnClickedChbremote()
{
    UpdateData(TRUE);

    // Show or Hide control according message is a RTR
    //
    EnableDisableDataFields(chbRemote ? 0 : nudLength.GetPos());
}

void CPCANBasicExampleDlg::OnNMDblclkLstmessages(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    OnBnClickedButtonreadingclear();
}

void CPCANBasicExampleDlg::OnBnClickedBtninit()
{
    TPCANStatus stsResult;

    UpdateData(TRUE);

    // Connects a selected PCAN-Basic channel
    //

    stsResult = m_objPCANBasic->Initialize(m_PcanHandle, m_Baudrate);

    if (stsResult != PCAN_ERROR_OK)
        if (stsResult != PCAN_ERROR_CAUTION)
            ::MessageBox(NULL, GetFormattedError(stsResult), "Error!",MB_ICONERROR);
        else
        {
            IncludeTextMessage("******************************************************");
            IncludeTextMessage("The bitrate being used is different than the given one");
            IncludeTextMessage("******************************************************");
            stsResult = PCAN_ERROR_OK;
        }
    else
        // Prepares the PCAN-Basic's PCAN-Trace file
        //
        ConfigureTraceFile();

    // Sets the connection status of the main-form
    //
    SetConnectionStatus(stsResult == PCAN_ERROR_OK);
}

void CPCANBasicExampleDlg::OnBnClickedBtnrelease()
{
    // Terminate Read Thread if it exists
    //
    if(m_hThread != NULL)
    {
        m_Terminated = true;
        WaitForSingleObject(m_hThread,-1);
        m_hThread = NULL;
    }

    // We stop to read from the CAN queue
    //
    SetTimerRead(false);

    // Releases a current connected PCAN-Basic channel
    //
    m_objPCANBasic->Uninitialize(m_PcanHandle);

    // Sets the connection status of the main-form
    //
    SetConnectionStatus(false);
}

TPCANStatus CPCANBasicExampleDlg::WriteFrame()
{
    TPCANMsg CANMsg;

    // We configurate the Message.  The ID (max 0x1FF),
    // Length of the Data, Message Type (Standard in
    // this example) and die data
    //
    CANMsg.ID = HexTextToInt(txtID);
    CANMsg.LEN = (BYTE)nudLength.GetPos32();
    CANMsg.MSGTYPE = (chbExtended) ? PCAN_MESSAGE_EXTENDED : PCAN_MESSAGE_STANDARD;
    // If a remote frame will be sent, the data bytes are not important.
    //
    if (chbRemote)
        CANMsg.MSGTYPE = CANMsg.MSGTYPE | PCAN_MESSAGE_RTR;
    else
    {
        // We get so much data as the Len of the message
        //
        CANMsg.DATA[0] = (BYTE)HexTextToInt(txtData0);
        CANMsg.DATA[1] = (BYTE)(HexTextToInt(txtData1));
        CANMsg.DATA[2] = (BYTE)(HexTextToInt(txtData2));
        CANMsg.DATA[3] = (BYTE)(HexTextToInt(txtData3));
        CANMsg.DATA[4] = (BYTE)(HexTextToInt(txtData4));
        CANMsg.DATA[5] = (BYTE)(HexTextToInt(txtData5));
        CANMsg.DATA[6] = (BYTE)(HexTextToInt(txtData6));
        CANMsg.DATA[7] = (BYTE)(HexTextToInt(txtData7));
    }

    // The message is sent to the configured hardware
    //
    return m_objPCANBasic->Write(m_PcanHandle, &CANMsg);
}

void CPCANBasicExampleDlg::OnBnClickedBtnwrite()
{
    TPCANStatus stsResult;

    // The message is sent
    //
    stsResult = WriteFrame();

    // The Hardware was successfully sent
    //
    if (stsResult == PCAN_ERROR_OK)
        IncludeTextMessage("Message was successfully SENT");
    else
        // An error occurred.  We show the error.
        //
        ::MessageBox(NULL, GetFormattedError(stsResult), "Error!",MB_ICONERROR);
}

void CPCANBasicExampleDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
}

void CPCANBasicExampleDlg::OnBnClickedRdbtimer()
{
    // Check reading mode selection change
    //
    if(rdbReadingTimer.GetCheck() && (m_ActiveReadingMode != 0))
    {
        // Process change
        //
        m_ActiveReadingMode = 0;
        ReadingModeChanged();
    }
}

void CPCANBasicExampleDlg::OnBnClickedRdbevent()
{
    // Check reading mode selection change
    //
    if(rdbReadingEvent.GetCheck() && (m_ActiveReadingMode != 1))
    {
        // Process change
        //
        m_ActiveReadingMode = 1;
        ReadingModeChanged();
    }
}

void CPCANBasicExampleDlg::OnBnClickedChbtimestamp()
{
    MessageStatus* msgStsCurrentMessage;
    CString str;
    POSITION pos;
    BOOL bChecked;

    // According with the check-value of this checkbox,
    // the received time of a messages will be interpreted as
    // period (time between the two last messages) or as time-stamp
    // (the elapsed time since windows was started).
    // - (Protected environment)
    //
    {
        clsCritical locker(m_objpCS);

        pos = m_LastMsgsList->GetHeadPosition();
        bChecked = chbReadingTimeStamp.GetCheck();
        while(pos)
        {
            msgStsCurrentMessage = (MessageStatus*)m_LastMsgsList->GetNext(pos);
            msgStsCurrentMessage->ShowingPeriod = bChecked > 0;
        }
    }
}

void CPCANBasicExampleDlg::OnBnClickedButtonHwrefresh()
{
    TPCANChannelInformation *info;
    TPCANStatus stsResult;
    DWORD iChannelsCount;

    // Clears the Channel comboBox and fill it again with
    // the detected hardware
    //
    cbbChannel.ResetContent();

    stsResult = m_objPCANBasic->GetValue(PCAN_NONEBUS, PCAN_ATTACHED_CHANNELS_COUNT, (void*)&iChannelsCount, sizeof(iChannelsCount));
    if (stsResult == PCAN_ERROR_OK)
    {
        info = new TPCANChannelInformation[iChannelsCount];
        stsResult = m_objPCANBasic->GetValue(PCAN_NONEBUS, PCAN_ATTACHED_CHANNELS, (void*)info, iChannelsCount * sizeof(TPCANChannelInformation));
        if (stsResult == PCAN_ERROR_OK)
            // Include only connectable channels
            //
            for (int i=0; i < (int)iChannelsCount; i++)
                if (info[i].channel_condition & PCAN_CHANNEL_AVAILABLE)
                {
                    CANProtocol protocol = CAN;
                    if ((info[i].device_features & FEATURE_XL_CAPABLE) == FEATURE_XL_CAPABLE)
                        protocol = static_cast<CANProtocol>(protocol | CAN_XL);

                    if ((info[i].device_features & FEATURE_FD_CAPABLE) == FEATURE_FD_CAPABLE)
                        protocol = static_cast<CANProtocol>(protocol | CAN_FD);

                    cbbChannel.AddString(FormatChannelName(info[i].channel_handle, protocol));
                }

        delete [] info;
    }

    // Select Last One
    //
    cbbChannel.SetCurSel(cbbChannel.GetCount() - 1);
    OnCbnSelchangecbbChannel();
}

void CPCANBasicExampleDlg::OnBnClickedChbfilterextended()
{
    int iMaxValue;

    iMaxValue = (chbFilterExtended.GetCheck()) ? 0x1FFFFFFF : 0x7FF;

    // We check that the maximum value for a selected filter
    // mode is used
    //
    nudFilterTo.SetRange32(0,iMaxValue);
    if (nudFilterTo.GetPos32() > iMaxValue)
    {
        nudFilterTo.SetPos32(iMaxValue);
        txtFilterTo.Format("%X", iMaxValue);
    }

    nudFilterFrom.SetRange32(0,iMaxValue);
    if (nudFilterFrom.GetPos32() > iMaxValue)
    {
        nudFilterFrom.SetPos32(iMaxValue);
        txtFilterFrom.Format("%X", iMaxValue);
    }

    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnDeltaposNudfilterfrom(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    int iNewVal;

    //Compute new selected From value
    iNewVal =  pNMUpDown->iPos + ((pNMUpDown->iDelta > 0) ? 1 : -1);
    if(iNewVal < 0)
        iNewVal = 0;

    //Update textBox
    txtFilterFrom.Format("%X", iNewVal);
    UpdateData(FALSE);

    *pResult = 0;
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtfilterfrom()
{
    int iMaxValue;
    iMaxValue = (chbFilterExtended.GetCheck()) ? 0x1FFFFFFF : 0x7FF;
    int newValue;
    UpdateData(TRUE);

    // Compute new edited value
    //
    newValue = HexTextToInt(txtFilterFrom);
    if(newValue > iMaxValue)
        newValue = iMaxValue;
    else if(newValue < 0)
        newValue = 0;
    // Update Nud control
    //
    nudFilterFrom.SetPos32(newValue);
    txtFilterFrom.Format("%X", newValue);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnDeltaposNudfilterto(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    int iNewVal;
    int iMaxValue;

    iMaxValue = (chbFilterExtended.GetCheck()) ? 0x1FFFFFFF : 0x7FF;

    // Compute new selected From value
    //
    iNewVal =  pNMUpDown->iPos + ((pNMUpDown->iDelta > 0) ? 1 : -1);
    if (iNewVal < 0)
        iNewVal = 0;

    if (iNewVal > iMaxValue)
        iNewVal = iMaxValue;

    // Update textBox
    //
    txtFilterTo.Format("%X", iNewVal);
    UpdateData(FALSE);

    *pResult = 0;
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtfilterto()
{
    int iMaxValue;
    iMaxValue = (chbFilterExtended.GetCheck()) ? 0x1FFFFFFF : 0x7FF;
    int newValue;
    UpdateData(TRUE);

    // Compute new edited value
    //
    newValue = HexTextToInt(txtFilterTo);
    if(newValue > iMaxValue)
        newValue = iMaxValue;
    else if(newValue < 0)
        newValue = 0;
    // Update Nud control
    //
    nudFilterTo.SetPos32(newValue);
    txtFilterTo.Format("%X", newValue);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnBnClickedButtonfilterapply()
{
    int iBuffer;
    CString info;
    TPCANStatus stsResult;

    // Gets the current status of the message filter
    //
    if (!GetFilterStatus(&iBuffer))
        return;

    // Configures the message filter for a custom range of messages
    //
    if (rdbFilterCustom.GetCheck())
    {
        // Sets the custom filter
        //
        stsResult = m_objPCANBasic->FilterMessages(m_PcanHandle, nudFilterFrom.GetPos32(), nudFilterTo.GetPos32(), chbFilterExtended.GetCheck() ? PCAN_MODE_EXTENDED : PCAN_MODE_STANDARD);
        // If success, an information message is written, if it is not, an error message is shown
        //
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The filter was customized. IDs from {%X} to {%X} will be received", nudFilterFrom.GetPos32(), nudFilterTo.GetPos32());
            IncludeTextMessage(info);
        }
        else
            ::MessageBox(NULL, GetFormattedError(stsResult), "Error!",MB_ICONERROR);

        return;
    }

    // The filter will be full opened or complete closed
    //
    if (rdbFilterClose.GetCheck())
        iBuffer = PCAN_FILTER_CLOSE;
    else
        iBuffer = PCAN_FILTER_OPEN;

    // The filter is configured
    //
    stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_MESSAGE_FILTER, (void*)&iBuffer, sizeof(int));

    // If success, an information message is written, if it is not, an error message is shown
    //
    if (stsResult == PCAN_ERROR_OK)
    {
        info.Format("The filter was successfully %s", rdbFilterClose.GetCheck() ? "closed." : "opened.");
        IncludeTextMessage(info);
    }
    else
        ::MessageBox(NULL, GetFormattedError(stsResult), "Error!",MB_ICONERROR);
}

void CPCANBasicExampleDlg::OnBnClickedButtonfilterquery()
{
    int iBuffer;

    // Queries the current status of the message filter
    //
    if (GetFilterStatus(&iBuffer))
    {
        switch(iBuffer)
        {
            // The filter is closed
            //
        case PCAN_FILTER_CLOSE:
            IncludeTextMessage("The Status of the filter is: closed.");
            break;
            // The filter is fully opened
            //
        case PCAN_FILTER_OPEN:
            IncludeTextMessage("The Status of the filter is: full opened.");
            break;
            // The filter is customized
            //
        case PCAN_FILTER_CUSTOM:
            IncludeTextMessage("The Status of the filter is: customized.");
            break;
            // The status of the filter is undefined. (Should never happen)
            //
        default:
            IncludeTextMessage("The Status of the filter is: Invalid.");
            break;
        }
    }
}

void CPCANBasicExampleDlg::OnBnClickedRdbmanual()
{
    // Check reading mode selection change
    //
    if(rdbReadingManual.GetCheck() && (m_ActiveReadingMode != 2))
    {
        // Process change
        //
        m_ActiveReadingMode = 2;
        ReadingModeChanged();
    }
}

void CPCANBasicExampleDlg::OnBnClickedButtonread()
{
    TPCANStatus stsResult;

    // We execute the "Read" function of the PCANBasic
    //
    stsResult = ReadMessage();
    if (stsResult != PCAN_ERROR_OK)
        // If an error occurred, an information message is included
        //
        IncludeTextMessage(GetFormattedError(stsResult));
}

void CPCANBasicExampleDlg::OnBnClickedButtonreadingclear()
{
    // (Protected environment)
    //
    {
        clsCritical locker(m_objpCS);

        // Remove all messages
        //
        lstMessages.DeleteAllItems();
        while(m_LastMsgsList->GetCount())
            delete m_LastMsgsList->RemoveHead();
    }
}

void CPCANBasicExampleDlg::OnCbnSelchangeComboparameter()
{
    // Activates/deactivates controls according with the selected
    // PCAN-Basic parameter
    //
    rdbParameterActive.EnableWindow((cbbParameter.GetCurSel() != 0) && (cbbParameter.GetCurSel() != 18));
    rdbParameterInactive.EnableWindow(rdbParameterActive.IsWindowEnabled());
    nudParameterDevNumberOrDelay.EnableWindow(!rdbParameterActive.IsWindowEnabled());
    editParameterDevNumberOrDelay.EnableWindow(!rdbParameterActive.IsWindowEnabled());
    labelDeviceOrDelay.SetWindowText((cbbParameter.GetCurSel() == 18) ? "Delay (µs):" : "Device ID (Hex):");
    nudParameterDevNumberOrDelay.SetPos32(0);
    txtParameterDevNumber = "0";
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnDeltaposNudparamdevnumber(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    int iNewVal;

    // Compute new selected From value
    //
    iNewVal =  pNMUpDown->iPos + ((pNMUpDown->iDelta > 0) ? 1 : -1);
    if(iNewVal < 0)
        iNewVal = 0;
    // Update textBox value
    //
    if(cbbParameter.GetCurSel() == 0)
        txtParameterDevNumber.Format("%X", iNewVal);
    else
        txtParameterDevNumber.Format("%d", iNewVal);
    UpdateData(FALSE);

    *pResult = 0;
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtparamdevnumber()
{
    int newValue;
    bool isDeviceId = (cbbParameter.GetCurSel() == 0);
    UpdateData(TRUE);
    // Compute new edited value
    //
    if(isDeviceId)
        sscanf(txtParameterDevNumber.GetBuffer(), "%X", &newValue);
    else
        newValue = atoi(txtParameterDevNumber);

    if (newValue > MAXUINT)
        newValue = MAXUINT;

    // Update Nud control
    //
    nudParameterDevNumberOrDelay.SetPos32(newValue);
    txtParameterDevNumber.Format(isDeviceId ? "%X" : "%d", newValue);
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::OnBnClickedButtonparamset()
{
    TPCANStatus stsResult;
    int iBuffer;
    CString info;
    TCHAR szDirectory[MAX_PATH] = "";
    bool bActivate;

    bActivate = rdbParameterActive.GetCheck() > 0;

    // Sets a PCAN-Basic parameter value
    //
    switch (cbbParameter.GetCurSel())
    {
        // The device identifier of a channel will be set
        //
    case 0:
        iBuffer = nudParameterDevNumberOrDelay.GetPos32();
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_DEVICE_ID, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
            IncludeTextMessage("The desired Device-ID was successfully configured");
        break;

        // The 5 Volt Power feature of a channel will be set
        //
    case 1:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_5VOLTS_POWER, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The USB/PC-Card 5 power was successfully %s", bActivate ? "activated" : "deactivated");
            IncludeTextMessage(info);
        }
        break;

        // The feature for automatic reset on BUS-OFF will be set
        //
    case 2:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_BUSOFF_AUTORESET, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The automatic-reset on BUS-OFF was successfully %s", bActivate ? "activated" : "deactivated");
            IncludeTextMessage(info);
        }
        break;

        // The CAN option "Listen Only" will be set
        //
    case 3:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_LISTEN_ONLY, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The CAN option \"Listen Only\" was successfully %s", bActivate ? "activated" : "deactivated");
            IncludeTextMessage(info);
        }
        break;

        // The feature for logging debug-information will be set
        //
    case 4:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(PCAN_NONEBUS, PCAN_LOG_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The feature for logging debug information was successfully %s", bActivate ? "activated" : "deactivated");
            IncludeTextMessage(info);
            ::GetCurrentDirectory(sizeof(szDirectory) - 1, szDirectory);
            info.Format("Log file folder: %s" , szDirectory);
            IncludeTextMessage(info);
        }
        break;

        // The channel option "Receive Status" will be set
        //
    case 5:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_RECEIVE_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The channel option \"Receive Status\" was set to %s", bActivate ? "ON" : "OFF");
            IncludeTextMessage(info);
        }
        break;

        // The feature for tracing will be set
        //
    case 7:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_TRACE_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The feature for tracing data was successfully %s", bActivate ? "activated" : "deactivated");
            IncludeTextMessage(info);
        }
        break;

        // The feature for identifying an USB Channel will be set
        //
    case 8:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_CHANNEL_IDENTIFYING, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The procedure for channel identification was successfully %s", bActivate ? "activated" : "deactivated");
            IncludeTextMessage(info);
        }
        break;

        // The feature for using an already configured speed will be set
        //
    case 10:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_BITRATE_ADAPTING, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The feature for bit rate adaptation was successfully %s", bActivate ? "activated" : "deactivated");
            IncludeTextMessage(info);
        }
        break;

        // The option "Allow Status Frames" will be set
        //
    case 15:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_ALLOW_STATUS_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of Status frames was successfully %s", bActivate ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The option "Allow RTR Frames" will be set
        //
    case 16:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_ALLOW_RTR_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of RTR frames was successfully %s", bActivate ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The option "Allow Error Frames" will be set
        //
    case 17:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_ALLOW_ERROR_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of Error frames was successfully %s", bActivate ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The option "Interframes Delay" will be set
        //
    case 18:
        iBuffer = nudParameterDevNumberOrDelay.GetPos32();
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_INTERFRAME_DELAY, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
            IncludeTextMessage("The delay between transmitting frames was successfully set");
        break;

        // The option "Allow Echo Frames" will be set
        //
    case 19:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_ALLOW_ECHO_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of Echo frames was successfully %s", bActivate ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The option "Hard Reset Status" will be set
        //
    case 20:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_HARD_RESET_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The activation of a hard reset within the method PCANBasic.Reset was successfully %s", bActivate ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The current parameter is invalid
        //
    default:
        stsResult = PCAN_ERROR_UNKNOWN;
        ::MessageBox(NULL, "Wrong parameter code.", "Error!",MB_ICONERROR);
        return;
    }

    // If the function fail, an error message is shown
    //
    if(stsResult != PCAN_ERROR_OK)
        ::MessageBox(NULL, GetFormattedError(stsResult), "Error!",MB_ICONERROR);
}

void CPCANBasicExampleDlg::OnBnClickedButtonparamget()
{
    TPCANStatus stsResult;
    int iBuffer;
    char strBuffer[256];
    CString info;

    // Sets a PCAN-Basic parameter value
    //
    switch (cbbParameter.GetCurSel())
    {
        // The device identifier of a channel will be get
        //
    case 0:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_DEVICE_ID, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The configured Device-ID is 0x%X", iBuffer);
            IncludeTextMessage(info);
        }
        break;

        // The 5 Volt Power feature of a channel will be get
        //
    case 1:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_5VOLTS_POWER, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The 5-Volt Power of the USB/PC-Card is %s", (iBuffer == PCAN_PARAMETER_ON) ? "activated" : "deactivated");
            IncludeTextMessage(info);
        }
        break;

        // The feature for automatic reset on BUS-OFF will be get
        //
    case 2:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_BUSOFF_AUTORESET, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The automatic-reset on BUS-OFF is %s", (iBuffer == PCAN_PARAMETER_ON) ? "activated" : "deactivated");
            IncludeTextMessage(info);
        }
        break;

        // The CAN option "Listen Only" will be get
        //
    case 3:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_LISTEN_ONLY, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The CAN option \"Listen Only\" is %s", (iBuffer == PCAN_PARAMETER_ON) ? "activated" : "deactivated");
            IncludeTextMessage(info);
        }
        break;

        // The feature for logging debug-information will be get
        //
    case 4:
        stsResult = m_objPCANBasic->GetValue(PCAN_NONEBUS, PCAN_LOG_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The feature for logging debug information is %s", (iBuffer == PCAN_PARAMETER_ON) ? "activated" : "deactivated");
            IncludeTextMessage(info);
        }
        break;

        // The activation status of the channel option "Receive Status"  will be retrieved
        //
    case 5:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_RECEIVE_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The channel option \"Receive Status\" is %s", (iBuffer == PCAN_PARAMETER_ON) ? "ON" : "OFF");
            IncludeTextMessage(info);
        }
        break;

        // The Number of the CAN-Controller used by a PCAN-Channel
        //
    case 6:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_CONTROLLER_NUMBER, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The CAN Controller number is %d", iBuffer);
            IncludeTextMessage(info);
        }
        break;

        // The activation status for the feature for tracing data will be retrieved
        //
    case 7:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_TRACE_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The feature for tracing data is %s", (iBuffer == PCAN_PARAMETER_ON) ? "ON" : "OFF");
            IncludeTextMessage(info);
        }
        break;

        // The activation status of the Channel Identifying procedure will be retrieved
        //
    case 8:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_CHANNEL_IDENTIFYING, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The identification procedure of the selected channel is %s", (iBuffer == PCAN_PARAMETER_ON) ? "ON" : "OFF");
            IncludeTextMessage(info);
        }
        break;

        // The activation status of the Channel Identifying procedure will be retrieved
        //
    case 9:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_CHANNEL_FEATURES, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The channel %s Flexible Data-Rate (CAN-FD)", (iBuffer & FEATURE_FD_CAPABLE) ? "does support" : "DOESN'T SUPPORT");
            IncludeTextMessage(info);
            info.Format("The channel %s Extra Long (CAN-XL)", (iBuffer & FEATURE_XL_CAPABLE) ? "does support" : "DOESN'T SUPPORT");
            IncludeTextMessage(info);
            info.Format("The channel %s an inter-frame delay for sending messages", (iBuffer & FEATURE_DELAY_CAPABLE) ? "does support" : "DOESN'T SUPPORT");
            IncludeTextMessage(info);
            info.Format("The channel %s using I/O pins", (iBuffer & FEATURE_IO_CAPABLE) ? "does allow" : "DOESN'T ALLOW");
            IncludeTextMessage(info);
        }
        break;

        // The status of the speed adapting feature will be retrieved
        //
    case 10:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_BITRATE_ADAPTING, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The feature for bit rate adaptation is %s", (iBuffer == PCAN_PARAMETER_ON) ? "ON" : "OFF");
            IncludeTextMessage(info);
        }
        break;

        // The bitrate of the connected channel will be retrieved (BTR0-BTR1 value)
        //
    case 11:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_BITRATE_INFO_BTR, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The bit rate of the channel is %.4Xh", iBuffer);
            IncludeTextMessage(info);
        }
        break;

        // The nominal speed configured on the CAN bus
        //
    case 12:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_BUSSPEED_NOMINAL, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The nominal speed of the channel is %d bit/s", iBuffer);
            IncludeTextMessage(info);
        }
        break;

        // The IP address of a LAN channel as string, in IPv4 format
        //
    case 13:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_IP_ADDRESS, strBuffer, 255);
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The IP address of the channel is %s", strBuffer);
            IncludeTextMessage(info);
        }
        break;

        // The running status of the LAN Service
        //
    case 14:
        stsResult = m_objPCANBasic->GetValue(PCAN_NONEBUS, PCAN_LAN_SERVICE_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The LAN service is %s", (iBuffer == SERVICE_STATUS_RUNNING) ? "running" : "NOT running");
            IncludeTextMessage(info);
        }
        break;

        // The reception of Status frames
        //
    case 15:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_ALLOW_STATUS_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of Status frames is %s", (iBuffer == PCAN_PARAMETER_ON) ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The reception of RTR frames
        //
    case 16:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_ALLOW_RTR_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of RTR frames is %s", (iBuffer == PCAN_PARAMETER_ON) ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The reception of Error frames
        //
    case 17:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_ALLOW_ERROR_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of Error frames is %s", (iBuffer == PCAN_PARAMETER_ON) ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The Interframe delay of an USB channel will be retrieved
        //
    case 18:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_INTERFRAME_DELAY, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The configured interframe delay is %d µs", iBuffer);
            IncludeTextMessage(info);
        }
        break;

        // The reception of Echo frames
        //
    case 19:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_ALLOW_ECHO_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of Echo frames is %s", (iBuffer == PCAN_PARAMETER_ON) ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The activation of Hard Reset
        //
    case 20:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_HARD_RESET_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The method PCANBasic.Reset is %s a hardware reset", (iBuffer == PCAN_PARAMETER_ON) ? "performing" : "NOT performing");
            IncludeTextMessage(info);
        }
        break;

        // The direction of the communication with a LAN channel
        //
    case 21:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_LAN_CHANNEL_DIRECTION, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            switch (iBuffer)
            {
                case LAN_DIRECTION_READ:
                    IncludeTextMessage("The communication flow is: incoming only");
                    break;
                case LAN_DIRECTION_WRITE:
                    IncludeTextMessage("The communication flow is: outgoing only");
                    break;
                case LAN_DIRECTION_READ_WRITE:
                    IncludeTextMessage("The communication flow is: bidirectional");
                    break;
                default:
                    info.Format("The communication flow is: undefined (0x%.4X)", iBuffer);
                    IncludeTextMessage(info);
                    break;
            }
        }
        break;

        // The GUID of the device
        // 
    case 22:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_DEVICE_GUID, strBuffer, 255);
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The GUID of the device is %s", strBuffer);
            IncludeTextMessage(info);
        }
        break;

        // The current parameter is invalid
        //
    default:
        stsResult = PCAN_ERROR_UNKNOWN;
        ::MessageBox(NULL, "Wrong parameter code.", "Error!",MB_ICONERROR);
        return;
    }

    // If the function fail, an error message is shown
    //
    if(stsResult != PCAN_ERROR_OK)
        ::MessageBox(NULL, GetFormattedError(stsResult), "Error!",MB_ICONERROR);
}

void CPCANBasicExampleDlg::OnClose()
{
    // Release Hardware if need be
    //
    if(btnRelease.IsWindowEnabled())
        OnBnClickedBtnrelease();

    // Close the Read-Event
    //
    CloseHandle(m_hEvent);

    // (Protected environment)
    //
    {
        clsCritical locker(m_objpCS);
        //Free Resources
        //
        delete m_objPCANBasic;

        while(m_LastMsgsList->GetCount())
            delete m_LastMsgsList->RemoveHead();
        delete m_LastMsgsList;
    }

    // Uninitialize the Critical Section
    //
    FinalizeProtection();

    CDialog::OnClose();
}

void CPCANBasicExampleDlg::OnBnClickedButtonstatus()
{
    TPCANStatus status;
    CString errorName;
    CString info;

    // Gets the current BUS status of a PCAN Channel.
    //
    status = m_objPCANBasic->GetStatus(m_PcanHandle);

    // Switch On Error Name
    //
    switch(status)
    {
        case PCAN_ERROR_INITIALIZE:
            errorName = "PCAN_ERROR_INITIALIZE";
            break;

        case PCAN_ERROR_BUSLIGHT:
            errorName = "PCAN_ERROR_BUSLIGHT";
            break;

        case PCAN_ERROR_BUSHEAVY: // PCAN_ERROR_BUSWARNING
            errorName = "PCAN_ERROR_BUSHEAVY";
            break;

        case PCAN_ERROR_BUSPASSIVE:
            errorName = "PCAN_ERROR_BUSPASSIVE";
            break;

        case PCAN_ERROR_BUSOFF:
            errorName = "PCAN_ERROR_BUSOFF";
            break;

        case PCAN_ERROR_OK:
            errorName = "PCAN_ERROR_OK";
            break;

        default:
            errorName = "See Documentation";
            break;
    }

    // Display Message
    //
    info.Format("Status: %s (%Xh)", errorName, status);
    IncludeTextMessage(info);
}

void CPCANBasicExampleDlg::OnBnClickedButtonreset()
{
    TPCANStatus stsResult;

    // Resets the receive and transmit queues of a PCAN Channel.
    //
    stsResult = m_objPCANBasic->Reset(m_PcanHandle);

    // If it fails, a error message is shown
    //
    if (stsResult != PCAN_ERROR_OK)
        ::MessageBox(NULL, GetFormattedError(stsResult), "Error!",MB_ICONERROR);
    else
        IncludeTextMessage("Receive and transmit queues successfully reset");
}

void CPCANBasicExampleDlg::OnBnClickedButtonversion()
{
    TPCANStatus stsResult;
    char buffer[256];
    CString info, strToken;
    int iPos = 0;

    memset(buffer,'\0',255);

    // We get the version of the PCAN-Basic API
    //
    stsResult = m_objPCANBasic->GetValue(PCAN_NONEBUS, PCAN_API_VERSION, buffer, 256);
    if (stsResult == PCAN_ERROR_OK)
    {
        info.Format("API Version: %s", buffer);
        IncludeTextMessage(info);

        // We get the version of the firmware on the device
        //
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_FIRMWARE_VERSION, buffer, 256);
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("Firmware Version: %s", buffer);
            IncludeTextMessage(info);
        }

        // We get the driver version of the channel being used
        //
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_CHANNEL_VERSION, buffer, 256);
        if (stsResult == PCAN_ERROR_OK)
        {
            info = buffer;
            IncludeTextMessage("Channel/Driver Version: ");

            // Because this information contains line control characters (several lines)
            // we split this also in several entries in the Information List-Box
            //
            strToken = info.Tokenize("\n",iPos);
            while(strToken != "")
            {
                strToken.Insert(0,"     * ");
                IncludeTextMessage(strToken);
                strToken = info.Tokenize("\n",iPos);
            }
        }
    }

    // If the function fail, an error message is shown
    //
    if(stsResult != PCAN_ERROR_OK)
        ::MessageBox(NULL, GetFormattedError(stsResult), "Error!",MB_ICONERROR);
}

void CPCANBasicExampleDlg::OnBnClickedButtoninfoclear()
{
    //Reset listBox Content
    listBoxInfo.ResetContent();
    UpdateData(TRUE);
}

CString CPCANBasicExampleDlg::IntToStr(int iValue)
{
    char chToReceive[20];

    _itoa_s(iValue,chToReceive,10);
    return chToReceive;
}

CString CPCANBasicExampleDlg::IntToHex(int iValue, short iDigits)
{
    CString strTemp, strtest;

    strTemp.Format("%0" + IntToStr(iDigits) + "X",iValue);

    return strTemp;
}

DWORD CPCANBasicExampleDlg::HexTextToInt(CString ToConvert)
{
    DWORD iToReturn = 0;
    int iExp = 0;
    char chByte;

    // The string to convert is empty
    //
    if(ToConvert == "")
        return 0;
    // The string have more than 8 character (the equivalent value
    // exceeds the DWORD capacity
    //
    if(ToConvert.GetLength() > 8)
        return 0;
    // We convert any character to its Upper case
    //
    ToConvert = ToConvert.MakeUpper();

    try
    {
        // We calculate the number using the Hex To Decimal formula
        //
        for(int i= ToConvert.GetLength()-1; i >= 0; i--){
            chByte = ToConvert[i];
            switch(int(chByte)){
                case 65:
                    iToReturn += (DWORD)(10*pow(16.0f,iExp));
                    break;
                case 66:
                    iToReturn += (DWORD)(11*pow(16.0f,iExp));
                    break;
                case 67:
                    iToReturn += (DWORD)(12*pow(16.0f,iExp));
                    break;
                case 68:
                    iToReturn += (DWORD)(13*pow(16.0f,iExp));
                    break;
                case 69:
                    iToReturn += (DWORD)(14*pow(16.0f,iExp));
                    break;
                case 70:
                    iToReturn += (DWORD)(15*pow(16.0f,iExp));
                    break;
                default:
                    if((int(chByte) <48)||(int(chByte)>57))
                        return -1;
                    iToReturn += (DWORD)(atoi(&chByte)*pow(16.0f,iExp));
                    break;

            }
            iExp++;
        }
    }
    catch(...)
    {
        // Error, return 0
        //
        return 0;
    }

    return iToReturn;
}

void CPCANBasicExampleDlg::CheckHexEditBox(CString* txtData)
{
    int iTest;

    txtData->MakeUpper();

    // We test if the given ID is a valid hexadecimal number.
    //
    iTest = HexTextToInt(*txtData);
    if(iTest > 0)
        *txtData = IntToHex(iTest,2);
    else
        *txtData = "00";
}

int CPCANBasicExampleDlg::AddLVItem(CString Caption)
{
    LVITEM NewItem;

    // Initialize LVITEM
    //
    NewItem.mask = LVIF_TEXT;
    NewItem.iSubItem = 0;
    NewItem.pszText = Caption.GetBuffer();
    NewItem.iItem = lstMessages.GetItemCount();

    // Insert it in the message list
    //
    lstMessages.InsertItem(&NewItem);

    return NewItem.iItem;
}

void CPCANBasicExampleDlg::SetTimerRead(bool bEnable)
{
    // Init Timer
    //
    if(bEnable)
        m_tmrRead = SetTimer(1, 50, 0);
    else
    {
        //Delete Timer
        KillTimer(m_tmrRead);
        m_tmrRead = 0;
    }
}

void CPCANBasicExampleDlg::SetTimerDisplay(bool bEnable)
{
    if(bEnable)
        m_tmrDisplay = SetTimer(2, 100, 0);
    else
    {
        KillTimer(m_tmrDisplay);
        m_tmrDisplay = 0;
    }
}

void CPCANBasicExampleDlg::DisplayMessages()
{
    POSITION pos;
    int iCurrentItem;
    MessageStatus *msgStatus;
    CString strTempCount;

    // We search if a message (Same ID and Type) is
    // already received or if this is a new message
    // (in a protected environment)
    //
    {
        clsCritical locker(m_objpCS);

        pos = m_LastMsgsList->GetHeadPosition();
        for(int i=0; i < m_LastMsgsList->GetCount(); i++)
        {
            msgStatus = (MessageStatus*)m_LastMsgsList->GetNext(pos);
            if(msgStatus->MarkedAsUpdated)
            {
                msgStatus->MarkedAsUpdated = false;

                iCurrentItem = msgStatus->Position;

                strTempCount = lstMessages.GetItemText(iCurrentItem,MSG_COUNT);

                lstMessages.SetItemText(iCurrentItem,MSG_LENGTH,IntToStr(msgStatus->CANMsg.LEN));
                lstMessages.SetItemText(iCurrentItem,MSG_COUNT,IntToStr(msgStatus->Count));
                lstMessages.SetItemText(iCurrentItem, MSG_TIME, msgStatus->TimeString);
                lstMessages.SetItemText(iCurrentItem,MSG_DATA,msgStatus->DataString);
            }
        }
    }
}

void CPCANBasicExampleDlg::InsertMsgEntry(TPCANMsg NewMsg, TPCANTimestampULong timeStamp)
{
    MessageStatus *msgStsCurrentMsg;
    int iCurrentItem;

    // (Protected environment)
    //
    {
        clsCritical locker(m_objpCS);

        // We add this status in the last message list
        //
        msgStsCurrentMsg = new MessageStatus(NewMsg, timeStamp, lstMessages.GetItemCount());
        msgStsCurrentMsg->ShowingPeriod = chbReadingTimeStamp.GetCheck() > 0;
        m_LastMsgsList->AddTail(msgStsCurrentMsg);

        // Add the new ListView Item with the Type of the message
        //
        iCurrentItem = AddLVItem(msgStsCurrentMsg->TypeString);
        // We set the ID of the message
        //
        lstMessages.SetItemText(iCurrentItem,MSG_ID,msgStsCurrentMsg->IdString);
        // We set the length of the Message
        //
        lstMessages.SetItemText(iCurrentItem,MSG_LENGTH,IntToStr(NewMsg.LEN));
        // we set the message count message (this is the First, so count is 1)
        //
        lstMessages.SetItemText(iCurrentItem,MSG_COUNT,IntToStr(msgStsCurrentMsg->Count));
        // Add timestamp information
        //
        lstMessages.SetItemText(iCurrentItem, MSG_TIME, msgStsCurrentMsg->TimeString);
        // We set the data of the message.
        //
        lstMessages.SetItemText(iCurrentItem,MSG_DATA,msgStsCurrentMsg->DataString);
    }
}

void CPCANBasicExampleDlg::ProcessMessage(TPCANMsg theMsg, TPCANTimestamp itsTimeStamp)
{
    TPCANTimestampULong timestamp = itsTimeStamp.micros + (1000ULL * itsTimeStamp.millis) + (0x100'000'000ULL * 1000ULL * itsTimeStamp.millis_overflow);

    POSITION pos;
    MessageStatus *msg;

    // We search if a message (Same ID and Type) is
    // already received or if this is a new message
    // (in a protected environment)
    //
    {
        clsCritical locker(m_objpCS);

        pos = m_LastMsgsList->GetHeadPosition();
        for(int i=0; i < m_LastMsgsList->GetCount(); i++)
        {
            msg = (MessageStatus*)m_LastMsgsList->GetNext(pos);
            if((msg->CANMsg.ID == theMsg.ID) && (msg->CANMsg.MSGTYPE == theMsg.MSGTYPE))
            {
                // Modify the message and exit
                //
                msg->Update(theMsg, timestamp);
                return;
            }
        }
        // Message not found. It will created
        //
        InsertMsgEntry(theMsg, timestamp);
    }
}

TPCANStatus CPCANBasicExampleDlg::ReadMessage()
{
    TPCANMsg CANMsg;
    TPCANTimestamp CANTimeStamp;
    TPCANStatus stsResult;

    // We execute the "Read" function of the PCANBasic
    //
    stsResult = m_objPCANBasic->Read(m_PcanHandle, &CANMsg, &CANTimeStamp);
    if (stsResult != PCAN_ERROR_QRCVEMPTY)
        // We process the received message
        //
        ProcessMessage(CANMsg, CANTimeStamp);

    return stsResult;
}

void CPCANBasicExampleDlg::ReadMessages()
{
    TPCANStatus stsResult;

    // We read at least one time the queue looking for messages.
    // If a message is found, we look again trying to find more.
    // If the queue is empty or an error occur, we get out from
    // the do-while statement.
    //
    do
    {
        stsResult =  ReadMessage();
        if (stsResult == PCAN_ERROR_ILLOPERATION)
            break;
    } while (btnRelease.IsWindowEnabled() && (!(stsResult & PCAN_ERROR_QRCVEMPTY)));
}

DWORD WINAPI CPCANBasicExampleDlg::CallCANReadThreadFunc(LPVOID lpParam)
{
    // Cast lpParam argument to PCANBasicExampleDlg*
    //
    CPCANBasicExampleDlg* dialog = (CPCANBasicExampleDlg*)lpParam;

    // Call PCANBasicExampleDlg Thread member function
    //
    return dialog->CANReadThreadFunc(NULL);
}

DWORD WINAPI CPCANBasicExampleDlg::CANReadThreadFunc(LPVOID lpParam)
{
    TPCANStatus stsResult;
    DWORD result, dwTemp;

    m_Terminated = false;

    // Sets the handle of the Receive-Event.
    //
    stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_RECEIVE_EVENT ,&m_hEvent, sizeof(m_hEvent));

    // If it fails, a error message is shown
    //
    if (stsResult != PCAN_ERROR_OK)
    {
        ::MessageBox(NULL, GetFormattedError(stsResult), "Error!",MB_ICONERROR);
        m_Terminated = true;
        return 1;
    }

    // While this mode is selected
    //
    while(!m_Terminated)
    {
        //Wait for CAN Data...
        result = WaitForSingleObject(m_hEvent, 1);

        if (result == WAIT_OBJECT_0)
            ReadMessages();
    }

    // Resets the Event-handle configuration
    //
    dwTemp = 0;
    m_objPCANBasic->SetValue(m_PcanHandle, PCAN_RECEIVE_EVENT ,&dwTemp, sizeof(dwTemp));

    return 0;
}

void CPCANBasicExampleDlg::ReadingModeChanged()
{
    if (!btnRelease.IsWindowEnabled())
        return;

    // If active reading mode is By Timer
    //
    if(m_ActiveReadingMode == 0)
    {
        // Terminate Read Thread if it exists
        //
        if(m_hThread != NULL)
        {
            m_Terminated = true;
            WaitForSingleObject(m_hThread,-1);
            m_hThread = NULL;
        }
        // We start to read
        //
        SetTimerRead(true);
    }
    // If active reading mode is By Event
    //
    else if(m_ActiveReadingMode == 1)
    {
        // We stop to read from the CAN queue
        //
        SetTimerRead(false);

        // Create Reading Thread ....
        //
        m_hThread = CreateThread(NULL, NULL, CPCANBasicExampleDlg::CallCANReadThreadFunc, (LPVOID)this, NULL, NULL);

        if(m_hThread == NULL)
            ::MessageBox(NULL,"Create CANRead-Thread failed","Error!",MB_ICONERROR);
    }
    else
    {
        // Terminate Read Thread if it exists
        //
        if(m_hThread != NULL)
        {
            m_Terminated = true;
            WaitForSingleObject(m_hThread,-1);
            m_hThread = NULL;
        }
        // We start to read
        //
        SetTimerRead(false);
    }
    btnRead.EnableWindow(btnRelease.IsWindowEnabled() && rdbReadingManual.GetCheck());
}

void CPCANBasicExampleDlg::FillComboBoxData()
{
    // Channels will be check
    //
    OnBnClickedButtonHwrefresh();

    // TPCANBaudrate
    //
    cbbBaudrates.SetCurSel(2); // 500 K
    OnCbnSelchangeCbbbaudrates();

    // Parameters for GetValue and SetValue function calls
    //
    cbbParameter.SetCurSel(0);
    OnCbnSelchangeComboparameter();
}

CString CPCANBasicExampleDlg::FormatChannelName(TPCANHandle handle, CANProtocol protocol)
{
    CString result;
    BYTE byChannel;

    // Gets the owner device and channel for a
    // PCAN-Basic handle
    //
    if (handle < 0x100)
        byChannel = (BYTE)(handle) & 0xF;
    else 
        byChannel = (BYTE)(handle) & 0xFF;
        
    // Constructs the PCAN-Basic Channel name and return it
    //
    bool bIsXL = (protocol & CAN_XL) == CAN_XL;
    bool bIsFD = (protocol & CAN_FD) == CAN_FD;

    CString strProtocol;
    if (bIsXL && bIsFD)
        strProtocol = _T("FD/XL");
    else if (bIsXL)
        strProtocol = _T("XL");
    else if (bIsFD)
        strProtocol = _T("FD");
    else
        strProtocol = _T("");

    result.Format(_T("%s:%s %u (%02Xh)"),
        GetTPCANHandleName(handle),
        strProtocol,
        byChannel,
        handle);

    return result;
}

CString CPCANBasicExampleDlg::GetTPCANHandleName(TPCANHandle handle)
{
    CString result = "PCAN_NONE";
    switch(handle)
    {
    case PCAN_PCIBUS1:
    case PCAN_PCIBUS2:
    case PCAN_PCIBUS3:
    case PCAN_PCIBUS4:
    case PCAN_PCIBUS5:
    case PCAN_PCIBUS6:
    case PCAN_PCIBUS7:
    case PCAN_PCIBUS8:
    case PCAN_PCIBUS9:
    case PCAN_PCIBUS10:
    case PCAN_PCIBUS11:
    case PCAN_PCIBUS12:
    case PCAN_PCIBUS13:
    case PCAN_PCIBUS14:
    case PCAN_PCIBUS15:
    case PCAN_PCIBUS16:
        result = "PCAN_PCI";
        break;

    case PCAN_USBBUS1:
    case PCAN_USBBUS2:
    case PCAN_USBBUS3:
    case PCAN_USBBUS4:
    case PCAN_USBBUS5:
    case PCAN_USBBUS6:
    case PCAN_USBBUS7:
    case PCAN_USBBUS8:
    case PCAN_USBBUS9:
    case PCAN_USBBUS10:
    case PCAN_USBBUS11:
    case PCAN_USBBUS12:
    case PCAN_USBBUS13:
    case PCAN_USBBUS14:
    case PCAN_USBBUS15:
    case PCAN_USBBUS16:
        result = "PCAN_USB";
        break;

    case PCAN_LANBUS1:
    case PCAN_LANBUS2:
    case PCAN_LANBUS3:
    case PCAN_LANBUS4:
    case PCAN_LANBUS5:
    case PCAN_LANBUS6:
    case PCAN_LANBUS7:
    case PCAN_LANBUS8:
    case PCAN_LANBUS9:
    case PCAN_LANBUS10:
    case PCAN_LANBUS11:
    case PCAN_LANBUS12:
    case PCAN_LANBUS13:
    case PCAN_LANBUS14:
    case PCAN_LANBUS15:
    case PCAN_LANBUS16:
        result = "PCAN_LAN";
        break;
    }
    return result;
}

CString CPCANBasicExampleDlg::GetComboBoxSelectedLabel(CComboBox* ccb)
{
    CString strTemp;
    int item = ccb->GetCurSel();
    if(item != CB_ERR)
        ccb->GetLBText(item, strTemp);

    return strTemp;
}

CString CPCANBasicExampleDlg::GetFormattedError(TPCANStatus error)
{
    TPCANStatus status;
    char buffer[256];
    CString result;

    memset(buffer,'\0',255);
    // Gets the text using the GetErrorText API function
    // If the function success, the translated error is returned. If it fails,
    // a text describing the current error is returned.
    //
    status = m_objPCANBasic->GetErrorText(error, 0, buffer);
    if(status != PCAN_ERROR_OK)
        result.Format("An error occurred. Error-code's text (%Xh) couldn't be retrieved", error);
    else
        result = buffer;
    return result;
}

void CPCANBasicExampleDlg::SetConnectionStatus(bool bConnected)
{
    // Buttons
    //
    btnInit.EnableWindow(!bConnected);
    btnRead.EnableWindow(bConnected && rdbReadingManual.GetCheck());
    btnWrite.EnableWindow(bConnected);
    btnRelease.EnableWindow(bConnected);
    btnFilterApply.EnableWindow(bConnected);
    btnFilterQuery.EnableWindow(bConnected);
    btnVersions.EnableWindow(bConnected);
    btnRefresh.EnableWindow(!bConnected);
    btnStatus.EnableWindow(bConnected);
    btnReset.EnableWindow(bConnected);

    // ComboBoxes
    //
    cbbChannel.EnableWindow(!bConnected);
    cbbBaudrates.EnableWindow(!bConnected);

    // Hardware configuration and read mode
    //
    if (!bConnected)
        OnCbnSelchangecbbChannel();
    else
        ReadingModeChanged();

    // Display messages in grid
    //
    SetTimerDisplay(bConnected);
}

bool CPCANBasicExampleDlg::GetFilterStatus(int* status)
{
    TPCANStatus stsResult;

    // Tries to get the status of the filter for the current connected hardware
    //
    stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_MESSAGE_FILTER, (void*)status, sizeof(int));

    // If it fails, a error message is shown
    //
    if (stsResult != PCAN_ERROR_OK)
    {
        ::MessageBox(NULL, GetFormattedError(stsResult), "Error!",MB_ICONERROR);
        return false;
    }
    return true;
}

void CPCANBasicExampleDlg::IncludeTextMessage(CString strMsg)
{
    listBoxInfo.AddString(strMsg);
    listBoxInfo.SetCurSel(listBoxInfo.GetCount() - 1);
}

void CPCANBasicExampleDlg::ConfigureLogFile()
{
    int iBuffer;

    // Sets the mask to catch all events
    //
    iBuffer = LOG_FUNCTION_ALL;

    // Configures the log file.
    // NOTE: The Log capability is to be used with the NONEBUS Handle. Other handle than this will
    // cause the function fail.
    //
    m_objPCANBasic->SetValue(PCAN_NONEBUS, PCAN_LOG_CONFIGURE, (void*)&iBuffer, sizeof(iBuffer));
}

void CPCANBasicExampleDlg::ConfigureTraceFile()
{
    int iBuffer;
    TPCANStatus stsResult;

    // Configure the maximum size of a trace file to 5 megabytes
    //
    iBuffer = 5;
    stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_TRACE_SIZE, (void*)&iBuffer, sizeof(iBuffer));
    if (stsResult != PCAN_ERROR_OK)
        IncludeTextMessage(GetFormattedError(stsResult));

    // Configure the way how trace files are created:
    // * Standard name is used
    // * Existing file is overwritten,
    // * Only one file is created.
    // * Recording stops when the file size reaches 5 megabytes.
    //
    iBuffer = TRACE_FILE_SINGLE | TRACE_FILE_OVERWRITE;
    stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_TRACE_CONFIGURE, (void*)&iBuffer, sizeof(iBuffer));
    if (stsResult != PCAN_ERROR_OK)
        IncludeTextMessage(GetFormattedError(stsResult));
}

void CPCANBasicExampleDlg::OnLbnDblclkListinfo()
{
    OnBnClickedButtoninfoclear();
}
