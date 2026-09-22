#pragma once

namespace PCANBasicExample {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for EditDataWindow
	/// </summary>
	public ref class EditDataForm : public System::Windows::Forms::Form
	{
	public:
		EditDataForm(array<unsigned char>^ data, Decimal dlc)
		{
			m_data = data;
			m_dataLength = static_cast<int>(dlc) + 1;

			StartPosition = FormStartPosition::CenterParent;

			InitializeComponent();

			InitializeHexDataGrid(data);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EditDataForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^ okButton;
	private: System::Windows::Forms::Button^ cancelButton;
	private: System::Windows::Forms::DataGridView^ hexDataGridView;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex0;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex5;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex6;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex7;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex8;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex9;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex10;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex11;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex12;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex13;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex14;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex15;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex16;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex17;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex18;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex19;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex20;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex21;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex22;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex23;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex24;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex25;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex26;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex27;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex28;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex29;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex30;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hex31;

	private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle4 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle5 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle6 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->okButton = (gcnew System::Windows::Forms::Button());
			this->cancelButton = (gcnew System::Windows::Forms::Button());
			this->hexDataGridView = (gcnew System::Windows::Forms::DataGridView());
			this->hex0 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex6 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex7 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex8 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex9 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex10 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex11 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex12 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex13 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex14 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex15 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex16 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex17 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex18 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex19 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex20 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex21 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex22 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex23 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex24 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex25 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex26 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex27 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex28 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex29 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex30 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hex31 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->hexDataGridView))->BeginInit();
			this->SuspendLayout();
			// 
			// okButton
			// 
			this->okButton->Location = System::Drawing::Point(758, 405);
			this->okButton->Name = L"okButton";
			this->okButton->Size = System::Drawing::Size(64, 20);
			this->okButton->TabIndex = 6;
			this->okButton->Text = L"OK";
			this->okButton->UseVisualStyleBackColor = true;
			this->okButton->Click += gcnew System::EventHandler(this, &EditDataForm::okButton_Click);
			// 
			// cancelButton
			// 
			this->cancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->cancelButton->Location = System::Drawing::Point(827, 405);
			this->cancelButton->Name = L"cancelButton";
			this->cancelButton->Size = System::Drawing::Size(64, 20);
			this->cancelButton->TabIndex = 5;
			this->cancelButton->Text = L"Cancel";
			this->cancelButton->UseVisualStyleBackColor = true;
			this->cancelButton->Click += gcnew System::EventHandler(this, &EditDataForm::cancelButton_Click);
			// 
			// hexDataGridView
			// 
			this->hexDataGridView->AllowUserToAddRows = false;
			this->hexDataGridView->AllowUserToDeleteRows = false;
			this->hexDataGridView->AllowUserToResizeColumns = false;
			this->hexDataGridView->AllowUserToResizeRows = false;
			this->hexDataGridView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->hexDataGridView->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->hexDataGridView->CellBorderStyle = System::Windows::Forms::DataGridViewCellBorderStyle::None;
			dataGridViewCellStyle4->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
			dataGridViewCellStyle4->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle4->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			dataGridViewCellStyle4->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle4->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle4->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle4->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->hexDataGridView->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle4;
			this->hexDataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->hexDataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(32) {
				this->hex0,
					this->hex1, this->hex2, this->hex3, this->hex4, this->hex5, this->hex6, this->hex7, this->hex8, this->hex9, this->hex10, this->hex11,
					this->hex12, this->hex13, this->hex14, this->hex15, this->hex16, this->hex17, this->hex18, this->hex19, this->hex20, this->hex21,
					this->hex22, this->hex23, this->hex24, this->hex25, this->hex26, this->hex27, this->hex28, this->hex29, this->hex30, this->hex31
			});
			dataGridViewCellStyle5->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
			dataGridViewCellStyle5->BackColor = System::Drawing::SystemColors::Window;
			dataGridViewCellStyle5->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			dataGridViewCellStyle5->ForeColor = System::Drawing::SystemColors::ControlText;
			dataGridViewCellStyle5->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle5->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle5->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
			this->hexDataGridView->DefaultCellStyle = dataGridViewCellStyle5;
			this->hexDataGridView->Location = System::Drawing::Point(10, 9);
			this->hexDataGridView->Name = L"hexDataGridView";
			dataGridViewCellStyle6->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle6->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle6->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			dataGridViewCellStyle6->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle6->Format = L"N0";
			dataGridViewCellStyle6->NullValue = nullptr;
			dataGridViewCellStyle6->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle6->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle6->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->hexDataGridView->RowHeadersDefaultCellStyle = dataGridViewCellStyle6;
			this->hexDataGridView->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
			this->hexDataGridView->Size = System::Drawing::Size(881, 391);
			this->hexDataGridView->TabIndex = 4;
			this->hexDataGridView->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &EditDataForm::hexDataGridView_CellEndEdit);
			this->hexDataGridView->EditingControlShowing += gcnew System::Windows::Forms::DataGridViewEditingControlShowingEventHandler(this, &EditDataForm::hexDataGridView_EditingControlShowing);
			this->hexDataGridView->RowPostPaint += gcnew System::Windows::Forms::DataGridViewRowPostPaintEventHandler(this, &EditDataForm::hexDataGridView_RowPostPaint);
			// 
			// hex0
			// 
			this->hex0->HeaderText = L"0";
			this->hex0->Name = L"hex0";
			this->hex0->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex1
			// 
			this->hex1->HeaderText = L"1";
			this->hex1->Name = L"hex1";
			this->hex1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex2
			// 
			this->hex2->HeaderText = L"2";
			this->hex2->Name = L"hex2";
			this->hex2->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex3
			// 
			this->hex3->HeaderText = L"3";
			this->hex3->Name = L"hex3";
			this->hex3->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex4
			// 
			this->hex4->HeaderText = L"4";
			this->hex4->Name = L"hex4";
			this->hex4->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex5
			// 
			this->hex5->HeaderText = L"5";
			this->hex5->Name = L"hex5";
			this->hex5->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex6
			// 
			this->hex6->HeaderText = L"6";
			this->hex6->Name = L"hex6";
			this->hex6->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex7
			// 
			this->hex7->HeaderText = L"7";
			this->hex7->Name = L"hex7";
			this->hex7->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex8
			// 
			this->hex8->HeaderText = L"8";
			this->hex8->Name = L"hex8";
			this->hex8->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex9
			// 
			this->hex9->HeaderText = L"9";
			this->hex9->Name = L"hex9";
			this->hex9->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex10
			// 
			this->hex10->HeaderText = L"10";
			this->hex10->Name = L"hex10";
			this->hex10->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex11
			// 
			this->hex11->HeaderText = L"11";
			this->hex11->Name = L"hex11";
			this->hex11->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex12
			// 
			this->hex12->HeaderText = L"12";
			this->hex12->Name = L"hex12";
			this->hex12->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex13
			// 
			this->hex13->HeaderText = L"13";
			this->hex13->Name = L"hex13";
			this->hex13->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex14
			// 
			this->hex14->HeaderText = L"14";
			this->hex14->Name = L"hex14";
			this->hex14->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex15
			// 
			this->hex15->HeaderText = L"15";
			this->hex15->Name = L"hex15";
			this->hex15->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex16
			// 
			this->hex16->HeaderText = L"16";
			this->hex16->Name = L"hex16";
			this->hex16->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex17
			// 
			this->hex17->HeaderText = L"17";
			this->hex17->Name = L"hex17";
			this->hex17->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex18
			// 
			this->hex18->HeaderText = L"18";
			this->hex18->Name = L"hex18";
			this->hex18->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex19
			// 
			this->hex19->HeaderText = L"19";
			this->hex19->Name = L"hex19";
			this->hex19->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex20
			// 
			this->hex20->HeaderText = L"20";
			this->hex20->Name = L"hex20";
			this->hex20->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex21
			// 
			this->hex21->HeaderText = L"21";
			this->hex21->Name = L"hex21";
			this->hex21->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex22
			// 
			this->hex22->HeaderText = L"22";
			this->hex22->Name = L"hex22";
			this->hex22->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex23
			// 
			this->hex23->HeaderText = L"23";
			this->hex23->Name = L"hex23";
			this->hex23->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex24
			// 
			this->hex24->HeaderText = L"24";
			this->hex24->Name = L"hex24";
			this->hex24->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex25
			// 
			this->hex25->HeaderText = L"25";
			this->hex25->Name = L"hex25";
			this->hex25->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex26
			// 
			this->hex26->HeaderText = L"26";
			this->hex26->Name = L"hex26";
			this->hex26->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex27
			// 
			this->hex27->HeaderText = L"27";
			this->hex27->Name = L"hex27";
			this->hex27->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex28
			// 
			this->hex28->HeaderText = L"28";
			this->hex28->Name = L"hex28";
			this->hex28->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex29
			// 
			this->hex29->HeaderText = L"29";
			this->hex29->Name = L"hex29";
			this->hex29->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex30
			// 
			this->hex30->HeaderText = L"30";
			this->hex30->Name = L"hex30";
			this->hex30->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// hex31
			// 
			this->hex31->HeaderText = L"31";
			this->hex31->Name = L"hex31";
			this->hex31->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// EditDataForm
			// 
			this->AcceptButton = this->okButton;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->cancelButton;
			this->ClientSize = System::Drawing::Size(901, 434);
			this->Controls->Add(this->okButton);
			this->Controls->Add(this->cancelButton);
			this->Controls->Add(this->hexDataGridView);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"EditDataForm";
			this->Text = L"Edit data";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->hexDataGridView))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

