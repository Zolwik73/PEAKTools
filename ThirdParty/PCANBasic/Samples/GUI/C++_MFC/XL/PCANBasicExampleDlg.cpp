// PCANBasicExampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PCANBasicExample.h"
#include "PCANBasicExampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <random>

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

/// <summary>
/// Converts a CAN DLC value into the actual data length of the CAN-XL frame.
/// </summary>
/// <param name="dlc">A value between 0 and 2047 (CAN XL DLC range)</param>
/// <param name="messageTypeXL">The message type of the message</param>
/// <returns>The length represented by the DLC</returns>
static int GetLengthFromDLC(int dlc, TPCANMessageTypeXL msgType)
{
    if ((msgType & PCAN_MESSAGE_STATUS) == PCAN_MESSAGE_STATUS ||
        (msgType & PCAN_MESSAGE_XL) != PCAN_MESSAGE_XL) 
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

void CPCANBasicExampleDlg::RandomizeData()
{
    for (size_t i = 0; i < m_dataDlc + 1; i++)
    {
        // Generates a random byte value in a range between 0 and 255
        // 
        m_data[i] = static_cast<unsigned char>(rand() % 256);
    }
}

void CPCANBasicExampleDlg::InitializeData()
{
    CString strData = "";

    for (size_t i = 0; i < m_dataDlc + 1 ; i++)
    {
        // Converts each byte to a hex value and adds it to strData
        // 
        CString hexByte;
        hexByte.Format(TEXT("%02X "), m_data[i]);
        strData.Append(hexByte);
    }

    // Displays the newly generated dataset
    //
    txtData = strData;

    // Updates the ui
    UpdateData(FALSE);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// MessageStatus class
//
#pragma region Message Status class
MessageStatus::MessageStatus(TPCANMsgXL canMsg, TPCANTimestampXL canTimestamp, int listIndex)
{
    m_Msg = canMsg;
    m_TimeStamp = canTimestamp;
    m_oldTimeStamp = canTimestamp;
    m_iIndex = listIndex;
    m_Count = 1;
    m_bShowPeriod = true;
    m_bWasChanged = false;
}

void MessageStatus::Update(TPCANMsgXL canMsg, TPCANTimestampXL canTimestamp)
{
    m_Msg = canMsg;
    m_oldTimeStamp = m_TimeStamp;
    m_TimeStamp = canTimestamp;
    m_bWasChanged = true;
    m_Count += 1;
}

TPCANMsgXL MessageStatus::GetCANMsg()
{
    return m_Msg;
}

TPCANTimestampFD MessageStatus::GetTimestamp()
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

    if ((m_Msg.MSGTYPE & PCAN_MESSAGE_STATUS) == PCAN_MESSAGE_STATUS)
        return _T("STATUS");

    if ((m_Msg.MSGTYPE & PCAN_MESSAGE_ERRFRAME) == PCAN_MESSAGE_ERRFRAME)
        return _T("ERROR");

    if ((m_Msg.MSGTYPE & PCAN_MESSAGE_XL) == PCAN_MESSAGE_XL)
        strTemp = _T("XL");
    else if ((m_Msg.MSGTYPE & PCAN_MESSAGE_EXTENDED) == PCAN_MESSAGE_EXTENDED)
        strTemp = _T("EXT");
    else
        strTemp = _T("STD");

    if ((m_Msg.MSGTYPE & PCAN_MESSAGE_RTR) == PCAN_MESSAGE_RTR)
        strTemp += isEcho ? _T("/RTR [ ECHO ]") : _T("/RTR");
    else
    {
        if (isEcho)
            strTemp += _T(" [ ECHO");
        else
            strTemp += _T(" [ ");

        if ((int)m_Msg.MSGTYPE >= (unsigned int)PCAN_MESSAGE_XL)
        {
            if (m_Msg.RRS)
                strTemp += _T(" RRS");
            if (m_Msg.SEC)
                strTemp += _T(" SEC");
            strTemp += _T(" ]");
        }
        else if ((int)m_Msg.MSGTYPE > (int)PCAN_MESSAGE_EXTENDED)
        {
            if ((m_Msg.MSGTYPE & PCAN_MESSAGE_FD) == PCAN_MESSAGE_FD)
                strTemp += _T(" FD");
            if ((m_Msg.MSGTYPE & PCAN_MESSAGE_BRS) == PCAN_MESSAGE_BRS)
                strTemp += _T(" BRS");
            if ((m_Msg.MSGTYPE & PCAN_MESSAGE_ESI) == PCAN_MESSAGE_ESI)
                strTemp += _T(" ESI");
            strTemp += _T(" ]");
        }
    }

    return strTemp;
}

CString MessageStatus::GetPIDString()
{
    CString strTemp;

    // We format the PID of the message and show it
    //
    strTemp.Format("%03Xh",m_Msg.PID);
    return strTemp;
}

CString MessageStatus::GetSDTString()
{
    CString strTemp;

    // We format the SDT of the message and show it
    //
    strTemp.Format("%02Xh", m_Msg.SDT);
    return strTemp;
}

CString MessageStatus::GetVCIDString()
{
    CString strTemp;

    // We format the VCID of the message and show it
    //
    strTemp.Format("%02Xh", m_Msg.VCID);
    return strTemp;
}

CString MessageStatus::GetAFString()
{
    CString strTemp;

    // We format the AF of the message and show it
    //
    strTemp.Format("%08Xh", m_Msg.AF);
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
        for(int i=0; i < GetLengthFromDLC(m_Msg.DLC, m_Msg.MSGTYPE); i++)
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
    , txtPID(_T(""))
    , txtSDT(_T(""))
    , txtVCID(_T(""))
    , txtAF(_T(""))
, txtLength(_T(""))
, txtBitrate(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPCANBasicExampleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_CBBCHANNEL, cbbChannel);
    DDX_Text(pDX, IDC_TXTBITRATE, txtBitrate);
    DDX_Control(pDX, IDC_BTNINIT, btnInit);
    DDX_Control(pDX, IDC_BTNRELEASE, btnRelease);
    DDX_Control(pDX, IDC_RADIOFILTEROPEN, rdbFilterOpen);
    DDX_Control(pDX, IDC_RADIOFILTERCLOSE, rdbFilterClose);
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
    DDX_Control(pDX, IDC_CHBRRS, chbRRS);
    DDX_Control(pDX, IDC_CHBSEC, chbSEC);
    DDX_Control(pDX, IDC_NUDLENGTH, nudLength);
    DDX_Text(pDX, IDC_TXTPID, txtPID);
    DDV_MaxChars(pDX, txtPID, 3);
    DDX_Text(pDX, IDC_TXTSDT, txtSDT);
    DDV_MaxChars(pDX, txtSDT, 2);
    DDX_Text(pDX, IDC_TXTVCID, txtVCID);
    DDV_MaxChars(pDX, txtVCID, 2);;
    DDX_Text(pDX, IDC_TXTAF, txtAF);
    DDV_MaxChars(pDX, txtAF, 8);
    DDX_Text(pDX, IDC_TXTDATA, txtData);
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
    ON_EN_KILLFOCUS(IDC_TXTPID, OnEnKillfocusTxtpid)
    ON_NOTIFY(UDN_DELTAPOS, IDC_NUDLENGTH, OnDeltaposNudlength)
    ON_NOTIFY(NM_DBLCLK, IDC_LSTMESSAGES, OnNMDblclkLstmessages)
    ON_BN_CLICKED(IDC_BTNINIT, OnBnClickedBtninit)
    ON_BN_CLICKED(IDC_BTNRELEASE, OnBnClickedBtnrelease)
    ON_BN_CLICKED(IDC_BTNWRITE, OnBnClickedBtnwrite)
    ON_WM_SHOWWINDOW()
    ON_BN_CLICKED(IDC_RDBTIMER, &CPCANBasicExampleDlg::OnBnClickedRdbtimer)
    ON_BN_CLICKED(IDC_RDBEVENT, &CPCANBasicExampleDlg::OnBnClickedRdbevent)
    ON_BN_CLICKED(IDC_CHBTIMESTAMP, &CPCANBasicExampleDlg::OnBnClickedChbtimestamp)
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
    ON_EN_KILLFOCUS(IDC_TXTPID, &CPCANBasicExampleDlg::OnEnKillfocusTxtpid)
    ON_EN_KILLFOCUS(IDC_TXTSDT, &CPCANBasicExampleDlg::OnEnKillfocusTxtsdt)
    ON_EN_KILLFOCUS(IDC_TXTVCID, &CPCANBasicExampleDlg::OnEnKillfocusTxtvcid)
    ON_EN_KILLFOCUS(IDC_TXTAF, &CPCANBasicExampleDlg::OnEnKillfocusTxtaf)
    ON_EN_KILLFOCUS(IDC_TXTLENGTH, &CPCANBasicExampleDlg::OnEnKillfocusTxtlength)
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

    // Set seed for random data generation
    srand(static_cast<unsigned int>(time(NULL)));

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

    lstMessages.InsertColumn(MSG_TYPE, "Type", LVCFMT_LEFT, 70, 0);
    lstMessages.InsertColumn(MSG_PID, "PID", LVCFMT_LEFT, 65, 1);
    lstMessages.InsertColumn(MSG_LENGTH, "Length", LVCFMT_LEFT, 70, 2);
    lstMessages.InsertColumn(MSG_SDT, "SDT", LVCFMT_LEFT, 65, 3);
    lstMessages.InsertColumn(MSG_VCID, "VCID", LVCFMT_LEFT, 65, 4);
    lstMessages.InsertColumn(MSG_AF, "AF", LVCFMT_LEFT, 120, 5);
    lstMessages.InsertColumn(MSG_TIME, "Rcv Time", LVCFMT_LEFT, 130, 6);
    lstMessages.InsertColumn(MSG_COUNT, "Count", LVCFMT_LEFT, 60, 7);
    lstMessages.InsertColumn(MSG_DATA, "Data", LVCFMT_LEFT, 170, 8);
    lstMessages.SetExtendedStyle(lstMessages.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

    // Initializes Edit Controls
    //
    txtPID = "000";
    txtSDT = "00";
    txtVCID = "00";
    txtAF = "00000000";
    txtLength = "8";

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

    // Create a buffer of 2048 bytes for storing random CAN data for message writing
    //
    m_data = new unsigned char[2048];
    
    // Set current dlc to 8 (9 Bytes)
    //
    m_dataDlc = 8;

    // UpDown Length
    //
    nudLength.SetRange(0,2047);
    nudLength.SetPos(8);

    // Randomizes and show the CAN data to send
    //
    FillData();

    // Prepares the PCAN-Basic's debug-Log file
    //
    FillComboBoxData();


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

void CPCANBasicExampleDlg::FillData()
{
    RandomizeData();
    InitializeData();
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

    if (strTemp.IsEmpty())
        return;
    if (strTemp.Find(TEXT("refresh")) != -1)
        DoRefresh();
    else
    {
        strTemp = strTemp.Mid(strTemp.Find('(') + 1, 3);

        strTemp.Replace('h', ' ');
        strTemp.Trim(' ');

        m_PcanHandle = (WORD)HexTextToInt(strTemp);
    }
}

void CPCANBasicExampleDlg::OnDeltaposNudlength(NMHDR *pNMHDR, LRESULT *pResult)
{
    int iNewVal;
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    // Compute new selected value
    //
    iNewVal = pNMUpDown->iPos + pNMUpDown->iDelta;

    if (iNewVal > 2047)
    {
        iNewVal = 2047;
        pNMUpDown->iPos -= 1;
    }
    if(iNewVal < 0)
        iNewVal = 0;
    
    m_dataDlc = iNewVal;

    GetDlgItem(IDC_LALENGTH)->SetWindowText(__T("Length: ") + IntToStr(iNewVal + 1) + __T(" B."));

    *pResult = 0;

    // Fill txtData with newly generated random datasets
    //
    FillData();
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
    stsResult = m_objPCANBasic->InitializeXL(m_PcanHandle, txtBitrate.GetBuffer());

    if (stsResult != PCAN_ERROR_OK)
        if (stsResult != PCAN_ERROR_CAUTION)
            ::MessageBox(NULL, GetFormatedError(stsResult), "Error!",MB_ICONERROR);
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

void CPCANBasicExampleDlg::OnBnClickedBtnwrite()
{
    TPCANStatus stsResult;

    // The message is sent
    //
    stsResult = WriteFrameXL();

    // The Hardware was successfully sent
    //
    if (stsResult == PCAN_ERROR_OK)
        IncludeTextMessage("Message was successfully SENT");
    else
        // An error occurred.  We show the error.
        //
        ::MessageBox(NULL, GetFormatedError(stsResult), "Error!",MB_ICONERROR);
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

void CPCANBasicExampleDlg::OnBnClickedButtonfilterapply()
{
    int iBuffer;
    CString info;
    TPCANStatus stsResult;

    // Gets the current status of the message filter
    //
    if (!GetFilterStatus(&iBuffer))
        return;

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
        ::MessageBox(NULL, GetFormatedError(stsResult), "Error!",MB_ICONERROR);
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
    stsResult = ReadMessageXL();
    if (stsResult != PCAN_ERROR_OK)
        // If an error occurred, an information message is included
        //
        IncludeTextMessage(GetFormatedError(stsResult));
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
    rdbParameterActive.EnableWindow((cbbParameter.GetCurSel() != 0) && (cbbParameter.GetCurSel() != 22));
    rdbParameterInactive.EnableWindow(rdbParameterActive.IsWindowEnabled());
    nudParameterDevNumberOrDelay.EnableWindow(!rdbParameterActive.IsWindowEnabled());
    editParameterDevNumberOrDelay.EnableWindow(!rdbParameterActive.IsWindowEnabled());
    labelDeviceOrDelay.SetWindowText((cbbParameter.GetCurSel() == 22) ? "Delay (µs):" : "Device ID (Hex):");
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
    case 19:
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
    case 20:
        iBuffer = bActivate ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_ALLOW_RTR_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of RTR frames was successfully %s", bActivate ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The option "Allow Error Frames" will be set
        //
    case 21:
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
    case 22:
        iBuffer = nudParameterDevNumberOrDelay.GetPos32();
        stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_INTERFRAME_DELAY, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
            IncludeTextMessage("The delay between transmitting frames was successfully set");
        break;

        // The option "Allow Echo Frames" will be set
        //
    case 23:
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
    case 24:
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
        ::MessageBox(NULL, GetFormatedError(stsResult), "Error!",MB_ICONERROR);
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
            info.Format("The channel %s Extra Long (CAN-XL)", (iBuffer& FEATURE_XL_CAPABLE) ? "does support" : "DOESN'T SUPPORT");
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

        // The bitrate of the connected Nominal channel will be retrieved (String value)
        //
    case 11:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_BITRATE_INFO_CC, strBuffer, 255);
        if (stsResult == PCAN_ERROR_OK)
        {
            int partPos = 0;
            CString strPart = ((CString)strBuffer).Tokenize(",", partPos);

            IncludeTextMessage("The bit rate Nominal of the channel is represented by the following values:");
            while (!strPart.IsEmpty())
            {
                IncludeTextMessage("   * " + strPart);
                strPart = ((CString)strBuffer).Tokenize(",", partPos);
            }
        }
        break;

        // The bitrate of the connected FD channel will be retrieved (String value)
        //
    case 12:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_BITRATE_INFO_FD, strBuffer, 255);
        if (stsResult == PCAN_ERROR_OK)
        {
            int partPos = 0;
            CString strPart = ((CString)strBuffer).Tokenize(",", partPos);

            IncludeTextMessage("The bit rate FD of the channel is represented by the following values:");
            while(!strPart.IsEmpty())
            {
                IncludeTextMessage("   * " + strPart);
                strPart = ((CString)strBuffer).Tokenize(",", partPos);
            }
        }
        break;

        // The bitrate of the connected XL channel will be retrieved (String value)
        //
    case 13:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_BITRATE_INFO_XL, strBuffer, 255);
        if (stsResult == PCAN_ERROR_OK)
        {
            int partPos = 0;
            CString strPart = ((CString)strBuffer).Tokenize(",", partPos);

            IncludeTextMessage("The bit rate XL of the channel is represented by the following values:");
            while (!strPart.IsEmpty())
            {
                IncludeTextMessage("   * " + strPart);
                strPart = ((CString)strBuffer).Tokenize(",", partPos);
            }
        }
        break;

        // The nominal speed configured on the CAN bus
        //
    case 14:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_BUSSPEED_NOMINAL, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The nominal speed of the channel is %d bit/s", iBuffer);
            IncludeTextMessage(info);
        }
        break;
        // The FD speed configured on the CAN bus
        //
    case 15:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_BUSSPEED_FD, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The FD speed of the channel is %d bit/s", iBuffer);
            IncludeTextMessage(info);
        }
        break;
        // The XL speed configured on the CAN bus
        //
    case 16:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_BUSSPEED_XL, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The XL speed of the channel is %d bit/s", iBuffer);
            IncludeTextMessage(info);
        }
        break;
        // The IP address of a LAN channel as string, in IPv4 format
        //
    case 17:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_IP_ADDRESS, strBuffer, 255);
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The IP address of the channel is %s", strBuffer);
            IncludeTextMessage(info);
        }
        break;
        // The running status of the LAN Service
        //
    case 18:
        stsResult = m_objPCANBasic->GetValue(PCAN_NONEBUS, PCAN_LAN_SERVICE_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The LAN service is %s", (iBuffer == SERVICE_STATUS_RUNNING) ? "running" : "NOT running");
            IncludeTextMessage(info);
        }
        break;
        // The reception of Status frames
        //
    case 19:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_ALLOW_STATUS_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of Status frames is %s", (iBuffer == PCAN_PARAMETER_ON) ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;
        // The reception of RTR frames
        //
    case 20:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_ALLOW_RTR_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of RTR frames is %s", (iBuffer == PCAN_PARAMETER_ON) ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;
        // The reception of Error frames
        //
    case 21:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_ALLOW_ERROR_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of Error frames is %s", (iBuffer == PCAN_PARAMETER_ON) ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The Interframe delay of an USB channel will be retrieved
        //
    case 22:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_INTERFRAME_DELAY, (void*)&iBuffer, sizeof(iBuffer));
        if(stsResult == PCAN_ERROR_OK)
        {
            info.Format("The configured interframe delay is %d µs", iBuffer);
            IncludeTextMessage(info);
        }
        break;

        // The reception of Echo frames
        //
    case 23:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_ALLOW_ECHO_FRAMES, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The reception of Echo frames is %s", (iBuffer == PCAN_PARAMETER_ON) ? "enabled" : "disabled");
            IncludeTextMessage(info);
        }
        break;

        // The activation of Hard Reset
        //
    case 24:
        stsResult = m_objPCANBasic->GetValue(m_PcanHandle, PCAN_HARD_RESET_STATUS, (void*)&iBuffer, sizeof(iBuffer));
        if (stsResult == PCAN_ERROR_OK)
        {
            info.Format("The method PCANBasic.Reset is %s a hardware reset", (iBuffer == PCAN_PARAMETER_ON) ? "performing" : "NOT performing");
            IncludeTextMessage(info);
        }
        break;

        // The direction of the communication with a LAN channel
        //
    case 25:
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
    case 26:
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
        ::MessageBox(NULL, GetFormatedError(stsResult), "Error!",MB_ICONERROR);
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

        case PCAN_ERROR_BUSWARNING:
            errorName = "PCAN_ERROR_BUSWARNING";
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
        ::MessageBox(NULL, GetFormatedError(stsResult), "Error!",MB_ICONERROR);
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
            info.Format("Firmare Version: %s", buffer);
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
        ::MessageBox(NULL, GetFormatedError(stsResult), "Error!",MB_ICONERROR);
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

    // We test if the given PID is a valid hexadecimal number.
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

    // We search if a message is
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
                lstMessages.SetItemText(iCurrentItem,MSG_LENGTH,IntToStr(GetLengthFromDLC(msgStatus->CANMsg.DLC, msgStatus->CANMsg.MSGTYPE)));
                lstMessages.SetItemText(iCurrentItem, MSG_SDT, msgStatus->SDTString);
                lstMessages.SetItemText(iCurrentItem, MSG_VCID, msgStatus->VCIDString);
                lstMessages.SetItemText(iCurrentItem, MSG_AF, msgStatus->AFString);
                lstMessages.SetItemText(iCurrentItem, MSG_TIME, msgStatus->TimeString);
                lstMessages.SetItemText(iCurrentItem,MSG_COUNT,IntToStr(msgStatus->Count));
                lstMessages.SetItemText(iCurrentItem,MSG_DATA,msgStatus->DataString);
            }
        }
    }
}

