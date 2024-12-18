#pragma once
#include "stdafx.h"
#include "Form1.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


/// <summary>
/// Сводка для NewGame
/// </summary>
public ref class NewGame : public System::Windows::Forms::Form
{
public:
	NewGame(Form1 ^parentForm)
	{
		InitializeComponent();

		this->parentForm = parentForm;
	}

protected:
	Form1 ^parentForm;

	/// <summary>
	/// Освободить все используемые ресурсы.
	/// </summary>
	~NewGame()
	{
		if (components)
		{
			delete components;
		}
	}
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::NumericUpDown^  numericUpDownPlayers;

private: System::Windows::Forms::Button^  buttonStart;


private: System::Windows::Forms::Button^  buttonCansel;
private: System::Windows::Forms::NumericUpDown^  numericUpDownPoints;

private: System::Windows::Forms::Label^  label2;

protected:

private:
	/// <summary>
	/// Обязательная переменная конструктора.
	/// </summary>
	System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->numericUpDownPlayers = (gcnew System::Windows::Forms::NumericUpDown());
			this->buttonStart = (gcnew System::Windows::Forms::Button());
			this->buttonCansel = (gcnew System::Windows::Forms::Button());
			this->numericUpDownPoints = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDownPlayers))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDownPoints))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(113, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Количество игроков:";
			// 
			// numericUpDownPlayers
			// 
			this->numericUpDownPlayers->Location = System::Drawing::Point(140, 21);
			this->numericUpDownPlayers->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 4, 0, 0, 0 });
			this->numericUpDownPlayers->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
			this->numericUpDownPlayers->Name = L"numericUpDownPlayers";
			this->numericUpDownPlayers->Size = System::Drawing::Size(67, 20);
			this->numericUpDownPlayers->TabIndex = 1;
			this->numericUpDownPlayers->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
			// 
			// buttonStart
			// 
			this->buttonStart->Location = System::Drawing::Point(138, 84);
			this->buttonStart->Name = L"buttonStart";
			this->buttonStart->Size = System::Drawing::Size(109, 23);
			this->buttonStart->TabIndex = 2;
			this->buttonStart->Text = L"Начать игру";
			this->buttonStart->UseVisualStyleBackColor = true;
			this->buttonStart->Click += gcnew System::EventHandler(this, &NewGame::buttonStart_Click);
			// 
			// buttonCansel
			// 
			this->buttonCansel->Location = System::Drawing::Point(15, 84);
			this->buttonCansel->Name = L"buttonCansel";
			this->buttonCansel->Size = System::Drawing::Size(109, 23);
			this->buttonCansel->TabIndex = 3;
			this->buttonCansel->Text = L"Отмена";
			this->buttonCansel->UseVisualStyleBackColor = true;
			this->buttonCansel->Click += gcnew System::EventHandler(this, &NewGame::buttonCansel_Click);
			// 
			// numericUpDownPoints
			// 
			this->numericUpDownPoints->Location = System::Drawing::Point(140, 47);
			this->numericUpDownPoints->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->numericUpDownPoints->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 50, 0, 0, 0 });
			this->numericUpDownPoints->Name = L"numericUpDownPoints";
			this->numericUpDownPoints->Size = System::Drawing::Size(67, 20);
			this->numericUpDownPoints->TabIndex = 5;
			this->numericUpDownPoints->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 50, 0, 0, 0 });
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 49);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(102, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"До скольки очков:";
			// 
			// NewGame
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(281, 119);
			this->Controls->Add(this->numericUpDownPoints);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->buttonCansel);
			this->Controls->Add(this->buttonStart);
			this->Controls->Add(this->numericUpDownPlayers);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"NewGame";
			this->Text = L"Новая игра";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDownPlayers))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDownPoints))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void buttonCansel_Click(System::Object^  sender, System::EventArgs^  e) {
		Close();
	}
	private: System::Void buttonStart_Click(System::Object^  sender, System::EventArgs^  e);
};

