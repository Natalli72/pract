#include "stdafx.h"
#include "Game.h"
#include <random>
#include <ctime>

Game::Game(int numberPlayers)
{
	this->numberPlayers = numberPlayers;

	selected = -1;

	int cnt = 0;

	turn = 0;

	//������� ������
	for (int i=0; i<=6; i++)
		for (int j = i; j <= 6; j++)
		{
			allDominos.push_back(Domino(i, j));
			cnt++;
		}

	frontState = 0;
	backState = 0;

	isGameEnd = false;

	mix();	//����������
	giveToPlayers();	//������� �������
	findFirstTurn();	//����� ��� ����� ������ ������
}


Game::~Game()
{
	for (auto pl = players.begin(); pl != players.end(); pl++)
		delete *pl;
}

void Game::findFirstTurn()
{
	//���� ������ � �������� ������ � ���������� ������ �� ����������� 0-0


	int minNumber = 7;
	int playerInd = 0;
	int i = 0;
	
	//�������� �� ���� �������
	for (auto player=players.begin(); player!=players.end(); player++, i++)
	{
		//�������� �� ���� ������
		for (auto domino = (*player)->dominos.begin(); domino != (*player)->dominos.end(); domino++)
		{
			//���� ���������� �����
			if (domino->a == domino->b && domino->a != 0 && domino->a < minNumber)
			{
				minNumber = domino->a;
				playerInd = i;
			}
		}
	}

	turn = playerInd;
}

//������������� ������
void Game::mix()
{
	srand(time(NULL));	//�������������� ��������� ��������
	int size = allDominos.size();
	for (int i = 0; i < size*1000; i++)	//���������� �������� ��� ��������������
	{
		int ind1 = i%size;//����� ������ ������
		int ind2 = rand() % size;	//������ ������ �������� ��������
		//���� ������� �����
		if (ind1 == ind2)
			ind2++;
		if (ind2 >= size)	
			ind2 = rand() % (size - ind1);

		//������ �������
		auto iter1 = std::next(allDominos.begin(), ind1);
		auto iter2 = std::next(allDominos.begin(), ind2);
		Domino temp = *iter1;
		*iter1 = *iter2;
		*iter2 = temp;
	}
}

//������� �������
void Game::giveToPlayers()
{
	//������� ���������
	int domNumb = 7;
	if (numberPlayers > 2)
		domNumb = 5;

	//�������
	for (int i = 0; i < numberPlayers; i++)
	{
		Player *player = new Player();
		for (int i = 0; i < domNumb; i++)
		{
			player->dominos.push_back(allDominos.front());	//��������� ������
			allDominos.pop_front();		//������� �� ������ ������
		}

		players.push_back(player);	//��������� ������
	}
}

//��������� �� ����
void Game::addToTable(int playerIndex, int dominoIndex)
{
	auto player = std::next(players.begin(), playerIndex);	//����� ������
	auto chD = std::next((*player)->dominos.begin(), dominoIndex);	//����� ��� ������

	float x=0, y=0;
	int angle;

	//���� �����
	if (chD->a == chD->b)
	{
		angle = 0;
		y = -0.25f;
	}
	else
	{
		//������������ (���������� �������� ����������� �����������)
		angle = 270;			
	}

	//���� ������ ��� ������
	if (steps.size() == 0)
	{
		if (chD->a != chD->b || chD->a==0)
			return;

		//���� ����� ������, ���������, ������ �������� ������� ����� ��� ���
		if (playerIndex == 0)
		{
			int minNumber = 7;
			int dominoInd = 0;
			int i = 0;
			for (auto domino = (*player)->dominos.begin(); domino != (*player)->dominos.end(); domino++, i++)
			{
				if (domino->a == domino->b && domino->a != 0 && domino->a < minNumber)
				{
					minNumber = domino->a;
					dominoInd = i;
				}
			}

			if (dominoInd != dominoIndex)
				return;
		}

		steps.push_back(Step(x, y, 0, *chD));
	}
	//���������, �������� �� ���������� ������ � ������ �� ������
	else if (steps.back().domino.b == chD->a)
	{
		getCoordinate(false, angle, x, y);

		steps.push_back(Step(x, y, angle, *chD));
	}
	else if (steps.back().domino.b == chD->b)
	{
		getCoordinate(false, angle, x, y);

		steps.push_back(Step(x, y, angle, Domino(chD->b, chD->a)));
	}
	else if (steps.front().domino.a == chD->a) 
	{
		getCoordinate(true, angle, x, y);
		
		steps.push_front(Step(x, y, angle, Domino(chD->b, chD->a)));
	}
	else if (steps.front().domino.a == chD->b)
	{
		getCoordinate(true, angle, x, y);

		steps.push_front(Step(x, y, angle, *chD));
	}	
	else
		return;

	//������� � ������ ��������
	(*player)->dominos.erase(chD);

	//��������� �� ��������� �� � ������ ������
	if ((*player)->dominos.size() == 0)
		endGame();
	else if (playerIndex == 0)
	{
		nextTurn();
		playerPass = 0;
	}
}