void CPCANBasicExampleDlg::InsertMsgEntry(TPCANMsgXL NewMsg, TPCANTimestampXL timeStamp)
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
        // We set the PID of the message
        //
        lstMessages.SetItemText(iCurrentItem,MSG_PID,msgStsCurrentMsg->PidString);
        // We set the length of the Message
        //
        lstMessages.SetItemText(iCurrentItem,MSG_LENGTH,IntToStr(GetLengthFromDLC(NewMsg.DLC, NewMsg.MSGTYPE)));
        // We set the SDT information of the message
        //
        lstMessages.SetItemText(iCurrentItem, MSG_SDT, msgStsCurrentMsg->SDTString);
        // We set the VCID information of the message
        //
        lstMessages.SetItemText(iCurrentItem, MSG_VCID, msgStsCurrentMsg->VCIDString);
        // We set the AF information of the message
        //
        lstMessages.SetItemText(iCurrentItem, MSG_AF, msgStsCurrentMsg->AFString);
        // Add timestamp information
        //
        lstMessages.SetItemText(iCurrentItem, MSG_TIME, msgStsCurrentMsg->TimeString);
        // we set the message count message (this is the First, so count is 1)
        //
        lstMessages.SetItemText(iCurrentItem,MSG_COUNT,IntToStr(msgStsCurrentMsg->Count));
        // We set the data of the message.
        //
        lstMessages.SetItemText(iCurrentItem,MSG_DATA,msgStsCurrentMsg->DataString);
    }
}

