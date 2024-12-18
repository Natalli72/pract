#include "stdafx.h"
#include "Form1.h"
#include "NewGame.h"
#include "HelpForm.h"

Form1::Form1(void)
{
	InitializeComponent();

	game = NULL;
	//загружаем изображения
	deskImage = Image::FromFile("images/wooden-texture.png");
	backImage = Image::FromFile("images/back.png");
	stepImage = Image::FromFile("images/hourglass-icon.png");
	dominoImages = gcnew Dictionary<String^, Image^>(28);
	for (int i=0; i<7; i++)
		for (int j = i; j < 7; j++)
		{
			dominoImages->Add(i + "-" + j, Image::FromFile("images/" + i + "-" + j + ".png"));
		}	
	
	//таймер
	timer1->Interval = 1000;
	timer1->Enabled = false;
}

//начало новой игры
void Form1::startNewGame(int numPlayers, int numPoints)
{
	endScores = numPoints;
	game = new Game(numPlayers);
	//создаем массив очков
	playerScores = new int[numPlayers];
	for (int i = 0; i < numPlayers; i++)
		playerScores[i] = 0;
	//запускаем таймер
	timer1->Enabled = true;
	myPanel1->Invalidate();
}

//вывод на форму
System::Void Form1::myPanel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
{
	//очищаем поле
	e->Graphics->Clear(Color::White);

	Rectangle destRect = Rectangle(0, 0, myPanel1->Width, myPanel1->Height);	//область куда выводить
	Rectangle srcRect = Rectangle(0, 0, 3927, 1745);						//область откуда выводить
	GraphicsUnit units = GraphicsUnit::Pixel;

	//вывод фона
	e->Graphics->DrawImage(deskImage, destRect, srcRect, units);

	if (!game)
		return;

	int x = 100;
	int y = myPanel1->Height - 90;
	int i = 0;

	Pen^ pen = gcnew Pen(Color::Blue, 1.0f);	//pen для подсветки действий
	
	//выводим кости игрока
	auto player = game->players.begin();
	for (auto dominoBone = (*player)->dominos.begin(); dominoBone != (*player)->dominos.end(); dominoBone++)
	{
		destRect = Rectangle(x + i * 40, y, 107 * SCALE, 211 * SCALE);

		Rectangle srcRect = Rectangle(0, 0, 107, 211);
		
		e->Graphics->DrawImage(dominoImages[dominoBone->a + "-" + dominoBone->b], destRect, srcRect, units);

		//если есть выделенная кость, выводим выделяющий прямоугольник
		if (i == game->selected)
		{
			e->Graphics->DrawRectangle(pen, (float)x + i * 40, (float)y, (float)107 * SCALE, (float)211 * SCALE);
		}

		i++;
	}

	//выводим кости игрока 2 сверху
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

	//выводим кости левого игрока
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

	//выводим кости правого игрока
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


	//выводим базар
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

	//обводка для базара
	if (game->selected==-2 && game->allDominos.size()>0)
	{
		e->Graphics->DrawRectangle(pen, (float)x, (float)y, (float)107 * SCALE + (game->allDominos.size()-1)*10, (float)211 * SCALE);
	}

	//вывод выложенных домино
	int width = 211 * SCALE;
	int height = 107 * SCALE;
	for (auto step = game->steps.begin(); step != game->steps.end(); step++)
	{
		Image ^dominoBone;
		bool backwards = false;
		//ищем домино в массиве
		if (!dominoImages->TryGetValue(step->domino.a + "-" + step->domino.b, dominoBone))
		{
			//если нет, то ищем с обратным номером
			dominoBone = dominoImages[step->domino.b + "-" + step->domino.a];
			backwards = true;
		}

		dominoBone = (Image^)dominoBone->Clone();
		
		//если обратное, то поворачиваем
		if (backwards)
			dominoBone->RotateFlip(RotateFlipType::Rotate180FlipNone);

		//распологаем в зависимости от угла поврота
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

	//вывод песочных часов 
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

//обработчик событий для кликов мыши по игровому полю
System::Void Form1::myPanel1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	//условия, при которых события не обрабатываются
	if (!game || game->isGameEnd || game->turn!=0)
		return;

	//клик по домино
	if (game->selected >= 0)
	{
		game->addToTable(0, game->selected);	//положить на стол
		timer1->Enabled = true;					//включить таймер
		myPanel1->Invalidate();
	}
	else if (game->selected == -2 && game->turn == 0)	//клик по базару
	{
		if (game->checkStepForPlayer() == 1)
		{
			game->takeFromBase(0);				//взять из базара
			timer1->Enabled = true;
			myPanel1->Invalidate();
		}		
	}
}

//обработчик событий мыши по движению по игровому полю
System::Void Form1::myPanel1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	//условия, при которых события не обрабатываются
	if (!game || game->isGameEnd || game->turn != 0)
		return;

	int x = 100;
	int y = myPanel1->Height - 90;
	int i = 0;

	//проверяем, есть ли пересечение с домино
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

	//пересечение с базаром
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
		
	//нет пересечений
	if (game->selected != -1)
	{
		game->selected = -1;
		myPanel1->Invalidate();
	}
}