//�������� ���������� ��� ������
void Game::getCoordinate(bool isFrontElement, int &angle, float &x, float &y)
{
	// � ����������� �� ��������� ��������� ���������� � ����. ��������� ����������� ����������� ������� � ����������
	y = 0;

	if (angle==0)
	{
		y = -0.25f;
	}

	//���������� � ������ ������
	if (isFrontElement)
	{
		if (frontState == 0 || frontState == 1 && angle == 0)
		{
			if (angle==270)
				x = steps.front().x - 1;
			else
				x = steps.front().x - 0.5f;

			if (x <= -1)
				frontState = 1;
		}
		else if (frontState == 1)
		{
			x = steps.front().x - 0.5f;

			if (steps.front().angle != 270)
				y = steps.front().y + 0.25f;
			else
				y = steps.front().y;
			

			angle = 180;

			frontState = 2;
		}
		else if (frontState == 2 || frontState==3 && angle == 0)
		{
			if (angle == 0)
			{
				y = steps.front().y + 1;
				x = steps.front().x - 0.25f;

				angle = 270;
			}
			else
			{
				

				if (steps.front().angle != 180)
				{
					x = steps.front().x + 0.25f;
					y = steps.front().y + 0.5f;
				}
				else
				{
					x = steps.front().x;
					y = steps.front().y + 1;
				}

				angle = 180;
			}

			if (y >= 3)
				frontState = 3;
		}
		else if (frontState == 3)
		{
			if (steps.front().angle % 90 == 0 && steps.front().angle % 180 != 0)
			{
				x = steps.front().x + 0.25f;
				y = steps.front().y + 0.5f;
			}
			else
			{
				x = steps.front().x;// +0.5f;
				y = steps.front().y + 1;
			}



			if (angle == 270)
				angle = 90;

			frontState = 4;
		}
		else if (frontState == 4 || frontState == 5 && angle == 0)
		{			
			if (angle == 270)
			{
				x = steps.front().x + 1;

				if (steps.front().angle == 0)
				{
					y = steps.front().y + 0.25f;
					x = steps.front().x + 0.5f;
				}
				else
				{
					y = steps.front().y;
					x = steps.front().x + 1;
				}
			}
			else
			{
				x = steps.front().x + 1;
				y = steps.front().y - 0.25f;
			}

			if (angle == 270)
				angle = 90;

			frontState = 4;

			if (x >= 6)
				frontState = 5;
		}
		else if (frontState == 5)
		{
			

			if (steps.front().angle != 90)
			{
				y = steps.front().y - 0.25f;
				x = steps.front().x + 0.5f;
			}
			else
			{
				y = steps.front().y - 0.5f;
				x = steps.front().x + 1;
			}
			
			angle = 0;
			
			frontState = 6;
		}
		else if (frontState == 6 || frontState == 7 && angle == 0)
		{
			if (angle == 0)
			{
				y = steps.front().y - 0.5f;
				x = steps.front().x - 0.25f;

				angle = 270;
			}
			else
			{
				if (steps.front().angle != 0)
				{
					x = steps.front().x + 0.25f;
					y = steps.front().y - 1;
				}
				else
				{
					x = steps.front().x;
					y = steps.front().y - 1;
				}

				angle = 0;
			}

			if (y <= 1 )
				frontState = 7;
		}
		else if (frontState == 7)
		{
			if (steps.front().angle == 270)
			{
				y = steps.front().y - 0.5f;
				x = steps.front().x - 0.25f;
			}
			else
			{
				y = steps.front().y-0.5f;
				x = steps.front().x - 0.5f;
			}


			frontState = 8;
		}
		else if (frontState == 8)
		{
			if (angle == 270)
			{
				x = steps.front().x - 1;

				if (steps.front().angle == 0)
				{
					y = steps.front().y + 0.25f;
					x = steps.front().x - 1;
				}
				else
				{
					y = steps.front().y;
					x = steps.front().x - 1;
				}
			}
			else
			{
				x = steps.front().x - 0.5f;
				y = steps.front().y - 0.25f;
			}

		}
	}
	else //���������� � ����� ������
	{
		if (backState == 0 || backState==1 && angle == 0)
		{
			if (steps.back().angle % 90 == 0 && steps.back().angle % 180 != 0)
				x = steps.back().x + 1;
			else
				x = steps.back().x + 0.5f;

			if (x >= 6)
				backState = 1;
		}
		else if (backState == 1)
		{
			if (steps.back().angle != 270)
			{
				y = steps.back().y + 0.25f;
				x = steps.back().x + 0.5f;
			}
			else
			{
				y = steps.back().y;
				x = steps.back().x + 1;
			}

			angle = 0;

			backState = 2;
		}
		else if (backState == 2 || backState == 3 && angle == 0)
		{
			if (angle == 0)
			{
				y = steps.back().y + 1;
				x = steps.back().x - 0.25f;

				angle = 270;
			}
			else
			{
				if (steps.back().angle != 0)
				{
					x = steps.back().x + 0.25f;
					y = steps.back().y + 0.5f;
				}
				else
				{
					x = steps.back().x;
					y = steps.back().y + 1;
				}

				angle = 0;
			}

			if (y >= 3)
				backState = 3;
		}
		else if (backState == 3)
		{			
			if (steps.back().angle % 90 == 0 && steps.back().angle % 180 != 0)
			{
				x = steps.back().x - 0.25f;
				y = steps.back().y + 0.5f;
			}
			else
			{
				x = steps.back().x-0.5f;
				y = steps.back().y + 1;
			}

			if (angle == 270)
				angle = 90;

			backState = 4;
		}
		else if (backState == 4 || backState == 5 && angle == 0)
		{
			if (angle == 270)
			{
				x = steps.back().x + 1;

				if (steps.back().angle == 0)
				{
					y = steps.back().y + 0.25f;
					x = steps.back().x - 1;
				}
				else
				{
					y = steps.back().y;
					x = steps.back().x - 1;
				}
			}
			else
			{
				x = steps.back().x - 0.5f;
				y = steps.back().y - 0.25f;
			}

			if (angle == 270)
				angle = 90;

			backState = 4;

			if (x <= -2)
				backState = 5;
		}
		else if (backState == 5)
		{

			if (steps.back().angle != 90)
			{
				y = steps.back().y - 0.25f;
				x = steps.back().x - 0.5f;
			}
			else
			{
				y = steps.back().y - 0.5f;
				x = steps.back().x - 0.5;
			}

			angle = 180;

			backState = 6;
		}
		else if (backState == 6 || backState == 7 && angle == 0)
		{
			if (angle == 0)
			{
				y = steps.back().y - 0.5f;
				x = steps.back().x - 0.25f;

				angle = 270;
			}
			else
			{
				if (steps.back().angle != 180)
				{
					x = steps.back().x + 0.25f;
					y = steps.back().y - 1;
				}
				else
				{
					x = steps.back().x;
					y = steps.back().y - 1;
				}

				angle = 180;
			}

			if (y <= 1)
				backState = 7;
		}
		else if (backState == 7)
		{
			if (steps.back().angle == 270)
			{
				y = steps.back().y - 0.5f;
				x = steps.back().x + 0.25f;
			}
			else
			{
				y = steps.back().y - 0.5f;
				x = steps.back().x ;
			}


			backState = 8;
		}
		else if (backState == 8)
		{
			if (angle == 270)
			{
				if (steps.back().angle == 0)
				{
					y = steps.back().y + 0.25f;
					x = steps.back().x + 0.5f;
				}
				else
				{
					y = steps.back().y;
					x = steps.back().x + 1;
				}
			}
			else
			{
				x = steps.back().x + 1;
				y = steps.back().y - 0.25f;
			}

		}
	}
}

