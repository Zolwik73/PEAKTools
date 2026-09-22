using System;
using System.Drawing;
using System.Windows.Forms;

namespace PCANBasicExample.Windows
{
    partial class EditDataForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            this.hexDataGridView = new System.Windows.Forms.DataGridView();
            this.hex0 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex4 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex5 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex6 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex7 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex8 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex9 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex10 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex11 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex12 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex13 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex14 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex15 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex16 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex17 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex18 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex19 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex20 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex21 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex22 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex23 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex24 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex25 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex26 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex27 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex28 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex29 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex30 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.hex31 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.cancelButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.hexDataGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // hexDataGridView
            // 
            this.hexDataGridView.AllowUserToAddRows = false;
            this.hexDataGridView.AllowUserToDeleteRows = false;
            this.hexDataGridView.AllowUserToResizeColumns = false;
            this.hexDataGridView.AllowUserToResizeRows = false;
            this.hexDataGridView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.hexDataGridView.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.hexDataGridView.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.None;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("Segoe UI", 9F);
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.hexDataGridView.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this.hexDataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.hexDataGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.hex0,
            this.hex1,
            this.hex2,
            this.hex3,
            this.hex4,
            this.hex5,
            this.hex6,
            this.hex7,
            this.hex8,
            this.hex9,
            this.hex10,
            this.hex11,
            this.hex12,
            this.hex13,
            this.hex14,
            this.hex15,
            this.hex16,
            this.hex17,
            this.hex18,
            this.hex19,
            this.hex20,
            this.hex21,
            this.hex22,
            this.hex23,
            this.hex24,
            this.hex25,
            this.hex26,
            this.hex27,
            this.hex28,
            this.hex29,
            this.hex30,
            this.hex31});
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle2.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle2.Font = new System.Drawing.Font("Segoe UI", 9F);
            dataGridViewCellStyle2.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.hexDataGridView.DefaultCellStyle = dataGridViewCellStyle2;
            this.hexDataGridView.Location = new System.Drawing.Point(9, 10);
            this.hexDataGridView.Name = "hexDataGridView";
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle3.Font = new System.Drawing.Font("Segoe UI", 9F);
            dataGridViewCellStyle3.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle3.Format = "N0";
            dataGridViewCellStyle3.NullValue = null;
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle3.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.hexDataGridView.RowHeadersDefaultCellStyle = dataGridViewCellStyle3;
            this.hexDataGridView.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            this.hexDataGridView.Size = new System.Drawing.Size(881, 391);
            this.hexDataGridView.TabIndex = 0;
            this.hexDataGridView.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.hexDataGridView_CellEndEdit);
            this.hexDataGridView.EditingControlShowing += new System.Windows.Forms.DataGridViewEditingControlShowingEventHandler(this.hexDataGridView_EditingControlShowing);
            this.hexDataGridView.RowPostPaint += new System.Windows.Forms.DataGridViewRowPostPaintEventHandler(this.hexDataGridView_RowPostPaint);
            // 
            // hex0
            // 
            this.hex0.HeaderText = "0";
            this.hex0.Name = "hex0";
            this.hex0.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex1
            // 
            this.hex1.HeaderText = "1";
            this.hex1.Name = "hex1";
            this.hex1.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex2
            // 
            this.hex2.HeaderText = "2";
            this.hex2.Name = "hex2";
            this.hex2.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex3
            // 
            this.hex3.HeaderText = "3";
            this.hex3.Name = "hex3";
            this.hex3.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex4
            // 
            this.hex4.HeaderText = "4";
            this.hex4.Name = "hex4";
            this.hex4.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex5
            // 
            this.hex5.HeaderText = "5";
            this.hex5.Name = "hex5";
            this.hex5.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex6
            // 
            this.hex6.HeaderText = "6";
            this.hex6.Name = "hex6";
            this.hex6.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex7
            // 
            this.hex7.HeaderText = "7";
            this.hex7.Name = "hex7";
            this.hex7.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex8
            // 
            this.hex8.HeaderText = "8";
            this.hex8.Name = "hex8";
            this.hex8.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex9
            // 
            this.hex9.HeaderText = "9";
            this.hex9.Name = "hex9";
            this.hex9.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex10
            // 
            this.hex10.HeaderText = "10";
            this.hex10.Name = "hex10";
            this.hex10.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex11
            // 
            this.hex11.HeaderText = "11";
            this.hex11.Name = "hex11";
            this.hex11.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex12
            // 
            this.hex12.HeaderText = "12";
            this.hex12.Name = "hex12";
            this.hex12.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex13
            // 
            this.hex13.HeaderText = "13";
            this.hex13.Name = "hex13";
            this.hex13.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex14
            // 
            this.hex14.HeaderText = "14";
            this.hex14.Name = "hex14";
            this.hex14.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex15
            // 
            this.hex15.HeaderText = "15";
            this.hex15.Name = "hex15";
            this.hex15.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex16
            // 
            this.hex16.HeaderText = "16";
            this.hex16.Name = "hex16";
            this.hex16.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex17
            // 
            this.hex17.HeaderText = "17";
            this.hex17.Name = "hex17";
            this.hex17.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex18
            // 
            this.hex18.HeaderText = "18";
            this.hex18.Name = "hex18";
            this.hex18.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex19
            // 
            this.hex19.HeaderText = "19";
            this.hex19.Name = "hex19";
            this.hex19.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex20
            // 
            this.hex20.HeaderText = "20";
            this.hex20.Name = "hex20";
            this.hex20.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex21
            // 
            this.hex21.HeaderText = "21";
            this.hex21.Name = "hex21";
            this.hex21.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex22
            // 
            this.hex22.HeaderText = "22";
            this.hex22.Name = "hex22";
            this.hex22.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex23
            // 
            this.hex23.HeaderText = "23";
            this.hex23.Name = "hex23";
            this.hex23.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex24
            // 
            this.hex24.HeaderText = "24";
            this.hex24.Name = "hex24";
            this.hex24.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex25
            // 
            this.hex25.HeaderText = "25";
            this.hex25.Name = "hex25";
            this.hex25.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex26
            // 
            this.hex26.HeaderText = "26";
            this.hex26.Name = "hex26";
            this.hex26.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex27
            // 
            this.hex27.HeaderText = "27";
            this.hex27.Name = "hex27";
            this.hex27.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex28
            // 
            this.hex28.HeaderText = "28";
            this.hex28.Name = "hex28";
            this.hex28.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex29
            // 
            this.hex29.HeaderText = "29";
            this.hex29.Name = "hex29";
            this.hex29.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex30
            // 
            this.hex30.HeaderText = "30";
            this.hex30.Name = "hex30";
            this.hex30.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // hex31
            // 
            this.hex31.HeaderText = "31";
            this.hex31.Name = "hex31";
            this.hex31.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(826, 406);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(64, 20);
            this.cancelButton.TabIndex = 2;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(757, 406);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(64, 20);
            this.okButton.TabIndex = 3;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // EditDataForm
            // 
            this.AcceptButton = this.okButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(901, 434);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.hexDataGridView);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "EditDataForm";
            this.Text = "Edit data";
            ((System.ComponentModel.ISupportInitialize)(this.hexDataGridView)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private DataGridView hexDataGridView;
        private Button cancelButton;
        private Button okButton;
        private DataGridViewTextBoxColumn hex0;
        private DataGridViewTextBoxColumn hex1;
        private DataGridViewTextBoxColumn hex2;
        private DataGridViewTextBoxColumn hex3;
        private DataGridViewTextBoxColumn hex4;
        private DataGridViewTextBoxColumn hex5;
        private DataGridViewTextBoxColumn hex6;
        private DataGridViewTextBoxColumn hex7;
        private DataGridViewTextBoxColumn hex8;
        private DataGridViewTextBoxColumn hex9;
        private DataGridViewTextBoxColumn hex10;
        private DataGridViewTextBoxColumn hex11;
        private DataGridViewTextBoxColumn hex12;
        private DataGridViewTextBoxColumn hex13;
        private DataGridViewTextBoxColumn hex14;
        private DataGridViewTextBoxColumn hex15;
        private DataGridViewTextBoxColumn hex16;
        private DataGridViewTextBoxColumn hex17;
        private DataGridViewTextBoxColumn hex18;
        private DataGridViewTextBoxColumn hex19;
        private DataGridViewTextBoxColumn hex20;
        private DataGridViewTextBoxColumn hex21;
        private DataGridViewTextBoxColumn hex22;
        private DataGridViewTextBoxColumn hex23;
        private DataGridViewTextBoxColumn hex24;
        private DataGridViewTextBoxColumn hex25;
        private DataGridViewTextBoxColumn hex26;
        private DataGridViewTextBoxColumn hex27;
        private DataGridViewTextBoxColumn hex28;
        private DataGridViewTextBoxColumn hex29;
        private DataGridViewTextBoxColumn hex30;
        private DataGridViewTextBoxColumn hex31;
    }
}