//событие таймера, выполняет вызов ходов компьютеров
System::Void Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	//если ходит пользователь
	if (game->turn == 0)
	{
		game->checkStepForPlayer();	//проверяем, может ли ходить пользователь
		if (game->turn==0)			//если может, то останавливаем таймер
			timer1->Enabled = false;
	}
	else
		game->doStep();	//ходит компьютер

	myPanel1->Invalidate();

	//если конец игры
	if (game->isGameEnd)
	{
		timer1->Enabled = false;
		countScores();		
	}	
}

//подсчет очков
void Form1::countScores()
{
	int i = 0;
	String ^text = "Очки:\r\n";
	int lose = -1;

	//считаем очки со всех пользователей
	for (auto player = game->players.begin(); player != game->players.end(); player++, i++)
	{
		int sum = 0;	//сумма очков пользователя
		for (auto domino = (*player)->dominos.begin(); domino != (*player)->dominos.end(); domino++)
		{
			if (domino->a == 0 && domino->b == 0)	//если дубль пустышка
				sum += 10;
			else
			{
				//очки костяшки
				sum += domino->a;
				sum += domino->b;
			}
		}

		playerScores[i] += sum;
		//если количество очков больше суммы
		if (playerScores[i] >= endScores)
		{
			//конец игры
			lose = i;					
		}

		//какой пользователь 
		String ^playerName;
		if (i == 0)
			playerName = "Вы";
		else if (i == 1)
			playerName = "Игрок сверху";
		else if (i == 2)
			playerName = "Игрок слева";
		else if (i == 3)
			playerName = "Игрок справа";

		//собираем все сообщение
		text += playerName + ": в раунде " + sum + ", всего " + playerScores[i] + "\r\n";
	}

	//если проиграл какой-нибудь игрок
	if (lose != -1)
	{
		String ^mes;
		if (lose == 0)
			mes = "Вы проиграли\r\n";
		else if (lose == 1)
			mes = "Игрок сверху проиграл\r\n" ;
		else if (lose == 2)
			mes = "Игрок слева проиграл\r\n";
		else if (lose == 3)
			mes = "Игрок справа проиграл\r\n";

		text = mes +  "\r\n" + text;

		MessageBox::Show(text);

		return;
	}

	//выводим предложение сыграть следующий раунд
	text += "\r\nСледующий раунд?";
	auto dr = MessageBox::Show(text, "Конец раунда", MessageBoxButtons::YesNo);
	if (dr == ::DialogResult::Yes)
	{
		int num = game->players.size();
		delete game;
		game = new Game(num);
		
		timer1->Enabled = true;
		myPanel1->Invalidate();
	}
}

//новая игра
System::Void Form1::новаяИграToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
	//вызываем форму
	NewGame ^form = gcnew NewGame(this);
	form->Show();
}

//вызов помощи
System::Void Form1::справкаToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
	//вызываем форму
	HelpForm ^form = gcnew HelpForm();
	form->Show();
}