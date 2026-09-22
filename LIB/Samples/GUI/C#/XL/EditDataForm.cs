using System;
using System.Drawing;
using System.Windows.Forms;

namespace PCANBasicExample.Windows
{
    public partial class EditDataForm : Form
    {
        #region Members
        // The dataset as an array of bytes
        byte[] m_data;

        // The DLC for the message increase by one inside the constructor
        decimal m_dataLength;
        #endregion

        #region Methods
        #region Constructors
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="data">The datasets as an array of bytes, provided by the window which calls this constructor</param>
        /// <param name="dlc">The data length code provided by the window which calls this constructor</param>
        /// 
        public EditDataForm(byte[] data, decimal dlc)
        {
            m_data = data;
            m_dataLength = dlc + 1;

            StartPosition = FormStartPosition.CenterParent;
            InitializeComponent();

            InitializeHexDataGrid(data);
        }
        #endregion

        #region Help functions
        /// <summary>
        /// Builds the data grid with given data, formatted in hex
        /// </summary>
        /// <param name="data">The datasets to display</param>
        private void InitializeHexDataGrid(byte[] data)
        {
            hexDataGridView.Rows.Clear();

            for (int i = 0; i < m_dataLength; i += 32)
            {
                string[] dataSet = new string[32];

                int index = 0;
                for (int j = i; j < i + 32 && j < m_dataLength; j++)
                {
                    /* Converts each item in the data array into a two digit hex format
                     and attaches to the dataset string array*/
                    dataSet[index++] = data[j].ToString("X2");
                }

                // Adds each dataset value to the current row
                hexDataGridView.Rows.Add(dataSet[0], dataSet[1], dataSet[2], dataSet[3],
                                         dataSet[4], dataSet[5], dataSet[6], dataSet[7],
                                         dataSet[8], dataSet[9], dataSet[10], dataSet[11],
                                         dataSet[12], dataSet[13], dataSet[14], dataSet[15],
                                         dataSet[16], dataSet[17], dataSet[18], dataSet[19],
                                         dataSet[20], dataSet[21], dataSet[22], dataSet[23],
                                         dataSet[24], dataSet[25], dataSet[26], dataSet[27],
                                         dataSet[28], dataSet[29], dataSet[30], dataSet[31]);
            }

            // Accesses the last row and...
            DataGridViewRow lastRow = hexDataGridView.Rows[hexDataGridView.Rows.Count - 1];

            // ...  and makes it uneditable if there is no value is to be inserted.
            foreach (DataGridViewCell cell in lastRow.Cells)
            {
                string content = cell.Value?.ToString();
                if (string.IsNullOrEmpty(content))
                {
                    cell.ReadOnly = true;
                }
            }
        }

        /// <summary>
        /// Gets a byte array of datasets from the grid view
        /// </summary>
        /// <returns>A byte array of datasets</returns>
        private byte[] GetByteArrayFromDataSets()
        {
            byte[] bytes = new byte[m_data.Length];

            int index = 0;
            foreach (DataGridViewRow row in hexDataGridView.Rows)
            {
                foreach (DataGridViewCell cell in row.Cells)
                {
                    string content = cell.Value?.ToString();

                    if (!string.IsNullOrEmpty(content))
                    {
                        bytes[index++] = Convert.ToByte(content, 16);
                    }
                }
            }

            return bytes;
        }
        #endregion

        #region Event handlers
        private void hexDataGridView_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            var cell = hexDataGridView.Rows[e.RowIndex].Cells[e.ColumnIndex];
            if (!(cell.Value?.ToString() is string input))
            {
                cell.Value = "00";
                return;
            }

            int value = Convert.ToInt32(input, 16);
            cell.Value = value.ToString("X2");
        }

        private void hexDataGridView_EditingControlShowing(object sender, DataGridViewEditingControlShowingEventArgs e)
        {
            if (e.Control is TextBox textBox)
            {
                textBox.MaxLength = 2;
                textBox.KeyPress -= HexKeyPress;
                textBox.KeyPress += HexKeyPress;
            }
        }

        private void hexDataGridView_RowPostPaint(object sender, DataGridViewRowPostPaintEventArgs e)
        {
            var grid = sender as DataGridView;
            var rowIdx = e.RowIndex.ToString();

            var centerFormat = new StringFormat()
            {
                Alignment = StringAlignment.Center,
                LineAlignment = StringAlignment.Center
            };

            var headerBounds = new Rectangle(e.RowBounds.Left, e.RowBounds.Top, grid.RowHeadersWidth, e.RowBounds.Height);
            e.Graphics.DrawString(rowIdx, this.Font, SystemBrushes.ControlText, headerBounds, centerFormat);
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            Result = GetByteArrayFromDataSets();
            DialogResult = DialogResult.OK;

            Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }

        private void HexKeyPress(object sender, KeyPressEventArgs e)
        {
            char c = e.KeyChar;

            if (char.IsControl(c)) return;

            if (!Uri.IsHexDigit(c))
            {
                e.Handled = true;
            }
        }
        #endregion
        #endregion

        #region Properties
        // The array of datasets
        public byte[] Result { get; private set; }
        #endregion
    }
}