#pragma region Members
		array<unsigned char>^ m_data;
		Decimal m_dataLength;
#pragma endregion

#pragma region Methods
#pragma region Help functions
	private:
		/// <summary>
/// Builds the data grid with given data, formatted in hex
/// </summary>
/// <param name="data">The datasets to display</param>
		void InitializeHexDataGrid(array<unsigned char>^ data)
		{
			hexDataGridView->Rows->Clear();

			for (int i = 0; i < m_dataLength; i += 32)
			{
				array<String^>^ dataSet = gcnew array<String^>(32);

				int index = 0;
				for (int j = i; j < i + 32 && j < m_dataLength; j++)
				{
					/* Converts each item in the data array into a two digit hex format
					 and attaches to the dataset string array*/
					dataSet[index++] = data[j].ToString("X2");
				}

				// Adds each dataset value to the current row
				hexDataGridView->Rows->Add(dataSet[0], dataSet[1], dataSet[2], dataSet[3],
					dataSet[4], dataSet[5], dataSet[6], dataSet[7],
					dataSet[8], dataSet[9], dataSet[10], dataSet[11],
					dataSet[12], dataSet[13], dataSet[14], dataSet[15],
					dataSet[16], dataSet[17], dataSet[18], dataSet[19],
					dataSet[20], dataSet[21], dataSet[22], dataSet[23],
					dataSet[24], dataSet[25], dataSet[26], dataSet[27],
					dataSet[28], dataSet[29], dataSet[30], dataSet[31]);
			}

			// Accesses the last row and...
			DataGridViewRow^ lastRow = hexDataGridView->Rows[hexDataGridView->Rows->Count - 1];

			// ...  and makes it uneditable if there is no value is to be inserted.
			for each (System::Windows::Forms::DataGridViewCell ^ cell in lastRow->Cells)
			{
				if (cell->Value == nullptr ||
					String::IsNullOrEmpty(cell->Value->ToString()))
				{
					cell->ReadOnly = true;
				}
			}
		}

		/// <summary>
		/// Gets a byte array of datasets from the grid view
		/// </summary>
		/// <returns>A byte array of datasets</returns>
		array<unsigned char>^ GetByteArrayFromDataSets()
		{
			array<unsigned char>^ bytes = gcnew array<unsigned char>(m_data->Length);

			int index = 0;
			for each (DataGridViewRow ^ row in hexDataGridView->Rows)
			{
				for each (DataGridViewCell ^ cell in row->Cells)
				{
					if (cell->Value == nullptr)
						continue;

					String^ content = cell->Value->ToString();
					bytes[index++] = Convert::ToByte(content, 16);
				}
			}

			return bytes;
		}