//����� �� ������
void Game::takeFromBase(int playerIndex)
{
	//���� ����� ������
	if (allDominos.size() == 0)
		return;

	//�������� ������
	auto player = std::next(players.begin(), playerIndex);
	//��������� ��� ��������
	(*player)->dominos.push_back(allDominos.front());
	allDominos.pop_front();
}

//������� ��� ����������
void Game::doStep()
{
	//���� ������ ���
	if (steps.size() == 0)
	{
		doFirstStep();
		return;
	}

	//�������� ������
	auto player = std::next(players.begin(), turn);
	int i = 0;
	//�������� �� ��� ������
	for (auto domino = (*player)->dominos.begin(); domino != (*player)->dominos.end(); domino++, i++)
	{
		if (domino->a == steps.front().domino.a || domino->b == steps.front().domino.a ||
			domino->a == steps.back().domino.b || domino->b == steps.back().domino.b)
		{
			addToTable(turn, i);

			if ((*player)->dominos.size() == 0)
				endGame();
			else
			{
				nextTurn();
				playerPass = 0;
			}
			
			return;			
		}
	}

	//���������� ���� �� ����� ������
	//���� ����� ����
	if (allDominos.size() == 0)
	{
		nextTurn();	//��������� �� ���������� ������
		playerPass++;

		//���� ��� ������ ���������
		if (playerPass >= numberPlayers)
		{
			endGame();
		}
	}
	else
		takeFromBase(turn);	//����� �� ������
}