void CPCANBasicExampleDlg::ProcessMessage(TPCANMsgXL theMsg, TPCANTimestampXL itsTimeStamp)
{
    POSITION pos;
    MessageStatus *msg;

    // We search if a message is
    // already received or if this is a new message
    // (in a protected environment)
    //
    {
        clsCritical locker(m_objpCS);

        pos = m_LastMsgsList->GetHeadPosition();
        for(int i=0; i < m_LastMsgsList->GetCount(); i++)
        {
            msg = (MessageStatus*)m_LastMsgsList->GetNext(pos);
            if((msg->CANMsg.PID == theMsg.PID) && (msg->CANMsg.MSGTYPE == theMsg.MSGTYPE) &&
                (msg->CANMsg.VCID == theMsg.VCID) && (msg->CANMsg.SDT == theMsg.SDT) &&
                (msg->CANMsg.AF == theMsg.AF) && (msg->CANMsg.RRS == theMsg.RRS) &&
                (msg->CANMsg.SEC == theMsg.SEC))
            {
                // Modify the message and exit
                //
                msg->Update(theMsg, itsTimeStamp);
                return;
            }
        }
        // Message not found. It will created
        //
        InsertMsgEntry(theMsg, itsTimeStamp);
    }
}

TPCANStatus CPCANBasicExampleDlg::ReadMessageXL()
{
    TPCANMsgXL CANMsg = {};
    TPCANTimestampXL CANTimeStamp;
    TPCANStatus stsResult;

    // We execute the "ReadXL" function of the PCANBasic
    //
    stsResult = m_objPCANBasic->ReadXL(m_PcanHandle, &CANMsg, &CANTimeStamp);
    if (stsResult != PCAN_ERROR_QRCVEMPTY)
        // We process the received message
        //
        ProcessMessage(CANMsg, CANTimeStamp);

    return stsResult;
}

