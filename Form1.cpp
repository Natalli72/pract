#include "stdafx.h"
#include "Form1.h"
#include "NewGame.h"
#include "HelpForm.h"

Form1::Form1(void)
{
	InitializeComponent();

	game = NULL;
	//��������� �����������
	deskImage = Image::FromFile("images/wooden-texture.png");
	backImage = Image::FromFile("images/back.png");
	stepImage = Image::FromFile("images/hourglass-icon.png");
	dominoImages = gcnew Dictionary<String^, Image^>(28);
	for (int i=0; i<7; i++)
		for (int j = i; j < 7; j++)
		{
			dominoImages->Add(i + "-" + j, Image::FromFile("images/" + i + "-" + j + ".png"));
		}	
	
	//������
	timer1->Interval = 1000;
	timer1->Enabled = false;
}

//������ ����� ����
void Form1::startNewGame(int numPlayers, int numPoints)
{
	endScores = numPoints;
	game = new Game(numPlayers);
	//������� ������ �����
	playerScores = new int[numPlayers];
	for (int i = 0; i < numPlayers; i++)
		playerScores[i] = 0;
	//��������� ������
	timer1->Enabled = true;
	myPanel1->Invalidate();
}

//����� �� �����
System::Void Form1::myPanel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
{
	//������� ����
	e->Graphics->Clear(Color::White);

	Rectangle destRect = Rectangle(0, 0, myPanel1->Width, myPanel1->Height);	//������� ���� ��������
	Rectangle srcRect = Rectangle(0, 0, 3927, 1745);						//������� ������ ��������
	GraphicsUnit units = GraphicsUnit::Pixel;

	//����� ����
	e->Graphics->DrawImage(deskImage, destRect, srcRect, units);

	if (!game)
		return;

	int x = 100;
	int y = myPanel1->Height - 90;
	int i = 0;

	Pen^ pen = gcnew Pen(Color::Blue, 1.0f);	//pen ��� ��������� ��������
	
	//������� ����� ������
	auto player = game->players.begin();
	for (auto dominoBone = (*player)->dominos.begin(); dominoBone != (*player)->dominos.end(); dominoBone++)
	{
		destRect = Rectangle(x + i * 40, y, 107 * SCALE, 211 * SCALE);

		Rectangle srcRect = Rectangle(0, 0, 107, 211);
		
		e->Graphics->DrawImage(dominoImages[dominoBone->a + "-" + dominoBone->b], destRect, srcRect, units);

		//���� ���� ���������� �����, ������� ���������� �������������
		if (i == game->selected)
		{
			e->Graphics->DrawRectangle(pen, (float)x + i * 40, (float)y, (float)107 * SCALE, (float)211 * SCALE);
		}

		i++;
	}

	//������� ����� ������ 2 ������
	if (game->players.size() >= 2)
	{
		x = 300;
		y = 5;
		i = 0;

		player++;
		for (auto dominoBone = (*player)->dominos.begin(); dominoBone != (*player)->dominos.end(); dominoBone++)
		{
			destRect = Rectangle(x + i * 30, y, 107 * SCALE, 211 * SCALE);
			srcRect = Rectangle(0, 0, 107, 211);

			e->Graphics->DrawImage(backImage, destRect, srcRect, units);

			i++;
		}
	}

	//������� ����� ������ ������
	if (game->players.size() >= 3)
	{
		x = 5;
		y = 100;
		i = 0;

		backImage->RotateFlip(RotateFlipType::Rotate90FlipNone);
		
		player++;
		for (auto dominoBone = (*player)->dominos.begin(); dominoBone != (*player)->dominos.end(); dominoBone++)
		{
			destRect = Rectangle(x, y + i * 30, 211 * SCALE, 107 * SCALE);
			srcRect = Rectangle(0, 0, 211, 107);
			e->Graphics->DrawImage(backImage, destRect, srcRect, units);

			i++;
		}

		backImage->RotateFlip(RotateFlipType::Rotate270FlipNone);
	}

	//������� ����� ������� ������
	if (game->players.size() >= 4)
	{
		x = myPanel1->Width - 80;
		y = 100;
		i = 0;

		backImage->RotateFlip(RotateFlipType::Rotate90FlipNone);

		player++;
		for (auto dominoBone = (*player)->dominos.begin(); dominoBone != (*player)->dominos.end(); dominoBone++)
		{
			destRect = Rectangle(x, y + i * 30, 211 * SCALE, 107 * SCALE);
			srcRect = Rectangle(0, 0, 211, 107);
			e->Graphics->DrawImage(backImage, destRect, srcRect, units);

			i++;
		}

		backImage->RotateFlip(RotateFlipType::Rotate270FlipNone);
	}


	//������� �����
	x = myPanel1->Width - 250;
	y = myPanel1->Height - 90;
	i = 0;
	for (auto dominoBone = game->allDominos.begin(); dominoBone != game->allDominos.end(); dominoBone++)
	{
		destRect = Rectangle(x + i * 10, y, 107 * SCALE, 211 * SCALE);
		srcRect = Rectangle(0, 0, 107, 211);
		e->Graphics->DrawImage(backImage, destRect, srcRect, units);

		i++;
	}

	//������� ��� ������
	if (game->selected==-2 && game->allDominos.size()>0)
	{
		e->Graphics->DrawRectangle(pen, (float)x, (float)y, (float)107 * SCALE + (game->allDominos.size()-1)*10, (float)211 * SCALE);
	}

	//����� ���������� ������
	int width = 211 * SCALE;
	int height = 107 * SCALE;
	for (auto step = game->steps.begin(); step != game->steps.end(); step++)
	{
		Image ^dominoBone;
		bool backwards = false;
		//���� ������ � �������
		if (!dominoImages->TryGetValue(step->domino.a + "-" + step->domino.b, dominoBone))
		{
			//���� ���, �� ���� � �������� �������
			dominoBone = dominoImages[step->domino.b + "-" + step->domino.a];
			backwards = true;
		}

		dominoBone = (Image^)dominoBone->Clone();
		
		//���� ��������, �� ������������
		if (backwards)
			dominoBone->RotateFlip(RotateFlipType::Rotate180FlipNone);

		//����������� � ����������� �� ���� �������
		if (step->angle == 270)
		{
			srcRect = Rectangle(0, 0, 211, 107);
			destRect = Rectangle(step->x*width + 300, step->y*width + 150, width, height);
			dominoBone->RotateFlip(RotateFlipType::Rotate270FlipNone);
		}
		else if (step->angle == 90)
		{
			srcRect = Rectangle(0, 0, 211, 107);
			destRect = Rectangle(step->x*width + 300, step->y*width + 150, width, height);
			dominoBone->RotateFlip(RotateFlipType::Rotate90FlipNone);
		}
		else if (step->angle == 180)
		{
			srcRect = Rectangle(0, 0, 107, 211);
			destRect = Rectangle(step->x*width + 300, step->y*width + 150, height, width);
			dominoBone->RotateFlip(RotateFlipType::Rotate180FlipNone);
		}
		else if (step->angle == 0)
		{
			srcRect = Rectangle(0, 0, 107, 211);
			destRect = Rectangle(step->x*width + 300, step->y*width + 150, height, width);
		}

		e->Graphics->DrawImage(dominoBone, destRect, srcRect, units);

		delete dominoBone;
	}

	//����� �������� ����� 
	srcRect = Rectangle(0, 0, 256, 256);
	if (game->turn == 0)
	{
		destRect = Rectangle(50, myPanel1->Height - 90, 32, 32);
		e->Graphics->DrawImage(stepImage, destRect, srcRect, units);
	}
	else if (game->turn == 1)
	{
		destRect = Rectangle(250, 5, 32, 32);		
		e->Graphics->DrawImage(stepImage, destRect, srcRect, units);
	}
	else if (game->turn == 2)
	{
		destRect = Rectangle(5, 50, 32, 32);
		e->Graphics->DrawImage(stepImage, destRect, srcRect, units);
	}
	else if (game->turn == 3)
	{
		destRect = Rectangle(myPanel1->Width - 80, 50, 32, 32);
		e->Graphics->DrawImage(stepImage, destRect, srcRect, units);
	}
}