//��������� ����� �� ������ �����. 0 - ����� ������ ��� ����� �� ������, 1 ���� ���, ������ ���
int Game::checkStepForPlayer()
{
	if (steps.size() != 0)
	{
		auto player = players.front();
		int i = 0;
		bool fndStep = false;
		for (auto domino = player->dominos.begin(); domino != player->dominos.end(); domino++, i++)
		{
			if (domino->a == steps.front().domino.a || domino->b == steps.front().domino.a ||
				domino->a == steps.back().domino.b || domino->b == steps.back().domino.b)
			{	
				fndStep = true;
				playerPass = 0;
				return 0;
			}
		}

		//���� �� ������ ��� � ����� ����
		if (!fndStep && allDominos.size() == 0)
		{
			turn = 1;
			playerPass++;

			//���� ��� ���������
			if (playerPass >= numberPlayers)
			{
				endGame();
			}			
		}

		return 1;
	}

	return 0;
}

//������� ������ ���
void Game::doFirstStep()
{
	int minNumber = 7;
	int dominoInd = 0;
	int i = 0;

	auto player = std::next(players.begin(), turn);

	for (auto domino = (*player)->dominos.begin(); domino != (*player)->dominos.end(); domino++, i++)
	{
		if (domino->a == domino->b && domino->a != 0 && domino->a < minNumber)
		{
			minNumber = domino->a;
			dominoInd = i;
		}
	}	

	addToTable(turn, dominoInd);
	nextTurn();
}

//������� �� ��������� ���
void Game::nextTurn()
{
	//��� ������������ ������� ���������� �������, ����� �����, 2 ����� ������, 3 �����, 4 ������

	if (numberPlayers == 4)
	{
		if (turn == 0)
			turn = 3;
		else if (turn == 3)
			turn = 1;
		else if (turn == 1)
			turn = 2;
		else if (turn == 2)
			turn = 0;
	}
	else 	
		turn = (turn + 1) % numberPlayers;
}

void Game::endGame()
{
	isGameEnd = true;
}