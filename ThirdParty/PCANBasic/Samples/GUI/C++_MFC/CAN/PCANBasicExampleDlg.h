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
#define MSG_ID          1
#define MSG_LENGTH      2
#define MSG_COUNT       3
#define MSG_TIME        4
#define MSG_DATA        5
#define TPCANTimestampULong  UINT64

// Enum for different CAN protocols
//
enum CANProtocol
{
    CAN = 0x1,
    CAN_FD = 0x2,
    CAN_XL = 0x4
};

// Critical Section class for thread-safe members access
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

// Message Status structure used to show CAN Messages
// in a ListView
//
#pragma region Message Status class
class MessageStatus
{
private:
    TPCANMsg m_Msg;
    TPCANTimestampULong m_TimeStamp;
    TPCANTimestampULong m_oldTimeStamp;
    int m_iIndex;
    int m_Count;
    bool m_bShowPeriod;
    bool m_bWasChanged;

public:
    MessageStatus(TPCANMsg canMsg, TPCANTimestampULong canTimestamp, int listIndex);
    void Update(TPCANMsg canMsg, TPCANTimestampULong canTimestamp);

    TPCANMsg GetCANMsg();
    TPCANTimestampULong GetTimestamp();
    int GetPosition();
    CString GetTypeString();
    CString GetIdString();
    CString GetDataString();
    CString GetTimeString();
    int GetCount();
    bool GetShowingPeriod();
    bool GetMarkedAsUpdated();

    void SetShowingPeriod(bool value);
    void SetMarkedAsUpdated(bool value);

    __declspec(property (get = GetCANMsg)) TPCANMsg CANMsg;
    __declspec(property (get = GetTimestamp)) TPCANTimestampULong Timestamp;
    __declspec(property (get = GetPosition)) int Position;
    __declspec(property (get = GetTypeString)) CString TypeString;
    __declspec(property (get = GetIdString)) CString IdString;
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
    CButton btnRefresh;
    CComboBox cbbBaudrates;
    CButton btnInit;
    CButton btnRelease;

    // Filtering
    CButton chbFilterExtended;
    CButton rdbFilterOpen;
    CButton rdbFilterClose;
    CButton rdbFilterCustom;
    CString txtFilterFrom;
    CString txtFilterTo;
    CSpinButtonCtrl nudFilterFrom;
    CSpinButtonCtrl nudFilterTo;
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
    CString txtID;
    CString txtLength;
    CString txtData0;
    CString txtData1;
    CString txtData2;
    CString txtData3;
    CString txtData4;
    CString txtData5;
    CString txtData6;
    CString txtData7;
    BOOL chbExtended;
    BOOL chbRemote;

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
    afx_msg void OnCbnSelchangeCbbbaudrates();
    afx_msg void OnEnKillfocusTxtid();
    afx_msg void OnEnKillfocusTxtlength();
    afx_msg void OnEnKillfocusTxtdata0();
    afx_msg void OnEnKillfocusTxtdata1();
    afx_msg void OnEnKillfocusTxtdata2();
    afx_msg void OnEnKillfocusTxtdata3();
    afx_msg void OnEnKillfocusTxtdata4();
    afx_msg void OnEnKillfocusTxtdata5();
    afx_msg void OnEnKillfocusTxtdata6();
    afx_msg void OnEnKillfocusTxtdata7();
    afx_msg void OnLbnDblclkListinfo();
    afx_msg void OnDeltaposNudlength(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedChbextended();
    afx_msg void OnBnClickedChbremote();
    afx_msg void OnNMDblclkLstmessages(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtninit();
    afx_msg void OnBnClickedBtnrelease();
    afx_msg void OnBnClickedBtnwrite();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnBnClickedRdbtimer();
    afx_msg void OnBnClickedRdbevent();
    afx_msg void OnBnClickedChbtimestamp();
    afx_msg void OnBnClickedButtonHwrefresh();
    afx_msg void OnBnClickedChbfilterextended();
    afx_msg void OnDeltaposNudfilterfrom(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnKillfocusTxtfilterfrom();
    afx_msg void OnDeltaposNudfilterto(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnKillfocusTxtfilterto();
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

    // Saves the baudrate register for a conenction
    //
    TPCANBaudrate m_Baudrate;

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

    // Handle for a Critical Section
    //
    CRITICAL_SECTION *m_objpCS;

    // ------------------------------------------------------------------------------------------
    // Help functions
    // ------------------------------------------------------------------------------------------
    // Convert a int value to a CString
    //
    CString IntToStr(int iValue);
    // Convert a int value to a CString formatted in Hexadecimal
    //
    CString IntToHex(int iValue, short iDigits);
    // Convert hexadecimal Cstring into int value (Zero if error)
    //
    DWORD HexTextToInt(CString ToConvert);
    // Check txtData in an hexadecimal value
    //
    void CheckHexEditBox(CString* txtData);
    // Enables/Disables Data text boxes according with a given length
    //
    void EnableDisableDataFields(int length);
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
    // Gets the formatted text for a PCAN-Basic channel handle
    //
    CString FormatChannelName(TPCANHandle handle, CANProtocol protocol);
    // Gets the name for a PCAN-Basic channel handle
    //
    CString GetTPCANHandleName(TPCANHandle handle);
    // Help Function used to get an error as text
    //
    CString GetFormattedError(TPCANStatus error);
    //Activates/deactivates the different controls of the main-form according
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
    // Message-processing functions
    // ------------------------------------------------------------------------------------------
    // Display CAN messages in the Message-ListView
    //
    void DisplayMessages();
    // Create new MessageStatus using provided parameters
    //
    void InsertMsgEntry(TPCANMsg NewMsg, TPCANTimestampULong MyTimeStamp);
    // Processes a received message, in order to show it in the Message-ListView
    //
    void ProcessMessage(TPCANMsg theMsg, TPCANTimestamp itsTimeStamp);
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
    TPCANStatus ReadMessage();
    void ReadMessages();
    // Functions for writing PCAN-Basic messages
    //
    TPCANStatus WriteFrame();

    // Critical section Ini/deinit functions
    //
    void InitializeProtection();
    void FinalizeProtection();

public:

    CStatic labelDeviceOrDelay;
};