#pragma endregion

#pragma region Event handlers
	private:
		void hexDataGridView_CellEndEdit(Object^ sender, DataGridViewCellEventArgs^ e)
		{
			DataGridViewCell^ cell = hexDataGridView->Rows[e->RowIndex]->Cells[e->ColumnIndex];

			String^ input = cell->Value != nullptr ? cell->Value->ToString() : "";

			if (String::IsNullOrEmpty(input))
			{
				cell->Value = "00";
				return;
			}

			int value = Convert::ToInt32(input, 16);
			cell->Value = value.ToString("X2");
		}

		void hexDataGridView_EditingControlShowing(Object^ sender, DataGridViewEditingControlShowingEventArgs^ e)
		{
			TextBox^ textBox = dynamic_cast<TextBox^>(e->Control);
			if (textBox != nullptr)
			{
				textBox->MaxLength = 2;
				textBox->KeyPress -= gcnew KeyPressEventHandler(this, &EditDataForm::HexKeyPress);;
				textBox->KeyPress += gcnew KeyPressEventHandler(this, &EditDataForm::HexKeyPress);;
			}
		}

		void hexDataGridView_RowPostPaint(Object^ sender, DataGridViewRowPostPaintEventArgs^ e)
		{
			DataGridView^ grid = static_cast<DataGridView^>(sender);
			String^ rowIdx = e->RowIndex.ToString();

			StringFormat^ centerFormat = gcnew StringFormat();
			centerFormat->Alignment = StringAlignment::Center;
			centerFormat->LineAlignment = StringAlignment::Center;

			Rectangle headerBounds = Rectangle(e->RowBounds.Left, e->RowBounds.Top, grid->RowHeadersWidth, e->RowBounds.Height);
			e->Graphics->DrawString(rowIdx, this->Font, SystemBrushes::ControlText, headerBounds, centerFormat);
		}

		void okButton_Click(Object^ sender, EventArgs^ e)
		{
			Result = GetByteArrayFromDataSets();
			DialogResult = System::Windows::Forms::DialogResult::OK;

			Close();
		}

		void cancelButton_Click(Object^ sender, EventArgs^ e)
		{
			DialogResult = System::Windows::Forms::DialogResult::Cancel;
			Close();
		}

		void HexKeyPress(Object^ sender, KeyPressEventArgs^ e)
		{
			char c = e->KeyChar;

			if (Char::IsControl(c)) return;

			if (!Uri::IsHexDigit(c))
			{
				e->Handled = true;
			}
		}
#pragma endregion
#pragma endregion

#pragma region Properties
	public:
		// The array of datasets
		property array<unsigned char>^ Result;
#pragma endregion
	};
}
