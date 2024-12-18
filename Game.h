#pragma once
#include <list>

//��������� �������� ������
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

//�����
class Player
{
public:
	std::list<Domino> dominos;
};

//���
struct Step
{
	//���������� �� ����
	float x;
	float y;
	int angle;	//���� ��������
	Domino domino;	//�������� ������
	
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

	std::list<Player*> players;	//������
	std::list<Domino> allDominos;	//�����

	std::list<Step> steps;	//����

	bool isGameEnd;	//��������� �� ����

	int turn;	//��� ���
	int selected;	//���������� ������

	int frontState;		//��������� ������ ������ 0-������,1-����,2-����,3-����,4-�����,5-����,6-�����,7-����,8-������
	int backState;		//��������� ����� ������ 0-�����,1-����,2-����,3-����,4-������,5-����,6-�����,7-����,8-�����

	void addToTable(int playerIndex, int dominoIndex);	//�������� ������ �� ����
	void takeFromBase(int playerIndex);					//����� ������ � ������
		
	void doStep();				//��� ������
	int checkStepForPlayer();	//���������, ����� �� ����� ������
private:
	int numberPlayers;	//���������� �������
	int playerPass;		//������� ������� ���������� ������ ��� (��� �������� ��������� ����)

	void nextTurn();	//������� ���� �� ���������� ������

	void doFirstStep();		//������� ������ ��� �����������
	void endGame();			//����� ����

	void findFirstTurn();		//����� ������ ���
	void getCoordinate(bool isFrontElement, int &angle, float &x, float &y);	//�������� ���������� ��� ������������ ������
	void mix();		//���������� �����
	void giveToPlayers();	//������� ������ �������
};