//���������� ������� ��� ������ ���� �� �������� ����
System::Void Form1::myPanel1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	//�������, ��� ������� ������� �� ��������������
	if (!game || game->isGameEnd || game->turn!=0)
		return;

	//���� �� ������
	if (game->selected >= 0)
	{
		game->addToTable(0, game->selected);	//�������� �� ����
		timer1->Enabled = true;					//�������� ������
		myPanel1->Invalidate();
	}
	else if (game->selected == -2 && game->turn == 0)	//���� �� ������
	{
		if (game->checkStepForPlayer() == 1)
		{
			game->takeFromBase(0);				//����� �� ������
			timer1->Enabled = true;
			myPanel1->Invalidate();
		}		
	}
}

//���������� ������� ���� �� �������� �� �������� ����
System::Void Form1::myPanel1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	//�������, ��� ������� ������� �� ��������������
	if (!game || game->isGameEnd || game->turn != 0)
		return;

	int x = 100;
	int y = myPanel1->Height - 90;
	int i = 0;

	//���������, ���� �� ����������� � ������
	for (auto player = game->players.front()->dominos.begin(); player != game->players.front()->dominos.end(); player++)
	{
		if (e->X >= x + i * 40 &&
			e->X <= x + i * 40 + 107 * SCALE &&
			e->Y >= y &&
			e->Y <= y + 211 * SCALE)
		{
			game->selected = i;
			myPanel1->Invalidate();
			return;
		}

		i++;
	}

	//����������� � �������
	x = myPanel1->Width - 250;
	y = myPanel1->Height - 90;
	i = 0;	
	if (e->X >= x &&
		e->X <= x + game->allDominos.size() * 40 + 107 * SCALE &&
		e->Y >= y &&
		e->Y <= y + 211 * SCALE)
	{
		game->selected = -2;
		myPanel1->Invalidate();
		return;
	}
		
	//��� �����������
	if (game->selected != -1)
	{
		game->selected = -1;
		myPanel1->Invalidate();
	}
}

