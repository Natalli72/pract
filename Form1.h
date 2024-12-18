#pragma once

#include "MyPanel.h"
#include "Game.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


/// <summary>
/// Summary for Form1
///
/// WARNING: If you change the name of this class, you will need to change the
///          'Resource File Name' property for the managed resource compiler tool
///          associated with all .resx files this class depends on.  Otherwise,
///          the designers will not be able to interact properly with localized
///          resources associated with this form.
/// </summary>
public ref class Form1 : public System::Windows::Forms::Form
{
public:
	Form1(void);

	void startNewGame(int numPlayers, int numPoints);

protected:
	Game *game;
	const float SCALE = 0.35f;	//масштабирование изображений домино

	int *playerScores;	//массив с очками игроков
	int endScores;		//сколько надо очков для завершения игры
	void countScores();	//подсчет очков
		
	Image^ deskImage;	//изображение стола
	Image^ stepImage;	//изображение песочных часов
	Image^ backImage;	//изображение обратной стороны домино
private: System::Windows::Forms::Timer^  timer1;
private: MyPanel^  myPanel1;
private: System::Windows::Forms::MenuStrip^  menuStrip1;
private: System::Windows::Forms::ToolStripMenuItem^  новаяИграToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  справкаToolStripMenuItem;

protected:
	Dictionary<String^, Image^>^ dominoImages;	//изображения костей домино

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~Form1()
	{
		if (components)
		{
			delete components;
		}
	}
private: System::ComponentModel::IContainer^  components;

private:
	/// <summary>
	/// Required designer variable.
	/// </summary>


#pragma region Windows Form Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
		this->components = (gcnew System::ComponentModel::Container());
		this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
		this->myPanel1 = (gcnew MyPanel());
		this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
		this->новаяИграToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
		this->справкаToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
		this->menuStrip1->SuspendLayout();
		this->SuspendLayout();
		// 
		// timer1
		// 
		this->timer1->Interval = 300;
		this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
		// 
		// myPanel1
		// 
		this->myPanel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->myPanel1->Location = System::Drawing::Point(0, 27);
		this->myPanel1->Name = L"myPanel1";
		this->myPanel1->Size = System::Drawing::Size(1104, 623);
		this->myPanel1->TabIndex = 0;
		this->myPanel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::myPanel1_Paint);
		this->myPanel1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::myPanel1_MouseClick);
		this->myPanel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::myPanel1_MouseMove);
		// 
		// menuStrip1
		// 
		this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
			this->новаяИграToolStripMenuItem,
				this->справкаToolStripMenuItem
		});
		this->menuStrip1->Location = System::Drawing::Point(0, 0);
		this->menuStrip1->Name = L"menuStrip1";
		this->menuStrip1->Size = System::Drawing::Size(1104, 24);
		this->menuStrip1->TabIndex = 1;
		this->menuStrip1->Text = L"menuStrip1";
		// 
		// новаяИграToolStripMenuItem
		// 
		this->новаяИграToolStripMenuItem->Name = L"новаяИграToolStripMenuItem";
		this->новаяИграToolStripMenuItem->Size = System::Drawing::Size(81, 20);
		this->новаяИграToolStripMenuItem->Text = L"Новая игра";
		this->новаяИграToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::новаяИграToolStripMenuItem_Click);
		// 
		// справкаToolStripMenuItem
		// 
		this->справкаToolStripMenuItem->Name = L"справкаToolStripMenuItem";
		this->справкаToolStripMenuItem->Size = System::Drawing::Size(65, 20);
		this->справкаToolStripMenuItem->Text = L"Справка";
		this->справкаToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::справкаToolStripMenuItem_Click);
		// 
		// Form1
		// 
		this->ClientSize = System::Drawing::Size(1104, 650);
		this->Controls->Add(this->myPanel1);
		this->Controls->Add(this->menuStrip1);
		this->Name = L"Form1";
		this->Text = L"Домино";
		this->menuStrip1->ResumeLayout(false);
		this->menuStrip1->PerformLayout();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion
	private: 
		System::Void myPanel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
		System::Void myPanel1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		System::Void myPanel1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
private: System::Void menuStrip1_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
}
private: System::Void новаяИграToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void справкаToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
};


