// PCANBasicExampleDlg.h : header file
//

#pragma once

// Includes
//
#include "afxwin.h"
#include "afxcmn.h"
#include <Math.h>
#include "PCANBasicClass.h"


using namespace std;

// Constant values to make the code more comprehensible
// to add and read items from the List view
//
#define MSG_TYPE        0
#define MSG_PID         1
#define MSG_LENGTH      2
#define MSG_SDT         3
#define MSG_VCID        4
#define MSG_AF          5
#define MSG_TIME        6
#define MSG_COUNT       7
#define MSG_DATA        8

// Critical Section class for thread-safe menbers access
//
#pragma region Critical Section Class
class clsCritical
{
private:
    CRITICAL_SECTION *m_objpCS;
    LONG volatile m_dwOwnerThread;
    LONG volatile m_dwLocked;
    bool volatile m_bDoRecursive;

public:
    explicit clsCritical(CRITICAL_SECTION *cs, bool createUnlocked = false, bool lockRecursively = false);
    ~clsCritical();
    int GetRecursionCount();
    bool IsLocked();
    int Enter();
    int Leave();
};
#pragma endregion

// Enum for different CAN protocols
//

enum CANProtocol
{
    CAN = 0x1,
    CAN_FD = 0x2,
    CAN_XL = 0x4
};

// Message Status structure used to show CAN Messages
// in a ListView
//
#pragma region Message Status class
class MessageStatus
{
private:
    TPCANMsgXL m_Msg;
    TPCANTimestampFD m_TimeStamp;
    TPCANTimestampFD m_oldTimeStamp;
    int m_iIndex;
    int m_Count;
    bool m_bShowPeriod;
    bool m_bWasChanged;

public:
    MessageStatus(TPCANMsgXL canMsg, TPCANTimestampXL canTimestamp, int listIndex);
    void Update(TPCANMsgXL canMsg, TPCANTimestampXL canTimestamp);

    TPCANMsgXL GetCANMsg();
    TPCANTimestampXL GetTimestamp();
    int GetPosition();
    CString GetTypeString();
    CString GetPIDString();
    CString GetSDTString();
    CString GetVCIDString();
    CString GetAFString();
    CString GetDataString();
    CString GetTimeString();
    int GetCount();
    bool GetShowingPeriod();
    bool GetMarkedAsUpdated();

    void SetShowingPeriod(bool value);
    void SetMarkedAsUpdated(bool value);

    __declspec(property (get = GetCANMsg)) TPCANMsgXL CANMsg;
    __declspec(property (get = GetTimestamp)) TPCANTimestampXL Timestamp;
    __declspec(property (get = GetPosition)) int Position;
    __declspec(property (get = GetTypeString)) CString TypeString;
    __declspec(property (get = GetPIDString)) CString PidString;
    __declspec(property (get = GetSDTString)) CString SDTString;
    __declspec(property (get = GetVCIDString)) CString VCIDString;
    __declspec(property (get = GetAFString)) CString AFString;
    __declspec(property (get = GetDataString)) CString DataString;
    __declspec(property (get = GetTimeString)) CString TimeString;
    __declspec(property (get = GetCount)) int Count;
    __declspec(property (get = GetShowingPeriod, put = SetShowingPeriod)) bool ShowingPeriod;
    __declspec(property (get = GetMarkedAsUpdated, put = SetMarkedAsUpdated)) bool MarkedAsUpdated;
};
#pragma endregion

