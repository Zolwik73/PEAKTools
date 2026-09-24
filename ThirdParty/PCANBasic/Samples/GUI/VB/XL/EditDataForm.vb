Public Class EditDataForm
#Region "Members"
    Dim m_result() As Byte

    ' The dataset as an array of bytes
    Dim m_data() As Byte

    ' The DLC for the message increase by one inside the constructor
    Dim m_dataLength As Decimal
#End Region

#Region "Methods"
#Region "Constructors"
    ' <summary>
    ' Constructor
    ' </summary>
    ' <param name="data">The datasets as an array of bytes, provided by the window which calls this constructor</param>
    ' <param name="dlc">The data length code provided by the window which calls this constructor</param>
    ' 
    Public Sub New(ByVal data() As Byte, ByVal dlc As Decimal)

        m_data = data
        m_dataLength = dlc + 1

        StartPosition = FormStartPosition.CenterParent

        ' Initializes EditDataForm's component
        '
        InitializeComponent()

        InitializeHexDataGrid(data)
    End Sub
#End Region

#Region "Help functions"
    ' <summary>
    ' Builds the data grid with given data, formatted in hex
    ' </summary>
    ' <param name="data">The datasets to display</param>
    Private Sub InitializeHexDataGrid(data() As Byte)
        hexDataGridView.Rows.Clear()

        For i As Integer = 0 To m_dataLength - 1 Step 32
            Dim dataSet(31) As String

            Dim index As Integer = 0
            For j As Integer = i To Math.Min(i + 31, m_dataLength - 1)
                ' Converts each item in the data array into a two digit hex format
                ' and attaches to the dataset string array
                dataSet(index) = data(j).ToString("X2")
                index += 1
            Next

            ' Adds each dataset value to the current row
            hexDataGridView.Rows.Add(
                dataSet(0), dataSet(1), dataSet(2), dataSet(3),
                dataSet(4), dataSet(5), dataSet(6), dataSet(7),
                dataSet(8), dataSet(9), dataSet(10), dataSet(11),
                dataSet(12), dataSet(13), dataSet(14), dataSet(15),
                dataSet(16), dataSet(17), dataSet(18), dataSet(19),
                dataSet(20), dataSet(21), dataSet(22), dataSet(23),
                dataSet(24), dataSet(25), dataSet(26), dataSet(27),
                dataSet(28), dataSet(29), dataSet(30), dataSet(31)
            )
        Next

        ' Accesses the last row and...
        Dim lastRow As DataGridViewRow = hexDataGridView.Rows(hexDataGridView.Rows.Count - 1)

        ' ... and makes it uneditable if there is no value is to be inserted.
        For Each cell As DataGridViewCell In lastRow.Cells
            Dim content As String = If(cell.Value, String.Empty).ToString()
            If String.IsNullOrEmpty(content) Then
                cell.ReadOnly = True
            End If
        Next
    End Sub

    ' <summary>
    ' Gets a byte array of datasets from the grid view
    ' </summary>
    ' <returns>A byte array of datasets</returns>
    Private Function GetByteArrayFromDataSets() As Byte()
        Dim bytes() As Byte = New Byte(m_data.Length) {}

        Dim index As Integer = 0
        For Each row As DataGridViewRow In hexDataGridView.Rows
            For Each cell As DataGridViewCell In row.Cells

                Dim content As String = cell.Value?.ToString()

                If Not content = String.Empty Then
                    bytes(index) = Convert.ToByte(content, 16)
                    index += 1
                End If
            Next
        Next

        Return bytes
    End Function
#End Region

#Region "Event handlers"
    Private Sub hexDataGridView_CellEndEdit(sender As Object, e As DataGridViewCellEventArgs) Handles hexDataGridView.CellEndEdit
        Dim cell As DataGridViewCell = hexDataGridView.Rows(e.RowIndex).Cells(e.ColumnIndex)
        If cell.Value?.ToString() = "" Then
            cell.Value = "00"
            Return
        End If

        Dim Input As String = cell.Value?.ToString()

        Dim value As Int32 = Convert.ToInt32(Input, 16)
        cell.Value = value.ToString("X2")
    End Sub

    Private Sub hexDataGridView_EditingControlShowing(sender As Object, e As DataGridViewEditingControlShowingEventArgs) Handles hexDataGridView.EditingControlShowing
        If e.Control Is GetType(TextBox) Then
            Dim textBox As TextBox = TryCast(sender, TextBox)
            textBox.MaxLength = 2
            RemoveHandler textBox.KeyPress, AddressOf HexKeyPress
            AddHandler textBox.KeyPress, AddressOf HexKeyPress
        End If
    End Sub

    Private Sub hexDataGridView_RowPostPaint(sender As Object, e As DataGridViewRowPostPaintEventArgs) Handles hexDataGridView.RowPostPaint
        Dim grid As DataGridView = TryCast(sender, DataGridView)
        Dim rowIdx As String = e.RowIndex.ToString()

        Dim centerFormat As StringFormat = New StringFormat() With
        {
                .Alignment = StringAlignment.Center,
                .LineAlignment = StringAlignment.Center
        }

        Dim headerBounds As Rectangle = New Rectangle(e.RowBounds.Left, e.RowBounds.Top, grid.RowHeadersWidth, e.RowBounds.Height)
        e.Graphics.DrawString(rowIdx, Me.Font, SystemBrushes.ControlText, headerBounds, centerFormat)
    End Sub

    Private Sub okButton_Click(sender As Object, e As EventArgs) Handles okButton.Click
        Result = GetByteArrayFromDataSets()
        DialogResult = DialogResult.OK

        Close()
    End Sub

    Private Sub cancelButton_Click(sender As Object, e As EventArgs) Handles cancelButton.Click
        DialogResult = DialogResult.Cancel
        Close()
    End Sub

    Private Sub HexKeyPress(sender As Object, e As KeyPressEventArgs)

        Dim c As Char = e.KeyChar

        If Char.IsControl(c) Then
            Return
        End If

        If Not Uri.IsHexDigit(c) Then
            e.Handled = True
        End If
    End Sub
#End Region
#End Region

#Region "Properties"
    Property Result As Byte()
        Get
            Return m_result
        End Get
        Private Set(value As Byte())
            m_result = value
        End Set
    End Property
#End Region
End Class