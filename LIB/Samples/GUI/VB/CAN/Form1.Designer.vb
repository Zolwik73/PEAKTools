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
        Me.groupBox1 = New System.Windows.Forms.GroupBox()
        Me.btnHwRefresh = New System.Windows.Forms.Button()
        Me.cbbChannel = New System.Windows.Forms.ComboBox()
        Me.cbbBaudrates = New System.Windows.Forms.ComboBox()
        Me.laBaudrate = New System.Windows.Forms.Label()
        Me.label1 = New System.Windows.Forms.Label()
        Me.btnInit = New System.Windows.Forms.Button()
        Me.btnRelease = New System.Windows.Forms.Button()
        Me.txtData2 = New System.Windows.Forms.TextBox()
        Me.txtData3 = New System.Windows.Forms.TextBox()
        Me.txtData4 = New System.Windows.Forms.TextBox()
        Me.txtData5 = New System.Windows.Forms.TextBox()
        Me.txtData6 = New System.Windows.Forms.TextBox()
        Me.txtData7 = New System.Windows.Forms.TextBox()
        Me.nudLength = New System.Windows.Forms.NumericUpDown()
        Me.txtID = New System.Windows.Forms.TextBox()
        Me.Label14 = New System.Windows.Forms.Label()
        Me.Label13 = New System.Windows.Forms.Label()
        Me.chbExtended = New System.Windows.Forms.CheckBox()
        Me.chbRemote = New System.Windows.Forms.CheckBox()
        Me.laLength = New System.Windows.Forms.Label()
        Me.Label12 = New System.Windows.Forms.Label()
        Me.groupBox6 = New System.Windows.Forms.GroupBox()
        Me.txtData1 = New System.Windows.Forms.TextBox()
        Me.txtData0 = New System.Windows.Forms.TextBox()
        Me.label11 = New System.Windows.Forms.Label()
        Me.btnWrite = New System.Windows.Forms.Button()
        Me.btnMsgClear = New System.Windows.Forms.Button()
        Me.lstMessages = New System.Windows.Forms.ListView()
        Me.clhType = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhID = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhLength = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhCount = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhRcvTime = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.clhData = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.rdbEvent = New System.Windows.Forms.RadioButton()
        Me.rdbManual = New System.Windows.Forms.RadioButton()
        Me.chbShowPeriod = New System.Windows.Forms.CheckBox()
        Me.groupBox5 = New System.Windows.Forms.GroupBox()
        Me.rdbTimer = New System.Windows.Forms.RadioButton()
        Me.btnRead = New System.Windows.Forms.Button()
        Me.tmrDisplay = New System.Windows.Forms.Timer(Me.components)
        Me.tmrRead = New System.Windows.Forms.Timer(Me.components)
        Me.rdbParamActive = New System.Windows.Forms.RadioButton()
        Me.label6 = New System.Windows.Forms.Label()
        Me.nudDelay = New System.Windows.Forms.NumericUpDown()
        Me.groupBox2 = New System.Windows.Forms.GroupBox()
        Me.btnParameterGet = New System.Windows.Forms.Button()
        Me.label10 = New System.Windows.Forms.Label()
        Me.nudDeviceId = New System.Windows.Forms.NumericUpDown()
        Me.laDeviceOrDelay = New System.Windows.Forms.Label()
        Me.cbbParameter = New System.Windows.Forms.ComboBox()
        Me.rdbParamInactive = New System.Windows.Forms.RadioButton()
        Me.btnParameterSet = New System.Windows.Forms.Button()
        Me.btnInfoClear = New System.Windows.Forms.Button()
        Me.lbxInfo = New System.Windows.Forms.ListBox()
        Me.btnGetVersions = New System.Windows.Forms.Button()
        Me.btnStatus = New System.Windows.Forms.Button()
        Me.btnReset = New System.Windows.Forms.Button()
        Me.groupBox4 = New System.Windows.Forms.GroupBox()
        Me.btnFilterApply = New System.Windows.Forms.Button()
        Me.rdbFilterClose = New System.Windows.Forms.RadioButton()
        Me.rdbFilterCustom = New System.Windows.Forms.RadioButton()
        Me.rdbFilterOpen = New System.Windows.Forms.RadioButton()
        Me.label7 = New System.Windows.Forms.Label()
        Me.label8 = New System.Windows.Forms.Label()
        Me.nudIdFrom = New System.Windows.Forms.NumericUpDown()
        Me.nudIdTo = New System.Windows.Forms.NumericUpDown()
        Me.chbFilterExt = New System.Windows.Forms.CheckBox()
        Me.groupBox3 = New System.Windows.Forms.GroupBox()
        Me.btnFilterQuery = New System.Windows.Forms.Button()
        Me.groupBox1.SuspendLayout()
        CType(Me.nudLength, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.groupBox6.SuspendLayout()
        Me.groupBox5.SuspendLayout()
        CType(Me.nudDelay, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.groupBox2.SuspendLayout()
        CType(Me.nudDeviceId, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.groupBox4.SuspendLayout()
        CType(Me.nudIdFrom, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.nudIdTo, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.groupBox3.SuspendLayout()
        Me.SuspendLayout()
        '
        'groupBox1
        '
        Me.groupBox1.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox1.Controls.Add(Me.btnHwRefresh)
        Me.groupBox1.Controls.Add(Me.cbbChannel)
        Me.groupBox1.Controls.Add(Me.cbbBaudrates)
        Me.groupBox1.Controls.Add(Me.laBaudrate)
        Me.groupBox1.Controls.Add(Me.label1)
        Me.groupBox1.Controls.Add(Me.btnInit)
        Me.groupBox1.Controls.Add(Me.btnRelease)
        Me.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.groupBox1.Location = New System.Drawing.Point(5, 5)
        Me.groupBox1.Name = "groupBox1"
        Me.groupBox1.Size = New System.Drawing.Size(714, 65)
        Me.groupBox1.TabIndex = 52
        Me.groupBox1.TabStop = False
        Me.groupBox1.Text = " Connection "
        '
        'btnHwRefresh
        '
        Me.btnHwRefresh.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnHwRefresh.Location = New System.Drawing.Point(139, 30)
        Me.btnHwRefresh.Name = "btnHwRefresh"
        Me.btnHwRefresh.Size = New System.Drawing.Size(59, 23)
        Me.btnHwRefresh.TabIndex = 45
        Me.btnHwRefresh.Text = "Refresh"
        '
        'cbbChannel
        '
        Me.cbbChannel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.cbbChannel.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cbbChannel.Items.AddRange(New Object() {"None", "DNG-Channel 1", "ISA-Channel 1", "ISA-Channel 2", "ISA-Channel 3", "ISA-Channel 4", "ISA-Channel 5", "ISA-Channel 6", "ISA-Channel 7", "ISA-Channel 8", "PCC-Channel 1", "PCC-Channel 2", "PCI-Channel 1", "PCI-Channel 2", "PCI-Channel 3", "PCI-Channel 4", "PCI-Channel 5", "PCI-Channel 6", "PCI-Channel 7", "PCI-Channel 8", "USB-Channel 1", "USB-Channel 2", "USB-Channel 3", "USB-Channel 4", "USB-Channel 5", "USB-Channel 6", "USB-Channel 7", "USB-Channel 8"})
        Me.cbbChannel.Location = New System.Drawing.Point(8, 31)
        Me.cbbChannel.Name = "cbbChannel"
        Me.cbbChannel.Size = New System.Drawing.Size(125, 21)
        Me.cbbChannel.TabIndex = 32
        '
        'cbbBaudrates
        '
        Me.cbbBaudrates.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.cbbBaudrates.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cbbBaudrates.Items.AddRange(New Object() {"1 MBit/sec", "800 kBit/sec", "500 kBit/sec", "250 kBit/sec", "125 kKBit/sec", "100 kBit/sec", "95,238 kBit/sec", "83,333 kBit/sec", "50 kBit/sec", "47,619 kBit/sec", "33,333 kBit/sec", "20 kBit/sec", "10 kBit/sec", "5 kBit/sec"})
        Me.cbbBaudrates.Location = New System.Drawing.Point(204, 31)
        Me.cbbBaudrates.Name = "cbbBaudrates"
        Me.cbbBaudrates.Size = New System.Drawing.Size(116, 21)
        Me.cbbBaudrates.TabIndex = 36
        '
        'laBaudrate
        '
        Me.laBaudrate.Location = New System.Drawing.Point(204, 15)
        Me.laBaudrate.Name = "laBaudrate"
        Me.laBaudrate.Size = New System.Drawing.Size(56, 23)
        Me.laBaudrate.TabIndex = 41
        Me.laBaudrate.Text = "Baudrate:"
        '
        'label1
        '
        Me.label1.Location = New System.Drawing.Point(7, 16)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(56, 23)
        Me.label1.TabIndex = 40
        Me.label1.Text = "Hardware:"
        '
        'btnInit
        '
        Me.btnInit.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnInit.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnInit.Location = New System.Drawing.Point(643, 11)
        Me.btnInit.Name = "btnInit"
        Me.btnInit.Size = New System.Drawing.Size(65, 23)
        Me.btnInit.TabIndex = 34
        Me.btnInit.Text = "Initialize"
        '
        'btnRelease
        '
        Me.btnRelease.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnRelease.Enabled = False
        Me.btnRelease.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnRelease.Location = New System.Drawing.Point(643, 36)
        Me.btnRelease.Name = "btnRelease"
        Me.btnRelease.Size = New System.Drawing.Size(65, 23)
        Me.btnRelease.TabIndex = 35
        Me.btnRelease.Text = "Release"
        '
        'txtData2
        '
        Me.txtData2.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtData2.Location = New System.Drawing.Point(71, 38)
        Me.txtData2.MaxLength = 2
        Me.txtData2.Name = "txtData2"
        Me.txtData2.Size = New System.Drawing.Size(24, 20)
        Me.txtData2.TabIndex = 72
        Me.txtData2.Text = "00"
        Me.txtData2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'txtData3
        '
        Me.txtData3.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtData3.Location = New System.Drawing.Point(100, 38)
        Me.txtData3.MaxLength = 2
        Me.txtData3.Name = "txtData3"
        Me.txtData3.Size = New System.Drawing.Size(24, 20)
        Me.txtData3.TabIndex = 73
        Me.txtData3.Text = "00"
        Me.txtData3.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'txtData4
        '
        Me.txtData4.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtData4.Location = New System.Drawing.Point(130, 38)
        Me.txtData4.MaxLength = 2
        Me.txtData4.Name = "txtData4"
        Me.txtData4.Size = New System.Drawing.Size(24, 20)
        Me.txtData4.TabIndex = 74
        Me.txtData4.Text = "00"
        Me.txtData4.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'txtData5
        '
        Me.txtData5.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtData5.Location = New System.Drawing.Point(160, 38)
        Me.txtData5.MaxLength = 2
        Me.txtData5.Name = "txtData5"
        Me.txtData5.Size = New System.Drawing.Size(24, 20)
        Me.txtData5.TabIndex = 75
        Me.txtData5.Text = "00"
        Me.txtData5.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'txtData6
        '
        Me.txtData6.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtData6.Location = New System.Drawing.Point(190, 38)
        Me.txtData6.MaxLength = 2
        Me.txtData6.Name = "txtData6"
        Me.txtData6.Size = New System.Drawing.Size(24, 20)
        Me.txtData6.TabIndex = 76
        Me.txtData6.Text = "00"
        Me.txtData6.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'txtData7
        '
        Me.txtData7.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtData7.Location = New System.Drawing.Point(220, 38)
        Me.txtData7.MaxLength = 2
        Me.txtData7.Name = "txtData7"
        Me.txtData7.Size = New System.Drawing.Size(24, 20)
        Me.txtData7.TabIndex = 77
        Me.txtData7.Text = "00"
        Me.txtData7.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'nudLength
        '
        Me.nudLength.BackColor = System.Drawing.Color.White
        Me.nudLength.Location = New System.Drawing.Point(618, 38)
        Me.nudLength.Maximum = New Decimal(New Integer() {8, 0, 0, 0})
        Me.nudLength.Name = "nudLength"
        Me.nudLength.Size = New System.Drawing.Size(41, 20)
        Me.nudLength.TabIndex = 135
        '
        'txtID
        '
        Me.txtID.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtID.Location = New System.Drawing.Point(500, 38)
        Me.txtID.MaxLength = 3
        Me.txtID.Name = "txtID"
        Me.txtID.Size = New System.Drawing.Size(112, 20)
        Me.txtID.TabIndex = 134
        Me.txtID.Text = "0"
        '
        'Label14
        '
        Me.Label14.AutoSize = True
        Me.Label14.Location = New System.Drawing.Point(497, 16)
        Me.Label14.Name = "Label14"
        Me.Label14.Size = New System.Drawing.Size(49, 13)
        Me.Label14.TabIndex = 140
        Me.Label14.Text = "ID (Hex):"
        '
        'Label13
        '
        Me.Label13.AutoSize = True
        Me.Label13.Location = New System.Drawing.Point(615, 16)
        Me.Label13.Name = "Label13"
        Me.Label13.Size = New System.Drawing.Size(31, 13)
        Me.Label13.TabIndex = 141
        Me.Label13.Text = "DLC:"
        '
        'chbExtended
        '
        Me.chbExtended.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.chbExtended.Location = New System.Drawing.Point(500, 64)
        Me.chbExtended.Name = "chbExtended"
        Me.chbExtended.Size = New System.Drawing.Size(62, 24)
        Me.chbExtended.TabIndex = 136
        Me.chbExtended.Text = "Extended"
        '
        'chbRemote
        '
        Me.chbRemote.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.chbRemote.Location = New System.Drawing.Point(568, 64)
        Me.chbRemote.Name = "chbRemote"
        Me.chbRemote.Size = New System.Drawing.Size(44, 24)
        Me.chbRemote.TabIndex = 137
        Me.chbRemote.Text = "RTR"
        '
        'laLength
        '
        Me.laLength.AutoSize = True
        Me.laLength.Location = New System.Drawing.Point(672, 41)
        Me.laLength.Name = "laLength"
        Me.laLength.Size = New System.Drawing.Size(26, 13)
        Me.laLength.TabIndex = 142
        Me.laLength.Text = "8 B."
        '
        'Label12
        '
        Me.Label12.AutoSize = True
        Me.Label12.Location = New System.Drawing.Point(665, 16)
        Me.Label12.Name = "Label12"
        Me.Label12.Size = New System.Drawing.Size(43, 13)
        Me.Label12.TabIndex = 143
        Me.Label12.Text = "Length:"
        '
        'groupBox6
        '
        Me.groupBox6.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox6.Controls.Add(Me.Label12)
        Me.groupBox6.Controls.Add(Me.laLength)
        Me.groupBox6.Controls.Add(Me.chbRemote)
        Me.groupBox6.Controls.Add(Me.chbExtended)
        Me.groupBox6.Controls.Add(Me.Label13)
        Me.groupBox6.Controls.Add(Me.Label14)
        Me.groupBox6.Controls.Add(Me.txtID)
        Me.groupBox6.Controls.Add(Me.nudLength)
        Me.groupBox6.Controls.Add(Me.txtData7)
        Me.groupBox6.Controls.Add(Me.txtData6)
        Me.groupBox6.Controls.Add(Me.txtData5)
        Me.groupBox6.Controls.Add(Me.txtData4)
        Me.groupBox6.Controls.Add(Me.txtData3)
        Me.groupBox6.Controls.Add(Me.txtData2)
        Me.groupBox6.Controls.Add(Me.txtData1)
        Me.groupBox6.Controls.Add(Me.txtData0)
        Me.groupBox6.Controls.Add(Me.label11)
        Me.groupBox6.Controls.Add(Me.btnWrite)
        Me.groupBox6.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.groupBox6.Location = New System.Drawing.Point(5, 350)
        Me.groupBox6.Name = "groupBox6"
        Me.groupBox6.Size = New System.Drawing.Size(714, 95)
        Me.groupBox6.TabIndex = 57
        Me.groupBox6.TabStop = False
        Me.groupBox6.Text = "Write Messages"
        '
        'txtData1
        '
        Me.txtData1.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtData1.Location = New System.Drawing.Point(41, 38)
        Me.txtData1.MaxLength = 2
        Me.txtData1.Name = "txtData1"
        Me.txtData1.Size = New System.Drawing.Size(24, 20)
        Me.txtData1.TabIndex = 71
        Me.txtData1.Text = "00"
        Me.txtData1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'txtData0
        '
        Me.txtData0.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper
        Me.txtData0.Location = New System.Drawing.Point(10, 38)
        Me.txtData0.MaxLength = 2
        Me.txtData0.Name = "txtData0"
        Me.txtData0.Size = New System.Drawing.Size(24, 20)
        Me.txtData0.TabIndex = 70
        Me.txtData0.Text = "00"
        Me.txtData0.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'label11
        '
        Me.label11.AutoSize = True
        Me.label11.Location = New System.Drawing.Point(7, 16)
        Me.label11.Name = "label11"
        Me.label11.Size = New System.Drawing.Size(61, 13)
        Me.label11.TabIndex = 32
        Me.label11.Text = "Data (Hex):"
        '
        'btnWrite
        '
        Me.btnWrite.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnWrite.Enabled = False
        Me.btnWrite.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnWrite.Location = New System.Drawing.Point(640, 65)
        Me.btnWrite.Name = "btnWrite"
        Me.btnWrite.Size = New System.Drawing.Size(65, 23)
        Me.btnWrite.TabIndex = 36
        Me.btnWrite.Text = "Write"
        '
        'btnMsgClear
        '
        Me.btnMsgClear.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnMsgClear.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnMsgClear.Location = New System.Drawing.Point(643, 36)
        Me.btnMsgClear.Name = "btnMsgClear"
        Me.btnMsgClear.Size = New System.Drawing.Size(65, 23)
        Me.btnMsgClear.TabIndex = 50
        Me.btnMsgClear.Text = "Clear"
        Me.btnMsgClear.UseVisualStyleBackColor = True
        '
        'lstMessages
        '
        Me.lstMessages.Columns.AddRange(New System.Windows.Forms.ColumnHeader() {Me.clhType, Me.clhID, Me.clhLength, Me.clhCount, Me.clhRcvTime, Me.clhData})
        Me.lstMessages.FullRowSelect = True
        Me.lstMessages.HideSelection = False
        Me.lstMessages.Location = New System.Drawing.Point(8, 37)
        Me.lstMessages.MultiSelect = False
        Me.lstMessages.Name = "lstMessages"
        Me.lstMessages.Size = New System.Drawing.Size(560, 96)
        Me.lstMessages.TabIndex = 28
        Me.lstMessages.UseCompatibleStateImageBehavior = False
        Me.lstMessages.View = System.Windows.Forms.View.Details
        '
        'clhType
        '
        Me.clhType.Text = "Type"
        Me.clhType.Width = 110
        '
        'clhID
        '
        Me.clhID.Text = "ID"
        Me.clhID.Width = 90
        '
        'clhLength
        '
        Me.clhLength.Text = "Length"
        Me.clhLength.Width = 50
        '
        'clhCount
        '
        Me.clhCount.Text = "Count"
        Me.clhCount.Width = 49
        '
        'clhRcvTime
        '
        Me.clhRcvTime.Text = "Rcv Time"
        Me.clhRcvTime.Width = 70
        '
        'clhData
        '
        Me.clhData.Text = "Data"
        Me.clhData.Width = 170
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
        'groupBox5
        '
        Me.groupBox5.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox5.Controls.Add(Me.chbShowPeriod)
        Me.groupBox5.Controls.Add(Me.rdbManual)
        Me.groupBox5.Controls.Add(Me.rdbEvent)
        Me.groupBox5.Controls.Add(Me.lstMessages)
        Me.groupBox5.Controls.Add(Me.btnMsgClear)
        Me.groupBox5.Controls.Add(Me.rdbTimer)
        Me.groupBox5.Controls.Add(Me.btnRead)
        Me.groupBox5.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.groupBox5.Location = New System.Drawing.Point(5, 204)
        Me.groupBox5.Name = "groupBox5"
        Me.groupBox5.Size = New System.Drawing.Size(714, 140)
        Me.groupBox5.TabIndex = 56
        Me.groupBox5.TabStop = False
        Me.groupBox5.Text = " Messages Reading "
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
        Me.btnRead.Location = New System.Drawing.Point(572, 36)
        Me.btnRead.Name = "btnRead"
        Me.btnRead.Size = New System.Drawing.Size(65, 23)
        Me.btnRead.TabIndex = 49
        Me.btnRead.Text = "Read"
        Me.btnRead.UseVisualStyleBackColor = True
        '
        'tmrDisplay
        '
        '
        'tmrRead
        '
        Me.tmrRead.Interval = 50
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
        'label6
        '
        Me.label6.Location = New System.Drawing.Point(7, 14)
        Me.label6.Name = "label6"
        Me.label6.Size = New System.Drawing.Size(64, 23)
        Me.label6.TabIndex = 43
        Me.label6.Text = "Parameter:"
        '
        'nudDelay
        '
        Me.nudDelay.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.nudDelay.Location = New System.Drawing.Point(408, 29)
        Me.nudDelay.Maximum = New Decimal(New Integer() {-1, 0, 0, 0})
        Me.nudDelay.Name = "nudDelay"
        Me.nudDelay.Size = New System.Drawing.Size(99, 20)
        Me.nudDelay.TabIndex = 55
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
        Me.groupBox2.Location = New System.Drawing.Point(5, 140)
        Me.groupBox2.Name = "groupBox2"
        Me.groupBox2.Size = New System.Drawing.Size(714, 58)
        Me.groupBox2.TabIndex = 54
        Me.groupBox2.TabStop = False
        Me.groupBox2.Text = " Configuration Parameters "
        '
        'btnParameterGet
        '
        Me.btnParameterGet.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnParameterGet.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnParameterGet.Location = New System.Drawing.Point(643, 28)
        Me.btnParameterGet.Name = "btnParameterGet"
        Me.btnParameterGet.Size = New System.Drawing.Size(65, 23)
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
        Me.cbbParameter.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cbbParameter.FormattingEnabled = True
        Me.cbbParameter.Items.AddRange(New Object() {"Device ID", "5V Power", "Auto-reset on BUS-OFF", "CAN Listen-Only", "Debug's Log", "Receive Status", "CAN Controller Number", "Trace File", "Channel Identification (USB)", "Channel Capabilities", "Bit rate Adaptation", "Get Bit rate BTR Information", "Get CAN Nominal Speed Bit/s", "Get IP Address", "Get LAN Service Status", "Reception of Status Frames", "Reception of RTR Frames", "Reception of Error Frames", "Interframe Transmit Delay", "Reception of Echo Frames", "Hard Reset Status", "Communication Direction", "Global Unique Identifier (GUID)"})
        Me.cbbParameter.Location = New System.Drawing.Point(10, 31)
        Me.cbbParameter.Name = "cbbParameter"
        Me.cbbParameter.Size = New System.Drawing.Size(217, 21)
        Me.cbbParameter.TabIndex = 44
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
        'btnParameterSet
        '
        Me.btnParameterSet.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnParameterSet.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnParameterSet.Location = New System.Drawing.Point(572, 28)
        Me.btnParameterSet.Name = "btnParameterSet"
        Me.btnParameterSet.Size = New System.Drawing.Size(65, 23)
        Me.btnParameterSet.TabIndex = 46
        Me.btnParameterSet.Text = "Set"
        Me.btnParameterSet.UseVisualStyleBackColor = True
        '
        'btnInfoClear
        '
        Me.btnInfoClear.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnInfoClear.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnInfoClear.Location = New System.Drawing.Point(643, 14)
        Me.btnInfoClear.Name = "btnInfoClear"
        Me.btnInfoClear.Size = New System.Drawing.Size(65, 23)
        Me.btnInfoClear.TabIndex = 59
        Me.btnInfoClear.Text = "Clear"
        Me.btnInfoClear.UseVisualStyleBackColor = True
        '
        'lbxInfo
        '
        Me.lbxInfo.FormattingEnabled = True
        Me.lbxInfo.Items.AddRange(New Object() {"Select a Hardware and a configuration for it. Then click ""Initialize"" button", "When activated, the Debug-Log file will be found in the same directory as this ap" &
                "plication", "When activated, the PCAN-Trace file will be found in the same directory as this a" &
                "pplication"})
        Me.lbxInfo.Location = New System.Drawing.Point(8, 15)
        Me.lbxInfo.Name = "lbxInfo"
        Me.lbxInfo.ScrollAlwaysVisible = True
        Me.lbxInfo.Size = New System.Drawing.Size(560, 56)
        Me.lbxInfo.TabIndex = 61
        '
        'btnGetVersions
        '
        Me.btnGetVersions.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnGetVersions.Enabled = False
        Me.btnGetVersions.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnGetVersions.Location = New System.Drawing.Point(572, 14)
        Me.btnGetVersions.Name = "btnGetVersions"
        Me.btnGetVersions.Size = New System.Drawing.Size(65, 23)
        Me.btnGetVersions.TabIndex = 60
        Me.btnGetVersions.Text = "Versions"
        Me.btnGetVersions.UseVisualStyleBackColor = True
        '
        'btnStatus
        '
        Me.btnStatus.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnStatus.Enabled = False
        Me.btnStatus.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnStatus.Location = New System.Drawing.Point(572, 49)
        Me.btnStatus.Name = "btnStatus"
        Me.btnStatus.Size = New System.Drawing.Size(65, 23)
        Me.btnStatus.TabIndex = 62
        Me.btnStatus.Text = "Status"
        Me.btnStatus.UseVisualStyleBackColor = True
        '
        'btnReset
        '
        Me.btnReset.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnReset.Enabled = False
        Me.btnReset.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnReset.Location = New System.Drawing.Point(643, 49)
        Me.btnReset.Name = "btnReset"
        Me.btnReset.Size = New System.Drawing.Size(65, 23)
        Me.btnReset.TabIndex = 63
        Me.btnReset.Text = "Reset"
        Me.btnReset.UseVisualStyleBackColor = True
        '
        'groupBox4
        '
        Me.groupBox4.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox4.Controls.Add(Me.btnReset)
        Me.groupBox4.Controls.Add(Me.btnStatus)
        Me.groupBox4.Controls.Add(Me.btnGetVersions)
        Me.groupBox4.Controls.Add(Me.lbxInfo)
        Me.groupBox4.Controls.Add(Me.btnInfoClear)
        Me.groupBox4.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.groupBox4.Location = New System.Drawing.Point(5, 451)
        Me.groupBox4.Name = "groupBox4"
        Me.groupBox4.Size = New System.Drawing.Size(714, 80)
        Me.groupBox4.TabIndex = 55
        Me.groupBox4.TabStop = False
        Me.groupBox4.Text = "Information"
        '
        'btnFilterApply
        '
        Me.btnFilterApply.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnFilterApply.Enabled = False
        Me.btnFilterApply.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnFilterApply.Location = New System.Drawing.Point(572, 28)
        Me.btnFilterApply.Name = "btnFilterApply"
        Me.btnFilterApply.Size = New System.Drawing.Size(65, 23)
        Me.btnFilterApply.TabIndex = 44
        Me.btnFilterApply.Text = "Apply"
        Me.btnFilterApply.UseVisualStyleBackColor = True
        '
        'rdbFilterClose
        '
        Me.rdbFilterClose.Location = New System.Drawing.Point(177, 32)
        Me.rdbFilterClose.Name = "rdbFilterClose"
        Me.rdbFilterClose.Size = New System.Drawing.Size(58, 17)
        Me.rdbFilterClose.TabIndex = 0
        Me.rdbFilterClose.Text = "Close"
        Me.rdbFilterClose.UseVisualStyleBackColor = True
        '
        'rdbFilterCustom
        '
        Me.rdbFilterCustom.Location = New System.Drawing.Point(238, 32)
        Me.rdbFilterCustom.Name = "rdbFilterCustom"
        Me.rdbFilterCustom.Size = New System.Drawing.Size(104, 17)
        Me.rdbFilterCustom.TabIndex = 1
        Me.rdbFilterCustom.Text = "Custom (expand)"
        Me.rdbFilterCustom.UseVisualStyleBackColor = True
        '
        'rdbFilterOpen
        '
        Me.rdbFilterOpen.Location = New System.Drawing.Point(120, 32)
        Me.rdbFilterOpen.Name = "rdbFilterOpen"
        Me.rdbFilterOpen.Size = New System.Drawing.Size(53, 17)
        Me.rdbFilterOpen.TabIndex = 2
        Me.rdbFilterOpen.Text = "Open"
        Me.rdbFilterOpen.UseVisualStyleBackColor = True
        '
        'label7
        '
        Me.label7.Location = New System.Drawing.Point(361, 12)
        Me.label7.Name = "label7"
        Me.label7.Size = New System.Drawing.Size(71, 23)
        Me.label7.TabIndex = 42
        Me.label7.Text = "From (Hex):"
        '
        'label8
        '
        Me.label8.Location = New System.Drawing.Point(436, 10)
        Me.label8.Name = "label8"
        Me.label8.Size = New System.Drawing.Size(71, 23)
        Me.label8.TabIndex = 43
        Me.label8.Text = "To (Hex):"
        '
        'nudIdFrom
        '
        Me.nudIdFrom.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.nudIdFrom.Hexadecimal = True
        Me.nudIdFrom.Location = New System.Drawing.Point(363, 29)
        Me.nudIdFrom.Maximum = New Decimal(New Integer() {536870911, 0, 0, 0})
        Me.nudIdFrom.Name = "nudIdFrom"
        Me.nudIdFrom.Size = New System.Drawing.Size(69, 20)
        Me.nudIdFrom.TabIndex = 5
        '
        'nudIdTo
        '
        Me.nudIdTo.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.nudIdTo.Hexadecimal = True
        Me.nudIdTo.Location = New System.Drawing.Point(438, 29)
        Me.nudIdTo.Maximum = New Decimal(New Integer() {2047, 0, 0, 0})
        Me.nudIdTo.Name = "nudIdTo"
        Me.nudIdTo.Size = New System.Drawing.Size(69, 20)
        Me.nudIdTo.TabIndex = 6
        Me.nudIdTo.Value = New Decimal(New Integer() {2047, 0, 0, 0})
        '
        'chbFilterExt
        '
        Me.chbFilterExt.AutoSize = True
        Me.chbFilterExt.Location = New System.Drawing.Point(10, 33)
        Me.chbFilterExt.Name = "chbFilterExt"
        Me.chbFilterExt.Size = New System.Drawing.Size(103, 17)
        Me.chbFilterExt.TabIndex = 44
        Me.chbFilterExt.Text = "Extended Frame"
        Me.chbFilterExt.UseVisualStyleBackColor = True
        '
        'groupBox3
        '
        Me.groupBox3.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox3.Controls.Add(Me.btnFilterQuery)
        Me.groupBox3.Controls.Add(Me.chbFilterExt)
        Me.groupBox3.Controls.Add(Me.nudIdTo)
        Me.groupBox3.Controls.Add(Me.nudIdFrom)
        Me.groupBox3.Controls.Add(Me.label8)
        Me.groupBox3.Controls.Add(Me.label7)
        Me.groupBox3.Controls.Add(Me.rdbFilterOpen)
        Me.groupBox3.Controls.Add(Me.rdbFilterCustom)
        Me.groupBox3.Controls.Add(Me.rdbFilterClose)
        Me.groupBox3.Controls.Add(Me.btnFilterApply)
        Me.groupBox3.Location = New System.Drawing.Point(5, 76)
        Me.groupBox3.Name = "groupBox3"
        Me.groupBox3.Size = New System.Drawing.Size(714, 58)
        Me.groupBox3.TabIndex = 53
        Me.groupBox3.TabStop = False
        Me.groupBox3.Text = " Message Filtering "
        '
        'btnFilterQuery
        '
        Me.btnFilterQuery.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnFilterQuery.Enabled = False
        Me.btnFilterQuery.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.btnFilterQuery.Location = New System.Drawing.Point(643, 28)
        Me.btnFilterQuery.Name = "btnFilterQuery"
        Me.btnFilterQuery.Size = New System.Drawing.Size(65, 23)
        Me.btnFilterQuery.TabIndex = 55
        Me.btnFilterQuery.Text = "Query"
        Me.btnFilterQuery.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(724, 534)
        Me.Controls.Add(Me.groupBox5)
        Me.Controls.Add(Me.groupBox6)
        Me.Controls.Add(Me.groupBox4)
        Me.Controls.Add(Me.groupBox3)
        Me.Controls.Add(Me.groupBox2)
        Me.Controls.Add(Me.groupBox1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.Name = "Form1"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "PCAN-Basic Sample"
        Me.groupBox1.ResumeLayout(False)
        CType(Me.nudLength, System.ComponentModel.ISupportInitialize).EndInit()
        Me.groupBox6.ResumeLayout(False)
        Me.groupBox6.PerformLayout()
        Me.groupBox5.ResumeLayout(False)
        Me.groupBox5.PerformLayout()
        CType(Me.nudDelay, System.ComponentModel.ISupportInitialize).EndInit()
        Me.groupBox2.ResumeLayout(False)
        CType(Me.nudDeviceId, System.ComponentModel.ISupportInitialize).EndInit()
        Me.groupBox4.ResumeLayout(False)
        CType(Me.nudIdFrom, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.nudIdTo, System.ComponentModel.ISupportInitialize).EndInit()
        Me.groupBox3.ResumeLayout(False)
        Me.groupBox3.PerformLayout()
        Me.ResumeLayout(False)

    End Sub

    Private WithEvents groupBox1 As GroupBox
    Private WithEvents btnHwRefresh As Button
    Private WithEvents cbbChannel As ComboBox
    Private WithEvents cbbBaudrates As ComboBox
    Private WithEvents laBaudrate As Label
    Private WithEvents label1 As Label
    Private WithEvents btnInit As Button
    Private WithEvents btnRelease As Button
    Private WithEvents txtData2 As TextBox
    Private WithEvents txtData3 As TextBox
    Private WithEvents txtData4 As TextBox
    Private WithEvents txtData5 As TextBox
    Private WithEvents txtData6 As TextBox
    Private WithEvents txtData7 As TextBox
    Private WithEvents nudLength As NumericUpDown
    Private WithEvents txtID As TextBox
    Private WithEvents Label14 As Label
    Private WithEvents Label13 As Label
    Private WithEvents chbExtended As CheckBox
    Private WithEvents chbRemote As CheckBox
    Private WithEvents laLength As Label
    Private WithEvents Label12 As Label
    Private WithEvents groupBox6 As GroupBox
    Private WithEvents txtData1 As TextBox
    Private WithEvents txtData0 As TextBox
    Private WithEvents label11 As Label
    Private WithEvents btnWrite As Button
    Private WithEvents btnMsgClear As Button
    Private WithEvents lstMessages As ListView
    Private WithEvents clhType As ColumnHeader
    Private WithEvents clhID As ColumnHeader
    Private WithEvents clhLength As ColumnHeader
    Private WithEvents clhCount As ColumnHeader
    Private WithEvents clhRcvTime As ColumnHeader
    Private WithEvents clhData As ColumnHeader
    Private WithEvents rdbEvent As RadioButton
    Private WithEvents rdbManual As RadioButton
    Private WithEvents chbShowPeriod As CheckBox
    Private WithEvents groupBox5 As GroupBox
    Private WithEvents rdbTimer As RadioButton
    Private WithEvents btnRead As Button
    Private WithEvents tmrDisplay As Timer
    Private WithEvents tmrRead As Timer
    Private WithEvents rdbParamActive As RadioButton
    Private WithEvents label6 As Label
    Private WithEvents nudDelay As NumericUpDown
    Private WithEvents groupBox2 As GroupBox
    Private WithEvents btnParameterGet As Button
    Private WithEvents label10 As Label
    Private WithEvents nudDeviceId As NumericUpDown
    Private WithEvents laDeviceOrDelay As Label
    Private WithEvents cbbParameter As ComboBox
    Private WithEvents rdbParamInactive As RadioButton
    Private WithEvents btnParameterSet As Button
    Private WithEvents btnInfoClear As Button
    Private WithEvents lbxInfo As ListBox
    Private WithEvents btnGetVersions As Button
    Private WithEvents btnStatus As Button
    Private WithEvents btnReset As Button
    Private WithEvents groupBox4 As GroupBox
    Private WithEvents btnFilterApply As Button
    Private WithEvents rdbFilterClose As RadioButton
    Private WithEvents rdbFilterCustom As RadioButton
    Private WithEvents rdbFilterOpen As RadioButton
    Private WithEvents label7 As Label
    Private WithEvents label8 As Label
    Private WithEvents nudIdFrom As NumericUpDown
    Private WithEvents nudIdTo As NumericUpDown
    Private WithEvents chbFilterExt As CheckBox
    Private WithEvents groupBox3 As GroupBox
    Private WithEvents btnFilterQuery As Button
End Class
