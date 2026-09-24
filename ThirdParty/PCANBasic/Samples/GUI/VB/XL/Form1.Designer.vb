<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class Form1
    Inherits System.Windows.Forms.Form

    ''' <summary>
    ''' Clean up any resources being used.
    ''' </summary>
    ''' <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    ''' <summary>
    ''' Required designer variable.
    ''' </summary>
    Private components As System.ComponentModel.IContainer

    ''' <summary>
    ''' Required method for Designer support - do not modify
    ''' the contents of this method with the code editor.
    ''' </summary>
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
        Me.chbShowPeriod = New System.Windows.Forms.CheckBox()
        Me.rdbManual = New System.Windows.Forms.RadioButton()
        Me.rdbEvent = New System.Windows.Forms.RadioButton()
        Me.btnMsgClear = New System.Windows.Forms.Button()
        Me.groupBox5 = New System.Windows.Forms.GroupBox()
        Me.lstMessages = New System.Windows.Forms.ListView()
        Me.clhType = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhPID = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhDLC = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhSDT = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhVCID = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhAF = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhRcvTime = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhCount = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhData = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.rdbTimer = New System.Windows.Forms.RadioButton()
        Me.btnRead = New System.Windows.Forms.Button()
        Me.groupBox6 = New System.Windows.Forms.GroupBox()
        Me.btnGenerate = New System.Windows.Forms.Button()
        Me.btnEdit = New System.Windows.Forms.Button()
        Me.label13 = New System.Windows.Forms.Label()
        Me.label7 = New System.Windows.Forms.Label()
        Me.nudDLC = New System.Windows.Forms.NumericUpDown()
        Me.txtData = New System.Windows.Forms.TextBox()
        Me.txtPID = New System.Windows.Forms.TextBox()
        Me.label5 = New System.Windows.Forms.Label()
        Me.laLength = New System.Windows.Forms.Label()
        Me.label12 = New System.Windows.Forms.Label()
        Me.txtAF = New System.Windows.Forms.TextBox()
        Me.btnWrite = New System.Windows.Forms.Button()
        Me.label3 = New System.Windows.Forms.Label()
        Me.chbRRS = New System.Windows.Forms.CheckBox()
        Me.txtVCID = New System.Windows.Forms.TextBox()
        Me.chbSEC = New System.Windows.Forms.CheckBox()
        Me.label4 = New System.Windows.Forms.Label()
        Me.txtSDT = New System.Windows.Forms.TextBox()
        Me.groupBox4 = New System.Windows.Forms.GroupBox()
        Me.btnReset = New System.Windows.Forms.Button()
        Me.btnStatus = New System.Windows.Forms.Button()
        Me.btnGetVersions = New System.Windows.Forms.Button()
        Me.lbxInfo = New System.Windows.Forms.ListBox()
        Me.btnInfoClear = New System.Windows.Forms.Button()
        Me.rdbParamInactive = New System.Windows.Forms.RadioButton()
        Me.tmrRead = New System.Windows.Forms.Timer(Me.components)
        Me.btnParameterGet = New System.Windows.Forms.Button()
        Me.label10 = New System.Windows.Forms.Label()
        Me.nudDeviceId = New System.Windows.Forms.NumericUpDown()
        Me.laDeviceOrDelay = New System.Windows.Forms.Label()
        Me.cbbParameter = New System.Windows.Forms.ComboBox()
        Me.groupBox2 = New System.Windows.Forms.GroupBox()
        Me.nudDelay = New System.Windows.Forms.NumericUpDown()
        Me.label6 = New System.Windows.Forms.Label()
        Me.rdbParamActive = New System.Windows.Forms.RadioButton()
        Me.btnParameterSet = New System.Windows.Forms.Button()
        Me.tmrDisplay = New System.Windows.Forms.Timer(Me.components)
        Me.groupBox1 = New System.Windows.Forms.GroupBox()
        Me.label2 = New System.Windows.Forms.Label()
        Me.label1 = New System.Windows.Forms.Label()
        Me.txtBitrate = New System.Windows.Forms.TextBox()
        Me.cbbChannel = New System.Windows.Forms.ComboBox()
        Me.btnInit = New System.Windows.Forms.Button()
        Me.btnRelease = New System.Windows.Forms.Button()
        Me.groupBox3 = New System.Windows.Forms.GroupBox()
        Me.rdbFilterOpen = New System.Windows.Forms.RadioButton()
        Me.rdbFilterClose = New System.Windows.Forms.RadioButton()
        Me.btnFilterQuery = New System.Windows.Forms.Button()
        Me.btnFilterApply = New System.Windows.Forms.Button()
        Me.groupBox5.SuspendLayout()
        Me.groupBox6.SuspendLayout()
        CType(Me.nudDLC, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.groupBox4.SuspendLayout()
        CType(Me.nudDeviceId, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.groupBox2.SuspendLayout()
        CType(Me.nudDelay, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.groupBox1.SuspendLayout()
        Me.groupBox3.SuspendLayout()
        Me.SuspendLayout()
        '
        'chbShowPeriod
        '
        Me.chbShowPeriod.AutoSize = True
        Me.chbShowPeriod.Location = New System.Drawing.Point(374, 15)
        Me.chbShowPeriod.Name = "chbShowPeriod"
        Me.chbShowPeriod.Size = New System.Drawing.Size(123, 17)
        Me.chbShowPeriod.TabIndex = 75
        Me.chbShowPeriod.Text = "Timestamp as period"
        Me.chbShowPeriod.UseVisualStyleBackColor = True
        '
        'rdbManual
        '
        Me.rdbManual.AutoSize = True
        Me.rdbManual.Location = New System.Drawing.Point(276, 14)
        Me.rdbManual.Name = "rdbManual"
        Me.rdbManual.Size = New System.Drawing.Size(89, 17)
        Me.rdbManual.TabIndex = 74
        Me.rdbManual.Text = "Manual Read"
        Me.rdbManual.UseVisualStyleBackColor = True
        '
        'rdbEvent
        '
        Me.rdbEvent.AutoSize = True
        Me.rdbEvent.Location = New System.Drawing.Point(131, 14)
        Me.rdbEvent.Name = "rdbEvent"
        Me.rdbEvent.Size = New System.Drawing.Size(139, 17)
        Me.rdbEvent.TabIndex = 73
        Me.rdbEvent.Text = "Reading using an Event"
        Me.rdbEvent.UseVisualStyleBackColor = True
        '
        'btnMsgClear
        '
        Me.btnMsgClear.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnMsgClear.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnMsgClear.Location = New System.Drawing.Point(643, 65)
        Me.btnMsgClear.Name = "btnMsgClear"
        Me.btnMsgClear.Size = New System.Drawing.Size(65, 23)
        Me.btnMsgClear.TabIndex = 50
        Me.btnMsgClear.Text = "Clear"
        Me.btnMsgClear.UseVisualStyleBackColor = True
        '
        'groupBox5
        '
        Me.groupBox5.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox5.Controls.Add(Me.lstMessages)
        Me.groupBox5.Controls.Add(Me.chbShowPeriod)
        Me.groupBox5.Controls.Add(Me.rdbManual)
        Me.groupBox5.Controls.Add(Me.rdbEvent)
        Me.groupBox5.Controls.Add(Me.btnMsgClear)
        Me.groupBox5.Controls.Add(Me.rdbTimer)
        Me.groupBox5.Controls.Add(Me.btnRead)
        Me.groupBox5.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.groupBox5.Location = New System.Drawing.Point(8, 209)
        Me.groupBox5.Name = "groupBox5"
        Me.groupBox5.Size = New System.Drawing.Size(714, 140)
        Me.groupBox5.TabIndex = 56
        Me.groupBox5.TabStop = False
        Me.groupBox5.Text = " Messages Reading "
        '
        'lstMessages
        '
        Me.lstMessages.Columns.AddRange(New System.Windows.Forms.ColumnHeader() {Me.clhType, Me.clhPID, Me.clhDLC, Me.clhSDT, Me.clhVCID, Me.clhAF, Me.clhRcvTime, Me.clhCount, Me.clhData})
        Me.lstMessages.FullRowSelect = True
        Me.lstMessages.HideSelection = False
        Me.lstMessages.Location = New System.Drawing.Point(8, 38)
        Me.lstMessages.MultiSelect = False
        Me.lstMessages.Name = "lstMessages"
        Me.lstMessages.Size = New System.Drawing.Size(629, 96)
        Me.lstMessages.TabIndex = 76
        Me.lstMessages.UseCompatibleStateImageBehavior = False
        Me.lstMessages.View = System.Windows.Forms.View.Details
        '
        'clhType
        '
        Me.clhType.Text = "Type"
        Me.clhType.Width = 80
        '
        'clhPID
        '
        Me.clhPID.Text = "PID"
        Me.clhPID.Width = 70
        '
        'clhDLC
        '
        Me.clhDLC.Text = "Length"
        Me.clhDLC.Width = 90
        '
        'clhSDT
        '
        Me.clhSDT.Text = "SDT"
        Me.clhSDT.Width = 70
        '
        'clhVCID
        '
        Me.clhVCID.Text = "VCID"
        Me.clhVCID.Width = 70
        '
        'clhAF
        '
        Me.clhAF.Text = "AF"
        Me.clhAF.Width = 110
        '
        'clhRcvTime
        '
        Me.clhRcvTime.Text = "Rcv Time"
        Me.clhRcvTime.Width = 110
        '
        'clhCount
        '
        Me.clhCount.Text = "Count"
        Me.clhCount.Width = 90
        '
        'clhData
        '
        Me.clhData.Text = "Data"
        Me.clhData.Width = 500
        '
        'rdbTimer
        '
        Me.rdbTimer.AutoSize = True
        Me.rdbTimer.Location = New System.Drawing.Point(8, 14)
        Me.rdbTimer.Name = "rdbTimer"
        Me.rdbTimer.Size = New System.Drawing.Size(117, 17)
        Me.rdbTimer.TabIndex = 72
        Me.rdbTimer.Text = "Read using a Timer"
        Me.rdbTimer.UseVisualStyleBackColor = True
        '
        'btnRead
        '
        Me.btnRead.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnRead.Enabled = False
        Me.btnRead.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnRead.Location = New System.Drawing.Point(643, 36)
        Me.btnRead.Name = "btnRead"
        Me.btnRead.Size = New System.Drawing.Size(65, 23)
        Me.btnRead.TabIndex = 49
        Me.btnRead.Text = "Read"
        Me.btnRead.UseVisualStyleBackColor = True
        '
        'groupBox6
        '
        Me.groupBox6.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox6.Controls.Add(Me.btnGenerate)
        Me.groupBox6.Controls.Add(Me.btnEdit)
        Me.groupBox6.Controls.Add(Me.label13)
        Me.groupBox6.Controls.Add(Me.label7)
        Me.groupBox6.Controls.Add(Me.nudDLC)
        Me.groupBox6.Controls.Add(Me.txtData)
        Me.groupBox6.Controls.Add(Me.txtPID)
        Me.groupBox6.Controls.Add(Me.label5)
        Me.groupBox6.Controls.Add(Me.laLength)
        Me.groupBox6.Controls.Add(Me.label12)
        Me.groupBox6.Controls.Add(Me.txtAF)
        Me.groupBox6.Controls.Add(Me.btnWrite)
        Me.groupBox6.Controls.Add(Me.label3)
        Me.groupBox6.Controls.Add(Me.chbRRS)
        Me.groupBox6.Controls.Add(Me.txtVCID)
        Me.groupBox6.Controls.Add(Me.chbSEC)
        Me.groupBox6.Controls.Add(Me.label4)
        Me.groupBox6.Controls.Add(Me.txtSDT)
        Me.groupBox6.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.groupBox6.Location = New System.Drawing.Point(8, 355)
        Me.groupBox6.Name = "groupBox6"
        Me.groupBox6.Size = New System.Drawing.Size(714, 149)
        Me.groupBox6.TabIndex = 57
        Me.groupBox6.TabStop = False
        Me.groupBox6.Text = "Write Messages"
        '
        'btnGenerate
        '
        Me.btnGenerate.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnGenerate.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnGenerate.Location = New System.Drawing.Point(643, 82)
        Me.btnGenerate.Name = "btnGenerate"
        Me.btnGenerate.Size = New System.Drawing.Size(65, 22)
        Me.btnGenerate.TabIndex = 137
        Me.btnGenerate.Text = "Generate"
        '
        'btnEdit
        '
        Me.btnEdit.Location = New System.Drawing.Point(643, 52)
        Me.btnEdit.Name = "btnEdit"
        Me.btnEdit.Size = New System.Drawing.Size(65, 22)
        Me.btnEdit.TabIndex = 136
        Me.btnEdit.Text = "Edit"
        Me.btnEdit.UseVisualStyleBackColor = True
        '
        'label13
        '
        Me.label13.AutoSize = True
        Me.label13.Location = New System.Drawing.Point(7, 29)
        Me.label13.Name = "label13"
        Me.label13.Size = New System.Drawing.Size(56, 13)
        Me.label13.TabIndex = 129
        Me.label13.Text = "PID (Hex):"
        '
        'label7
        '
        Me.label7.AutoSize = True
        Me.label7.Location = New System.Drawing.Point(242, 29)
        Me.label7.Name = "label7"
        Me.label7.Size = New System.Drawing.Size(33, 13)
        Me.label7.TabIndex = 135
        Me.label7.Text = "Data:"
        '
        'nudDLC
        '
        Me.nudDLC.BackColor = System.Drawing.Color.White
        Me.nudDLC.Location = New System.Drawing.Point(77, 51)
        Me.nudDLC.Maximum = New Decimal(New Integer() {2047, 0, 0, 0})
        Me.nudDLC.Name = "nudDLC"
        Me.nudDLC.Size = New System.Drawing.Size(52, 20)
        Me.nudDLC.TabIndex = 122
        '
        'txtData
        '
        Me.txtData.Location = New System.Drawing.Point(245, 51)
        Me.txtData.Multiline = True
        Me.txtData.Name = "txtData"
        Me.txtData.ReadOnly = True
        Me.txtData.ScrollBars = System.Windows.Forms.ScrollBars.Both
        Me.txtData.Size = New System.Drawing.Size(392, 82)
        Me.txtData.TabIndex = 134
        Me.txtData.TabStop = False
        '
        'txtPID
        '
        Me.txtPID.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtPID.Location = New System.Drawing.Point(10, 51)
        Me.txtPID.MaxLength = 3
        Me.txtPID.Name = "txtPID"
        Me.txtPID.Size = New System.Drawing.Size(53, 20)
        Me.txtPID.TabIndex = 121
        Me.txtPID.Text = "000"
        '
        'label5
        '
        Me.label5.AutoSize = True
        Me.label5.Location = New System.Drawing.Point(146, 91)
        Me.label5.Name = "label5"
        Me.label5.Size = New System.Drawing.Size(51, 13)
        Me.label5.TabIndex = 133
        Me.label5.Text = "AF (Hex):"
        '
        'laLength
        '
        Me.laLength.AutoSize = True
        Me.laLength.Location = New System.Drawing.Point(146, 29)
        Me.laLength.Name = "laLength"
        Me.laLength.Size = New System.Drawing.Size(65, 13)
        Me.laLength.TabIndex = 130
        Me.laLength.Text = "Length: 9 B."
        '
        'label12
        '
        Me.label12.AutoSize = True
        Me.label12.Location = New System.Drawing.Point(74, 29)
        Me.label12.Name = "label12"
        Me.label12.Size = New System.Drawing.Size(31, 13)
        Me.label12.TabIndex = 130
        Me.label12.Text = "DLC:"
        '
        'txtAF
        '
        Me.txtAF.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtAF.Location = New System.Drawing.Point(149, 113)
        Me.txtAF.MaxLength = 8
        Me.txtAF.Name = "txtAF"
        Me.txtAF.Size = New System.Drawing.Size(90, 20)
        Me.txtAF.TabIndex = 127
        Me.txtAF.Text = "00000000"
        '
        'btnWrite
        '
        Me.btnWrite.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnWrite.Enabled = False
        Me.btnWrite.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnWrite.Location = New System.Drawing.Point(643, 112)
        Me.btnWrite.Name = "btnWrite"
        Me.btnWrite.Size = New System.Drawing.Size(65, 22)
        Me.btnWrite.TabIndex = 128
        Me.btnWrite.Text = "Write"
        '
        'label3
        '
        Me.label3.AutoSize = True
        Me.label3.Location = New System.Drawing.Point(74, 91)
        Me.label3.Name = "label3"
        Me.label3.Size = New System.Drawing.Size(63, 13)
        Me.label3.TabIndex = 132
        Me.label3.Text = "VCID (Hex):"
        '
        'chbRRS
        '
        Me.chbRRS.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.chbRRS.Location = New System.Drawing.Point(149, 51)
        Me.chbRRS.Name = "chbRRS"
        Me.chbRRS.Size = New System.Drawing.Size(44, 24)
        Me.chbRRS.TabIndex = 123
        Me.chbRRS.Text = "RRS"
        '
        'txtVCID
        '
        Me.txtVCID.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtVCID.Location = New System.Drawing.Point(77, 113)
        Me.txtVCID.MaxLength = 2
        Me.txtVCID.Name = "txtVCID"
        Me.txtVCID.Size = New System.Drawing.Size(53, 20)
        Me.txtVCID.TabIndex = 126
        Me.txtVCID.Text = "00"
        '
        'chbSEC
        '
        Me.chbSEC.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.chbSEC.Location = New System.Drawing.Point(199, 51)
        Me.chbSEC.Name = "chbSEC"
        Me.chbSEC.Size = New System.Drawing.Size(40, 24)
        Me.chbSEC.TabIndex = 124
        Me.chbSEC.Text = "SEC"
        '
        'label4
        '
        Me.label4.AutoSize = True
        Me.label4.Location = New System.Drawing.Point(7, 91)
        Me.label4.Name = "label4"
        Me.label4.Size = New System.Drawing.Size(60, 13)
        Me.label4.TabIndex = 131
        Me.label4.Text = "SDT (Hex):"
        '
        'txtSDT
        '
        Me.txtSDT.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtSDT.Location = New System.Drawing.Point(10, 113)
        Me.txtSDT.MaxLength = 2
        Me.txtSDT.Name = "txtSDT"
        Me.txtSDT.Size = New System.Drawing.Size(53, 20)
        Me.txtSDT.TabIndex = 125
        Me.txtSDT.Text = "00"
        '
        'groupBox4
        '
        Me.groupBox4.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox4.Controls.Add(Me.btnReset)
        Me.groupBox4.Controls.Add(Me.btnStatus)
        Me.groupBox4.Controls.Add(Me.btnGetVersions)
        Me.groupBox4.Controls.Add(Me.lbxInfo)
        Me.groupBox4.Controls.Add(Me.btnInfoClear)
        Me.groupBox4.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.groupBox4.Location = New System.Drawing.Point(8, 517)
        Me.groupBox4.Name = "groupBox4"
        Me.groupBox4.Size = New System.Drawing.Size(714, 87)
        Me.groupBox4.TabIndex = 55
        Me.groupBox4.TabStop = False
        Me.groupBox4.Text = "Information"
        '
        'btnReset
        '
        Me.btnReset.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnReset.Enabled = False
        Me.btnReset.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnReset.Location = New System.Drawing.Point(643, 49)
        Me.btnReset.Name = "btnReset"
        Me.btnReset.Size = New System.Drawing.Size(65, 22)
        Me.btnReset.TabIndex = 63
        Me.btnReset.Text = "Reset"
        Me.btnReset.UseVisualStyleBackColor = True
        '
        'btnStatus
        '
        Me.btnStatus.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnStatus.Enabled = False
        Me.btnStatus.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnStatus.Location = New System.Drawing.Point(572, 49)
        Me.btnStatus.Name = "btnStatus"
        Me.btnStatus.Size = New System.Drawing.Size(65, 22)
        Me.btnStatus.TabIndex = 62
        Me.btnStatus.Text = "Status"
        Me.btnStatus.UseVisualStyleBackColor = True
        '
        'btnGetVersions
        '
        Me.btnGetVersions.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnGetVersions.Enabled = False
        Me.btnGetVersions.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnGetVersions.Location = New System.Drawing.Point(572, 21)
        Me.btnGetVersions.Name = "btnGetVersions"
        Me.btnGetVersions.Size = New System.Drawing.Size(65, 22)
        Me.btnGetVersions.TabIndex = 60
        Me.btnGetVersions.Text = "Versions"
        Me.btnGetVersions.UseVisualStyleBackColor = True
        '
        'lbxInfo
        '
        Me.lbxInfo.FormattingEnabled = True
        Me.lbxInfo.Items.AddRange(New Object() {"Select a Hardware and a configuration for it. Then click ""Initialize"" button", "When activated, the Debug-Log file will be found in the same directory as this ap" &
                "plication", "When activated, the PCAN-Trace file will be found in the same directory as this a" &
                "pplication"})
        Me.lbxInfo.Location = New System.Drawing.Point(10, 19)
        Me.lbxInfo.Name = "lbxInfo"
        Me.lbxInfo.ScrollAlwaysVisible = True
        Me.lbxInfo.Size = New System.Drawing.Size(558, 56)
        Me.lbxInfo.TabIndex = 61
        '
        'btnInfoClear
        '
        Me.btnInfoClear.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnInfoClear.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnInfoClear.Location = New System.Drawing.Point(643, 21)
        Me.btnInfoClear.Name = "btnInfoClear"
        Me.btnInfoClear.Size = New System.Drawing.Size(65, 22)
        Me.btnInfoClear.TabIndex = 59
        Me.btnInfoClear.Text = "Clear"
        Me.btnInfoClear.UseVisualStyleBackColor = True
        '
        'rdbParamInactive
        '
        Me.rdbParamInactive.Location = New System.Drawing.Point(300, 32)
        Me.rdbParamInactive.Name = "rdbParamInactive"
        Me.rdbParamInactive.Size = New System.Drawing.Size(67, 17)
        Me.rdbParamInactive.TabIndex = 0
        Me.rdbParamInactive.Text = "Inactive"
        Me.rdbParamInactive.UseVisualStyleBackColor = True
        '
        'tmrRead
        '
        Me.tmrRead.Interval = 50
        '
        'btnParameterGet
        '
        Me.btnParameterGet.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnParameterGet.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnParameterGet.Location = New System.Drawing.Point(643, 28)
        Me.btnParameterGet.Name = "btnParameterGet"
        Me.btnParameterGet.Size = New System.Drawing.Size(65, 22)
        Me.btnParameterGet.TabIndex = 54
        Me.btnParameterGet.Text = "Get"
        Me.btnParameterGet.UseVisualStyleBackColor = True
        '
        'label10
        '
        Me.label10.Location = New System.Drawing.Point(241, 11)
        Me.label10.Name = "label10"
        Me.label10.Size = New System.Drawing.Size(59, 23)
        Me.label10.TabIndex = 46
        Me.label10.Text = "Activation:"
        '
        'nudDeviceId
        '
        Me.nudDeviceId.Enabled = False
        Me.nudDeviceId.Hexadecimal = True
        Me.nudDeviceId.Location = New System.Drawing.Point(408, 29)
        Me.nudDeviceId.Maximum = New Decimal(New Integer() {-1, 0, 0, 0})
        Me.nudDeviceId.Name = "nudDeviceId"
        Me.nudDeviceId.Size = New System.Drawing.Size(99, 20)
        Me.nudDeviceId.TabIndex = 6
        '
        'laDeviceOrDelay
        '
        Me.laDeviceOrDelay.Location = New System.Drawing.Point(405, 12)
        Me.laDeviceOrDelay.Name = "laDeviceOrDelay"
        Me.laDeviceOrDelay.Size = New System.Drawing.Size(102, 23)
        Me.laDeviceOrDelay.TabIndex = 45
        Me.laDeviceOrDelay.Text = "Device ID (Hex):"
        '
        'cbbParameter
        '
        Me.cbbParameter.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.cbbParameter.FormattingEnabled = True
        Me.cbbParameter.Items.AddRange(New Object() {"Device ID", "5V Power", "Auto-reset on BUS-OFF", "CAN Listen-Only", "Debug's Log", "Receive Status", "CAN Controller Number", "Trace File", "Channel Identification (USB)", "Channel Capabilities", "Bit rate Adaptation", "Get Bit rate Nominal Information", "Get Bit rate FD Information", "Get Bit rate XL Information", "Get CAN Nominal Speed Bit/s", "Get CAN FD Speed Bit/s", "Get CAN XL Speed Bit/s" & Global.Microsoft.VisualBasic.ChrW(9), "Get IP Address", "Get LAN Service Status", "Reception of Status Frames", "Reception of RTR Frames", "Reception of Error Frames", "Interframe Transmit Delay", "Reception of Echo Frames", "Hard Reset Status", "Communication Direction", "Global Unique Identifier (GUID)"})
        Me.cbbParameter.Location = New System.Drawing.Point(10, 31)
        Me.cbbParameter.Name = "cbbParameter"
        Me.cbbParameter.Size = New System.Drawing.Size(217, 21)
        Me.cbbParameter.TabIndex = 44
        '
        'groupBox2
        '
        Me.groupBox2.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox2.Controls.Add(Me.nudDelay)
        Me.groupBox2.Controls.Add(Me.btnParameterGet)
        Me.groupBox2.Controls.Add(Me.label10)
        Me.groupBox2.Controls.Add(Me.nudDeviceId)
        Me.groupBox2.Controls.Add(Me.laDeviceOrDelay)
        Me.groupBox2.Controls.Add(Me.cbbParameter)
        Me.groupBox2.Controls.Add(Me.label6)
        Me.groupBox2.Controls.Add(Me.rdbParamActive)
        Me.groupBox2.Controls.Add(Me.rdbParamInactive)
        Me.groupBox2.Controls.Add(Me.btnParameterSet)
        Me.groupBox2.Location = New System.Drawing.Point(8, 151)
        Me.groupBox2.Name = "groupBox2"
        Me.groupBox2.Size = New System.Drawing.Size(714, 58)
        Me.groupBox2.TabIndex = 54
        Me.groupBox2.TabStop = False
        Me.groupBox2.Text = " Configuration Parameters "
        '
        'nudDelay
        '
        Me.nudDelay.Location = New System.Drawing.Point(408, 29)
        Me.nudDelay.Maximum = New Decimal(New Integer() {-1, 0, 0, 0})
        Me.nudDelay.Name = "nudDelay"
        Me.nudDelay.Size = New System.Drawing.Size(99, 20)
        Me.nudDelay.TabIndex = 55
        '
        'label6
        '
        Me.label6.Location = New System.Drawing.Point(7, 14)
        Me.label6.Name = "label6"
        Me.label6.Size = New System.Drawing.Size(64, 23)
        Me.label6.TabIndex = 43
        Me.label6.Text = "Parameter:"
        '
        'rdbParamActive
        '
        Me.rdbParamActive.Location = New System.Drawing.Point(238, 32)
        Me.rdbParamActive.Name = "rdbParamActive"
        Me.rdbParamActive.Size = New System.Drawing.Size(56, 17)
        Me.rdbParamActive.TabIndex = 2
        Me.rdbParamActive.Text = "Active"
        Me.rdbParamActive.UseVisualStyleBackColor = True
        '
        'btnParameterSet
        '
        Me.btnParameterSet.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnParameterSet.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnParameterSet.Location = New System.Drawing.Point(572, 28)
        Me.btnParameterSet.Name = "btnParameterSet"
        Me.btnParameterSet.Size = New System.Drawing.Size(65, 22)
        Me.btnParameterSet.TabIndex = 46
        Me.btnParameterSet.Text = "Set"
        Me.btnParameterSet.UseVisualStyleBackColor = True
        '
        'tmrDisplay
        '
        '
        'groupBox1
        '
        Me.groupBox1.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox1.Controls.Add(Me.label2)
        Me.groupBox1.Controls.Add(Me.label1)
        Me.groupBox1.Controls.Add(Me.txtBitrate)
        Me.groupBox1.Controls.Add(Me.cbbChannel)
        Me.groupBox1.Controls.Add(Me.btnInit)
        Me.groupBox1.Controls.Add(Me.btnRelease)
        Me.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.groupBox1.Location = New System.Drawing.Point(8, 8)
        Me.groupBox1.Name = "groupBox1"
        Me.groupBox1.Size = New System.Drawing.Size(714, 70)
        Me.groupBox1.TabIndex = 56
        Me.groupBox1.TabStop = False
        Me.groupBox1.Text = " Connection "
        '
        'label2
        '
        Me.label2.Location = New System.Drawing.Point(187, 22)
        Me.label2.Name = "label2"
        Me.label2.Size = New System.Drawing.Size(59, 15)
        Me.label2.TabIndex = 50
        Me.label2.Text = "Bit rate:"
        '
        'label1
        '
        Me.label1.Location = New System.Drawing.Point(14, 22)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(59, 15)
        Me.label1.TabIndex = 49
        Me.label1.Text = "Channel:"
        '
        'txtBitrate
        '
        Me.txtBitrate.Location = New System.Drawing.Point(188, 42)
        Me.txtBitrate.Name = "txtBitrate"
        Me.txtBitrate.ScrollBars = System.Windows.Forms.ScrollBars.Horizontal
        Me.txtBitrate.Size = New System.Drawing.Size(449, 20)
        Me.txtBitrate.TabIndex = 48
        Me.txtBitrate.Text = resources.GetString("txtBitrate.Text")
        '
        'cbbChannel
        '
        Me.cbbChannel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.cbbChannel.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cbbChannel.Location = New System.Drawing.Point(15, 42)
        Me.cbbChannel.Name = "cbbChannel"
        Me.cbbChannel.Size = New System.Drawing.Size(163, 21)
        Me.cbbChannel.TabIndex = 32
        '
        'btnInit
        '
        Me.btnInit.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnInit.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnInit.Location = New System.Drawing.Point(643, 16)
        Me.btnInit.Name = "btnInit"
        Me.btnInit.Size = New System.Drawing.Size(65, 23)
        Me.btnInit.TabIndex = 51
        Me.btnInit.Text = "Initialize"
        '
        'btnRelease
        '
        Me.btnRelease.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnRelease.Enabled = False
        Me.btnRelease.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnRelease.Location = New System.Drawing.Point(643, 41)
        Me.btnRelease.Name = "btnRelease"
        Me.btnRelease.Size = New System.Drawing.Size(65, 23)
        Me.btnRelease.TabIndex = 52
        Me.btnRelease.Text = "Release"
        '
        'groupBox3
        '
        Me.groupBox3.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox3.Controls.Add(Me.rdbFilterOpen)
        Me.groupBox3.Controls.Add(Me.rdbFilterClose)
        Me.groupBox3.Controls.Add(Me.btnFilterQuery)
        Me.groupBox3.Controls.Add(Me.btnFilterApply)
        Me.groupBox3.Location = New System.Drawing.Point(8, 84)
        Me.groupBox3.Name = "groupBox3"
        Me.groupBox3.Size = New System.Drawing.Size(714, 61)
        Me.groupBox3.TabIndex = 58
        Me.groupBox3.TabStop = False
        Me.groupBox3.Text = " Message Filtering "
        '
        'rdbFilterOpen
        '
        Me.rdbFilterOpen.Location = New System.Drawing.Point(20, 26)
        Me.rdbFilterOpen.Name = "rdbFilterOpen"
        Me.rdbFilterOpen.Size = New System.Drawing.Size(53, 17)
        Me.rdbFilterOpen.TabIndex = 57
        Me.rdbFilterOpen.Text = "Open"
        Me.rdbFilterOpen.UseVisualStyleBackColor = True
        '
        'rdbFilterClose
        '
        Me.rdbFilterClose.Location = New System.Drawing.Point(77, 26)
        Me.rdbFilterClose.Name = "rdbFilterClose"
        Me.rdbFilterClose.Size = New System.Drawing.Size(58, 17)
        Me.rdbFilterClose.TabIndex = 56
        Me.rdbFilterClose.Text = "Close"
        Me.rdbFilterClose.UseVisualStyleBackColor = True
        '
        'btnFilterQuery
        '
        Me.btnFilterQuery.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnFilterQuery.Enabled = False
        Me.btnFilterQuery.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnFilterQuery.Location = New System.Drawing.Point(643, 28)
        Me.btnFilterQuery.Name = "btnFilterQuery"
        Me.btnFilterQuery.Size = New System.Drawing.Size(65, 22)
        Me.btnFilterQuery.TabIndex = 57
        Me.btnFilterQuery.Text = "Query"
        Me.btnFilterQuery.UseVisualStyleBackColor = True
        '
        'btnFilterApply
        '
        Me.btnFilterApply.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnFilterApply.Enabled = False
        Me.btnFilterApply.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnFilterApply.Location = New System.Drawing.Point(572, 28)
        Me.btnFilterApply.Name = "btnFilterApply"
        Me.btnFilterApply.Size = New System.Drawing.Size(65, 22)
        Me.btnFilterApply.TabIndex = 56
        Me.btnFilterApply.Text = "Apply"
        Me.btnFilterApply.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(734, 612)
        Me.Controls.Add(Me.groupBox3)
        Me.Controls.Add(Me.groupBox1)
        Me.Controls.Add(Me.groupBox5)
        Me.Controls.Add(Me.groupBox6)
        Me.Controls.Add(Me.groupBox4)
        Me.Controls.Add(Me.groupBox2)
        Me.MaximizeBox = False
        Me.MinimumSize = New System.Drawing.Size(750, 651)
        Me.Name = "Form1"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "PCAN-Basic XL Sample"
        Me.groupBox5.ResumeLayout(False)
        Me.groupBox5.PerformLayout()
        Me.groupBox6.ResumeLayout(False)
        Me.groupBox6.PerformLayout()
        CType(Me.nudDLC, System.ComponentModel.ISupportInitialize).EndInit()
        Me.groupBox4.ResumeLayout(False)
        CType(Me.nudDeviceId, System.ComponentModel.ISupportInitialize).EndInit()
        Me.groupBox2.ResumeLayout(False)
        CType(Me.nudDelay, System.ComponentModel.ISupportInitialize).EndInit()
        Me.groupBox1.ResumeLayout(False)
        Me.groupBox1.PerformLayout()
        Me.groupBox3.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub
    Private WithEvents chbShowPeriod As System.Windows.Forms.CheckBox
    Private WithEvents rdbManual As System.Windows.Forms.RadioButton
    Private WithEvents rdbEvent As System.Windows.Forms.RadioButton
    Private WithEvents btnMsgClear As System.Windows.Forms.Button
    Private WithEvents groupBox5 As System.Windows.Forms.GroupBox
    Private WithEvents rdbTimer As System.Windows.Forms.RadioButton
    Private WithEvents btnRead As System.Windows.Forms.Button
    Private WithEvents groupBox6 As System.Windows.Forms.GroupBox
    Private WithEvents groupBox4 As System.Windows.Forms.GroupBox
    Private WithEvents rdbParamInactive As System.Windows.Forms.RadioButton
    Private WithEvents btnParameterGet As System.Windows.Forms.Button
    Private WithEvents label10 As System.Windows.Forms.Label
    Private WithEvents nudDeviceId As System.Windows.Forms.NumericUpDown
    Private WithEvents laDeviceOrDelay As System.Windows.Forms.Label
    Private WithEvents cbbParameter As System.Windows.Forms.ComboBox
    Private WithEvents groupBox2 As System.Windows.Forms.GroupBox
    Private WithEvents label6 As System.Windows.Forms.Label
    Private WithEvents rdbParamActive As System.Windows.Forms.RadioButton
    Private WithEvents btnParameterSet As System.Windows.Forms.Button
    Private WithEvents tmrRead As System.Windows.Forms.Timer
    Private WithEvents tmrDisplay As System.Windows.Forms.Timer
    Private WithEvents btnReset As System.Windows.Forms.Button
    Private WithEvents btnStatus As System.Windows.Forms.Button
    Private WithEvents btnGetVersions As System.Windows.Forms.Button
    Private WithEvents lbxInfo As System.Windows.Forms.ListBox
    Private WithEvents btnInfoClear As System.Windows.Forms.Button
    Private WithEvents nudDelay As NumericUpDown
    Private WithEvents groupBox1 As GroupBox
    Private WithEvents Button1 As Button
    Private WithEvents Button2 As Button
    Private WithEvents label2 As Label
    Private WithEvents label1 As Label
    Private WithEvents txtBitrate As TextBox
    Private WithEvents cbbChannel As ComboBox
    Private WithEvents btnInit As Button
    Private WithEvents btnRelease As Button
    Private WithEvents groupBox3 As GroupBox
    Private WithEvents rdbFilterOpen As RadioButton
    Private WithEvents rdbFilterClose As RadioButton
    Private WithEvents btnFilterQuery As Button
    Private WithEvents btnFilterApply As Button
    Private WithEvents Button3 As Button
    Private WithEvents btnEdit As Button
    Private WithEvents label13 As Label
    Private WithEvents label7 As Label
    Private WithEvents nudDLC As NumericUpDown
    Private WithEvents txtData As TextBox
    Private WithEvents txtPID As TextBox
    Private WithEvents label5 As Label
    Private WithEvents label12 As Label
    Private WithEvents txtAF As TextBox
    Private WithEvents btnWrite As Button
    Private WithEvents label3 As Label
    Private WithEvents chbRRS As CheckBox
    Private WithEvents txtVCID As TextBox
    Private WithEvents chbSEC As CheckBox
    Private WithEvents label4 As Label
    Private WithEvents txtSDT As TextBox
    Private WithEvents btnGenerate As Button
    Private WithEvents lstMessages As ListView
    Private WithEvents clhType As ColumnHeader
    Private WithEvents clhPID As ColumnHeader
    Private WithEvents clhDLC As ColumnHeader
    Private WithEvents clhSDT As ColumnHeader
    Private WithEvents clhVCID As ColumnHeader
    Private WithEvents clhAF As ColumnHeader
    Private WithEvents clhRcvTime As ColumnHeader
    Private WithEvents clhCount As ColumnHeader
    Private WithEvents clhData As ColumnHeader
    Private WithEvents laLength As Label
End Class
