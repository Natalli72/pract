#pragma once
#include <list>

//структура костяшки домино
struct Domino
{
	int a;
	int b;

	Domino()
	{}

	Domino(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
};

//игрок
class Player
{
public:
	std::list<Domino> dominos;
};

//ход
struct Step
{
	//координаты на поле
	float x;
	float y;
	int angle;	//угол поворота
	Domino domino;	//костяшка домино
	
	Step(float x, float y, int angle, Domino domino)
	{
		this->x = x; 
		this->y = y;
		this->angle = angle;
		this->domino = domino;
	}
};

class Game
{
public:
	Game(int numberPlayers);
	~Game();

	std::list<Player*> players;	//игроки
	std::list<Domino> allDominos;	//базар

	std::list<Step> steps;	//ходы

	bool isGameEnd;	//закончена ли игра

	int turn;	//чей ход
	int selected;	//выделенная домино

	int frontState;		//состояние начала списка 0-вправо,1-угол,2-вниз,3-угол,4-влево,5-угол,6-вверх,7-угол,8-вправо
	int backState;		//состояние конца списка 0-влево,1-угол,2-вниз,3-угол,4-вправо,5-угол,6-вверх,7-угол,8-влево

	void addToTable(int playerIndex, int dominoIndex);	//выложить домино на стол
	void takeFromBase(int playerIndex);					//взять домино с базара
		
	void doStep();				//ход игрока
	int checkStepForPlayer();	//проверить, может ли игрок ходить
private:
	int numberPlayers;	//количество игроков
	int playerPass;		//сколько игроков пропустило подряд ход (для проверки состояния рыбы)

	void nextTurn();	//перевод хода на следующего игрока

	void doFirstStep();		//сделать первый ход компьютером
	void endGame();			//конец игры

	void findFirstTurn();		//найти первый ход
	void getCoordinate(bool isFrontElement, int &angle, float &x, float &y);	//получить координаты для выкладывания домино
	void mix();		//перемешать базар
	void giveToPlayers();	//раздать домино игрокам
};