//������� �������, ��������� ����� ����� �����������
System::Void Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	//���� ����� ������������
	if (game->turn == 0)
	{
		game->checkStepForPlayer();	//���������, ����� �� ������ ������������
		if (game->turn==0)			//���� �����, �� ������������� ������
			timer1->Enabled = false;
	}
	else
		game->doStep();	//����� ���������

	myPanel1->Invalidate();

	//���� ����� ����
	if (game->isGameEnd)
	{
		timer1->Enabled = false;
		countScores();		
	}	
}

//������� �����
void Form1::countScores()
{
	int i = 0;
	String ^text = "����:\r\n";
	int lose = -1;

	//������� ���� �� ���� �������������
	for (auto player = game->players.begin(); player != game->players.end(); player++, i++)
	{
		int sum = 0;	//����� ����� ������������
		for (auto domino = (*player)->dominos.begin(); domino != (*player)->dominos.end(); domino++)
		{
			if (domino->a == 0 && domino->b == 0)	//���� ����� ��������
				sum += 10;
			else
			{
				//���� ��������
				sum += domino->a;
				sum += domino->b;
			}
		}

		playerScores[i] += sum;
		//���� ���������� ����� ������ �����
		if (playerScores[i] >= endScores)
		{
			//����� ����
			lose = i;					
		}

		//����� ������������ 
		String ^playerName;
		if (i == 0)
			playerName = "��";
		else if (i == 1)
			playerName = "����� ������";
		else if (i == 2)
			playerName = "����� �����";
		else if (i == 3)
			playerName = "����� ������";

		//�������� ��� ���������
		text += playerName + ": � ������ " + sum + ", ����� " + playerScores[i] + "\r\n";
	}

	//���� �������� �����-������ �����
	if (lose != -1)
	{
		String ^mes;
		if (lose == 0)
			mes = "�� ���������\r\n";
		else if (lose == 1)
			mes = "����� ������ ��������\r\n" ;
		else if (lose == 2)
			mes = "����� ����� ��������\r\n";
		else if (lose == 3)
			mes = "����� ������ ��������\r\n";

		text = mes +  "\r\n" + text;

		MessageBox::Show(text);

		return;
	}

	//������� ����������� ������� ��������� �����
	text += "\r\n��������� �����?";
	auto dr = MessageBox::Show(text, "����� ������", MessageBoxButtons::YesNo);
	if (dr == ::DialogResult::Yes)
	{
		int num = game->players.size();
		delete game;
		game = new Game(num);
		
		timer1->Enabled = true;
		myPanel1->Invalidate();
	}
}

//����� ����
System::Void Form1::���������ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
	//�������� �����
	NewGame ^form = gcnew NewGame(this);
	form->Show();
}

//����� ������
System::Void Form1::�������ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
	//�������� �����
	HelpForm ^form = gcnew HelpForm();
	form->Show();
}