TPCANStatus CPCANBasicExampleDlg::WriteFrameXL()
{
    TPCANMsgXL CANMsg;

    // We configurate the Message.
    //
    CANMsg.MSGTYPE = PCAN_MESSAGE_XL;
    CANMsg.PID = HexTextToInt(txtPID);
    CANMsg.DLC = nudLength.GetPos32();
    CANMsg.SDT = (BYTE)HexTextToInt(txtSDT);
    CANMsg.VCID = (BYTE)HexTextToInt(txtVCID);
    CANMsg.AF = HexTextToInt(txtAF);
    CANMsg.SEC = chbSEC.GetCheck() > 0;
    CANMsg.RRS = chbRRS.GetCheck() > 0;
    CopyMemory(CANMsg.DATA, m_data, nudLength.GetPos32() + 1);

    // The message is sent to the configured hardware
    //
    return m_objPCANBasic->WriteXL(m_PcanHandle, &CANMsg);
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
        stsResult =  ReadMessageXL();
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
        ::MessageBox(NULL, GetFormatedError(stsResult), "Error!",MB_ICONERROR);
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

    // Resets the Event-Handle configuration
    //
    dwTemp = 0;
    m_objPCANBasic->SetValue(m_PcanHandle, PCAN_RECEIVE_EVENT ,&dwTemp, sizeof(dwTemp));

    return 0;
}