// PCANBasicExampleDlg dialog
class CPCANBasicExampleDlg : public CDialog
{
// Construction
public:
    CPCANBasicExampleDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    enum { IDD = IDD_PCANBASICEXAMPLE_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Connection
    //
    CComboBox cbbChannel;
    CButton btnInit;
    CButton btnRelease;
    CString txtBitrate;

    // Filtering
    CButton rdbFilterOpen;
    CButton rdbFilterClose;
    CButton btnFilterApply;
    CButton btnFilterQuery;

    //Parameter
    CComboBox cbbParameter;
    CButton rdbParameterActive;
    CButton rdbParameterInactive;
    CEdit editParameterDevNumberOrDelay;
    CString txtParameterDevNumber;
    CSpinButtonCtrl nudParameterDevNumberOrDelay;
    CButton btnParameterGet;
    CButton btnParameterSet;

    //Reading
    CButton rdbReadingTimer;
    CButton rdbReadingEvent;
    CButton rdbReadingManual;
    CButton chbReadingTimeStamp;
    CButton btnRead;
    CButton btnReadingClear;
    CListCtrl lstMessages;

    //Writing
    CButton btnWrite;
    CSpinButtonCtrl nudLength;
    CString txtPID;
    CString txtSDT;
    CString txtVCID;
    CString txtAF;
    CString txtLength;
    CButton chbRRS;
    CButton chbSEC;
    CString txtData;
    CStatic labelDeviceOrDelay;

    //Information
    CListBox listBoxInfo;
    CButton btnStatus;
    CButton btnReset;
    CButton btnVersions;

    // Event functions
    //
    void InitializeControls(void);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnCbnSelchangecbbChannel();
    afx_msg void OnLbnDblclkListinfo();
    afx_msg void OnDeltaposNudlength(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkLstmessages(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtninit();
    afx_msg void OnBnClickedBtnrelease();
    afx_msg void OnBnClickedBtnwrite();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnBnClickedRdbtimer();
    afx_msg void OnBnClickedRdbevent();
    afx_msg void OnBnClickedChbtimestamp();
    afx_msg void OnBnClickedButtonfilterapply();
    afx_msg void OnBnClickedButtonfilterquery();
    afx_msg void OnBnClickedRdbmanual();
    afx_msg void OnBnClickedButtonread();
    afx_msg void OnBnClickedButtonreadingclear();
    afx_msg void OnCbnSelchangeComboparameter();
    afx_msg void OnDeltaposNudparamdevnumber(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnKillfocusTxtparamdevnumber();
    afx_msg void OnBnClickedButtonparamset();
    afx_msg void OnBnClickedButtonparamget();
    afx_msg void OnClose();
    afx_msg void OnBnClickedButtonversion();
    afx_msg void OnBnClickedButtoninfoclear();
    afx_msg void OnBnClickedButtonstatus();
    afx_msg void OnBnClickedButtonreset();
    afx_msg void OnEnKillfocusTxtpid();
    afx_msg void OnEnKillfocusTxtvcid();
    afx_msg void OnEnKillfocusTxtsdt();
    afx_msg void OnEnKillfocusTxtaf();

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()


private:
    // ------------------------------------------------------------------------------------------
    // Private Members
    // ------------------------------------------------------------------------------------------
    // Variables to store the current PCANBasic instance
    //
    bool m_Terminated;
    PCANBasicClass *m_objPCANBasic;

    // Saves the handle of a PCAN hardware
    //
    TPCANHandle m_PcanHandle;

    // Variables to store the current reading mode
    // 0 : Timer Mode
    // 1 : Event Mode
    // 2 : Manual Mode
    //
    int m_ActiveReadingMode;

    // Read Timer identifier
    //
    UINT_PTR m_tmrRead;

    //Display Timer identifier
    //
    UINT_PTR m_tmrDisplay;

    // CAN messages array. Store the message status for its display
    //
    CPtrList *m_LastMsgsList;

    // Handle to set Received-Event
    //
    HANDLE m_hEvent;

    // Handle to the thread to read using Received-Event method
    //
    HANDLE m_hThread;

    // CAN data for message writing
    //
    unsigned char* m_data;

    // DLC for data of message
    //
    unsigned int m_dataDlc;

    // Handle for a Critical Section
    //
    CRITICAL_SECTION *m_objpCS;

    // ------------------------------------------------------------------------------------------
    // Help functions
    // ------------------------------------------------------------------------------------------
    
    // Set the text for PID
    //
    void SetTextForPID();

    // Set the text for SDT
    //
    void SetTextForSDT();

    // Set the text for VCID
    //
    void SetTextForVCID();

    // Set the text for AF
    //
    void SetTextForAF();

    // Convert a int value to a CString
    //

    CString IntToStr(int iValue);
    // Convert a int value to a CString formated in Hexadecimal
    //
    CString IntToHex(int iValue, short iDigits);
    // Convert hexadecimal Cstring into int value (Zero if error)
    //
    DWORD HexTextToInt(CString ToConvert);
    // Check txtData in an hexadecimal value
    //
    void CheckHexEditBox(CString*);
    // Create a LVItem renderer for MFC ListView
    //
    int AddLVItem(CString Caption);
    // Enable/Disable Read Timer
    //
    void SetTimerRead(bool bEnable);
    // Enable/Disable Display Timer
    //
    void SetTimerDisplay(bool bEnable);
    // Configures the data of all ComboBox components of the main-form
    //
    void FillComboBoxData();
    // Gets the formated text for a PCAN-Basic channel handle
    //
    CString FormatChannelName(TPCANHandle handle, CANProtocol protocol);

    void DoRefresh();
    // Gets the name for a PCAN-Basic channel handle
    //
    CString GetTPCANHandleName(TPCANHandle handle);
    // Help Function used to get an error as text
    //
    CString GetFormatedError(TPCANStatus error);
    //Activates/deaactivates the different controls of the main-form according
    //with the current connection status
    //
    void SetConnectionStatus(bool bConnected);
    // Gets the current status of the PCAN-Basic message filter
    //
    bool GetFilterStatus(int* status);
    // Includes a new line of text into the information Listview
    //
    void IncludeTextMessage(CString strMsg);
    // Gets ComboBox selected label
    //
    CString GetComboBoxSelectedLabel(CComboBox* ccb);
    // Configures the Debug-Log file of PCAN-Basic
    //
    void ConfigureLogFile();
    // Configures the PCAN-Trace file for a PCAN-Basic Channel
    //
    void ConfigureTraceFile();

    // ------------------------------------------------------------------------------------------
    // Message-proccessing functions
    // ------------------------------------------------------------------------------------------
    // Display CAN messages in the Message-ListView
    //
    void DisplayMessages();
    // Create new MessageStatus using provided parameters
    //
    void InsertMsgEntry(TPCANMsgXL NewMsg, TPCANTimestampXL MyTimeStamp);
    // Processes a received message, in order to show it in the Message-ListView
    //
    void ProcessMessage(TPCANMsgXL theMsg, TPCANTimestampXL itsTimeStamp);
    // static Thread function to manage reading by event
    //
    static DWORD WINAPI CallCANReadThreadFunc(LPVOID lpParam);
    // member Thread function to manage reading by event
    //
    DWORD WINAPI CANReadThreadFunc(LPVOID lpParam);
    // Manage Reading method (Timer, Event or manual)
    //
    void ReadingModeChanged();
    // Functions for reading PCAN-Basic messages
    //
    TPCANStatus ReadMessageXL();
    void ReadMessages();
    // Functions for writing PCAN-Basic messages
    //
    TPCANStatus WriteFrameXL();

    // Critical section Ini/deinit functions
    //
    void InitializeProtection();
    void FinalizeProtection();

    // Generates a random dataset value with a given length and displays it in txt txtData-Control
    //
    void FillData();
    void RandomizeData();
    void InitializeData();
public:
    afx_msg void OnEnKillfocusTxtlength();
};
