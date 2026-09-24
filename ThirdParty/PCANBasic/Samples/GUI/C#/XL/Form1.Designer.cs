namespace ICDIBasic
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtBitrate = new System.Windows.Forms.TextBox();
            this.cbbChannel = new System.Windows.Forms.ComboBox();
            this.btnInit = new System.Windows.Forms.Button();
            this.btnRelease = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.rdbFilterOpen = new System.Windows.Forms.RadioButton();
            this.rdbFilterClose = new System.Windows.Forms.RadioButton();
            this.btnFilterQuery = new System.Windows.Forms.Button();
            this.btnFilterApply = new System.Windows.Forms.Button();
            this.btnParameterSet = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.nudDelay = new System.Windows.Forms.NumericUpDown();
            this.btnParameterGet = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.nudDeviceId = new System.Windows.Forms.NumericUpDown();
            this.laDeviceOrDelay = new System.Windows.Forms.Label();
            this.cbbParameter = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.rdbParamActive = new System.Windows.Forms.RadioButton();
            this.rdbParamInactive = new System.Windows.Forms.RadioButton();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.btnReset = new System.Windows.Forms.Button();
            this.btnStatus = new System.Windows.Forms.Button();
            this.btnGetVersions = new System.Windows.Forms.Button();
            this.lbxInfo = new System.Windows.Forms.ListBox();
            this.btnInfoClear = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.chbShowPeriod = new System.Windows.Forms.CheckBox();
            this.rdbManual = new System.Windows.Forms.RadioButton();
            this.rdbEvent = new System.Windows.Forms.RadioButton();
            this.lstMessages = new System.Windows.Forms.ListView();
            this.clhType = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.clhPID = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.clhDLC = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.clhSDT = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.clhVCID = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.clhAF = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.clhRcvTime = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.clhCount = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.clhData = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.btnMsgClear = new System.Windows.Forms.Button();
            this.rdbTimer = new System.Windows.Forms.RadioButton();
            this.btnRead = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.laLength = new System.Windows.Forms.Label();
            this.btnEdit = new System.Windows.Forms.Button();
            this.btnGenerate = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.txtData = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.txtAF = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txtVCID = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.txtSDT = new System.Windows.Forms.TextBox();
            this.chbSEC = new System.Windows.Forms.CheckBox();
            this.chbRRS = new System.Windows.Forms.CheckBox();
            this.btnWrite = new System.Windows.Forms.Button();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.txtPID = new System.Windows.Forms.TextBox();
            this.nudDLC = new System.Windows.Forms.NumericUpDown();
            this.tmrRead = new System.Windows.Forms.Timer(this.components);
            this.tmrDisplay = new System.Windows.Forms.Timer(this.components);
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudDelay)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudDeviceId)).BeginInit();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudDLC)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.txtBitrate);
            this.groupBox1.Controls.Add(this.cbbChannel);
            this.groupBox1.Controls.Add(this.btnInit);
            this.groupBox1.Controls.Add(this.btnRelease);
            this.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox1.Location = new System.Drawing.Point(8, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(714, 70);
            this.groupBox1.TabIndex = 42;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " Connection ";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(187, 22);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(59, 15);
            this.label2.TabIndex = 50;
            this.label2.Text = "Bit rate:";
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(14, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 15);
            this.label1.TabIndex = 49;
            this.label1.Text = "Channel:";
            // 
            // txtBitrate
            // 
            this.txtBitrate.Location = new System.Drawing.Point(188, 42);
            this.txtBitrate.Name = "txtBitrate";
            this.txtBitrate.ScrollBars = System.Windows.Forms.ScrollBars.Horizontal;
            this.txtBitrate.Size = new System.Drawing.Size(449, 20);
            this.txtBitrate.TabIndex = 48;
            this.txtBitrate.Text = resources.GetString("txtBitrate.Text");
            // 
            // cbbChannel
            // 
            this.cbbChannel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbbChannel.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbbChannel.Location = new System.Drawing.Point(15, 42);
            this.cbbChannel.Name = "cbbChannel";
            this.cbbChannel.Size = new System.Drawing.Size(163, 23);
            this.cbbChannel.TabIndex = 32;
            this.cbbChannel.SelectedIndexChanged += new System.EventHandler(this.cbbChannel_SelectedIndexChanged);
            // 
            // btnInit
            // 
            this.btnInit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnInit.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnInit.Location = new System.Drawing.Point(643, 16);
            this.btnInit.Name = "btnInit";
            this.btnInit.Size = new System.Drawing.Size(65, 22);
            this.btnInit.TabIndex = 34;
            this.btnInit.Text = "Initialize";
            this.btnInit.Click += new System.EventHandler(this.btnInit_Click);
            // 
            // btnRelease
            // 
            this.btnRelease.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnRelease.Enabled = false;
            this.btnRelease.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnRelease.Location = new System.Drawing.Point(643, 41);
            this.btnRelease.Name = "btnRelease";
            this.btnRelease.Size = new System.Drawing.Size(65, 22);
            this.btnRelease.TabIndex = 35;
            this.btnRelease.Text = "Release";
            this.btnRelease.Click += new System.EventHandler(this.btnRelease_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.rdbFilterOpen);
            this.groupBox3.Controls.Add(this.rdbFilterClose);
            this.groupBox3.Controls.Add(this.btnFilterQuery);
            this.groupBox3.Controls.Add(this.btnFilterApply);
            this.groupBox3.Location = new System.Drawing.Point(8, 84);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(714, 61);
            this.groupBox3.TabIndex = 44;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = " Message Filtering ";
            // 
            // rdbFilterOpen
            // 
            this.rdbFilterOpen.Checked = true;
            this.rdbFilterOpen.Location = new System.Drawing.Point(20, 26);
            this.rdbFilterOpen.Name = "rdbFilterOpen";
            this.rdbFilterOpen.Size = new System.Drawing.Size(53, 17);
            this.rdbFilterOpen.TabIndex = 57;
            this.rdbFilterOpen.TabStop = true;
            this.rdbFilterOpen.Text = "Open";
            this.rdbFilterOpen.UseVisualStyleBackColor = true;
            // 
            // rdbFilterClose
            // 
            this.rdbFilterClose.Location = new System.Drawing.Point(77, 26);
            this.rdbFilterClose.Name = "rdbFilterClose";
            this.rdbFilterClose.Size = new System.Drawing.Size(58, 17);
            this.rdbFilterClose.TabIndex = 56;
            this.rdbFilterClose.Text = "Close";
            this.rdbFilterClose.UseVisualStyleBackColor = true;
            // 
            // btnFilterQuery
            // 
            this.btnFilterQuery.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnFilterQuery.Enabled = false;
            this.btnFilterQuery.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnFilterQuery.Location = new System.Drawing.Point(643, 28);
            this.btnFilterQuery.Name = "btnFilterQuery";
            this.btnFilterQuery.Size = new System.Drawing.Size(65, 22);
            this.btnFilterQuery.TabIndex = 55;
            this.btnFilterQuery.Text = "Query";
            this.btnFilterQuery.UseVisualStyleBackColor = true;
            this.btnFilterQuery.Click += new System.EventHandler(this.btnFilterQuery_Click);
            // 
            // btnFilterApply
            // 
            this.btnFilterApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnFilterApply.Enabled = false;
            this.btnFilterApply.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnFilterApply.Location = new System.Drawing.Point(572, 28);
            this.btnFilterApply.Name = "btnFilterApply";
            this.btnFilterApply.Size = new System.Drawing.Size(65, 22);
            this.btnFilterApply.TabIndex = 44;
            this.btnFilterApply.Text = "Apply";
            this.btnFilterApply.UseVisualStyleBackColor = true;
            this.btnFilterApply.Click += new System.EventHandler(this.btnFilterApply_Click);
            // 
            // btnParameterSet
            // 
            this.btnParameterSet.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnParameterSet.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnParameterSet.Location = new System.Drawing.Point(572, 28);
            this.btnParameterSet.Name = "btnParameterSet";
            this.btnParameterSet.Size = new System.Drawing.Size(65, 22);
            this.btnParameterSet.TabIndex = 46;
            this.btnParameterSet.Text = "Set";
            this.btnParameterSet.UseVisualStyleBackColor = true;
            this.btnParameterSet.Click += new System.EventHandler(this.btnParameterSet_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.nudDelay);
            this.groupBox2.Controls.Add(this.btnParameterGet);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Controls.Add(this.nudDeviceId);
            this.groupBox2.Controls.Add(this.laDeviceOrDelay);
            this.groupBox2.Controls.Add(this.cbbParameter);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.rdbParamActive);
            this.groupBox2.Controls.Add(this.rdbParamInactive);
            this.groupBox2.Controls.Add(this.btnParameterSet);
            this.groupBox2.Location = new System.Drawing.Point(8, 151);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(714, 58);
            this.groupBox2.TabIndex = 45;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = " Configuration Parameters ";
            // 
            // nudDelay
            // 
            this.nudDelay.Location = new System.Drawing.Point(408, 29);
            this.nudDelay.Maximum = new decimal(new int[] {
            -1,
            0,
            0,
            0});
            this.nudDelay.Name = "nudDelay";
            this.nudDelay.Size = new System.Drawing.Size(99, 20);
            this.nudDelay.TabIndex = 55;
            // 
            // btnParameterGet
            // 
            this.btnParameterGet.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnParameterGet.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnParameterGet.Location = new System.Drawing.Point(643, 28);
            this.btnParameterGet.Name = "btnParameterGet";
            this.btnParameterGet.Size = new System.Drawing.Size(65, 22);
            this.btnParameterGet.TabIndex = 54;
            this.btnParameterGet.Text = "Get";
            this.btnParameterGet.UseVisualStyleBackColor = true;
            this.btnParameterGet.Click += new System.EventHandler(this.btnParameterGet_Click);
            // 
            // label10
            // 
            this.label10.Location = new System.Drawing.Point(241, 11);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(59, 23);
            this.label10.TabIndex = 46;
            this.label10.Text = "Activation:";
            // 
            // nudDeviceId
            // 
            this.nudDeviceId.Enabled = false;
            this.nudDeviceId.Hexadecimal = true;
            this.nudDeviceId.Location = new System.Drawing.Point(408, 29);
            this.nudDeviceId.Maximum = new decimal(new int[] {
            -1,
            0,
            0,
            0});
            this.nudDeviceId.Name = "nudDeviceId";
            this.nudDeviceId.Size = new System.Drawing.Size(99, 20);
            this.nudDeviceId.TabIndex = 6;
            // 
            // laDeviceOrDelay
            // 
            this.laDeviceOrDelay.Location = new System.Drawing.Point(405, 12);
            this.laDeviceOrDelay.Name = "laDeviceOrDelay";
            this.laDeviceOrDelay.Size = new System.Drawing.Size(102, 23);
            this.laDeviceOrDelay.TabIndex = 45;
            this.laDeviceOrDelay.Text = "Device ID (Hex):";
            // 
            // cbbParameter
            // 
            this.cbbParameter.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbbParameter.FormattingEnabled = true;
            this.cbbParameter.Items.AddRange(new object[] {
            "Device ID",
            "5V Power",
            "Auto-reset on BUS-OFF",
            "CAN Listen-Only",
            "Debug\'s Log",
            "Receive Status",
            "CAN Controller Number",
            "Trace File",
            "Channel Identification (USB)",
            "Channel Capabilities",
            "Bit rate Adaptation",
            "Get Bit rate Nominal Information",
            "Get Bit rate FD Information",
            "Get Bit rate XL Information",
            "Get CAN Nominal Speed Bit/s",
            "Get CAN FD Speed Bit/s",
            "Get CAN XL Speed Bit/s\t",
            "Get IP Address",
            "Get LAN Service Status",
            "Reception of Status Frames",
            "Reception of RTR Frames",
            "Reception of Error Frames",
            "Interframe Transmit Delay",
            "Reception of Echo Frames",
            "Hard Reset Status",
            "Communication Direction",
            "Global Unique Identifier (GUID)"});
            this.cbbParameter.Location = new System.Drawing.Point(10, 31);
            this.cbbParameter.Name = "cbbParameter";
            this.cbbParameter.Size = new System.Drawing.Size(217, 21);
            this.cbbParameter.TabIndex = 44;
            this.cbbParameter.SelectedIndexChanged += new System.EventHandler(this.cbbParameter_SelectedIndexChanged);
            // 
            // label6
            // 
            this.label6.Location = new System.Drawing.Point(7, 14);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(64, 23);
            this.label6.TabIndex = 43;
            this.label6.Text = "Parameter:";
            // 
            // rdbParamActive
            // 
            this.rdbParamActive.Checked = true;
            this.rdbParamActive.Location = new System.Drawing.Point(238, 32);
            this.rdbParamActive.Name = "rdbParamActive";
            this.rdbParamActive.Size = new System.Drawing.Size(56, 17);
            this.rdbParamActive.TabIndex = 2;
            this.rdbParamActive.TabStop = true;
            this.rdbParamActive.Text = "Active";
            this.rdbParamActive.UseVisualStyleBackColor = true;
            // 
            // rdbParamInactive
            // 
            this.rdbParamInactive.Location = new System.Drawing.Point(300, 32);
            this.rdbParamInactive.Name = "rdbParamInactive";
            this.rdbParamInactive.Size = new System.Drawing.Size(67, 17);
            this.rdbParamInactive.TabIndex = 0;
            this.rdbParamInactive.Text = "Inactive";
            this.rdbParamInactive.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox4.Controls.Add(this.btnReset);
            this.groupBox4.Controls.Add(this.btnStatus);
            this.groupBox4.Controls.Add(this.btnGetVersions);
            this.groupBox4.Controls.Add(this.lbxInfo);
            this.groupBox4.Controls.Add(this.btnInfoClear);
            this.groupBox4.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox4.Location = new System.Drawing.Point(8, 517);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(714, 87);
            this.groupBox4.TabIndex = 47;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Information";
            // 
            // btnReset
            // 
            this.btnReset.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnReset.Enabled = false;
            this.btnReset.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnReset.Location = new System.Drawing.Point(643, 49);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(65, 22);
            this.btnReset.TabIndex = 58;
            this.btnReset.Text = "Reset";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // btnStatus
            // 
            this.btnStatus.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnStatus.Enabled = false;
            this.btnStatus.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnStatus.Location = new System.Drawing.Point(572, 49);
            this.btnStatus.Name = "btnStatus";
            this.btnStatus.Size = new System.Drawing.Size(65, 22);
            this.btnStatus.TabIndex = 57;
            this.btnStatus.Text = "Status";
            this.btnStatus.UseVisualStyleBackColor = true;
            this.btnStatus.Click += new System.EventHandler(this.btnStatus_Click);
            // 
            // btnGetVersions
            // 
            this.btnGetVersions.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnGetVersions.Enabled = false;
            this.btnGetVersions.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnGetVersions.Location = new System.Drawing.Point(572, 21);
            this.btnGetVersions.Name = "btnGetVersions";
            this.btnGetVersions.Size = new System.Drawing.Size(65, 22);
            this.btnGetVersions.TabIndex = 53;
            this.btnGetVersions.Text = "Versions";
            this.btnGetVersions.UseVisualStyleBackColor = true;
            this.btnGetVersions.Click += new System.EventHandler(this.btnGetVersions_Click);
            // 
            // lbxInfo
            // 
            this.lbxInfo.FormattingEnabled = true;
            this.lbxInfo.Items.AddRange(new object[] {
            "Select a Hardware and a configuration for it. Then click \"Initialize\" button",
            "When activated, the Debug-Log file will be found in the same directory as this ap" +
                "plication",
            "When activated, the PCAN-Trace file will be found in the same directory as this a" +
                "pplication"});
            this.lbxInfo.Location = new System.Drawing.Point(10, 19);
            this.lbxInfo.Name = "lbxInfo";
            this.lbxInfo.ScrollAlwaysVisible = true;
            this.lbxInfo.Size = new System.Drawing.Size(558, 56);
            this.lbxInfo.TabIndex = 56;
            this.lbxInfo.DoubleClick += new System.EventHandler(this.lbxInfo_DoubleClick);
            // 
            // btnInfoClear
            // 
            this.btnInfoClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnInfoClear.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnInfoClear.Location = new System.Drawing.Point(643, 21);
            this.btnInfoClear.Name = "btnInfoClear";
            this.btnInfoClear.Size = new System.Drawing.Size(65, 22);
            this.btnInfoClear.TabIndex = 52;
            this.btnInfoClear.Text = "Clear";
            this.btnInfoClear.UseVisualStyleBackColor = true;
            this.btnInfoClear.Click += new System.EventHandler(this.btnInfoClear_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox5.Controls.Add(this.chbShowPeriod);
            this.groupBox5.Controls.Add(this.rdbManual);
            this.groupBox5.Controls.Add(this.rdbEvent);
            this.groupBox5.Controls.Add(this.lstMessages);
            this.groupBox5.Controls.Add(this.btnMsgClear);
            this.groupBox5.Controls.Add(this.rdbTimer);
            this.groupBox5.Controls.Add(this.btnRead);
            this.groupBox5.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox5.Location = new System.Drawing.Point(8, 216);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(714, 140);
            this.groupBox5.TabIndex = 48;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = " Messages Reading ";
            // 
            // chbShowPeriod
            // 
            this.chbShowPeriod.AutoSize = true;
            this.chbShowPeriod.Checked = true;
            this.chbShowPeriod.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chbShowPeriod.Location = new System.Drawing.Point(374, 15);
            this.chbShowPeriod.Name = "chbShowPeriod";
            this.chbShowPeriod.Size = new System.Drawing.Size(123, 17);
            this.chbShowPeriod.TabIndex = 75;
            this.chbShowPeriod.Text = "Timestamp as period";
            this.chbShowPeriod.UseVisualStyleBackColor = true;
            this.chbShowPeriod.CheckedChanged += new System.EventHandler(this.chbShowPeriod_CheckedChanged);
            // 
            // rdbManual
            // 
            this.rdbManual.AutoSize = true;
            this.rdbManual.Location = new System.Drawing.Point(276, 14);
            this.rdbManual.Name = "rdbManual";
            this.rdbManual.Size = new System.Drawing.Size(89, 17);
            this.rdbManual.TabIndex = 74;
            this.rdbManual.Text = "Manual Read";
            this.rdbManual.UseVisualStyleBackColor = true;
            this.rdbManual.CheckedChanged += new System.EventHandler(this.rdbTimer_CheckedChanged);
            // 
            // rdbEvent
            // 
            this.rdbEvent.AutoSize = true;
            this.rdbEvent.Location = new System.Drawing.Point(131, 14);
            this.rdbEvent.Name = "rdbEvent";
            this.rdbEvent.Size = new System.Drawing.Size(139, 17);
            this.rdbEvent.TabIndex = 73;
            this.rdbEvent.Text = "Reading using an Event";
            this.rdbEvent.UseVisualStyleBackColor = true;
            this.rdbEvent.CheckedChanged += new System.EventHandler(this.rdbTimer_CheckedChanged);
            // 
            // lstMessages
            // 
            this.lstMessages.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.clhType,
            this.clhPID,
            this.clhDLC,
            this.clhSDT,
            this.clhVCID,
            this.clhAF,
            this.clhRcvTime,
            this.clhCount,
            this.clhData});
            this.lstMessages.FullRowSelect = true;
            this.lstMessages.HideSelection = false;
            this.lstMessages.Location = new System.Drawing.Point(8, 37);
            this.lstMessages.MultiSelect = false;
            this.lstMessages.Name = "lstMessages";
            this.lstMessages.Size = new System.Drawing.Size(629, 96);
            this.lstMessages.TabIndex = 28;
            this.lstMessages.UseCompatibleStateImageBehavior = false;
            this.lstMessages.View = System.Windows.Forms.View.Details;
            this.lstMessages.DoubleClick += new System.EventHandler(this.lstMessages_DoubleClick);
            // 
            // clhType
            // 
            this.clhType.Text = "Type";
            this.clhType.Width = 80;
            // 
            // clhPID
            // 
            this.clhPID.Text = "PID";
            this.clhPID.Width = 70;
            // 
            // clhDLC
            // 
            this.clhDLC.Text = "Length";
            this.clhDLC.Width = 90;
            // 
            // clhSDT
            // 
            this.clhSDT.Text = "SDT";
            this.clhSDT.Width = 70;
            // 
            // clhVCID
            // 
            this.clhVCID.Text = "VCID";
            this.clhVCID.Width = 70;
            // 
            // clhAF
            // 
            this.clhAF.Text = "AF";
            this.clhAF.Width = 110;
            // 
            // clhRcvTime
            // 
            this.clhRcvTime.Text = "Rcv Time";
            this.clhRcvTime.Width = 110;
            // 
            // clhCount
            // 
            this.clhCount.Text = "Count";
            this.clhCount.Width = 90;
            // 
            // clhData
            // 
            this.clhData.Text = "Data";
            this.clhData.Width = 500;
            // 
            // btnMsgClear
            // 
            this.btnMsgClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnMsgClear.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnMsgClear.Location = new System.Drawing.Point(643, 65);
            this.btnMsgClear.Name = "btnMsgClear";
            this.btnMsgClear.Size = new System.Drawing.Size(65, 22);
            this.btnMsgClear.TabIndex = 50;
            this.btnMsgClear.Text = "Clear";
            this.btnMsgClear.UseVisualStyleBackColor = true;
            this.btnMsgClear.Click += new System.EventHandler(this.btnMsgClear_Click);
            // 
            // rdbTimer
            // 
            this.rdbTimer.AutoSize = true;
            this.rdbTimer.Checked = true;
            this.rdbTimer.Location = new System.Drawing.Point(8, 14);
            this.rdbTimer.Name = "rdbTimer";
            this.rdbTimer.Size = new System.Drawing.Size(117, 17);
            this.rdbTimer.TabIndex = 72;
            this.rdbTimer.TabStop = true;
            this.rdbTimer.Text = "Read using a Timer";
            this.rdbTimer.UseVisualStyleBackColor = true;
            this.rdbTimer.CheckedChanged += new System.EventHandler(this.rdbTimer_CheckedChanged);
            // 
            // btnRead
            // 
            this.btnRead.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnRead.Enabled = false;
            this.btnRead.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnRead.Location = new System.Drawing.Point(643, 36);
            this.btnRead.Name = "btnRead";
            this.btnRead.Size = new System.Drawing.Size(65, 22);
            this.btnRead.TabIndex = 49;
            this.btnRead.Text = "Read";
            this.btnRead.UseVisualStyleBackColor = true;
            this.btnRead.Click += new System.EventHandler(this.btnRead_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox6.Controls.Add(this.laLength);
            this.groupBox6.Controls.Add(this.btnEdit);
            this.groupBox6.Controls.Add(this.btnGenerate);
            this.groupBox6.Controls.Add(this.label7);
            this.groupBox6.Controls.Add(this.txtData);
            this.groupBox6.Controls.Add(this.label5);
            this.groupBox6.Controls.Add(this.txtAF);
            this.groupBox6.Controls.Add(this.label3);
            this.groupBox6.Controls.Add(this.txtVCID);
            this.groupBox6.Controls.Add(this.label4);
            this.groupBox6.Controls.Add(this.txtSDT);
            this.groupBox6.Controls.Add(this.chbSEC);
            this.groupBox6.Controls.Add(this.chbRRS);
            this.groupBox6.Controls.Add(this.btnWrite);
            this.groupBox6.Controls.Add(this.label12);
            this.groupBox6.Controls.Add(this.label13);
            this.groupBox6.Controls.Add(this.txtPID);
            this.groupBox6.Controls.Add(this.nudDLC);
            this.groupBox6.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox6.Location = new System.Drawing.Point(8, 362);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(714, 149);
            this.groupBox6.TabIndex = 51;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Write Messages";
            // 
            // laLength
            // 
            this.laLength.AutoSize = true;
            this.laLength.Location = new System.Drawing.Point(143, 29);
            this.laLength.Name = "laLength";
            this.laLength.Size = new System.Drawing.Size(62, 13);
            this.laLength.TabIndex = 121;
            this.laLength.Text = "Length: 9 B";
            // 
            // btnEdit
            // 
            this.btnEdit.Location = new System.Drawing.Point(642, 54);
            this.btnEdit.Name = "btnEdit";
            this.btnEdit.Size = new System.Drawing.Size(65, 22);
            this.btnEdit.TabIndex = 120;
            this.btnEdit.Text = "Edit";
            this.btnEdit.UseVisualStyleBackColor = true;
            this.btnEdit.Click += new System.EventHandler(this.btnEdit_Click);
            // 
            // btnGenerate
            // 
            this.btnGenerate.Location = new System.Drawing.Point(643, 82);
            this.btnGenerate.Name = "btnGenerate";
            this.btnGenerate.Size = new System.Drawing.Size(65, 22);
            this.btnGenerate.TabIndex = 120;
            this.btnGenerate.Text = "Generate";
            this.btnGenerate.UseVisualStyleBackColor = true;
            this.btnGenerate.Click += new System.EventHandler(this.btnGenerate_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(242, 29);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(33, 13);
            this.label7.TabIndex = 119;
            this.label7.Text = "Data:";
            // 
            // txtData
            // 
            this.txtData.Location = new System.Drawing.Point(245, 51);
            this.txtData.Multiline = true;
            this.txtData.Name = "txtData";
            this.txtData.ReadOnly = true;
            this.txtData.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtData.Size = new System.Drawing.Size(392, 82);
            this.txtData.TabIndex = 118;
            this.txtData.TabStop = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(146, 91);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(51, 13);
            this.label5.TabIndex = 117;
            this.label5.Text = "AF (Hex):";
            // 
            // txtAF
            // 
            this.txtAF.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.txtAF.Location = new System.Drawing.Point(149, 113);
            this.txtAF.MaxLength = 8;
            this.txtAF.Name = "txtAF";
            this.txtAF.Size = new System.Drawing.Size(90, 20);
            this.txtAF.TabIndex = 6;
            this.txtAF.Text = "00000000";
            this.txtAF.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtPID_KeyPress);
            this.txtAF.Leave += new System.EventHandler(this.txtAF_Leave);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(74, 91);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 13);
            this.label3.TabIndex = 115;
            this.label3.Text = "VCID (Hex):";
            // 
            // txtVCID
            // 
            this.txtVCID.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.txtVCID.Location = new System.Drawing.Point(77, 113);
            this.txtVCID.MaxLength = 2;
            this.txtVCID.Name = "txtVCID";
            this.txtVCID.Size = new System.Drawing.Size(53, 20);
            this.txtVCID.TabIndex = 5;
            this.txtVCID.Text = "00";
            this.txtVCID.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtPID_KeyPress);
            this.txtVCID.Leave += new System.EventHandler(this.txtVCID_Leave);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 91);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(60, 13);
            this.label4.TabIndex = 113;
            this.label4.Text = "SDT (Hex):";
            // 
            // txtSDT
            // 
            this.txtSDT.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.txtSDT.Location = new System.Drawing.Point(10, 113);
            this.txtSDT.MaxLength = 2;
            this.txtSDT.Name = "txtSDT";
            this.txtSDT.Size = new System.Drawing.Size(53, 20);
            this.txtSDT.TabIndex = 4;
            this.txtSDT.Text = "00";
            this.txtSDT.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtPID_KeyPress);
            this.txtSDT.Leave += new System.EventHandler(this.txtSDT_Leave);
            // 
            // chbSEC
            // 
            this.chbSEC.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.chbSEC.Location = new System.Drawing.Point(199, 51);
            this.chbSEC.Name = "chbSEC";
            this.chbSEC.Size = new System.Drawing.Size(40, 24);
            this.chbSEC.TabIndex = 3;
            this.chbSEC.Text = "SEC";
            // 
            // chbRRS
            // 
            this.chbRRS.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.chbRRS.Location = new System.Drawing.Point(149, 51);
            this.chbRRS.Name = "chbRRS";
            this.chbRRS.Size = new System.Drawing.Size(44, 24);
            this.chbRRS.TabIndex = 2;
            this.chbRRS.Text = "RRS";
            // 
            // btnWrite
            // 
            this.btnWrite.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnWrite.Enabled = false;
            this.btnWrite.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnWrite.Location = new System.Drawing.Point(643, 112);
            this.btnWrite.Name = "btnWrite";
            this.btnWrite.Size = new System.Drawing.Size(65, 22);
            this.btnWrite.TabIndex = 7;
            this.btnWrite.Text = "Write";
            this.btnWrite.Click += new System.EventHandler(this.btnWrite_Click);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(74, 29);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(31, 13);
            this.label12.TabIndex = 31;
            this.label12.Text = "DLC:";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(7, 29);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(56, 13);
            this.label13.TabIndex = 30;
            this.label13.Text = "PID (Hex):";
            // 
            // txtPID
            // 
            this.txtPID.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.txtPID.Location = new System.Drawing.Point(10, 51);
            this.txtPID.MaxLength = 3;
            this.txtPID.Name = "txtPID";
            this.txtPID.Size = new System.Drawing.Size(53, 20);
            this.txtPID.TabIndex = 0;
            this.txtPID.Text = "000";
            this.txtPID.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtPID_KeyPress);
            this.txtPID.Leave += new System.EventHandler(this.txtPID_Leave);
            // 
            // nudDLC
            // 
            this.nudDLC.BackColor = System.Drawing.Color.White;
            this.nudDLC.Location = new System.Drawing.Point(77, 51);
            this.nudDLC.Maximum = new decimal(new int[] {
            2047,
            0,
            0,
            0});
            this.nudDLC.Name = "nudDLC";
            this.nudDLC.Size = new System.Drawing.Size(52, 20);
            this.nudDLC.TabIndex = 1;
            this.nudDLC.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.nudDLC.ValueChanged += new System.EventHandler(this.nudDLC_ValueChanged);
            // 
            // tmrRead
            // 
            this.tmrRead.Interval = 50;
            this.tmrRead.Tick += new System.EventHandler(this.tmrRead_Tick);
            // 
            // tmrDisplay
            // 
            this.tmrDisplay.Tick += new System.EventHandler(this.tmrDisplay_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(734, 612);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimumSize = new System.Drawing.Size(750, 651);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "PCAN-Basic XL Sample";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.nudDelay)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudDeviceId)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudDLC)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox cbbChannel;
        private System.Windows.Forms.Button btnRelease;
        private System.Windows.Forms.Button btnInit;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnFilterApply;
        private System.Windows.Forms.Button btnParameterSet;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.RadioButton rdbParamActive;
        private System.Windows.Forms.RadioButton rdbParamInactive;
        private System.Windows.Forms.ComboBox cbbParameter;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.NumericUpDown nudDeviceId;
        private System.Windows.Forms.Label laDeviceOrDelay;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.RadioButton rdbEvent;
        private System.Windows.Forms.ListView lstMessages;
        private System.Windows.Forms.ColumnHeader clhPID;
        private System.Windows.Forms.ColumnHeader clhDLC;
        private System.Windows.Forms.ColumnHeader clhData;
        private System.Windows.Forms.ColumnHeader clhRcvTime;
        private System.Windows.Forms.RadioButton rdbTimer;
        private System.Windows.Forms.RadioButton rdbManual;
        private System.Windows.Forms.Button btnRead;
        private System.Windows.Forms.Button btnMsgClear;
        private System.Windows.Forms.CheckBox chbShowPeriod;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.CheckBox chbRRS;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox txtPID;
        private System.Windows.Forms.NumericUpDown nudDLC;
        private System.Windows.Forms.Button btnWrite;
        private System.Windows.Forms.Button btnInfoClear;
        private System.Windows.Forms.Button btnGetVersions;
        private System.Windows.Forms.Button btnParameterGet;
        private System.Windows.Forms.Button btnFilterQuery;
        private System.Windows.Forms.ListBox lbxInfo;
        private System.Windows.Forms.Timer tmrRead;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.Button btnStatus;
        private System.Windows.Forms.Timer tmrDisplay;
        private System.Windows.Forms.TextBox txtBitrate;
        private System.Windows.Forms.CheckBox chbSEC;
        private System.Windows.Forms.NumericUpDown nudDelay;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.RadioButton rdbFilterOpen;
        private System.Windows.Forms.RadioButton rdbFilterClose;
        private System.Windows.Forms.ColumnHeader clhSDT;
        private System.Windows.Forms.ColumnHeader clhVCID;
        private System.Windows.Forms.ColumnHeader clhAF;
        private System.Windows.Forms.ColumnHeader clhCount;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtSDT;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtAF;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtVCID;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox txtData;
        private System.Windows.Forms.ColumnHeader clhType;
        private System.Windows.Forms.Button btnGenerate;
        private System.Windows.Forms.Button btnEdit;
        private System.Windows.Forms.Label laLength;
    }
}