void CPCANBasicExampleDlg::SetTextForPID()
{
    int iTest;

    // Apply Pending Updates
    //
    UpdateData(TRUE);

    // Format to Upper
    //
    txtPID.MakeUpper();

    // Convert string value
    //
    iTest = HexTextToInt(txtPID);

    if (iTest > 0x7FF)
        txtPID = "7FF";
    else
    {
        // We test if the given PID is a valid hexadecimal number.
        //
        iTest = HexTextToInt(txtPID);
        if (iTest > 0)
            txtPID = IntToHex(iTest, 3);
        else
        {
            txtPID = "000";
        }

        UpdateData(FALSE);
    }
}

void CPCANBasicExampleDlg::SetTextForSDT()
{
    int iTest;
    int iTextLength = 2;

    // Apply Pending Updates
    //
    UpdateData(TRUE);

    // Format to Upper
    //
    txtSDT.MakeUpper();

    // Convert string value
    //
    iTest = HexTextToInt(txtSDT);

    while (txtSDT.GetLength() != iTextLength)
        txtSDT = ("0" + txtSDT);

    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::SetTextForVCID()
{
    int iTest;
    int iTextLength = 2;

    // Apply Pending Updates
    //
    UpdateData(TRUE);

    // Format to Upper
    //
    txtSDT.MakeUpper();

    // Convert string value
    //
    iTest = HexTextToInt(txtSDT);

    while (txtVCID.GetLength() != iTextLength)
        txtVCID = ("0" + txtVCID);

    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::SetTextForAF()
{
    int iTest;
    int iTextLength = 8;

    // Apply pending updates
    //
    UpdateData(TRUE);

    // Format to upper
    //
    txtAF.MakeUpper();

    // Convert string value
    //
    iTest = HexTextToInt(txtAF);

    while (txtAF.GetLength() != iTextLength)
        txtAF = ("0" + txtAF);
    
    UpdateData(FALSE);
}

void CPCANBasicExampleDlg::ReadingModeChanged()
{
    if (!btnRelease.IsWindowEnabled())
        return;

    // If active reading mode is by timer
    //
    if(m_ActiveReadingMode == 0)
    {
        // Terminate read thread if it exists
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
    // If active reading mode is by event
    //
    else if(m_ActiveReadingMode == 1)
    {
        // We stop to read from the CAN queue
        //
        SetTimerRead(false);

        // Create reading Thread ....
        //
        m_hThread = CreateThread(NULL, NULL, CPCANBasicExampleDlg::CallCANReadThreadFunc, (LPVOID)this, NULL, NULL);

        if(m_hThread == NULL)
            ::MessageBox(NULL,"Create CANRead-Thread failed","Error!",MB_ICONERROR);
    }
    else
    {
        // Terminate read thread if it exists
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
    // Channels will be checked
    //
    DoRefresh();

    // XL Bitrate: 
    //      Arbitration: 500 kbit/s 
    //      FD: 2 Mbit/s (FD data)
    //      XL: 8 Mbit/s (XL data)
    //
    txtBitrate = "f_clock_mhz=160,brp=1,nom_tseg1=255,nom_tseg2=64,nom_sjw=64,fd_tseg1=63,fd_tseg2=16,fd_sjw=16,fd_ssp_offset=0,xl_tseg1=10,xl_tseg2=9,xl_sjw=9,xl_ssp_offset=10,xl_error_signaling=1,xl_transceiver_mode_switch=0";

    // Parameters for GetValue and SetValue function calls
    //
    cbbParameter.SetCurSel(0);
    OnCbnSelchangeComboparameter();
}

void CPCANBasicExampleDlg::DoRefresh()
{
    TPCANChannelInformation* info;
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
            for (int i = 0; i < (int)iChannelsCount; i++) 
            {
                CANProtocol protocol = CANProtocol::CAN;

                if ((info[i].device_features & FEATURE_XL_CAPABLE) == FEATURE_XL_CAPABLE)
                    protocol = static_cast<CANProtocol>(protocol | CANProtocol::CAN_XL);
                if ((info[i].device_features & FEATURE_FD_CAPABLE) == FEATURE_FD_CAPABLE)
                    protocol = static_cast<CANProtocol>(protocol | CANProtocol::CAN_FD);

                if (info[i].channel_condition & PCAN_CHANNEL_AVAILABLE &&
                    (protocol & CANProtocol::CAN_XL) == CANProtocol::CAN_XL)
                {
                    cbbChannel.AddString(FormatChannelName(info[i].channel_handle, protocol));
                }
            }
                

        cbbChannel.AddString("... refresh ...");
        if (cbbChannel.GetCount() > 1)
            cbbChannel.SetCurSel(0);
        btnInit.EnableWindow(cbbChannel.GetCount() > 0);

        delete[] info;
    }
}

CString CPCANBasicExampleDlg::FormatChannelName(TPCANHandle handle, CANProtocol protocol)
{
    CString result;
    BYTE byChannel;

    // Gets the owner device and channel for a
    // PCAN-Basic handle
    //
    if(handle < 0x100)
        byChannel = (BYTE)(handle) & 0xF;
    else
        byChannel = (BYTE)(handle) & 0xFF;

    // Constructs the PCAN-Basic Channel name and return it
    //
    bool bIsFD = (protocol & CANProtocol::CAN_FD) == CANProtocol::CAN_FD;
    bool bIsXL = (protocol & CANProtocol::CAN_XL) == CANProtocol::CAN_XL;

    LPCSTR stringFormat = "%s:%s %d (%Xh)";

    if (bIsXL && bIsFD)
        result.Format(stringFormat, GetTPCANHandleName(handle), "FD/XL", byChannel, handle);
    else if (bIsFD)
        result.Format(stringFormat, GetTPCANHandleName(handle), "FD", byChannel, handle);
    else if (bIsXL)
        result.Format(stringFormat, GetTPCANHandleName(handle), "XL", byChannel, handle);
    else
        result.Format(stringFormat, GetTPCANHandleName(handle), "", byChannel, handle);
    
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


CString CPCANBasicExampleDlg::GetFormatedError(TPCANStatus error)
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
    btnStatus.EnableWindow(bConnected);
    btnReset.EnableWindow(bConnected);

    // ComboBoxes
    //
    cbbChannel.EnableWindow(!bConnected);

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
        ::MessageBox(NULL, GetFormatedError(stsResult), "Error!",MB_ICONERROR);
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
        IncludeTextMessage(GetFormatedError(stsResult));

    // Configure the way how trace files are created:
    // * Standard name is used
    // * Existing file is overwritten,
    // * Only one file is created.
    // * Recording stops when the file size reaches 5 megabytes.
    //
    iBuffer = TRACE_FILE_SINGLE | TRACE_FILE_OVERWRITE;
    stsResult = m_objPCANBasic->SetValue(m_PcanHandle, PCAN_TRACE_CONFIGURE, (void*)&iBuffer, sizeof(iBuffer));
    if (stsResult != PCAN_ERROR_OK)
        IncludeTextMessage(GetFormatedError(stsResult));
}


void CPCANBasicExampleDlg::OnLbnDblclkListinfo()
{
    OnBnClickedButtoninfoclear();
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtpid()
{
    SetTextForPID();
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtsdt()
{
    SetTextForSDT();
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtvcid()
{
    SetTextForVCID();
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtaf()
{
    SetTextForAF();
}

void CPCANBasicExampleDlg::OnEnKillfocusTxtlength()
{
    UpdateData(TRUE);

    // Convert the text length to a whole number
    //
    int tempTxtInt = atoi(txtLength);

    // Set constraints, so the dlc cannot exceed the XL range from 0 to 2047
    //
    if (tempTxtInt < 0)
    {
        m_dataDlc = 0;
    }
    else if (tempTxtInt > 2047)
    {
        m_dataDlc = 2047;
    }
    else
    {
        m_dataDlc = tempTxtInt;
    }

    // Display the text length on the label
    CString tempTxtLength;
    tempTxtLength.Format("%d", m_dataDlc);
    txtLength = tempTxtLength;

    // Update the dlc label
    GetDlgItem(IDC_LALENGTH)->SetWindowText(__T("Length: ") + IntToStr(m_dataDlc + 1) + __T(" B."));

    // Show datafields correctly
    //
    FillData();
    UpdateData(FALSE);
}
